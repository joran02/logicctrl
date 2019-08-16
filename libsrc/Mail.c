// Mail.cpp: implementation of the CMail class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>

#include <netinet/tcp.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>

#include "Mail.h"
#include "devfile.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int  BASE64_MAXLINE = 76;
const char EOL[] = "\r\n";
const char BASE64_TAB[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz0123456789+/";

#define BOUNDARY_TEXT "#BOUNDARY#"


#define MAX_LENGTH (3*128*1024)
#define MAX_BASE_BUF (3*128*1024)
#define MAX_ENCODE_NUM (100*1024)
int m_fdsock= 0;
char m_recvbuff[MAX_LENGTH];
char m_sendbuff[MAX_LENGTH];
char Base64In[MAX_BASE_BUF];
char Base64Out[MAX_BASE_BUF];

static int sendmail(char * pSmtpServer,int iPort,char * pUserName,char * pPass,
					char * pMailFrom,char * pMailSendto,
					char * pSubject,char* pData,char* p_attachment,int m_priority);

static int CloseSmtpSocket();

int ANSIToBase64(const char *szInANSI, int nInLen, char *szOutBase64, int nOutLen)
{
	 //Input Parameter validation
	 if ((szInANSI == NULL) || (nInLen == 0) || (szOutBase64 == NULL) || (nOutLen == 0))
	  return 0;
	 if (nOutLen < (nInLen*4/3 + 1 + nInLen*4/3/BASE64_MAXLINE*2 + 1 + 4))
	  return 0;

	 //Set up the parameters prior to the main encoding loop
	 int nInPos  = 0;
	 int nOutPos = 0;
	 int nLineLen = 0;
	 int c1, c2, c3;
	 int i;

	 // Get three characters at a time from the input buffer and encode them
	 for (i=0; i<nInLen/3; ++i)
	 {
	  //Get the next 2 characters
	  c1 = szInANSI[nInPos++] & 0xFF;
	  c2 = szInANSI[nInPos++] & 0xFF;
	  c3 = szInANSI[nInPos++] & 0xFF;

	  //Encode into the 4 6 bit characters
	  szOutBase64[nOutPos++] = BASE64_TAB[c1 >> 2];
	  szOutBase64[nOutPos++] = BASE64_TAB[((c1 << 4) | (c2 >> 4)) & 0x3F];
	  szOutBase64[nOutPos++] = BASE64_TAB[((c2 << 2) | (c3 >> 6)) & 0x3F];
	  szOutBase64[nOutPos++] = BASE64_TAB[c3 & 0x3F];
	  nLineLen += 4;

	  //Handle the case where we have gone over the max line boundary
	  if (nLineLen > BASE64_MAXLINE - 4)
	  {
	   szOutBase64[nOutPos++] = EOL[0];
	   szOutBase64[nOutPos++] = EOL[1];
	   nLineLen = 0;
	  }
	 }

	 // Encode the remaining one or two characters in the input buffer
	 switch (nInLen % 3)
	 {
	  case 0:
	  {
	   szOutBase64[nOutPos++] = EOL[0];
	   szOutBase64[nOutPos++] = EOL[1];
	   break;
	  }
	  case 1:
	  {
	   c1 = szInANSI[nInPos] & 0xFF;
	   szOutBase64[nOutPos++] = BASE64_TAB[(c1 & 0xFC) >> 2];
	   szOutBase64[nOutPos++] = BASE64_TAB[((c1 & 0x03) << 4)];
	   szOutBase64[nOutPos++] = '=';
	   szOutBase64[nOutPos++] = '=';
	   szOutBase64[nOutPos++] = EOL[0];
	   szOutBase64[nOutPos++] = EOL[1];
	   break;
	  }
	  case 2:
	  {
	   c1 = szInANSI[nInPos++] & 0xFF;
	   c2 = szInANSI[nInPos] & 0xFF;
	   szOutBase64[nOutPos++] = BASE64_TAB[(c1 & 0xFC) >> 2];
	   szOutBase64[nOutPos++] = BASE64_TAB[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
	   szOutBase64[nOutPos++] = BASE64_TAB[((c2 & 0x0F) << 2)];
	   szOutBase64[nOutPos++] = '=';
	   szOutBase64[nOutPos++] = EOL[0];
	   szOutBase64[nOutPos++] = EOL[1];
	   break;
	  }
	  default:
	  {
	   return 0;
	  }
 }

 szOutBase64[nOutPos] = 0;

 return nOutPos;
}

static void mail_get_host_name(char * pServerIp,char * buf)
{

	int a;
	struct hostent *myhost;
	char ** pp;
	struct in_addr addr;
	int is_url = 0;
	
	for(a = 0; a < 8;a++)
	{
		if( pServerIp[a] == (char)NULL )
			break;

		if( (pServerIp[a] < '0' || pServerIp[a] > '9') 
					&& (pServerIp[a] != '.')  )
		{
			is_url = 1;
			
			 printf("smtp pServerIp=%s\n",pServerIp);
			//不知道什么原因解析不了域名。
			myhost = gethostbyname(pServerIp); 

			if( myhost == NULL )
			{
				printf("gethostbyname can't get %s ip\n",pServerIp);
				//herror(pServerIp);
			/*
				printf("use ping to get ip\n");
				get_host_name_by_ping(pServerIp,buf);
				if( buf[0] == 0 )
					printf("get_host_name_by_ping also can't get %s ip!\n",pServerIp);
			*/
				 break;
			}
			 
			printf("host name is %s\n",myhost->h_name);		 
		 
			//for (pp = myhost->h_aliases;*pp!=NULL;pp++)
			//	printf("%02X is %s\n",*pp,*pp);
			
			pp = myhost->h_addr_list;
			while(*pp!=NULL)
			{
				addr.s_addr = *((unsigned int *)*pp);
				printf("address is %s\n",inet_ntoa(addr));
				sprintf(buf,"%s",inet_ntoa(addr));
				pp++;
			}
			
			break;
		}
	}

	if( is_url == 0 )
	{
		strcpy(buf,pServerIp);

		DPRINTK("is not url ,set ip buf %s\n",buf);
	}
}

static int ConnetToSmtp(char * pSmtpServer,int iPort)
{
	struct sockaddr_in sock_addr;	
	struct in_addr inaddr;
	char ip_buf[200];

	/*---------------根据域名获取网络地址，存放在struct in_addr中----------------*/


	memset(ip_buf,0,200);
	mail_get_host_name(pSmtpServer,ip_buf);//smtp.163.com
	if( ip_buf[0] == 0 )
	{			
		return CONNECTSERVER_DDNS_ERROR;
	}


	
	{
		struct  linger  linger_set;
		struct timeval stTimeoutVal;

		int i;
		
		if( -1 == (m_fdsock=  socket(PF_INET,SOCK_STREAM,0)))
		{			
			return SOCK_CREATE_ERROR;
		}
		
		i = 1;

		if (setsockopt(m_fdsock, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int)) < 0) 
		{		
			close( m_fdsock );
			m_fdsock = 0;
			DPRINTK("setsockopt SO_REUSEADDR error \n");		
			return SOCK_CREATE_ERROR;	
		}	

		linger_set.l_onoff = 1;
		linger_set.l_linger = 5;
	       if (setsockopt( m_fdsock, SOL_SOCKET, SO_LINGER , (char *)&linger_set, sizeof(struct  linger) ) < 0 )
	       {
			close( m_fdsock);
			m_fdsock = 0;
			
			return SOCK_CREATE_ERROR;
		}


		 
			stTimeoutVal.tv_sec = 10;
			stTimeoutVal.tv_usec = 0;
			if ( setsockopt( m_fdsock, SOL_SOCKET, SO_SNDTIMEO, (char *)&stTimeoutVal, sizeof(struct timeval) ) < 0 )
		      {
		      		close( m_fdsock );
					m_fdsock = 0;				
				return SOCK_CREATE_ERROR;
			}
				 	
			if ( setsockopt( m_fdsock, SOL_SOCKET, SO_RCVTIMEO, (char *)&stTimeoutVal, sizeof(struct timeval) ) < 0 )
		    {
		       		close( m_fdsock );
					m_fdsock = 0;
				printf( "setsockopt SO_RCVTIMEO error\n" );
				return SOCK_CREATE_ERROR;
			}
		  
	}
	
	sock_addr.sin_family=AF_INET;
	sock_addr.sin_port=htons(iPort);
	sock_addr.sin_addr=inaddr;//直接结构体赋值给结构体，或者写成：sock_addr.sin_addr.s_addr=inaddr.S_un.S_addr;
 	sock_addr.sin_addr.s_addr = inet_addr(ip_buf);      
	

	if (connect(m_fdsock,(struct sockaddr *)&sock_addr,sizeof(sock_addr)) == -1) 
	{
        if (errno == EINPROGRESS) 
		{
             DPRINTK("connect timeout\n");			
        }        
		
		 
		CloseSmtpSocket();
		return CONNECT_ERROR;
     }
	 
	return 1;
}

static int CloseSmtpSocket()
{
	int ret=1;

	if(m_fdsock)
	{
		ret = close(m_fdsock);
		m_fdsock = 0;
	}

	return ret;
}

static int recvdata()
{
	 int crecv;
	 crecv=recv(m_fdsock,m_recvbuff,MAX_LENGTH,0);
	 if(crecv<=0)
	 {

		return WSA_RECV_ERROR;
	 }	 
	 else
	 {
	 }

	return 1;
}

static int senddata()
{
 int idx = 0;
 int csend;
 int count=strlen(m_sendbuff);
 while(count>0)
 {
	  if((csend=send(m_fdsock,m_sendbuff+idx,count,0))< 0)
	  {
		   
		   return WSA_SEND_ERROR;
	  }

	  if(csend==0)
	  {
			break;
	  }
	  count-=csend;
	  idx+=csend;
 }

 return 1;
}


static int smtplogin(char * pSmtpServer,int iPort,char * pUserName,char * pPass)
{
	int ret;
	int encode_num;
 
	ret = ConnetToSmtp(pSmtpServer,iPort);
	if(ret<0) 
		return ret;

	ret=recvdata();
	if(ret<0) 
		return ret;

	if(strstr(m_recvbuff,"220")==0)//220
	{  
		 return UNKNOWEN_ERROR;
	} 
  
	 //发送helo 返回250
	 sprintf(m_sendbuff,"%s","helo 123\r\n");
	 ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = recvdata();
	 if(ret<0) 
		return ret;
 
	 if(strncmp(m_recvbuff,"250",3)!=0)
	 {	  
		return HELO_ERROR;
	 }
  
  
	//发送auth login 返回334
	 sprintf(m_sendbuff,"%s","auth login\r\n");
	 ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = recvdata();
	 if(ret<0) 
		return ret;

	 if(strncmp(m_recvbuff,"334",3)!=0)
	 {	  
		return AUTH_LOGIN_ERROR;
	 }

	 //发送经过base64编码后的用户名，返回334

	 strcpy(Base64In,pUserName);

	 encode_num = ANSIToBase64(Base64In, strlen(Base64In), Base64Out, MAX_BASE_BUF);

	 Base64Out[encode_num] = '\0';
	 
//	 sprintf(m_sendbuff,"%s%s",Base64Out,"\r\n");
	 sprintf(m_sendbuff,"%s",Base64Out);
	 
	 ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = recvdata();
	 if(ret<0) 
		return ret;

	 if(strncmp(m_recvbuff,"334",3)!=0)
	 {
	  	  return USER_ERROR;
	 }

	 //发送经过base64编码后的密码 返回235

	 strcpy(Base64In,pPass);

	 encode_num = ANSIToBase64(Base64In, strlen(Base64In), Base64Out, MAX_BASE_BUF);

	 Base64Out[encode_num] = '\0';
	 
	 //sprintf(m_sendbuff,"%s%s",Base64Out,"\r\n");
	 sprintf(m_sendbuff,"%s",Base64Out);

	 ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = recvdata();
	 if(ret<0) 
		return ret;

	 if(strncmp(m_recvbuff,"235",3)!=0)
	 {	   
	   return PASS_ERROR;
	 }

	 return 1;
}



static int SetMailHeader(char* header,char* pMailFrom,char* pMailer,char * pMailRev,
						 char * pSubject,char* p_attachment,int m_priority)
{
	// "Date:","From:","To:",
	//  "Message-ID:","Subject:","MIME-Version:",
	//  "Content-Type:","X-Priority:",
	//  "X-Mailer:","X-CM-TRANSID:","X-CM-SenderInfo:",
	//  "X-Coremail-Antispam:","\r\n\r\n"
	 char szDate[500];
	 char sztTime[500];

	 // Date: <SP> <dd> <SP> <mon> <SP> <yy> <SP> <hh> ":" <mm> ":" <ss> <SP> <zone> <CRLF>
/*	 SYSTEMTIME st={0};
	 ::GetSystemTime(&st);
	 ::GetDateFormat(MAKELCID(0x0409,SORT_DEFAULT),0,&st,"ddd\',\' dd MMM yyyy",szDate,sizeof(szDate));
	 ::GetTimeFormat(MAKELCID(0x0409,SORT_DEFAULT),TIME_FORCE24HOURFORMAT,&st,"HH\':\'mm\':\'ss",sztTime,sizeof(sztTime));
*/
	 char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	 char *wmon[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	 time_t timep;
	 time(&timep);
	 struct tm *p;

	 p = gmtime(&timep);

	 sprintf(header,"Date: %s,%d %s %d %0.2d:%0.2d:%0.2d\r\n", wday[p->tm_wday],p->tm_mday,wmon[p->tm_mon],
	 								(1900+p->tm_year),p->tm_hour, p->tm_min, p->tm_sec);
	
	// sprintf(header,"Date: %s\r\n", asctime(gmtime(&timep)));

	 DPRINTK("%s\n",header);
 
	 // From: <SP> <sender>  <SP> "<" <sender-email> ">" <CRLF>
	 if(pMailFrom!=NULL)
	 {
	  strcat(header,"From: ");
	  strcat(header," <");
	  strcat(header,pMailFrom);
	  strcat(header, ">\r\n");
	 }


	// X-Mailer: <SP> <xmailer-app> <CRLF>
	 if(pMailer!= NULL)
	 {
	  strcat(header,"X-Mailer: ");
	  strcat(header, pMailer);
	  strcat(header, "\r\n");
	 }

	 // X-Priority: <SP> <number> <CRLF>
	 switch(m_priority)
	 {
	  case XPRIORITY_HIGH:
	   strcat(header,"X-Priority: 2 (High)\r\n");
	   break;
	  case XPRIORITY_NORMAL:
	   strcat(header,"X-Priority: 3 (Normal)\r\n");
	   break;
	  case XPRIORITY_LOW:
	   strcat(header,"X-Priority: 4 (Low)\r\n");
	   break;
	  default:
	   strcat(header,"X-Priority: 3 (Normal)\r\n");
	 }

	 // To: <SP> <remote-user-mail> <CRLF>
	 if(pMailRev!=NULL)
	 {
		  strcat(header,"To: ");
		  strcat(header, pMailRev);
		  strcat(header, "\r\n");
	 }


 // Subject: <SP> <subject-text> <CRLF>
	 if(pSubject!=NULL)
	 {
	   strcat(header, "Subject: ");
	   strcat(header, pSubject);
	   strcat(header, "\r\n");
	 }
 
 // MIME-Version: <SP> 1.0 <CRLF>
	strcat(header,"MIME-Version: 1.0\r\n");


 //attachment
	 if(p_attachment==NULL)
	 { //如果没附件
	  strcat(header,"Content-type: text/plain; charset=GB2312\r\n");
	  strcat(header,"Content-Transfer-Encoding: 7bit\r\n");
	  strcat(header,"\r\n");
	 }
	 else
	 { //如果有附件，设置分界线,每个子段以--BOUNDARY_TEXT行开始，父段则以--BOUNDARY_TEXT--行结束，不同段之间用空行分隔
	  strcat(header,"Content-Type: multipart/mixed; boundary=\"");
	  strcat(header,BOUNDARY_TEXT);
	  strcat(header,"\"\r\n");
	  strcat(header,"\r\n");//这个空行不能少

	  //第一部分为文本内容
	  strcat(header,"--");strcat(header,BOUNDARY_TEXT);strcat(header,"\r\n");//加上分界线

	  //以下跟没附件时的一样
	  strcat(header,"Content-type: text/plain; charset=GB2312\r\n");//注意GB2312 否则会出现中文乱码
	  strcat(header,"Content-Transfer-Encoding: 7bit\r\n");
	 }
 
	 // done    
	 return 1;    
}


static int sendmail(char * pSmtpServer,int iPort,char * pUserName,char * pPass,
					char * pMailFrom,char * pMailSendto,
					char * pSubject,char* pData,char* p_attachment,int m_priority)
{
 int crecv;
 int ret;

	ret = smtplogin(pSmtpServer,iPort,pUserName,pPass);
	if(ret<0) 
		return ret;


 //发送mail from 返回250
 //sprintf(m_sendbuff,"%s%s%s","mail from:<",m_user,"@163.com>\r\n");

	sprintf(m_sendbuff,"%s%s%s","mail from:<",pMailFrom,">\r\n");
	ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = recvdata();
	 if(ret<0) 
		return ret;

	 if(strncmp(m_recvbuff,"250",3)!=0)
	 {	   
		return MAIL_FROM_ERROR;
	 }

 //发送rcpt to 返回250
	//sprintf(m_sendbuff,"%s%s%s","rcpt to:<",m_rcptto,">\r\n");
	sprintf(m_sendbuff,"%s%s%s","rcpt to:<",pMailSendto,">\r\n");
	ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = recvdata();
	 if(ret<0) 
		return ret;

	 if(strncmp(m_recvbuff,"250",3)!=0)
	 {	  
		return RCPT_TO_ERROR;
	 }

	 //发送data\r\n 返回354
	 sprintf(m_sendbuff,"%s","data\r\n");
	 ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = recvdata();
	 if(ret<0) 
		return ret;

	 if(strncmp(m_recvbuff,"354",3)!=0)
	 {	  
	   return DATA_ERROR;
	 }

 
	 //信头
	 if(!SetMailHeader(m_sendbuff,pMailFrom, "Dvr", pMailSendto,
						  pSubject,p_attachment,m_priority))
	 {	  
		return MAILHEADER_ERROR;
	 }

	 ret=senddata();
	 if(ret<0) 
		return ret;
  
	 //发送空行
	 sprintf(m_sendbuff,"%s","\r\n\r\n");
	 ret=senddata();
	 if(ret<0) 
		return ret;

	 //发送邮件body
	 sprintf(m_sendbuff,"%s%s",pData,"\r\n");
	 ret=senddata();
	 if(ret<0) 
		return ret;	

	 if(p_attachment!=NULL)
	 {
	  char *pp;
	  pp = p_attachment;
	  //for(pp=p_attachment;*pp!=0;pp++)
	  {
	   FILE* fp;
	   char *buf;
	 //准备发送附件，首先发送分界线,必须!
	   sprintf(m_sendbuff,"%s%s%s","--",BOUNDARY_TEXT,"\r\n");
	   ret=senddata();
		if(ret<0) 
		return ret;

	   sprintf(m_sendbuff,"%s%s%s","Content-Type: application/x-msdownload; name=\"",strrchr(pp,'\\')+1,"\"\r\n");
	   ret=senddata();
	   if(ret<0) 
		return ret;

	   sprintf(m_sendbuff,"%s","Content-Transfer-Encoding: base64\r\n");
	   ret=senddata();
	   if(ret<0) 
		return ret;

	   sprintf(m_sendbuff,"%s%s%s","Content-Disposition: attachment; filename=\"",strrchr(pp,'\\')+1,"\"\r\n\r\n");
	   ret=senddata();
	   if(ret<0) 
		return ret;

	   //将文件base64编码后发送
	   fp=fopen(pp,"rb");
	   if(fp==NULL)
	   {    
		  return OPEN_FILE_ERROR;
	   }
   
	   memset(Base64In,0,MAX_BASE_BUF);   

	   int readcount=0;
	   while(!feof(fp))
	   {
		readcount=fread(Base64In,1,MAX_ENCODE_NUM,fp);
		if(readcount>0)
		{
		 *(Base64In+readcount)='\0';

		 readcount = ANSIToBase64(Base64In, readcount, Base64Out, MAX_BASE_BUF);
		 //sprintf(m_sendbuff,"%s%s",Base64Out,"\r\n");
		 sprintf(m_sendbuff,"%s",Base64Out);
		 ret=senddata();
		 if(ret<0) 
		 {
			 if( fp )
			 {
				 fclose(fp);
				 fp = NULL;
			 }
			return ret;
		 }
		  memset(Base64In,0,MAX_BASE_BUF);  
		}
	   }


	   sprintf(m_sendbuff,"%s","\r\n");
	   ret=senddata();
	   if(ret<0) 
	   {
			 if( fp )
			 {
				 fclose(fp);
				 fp = NULL;
			 }
			return ret;
		}

		if( fp )
		{
			 fclose(fp);
			 fp = NULL;
		}
	  }
	 }

	 if(p_attachment!=NULL)
	 {
	 //附件发送完毕，发送分界线
	   sprintf(m_sendbuff,"%s%s%s","--",BOUNDARY_TEXT,"--\r\n");
		ret=senddata();
	   if(ret<0) 
		return ret;
	 }

 //发送.\r\n 结束  返回250
	sprintf(m_sendbuff,"%s",".\r\n");
	ret=senddata();
	   if(ret<0) 
		return ret;

	ret = recvdata();
	 if(ret<0) 
		return ret;

	 if(strncmp(m_recvbuff,"250",3)!=0)
	 {
		return END_DATA_ERROR;
	 }
	 else
	 {
	   DPRINTK("mail send success!\n");
	 }


	 //退出
	 sprintf(m_sendbuff,"%s","quit\r\n");
	 ret=senddata();
	 if(ret<0) 
		return ret;

	 ret = CloseSmtpSocket();
	 if(ret<0) 
		return ret;

	 
	return 1;
}


int dvr_send_mail(char * pSmtpServer,int iPort,char * pUserName,char * pPass,
					char * pMailFrom,char * pMailSendto,
					char * pSubject,char* pData,char* p_attachment,int m_priority)
{
	int rel = -1;


	DPRINTK("(%s:%d) (%s,%s) (%s,%s) (%s) (pri=%d)\n",pSmtpServer,iPort, pUserName, pPass,
					pMailFrom, pMailSendto,
					pSubject,m_priority);

	rel = sendmail( pSmtpServer,iPort, pUserName, pPass,
					pMailFrom, pMailSendto,
					pSubject,pData,p_attachment, m_priority);

	if( rel < 0 )
	{
		DPRINTK("rel = %d\n",rel);
		CloseSmtpSocket();
	}


	return rel;
	
}


