#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "ntp_timezone.h"
//#include "thread_func.h"

#ifndef DPRINTK
#define DPRINTK(fmt, args...)   printf("(%s,%d)%s: " fmt,__FILE__,__LINE__, __FUNCTION__ , ## args)
#endif

static  NTP_TIMEZONE_ST cur_timezone;
pthread_mutex_t ntp_mutex;

static struct timeval ntp_new_time;

ntp_callback get_time_callback = NULL;

static char ntp_server_url[256] = "pool.ntp.org";
//static char ntp_server_url[256] = "clock.via.net";
static int  ntp_port = 123;
static int now_select_ntp_addr_pos = 0;

//
//rfc1305 defined from 1900 so also  2208988800 (1900 - 1970 ) seconds left 
//
//timeval.tv_sec + JAN_1970 = timestamp.coarse
#define JAN_1970     		0x83aa7e80      

//timeval.tv_usec=>timestamp.fine
#define NTPFRAC(x) (4294 * (x) + ((1981 * (x))>>11))
//timeval.tv_usec<=timestamp.fine
#define USEC(x) (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

#define Data(i) ntohl(((unsigned int *)data)[i])

#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4 
#define PREC -6

struct ntptime 
{
	unsigned int coarse;
	unsigned int fine;
};


void send_packet(int fd)
{
    	unsigned int data[12];
    	struct timeval now;


    	if (sizeof(data) != 48) 
    	{
        	fprintf(stderr,"size error\n");
        	return;
    	}

    	memset((char*)data, 0, sizeof(data));
	data[0] = htonl((LI << 30) | (VN << 27) | (MODE << 24) 
	              | (STRATUM << 16) | (POLL << 8) | (PREC & 0xff));
	data[1] = htonl(1<<16);  /* Root Delay (seconds) */
	data[2] = htonl(1<<16);  /* Root Dispersion (seconds) */
	gettimeofday(&now, NULL);
	data[10] = htonl(now.tv_sec + JAN_1970); /* Transmit Timestamp coarse */
	data[11] = htonl(NTPFRAC(now.tv_usec));  /* Transmit Timestamp fine   */

        //printf("send to server time=>%s\n",ctime(&(now.tv_sec)));
	send(fd, data, 48, 0);
}

void get_udp_arrival_local_timestamp(struct ntptime *udp_arrival_local_timestamp)
{
	struct timeval udp_arrival_local_time;
	
	gettimeofday(&udp_arrival_local_time, NULL);

	
	//printf("ntp time=>%s\n",ctime(&(udp_arrival_local_time.tv_sec)));
}

void get_new_time(unsigned int *data,struct timeval* new_time)
{
	//int li, vn, mode, stratum, poll, prec;
	//int delay, disp, refid;
	//struct ntptime reftime, orgtime, rectime;
	struct ntptime trantime;
	/*li      = Data(0) >> 30 & 0x03;
	vn      = Data(0) >> 27 & 0x07;
	mode    = Data(0) >> 24 & 0x07;
	stratum = Data(0) >> 16 & 0xff;
	poll    = Data(0) >>  8 & 0xff;
	prec    = Data(0)       & 0xff;
	if (prec & 0x80) prec|=0xffffff00;
	delay   = Data(1);
	disp    = Data(2);
	refid   = Data(3);
	reftime.coarse = Data(4);
	reftime.fine   = Data(5);
	orgtime.coarse = Data(6);
	orgtime.fine   = Data(7);
	rectime.coarse = Data(8);
	rectime.fine   = Data(9);*/
	trantime.coarse = Data(10);
	trantime.fine   = Data(11);
	
	new_time->tv_sec = trantime.coarse - JAN_1970;
	new_time->tv_usec = USEC(trantime.fine);

	//print just precise to second
	//printf("server time=>%s\n",ctime(&(new_time->tv_sec)));
}

void set_local_time(struct timeval new_time)
{
	/* need root user. */
	if (0 != getuid() && 0 != geteuid())
	{
		printf("must be root user!:0)\n");
		return;
	}

	settimeofday(&new_time, NULL);
}



void ntp_get_host_name(char * pServerIp,char * buf)
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
			
			// printf("pServerIp=%s\n",pServerIp);
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
			 
			//printf("host name is %s\n",myhost->h_name);		 
		 
			//for (pp = myhost->h_aliases;*pp!=NULL;pp++)
			//	printf("%02X is %s\n",*pp,*pp);
			
			pp = myhost->h_addr_list;
			while(*pp!=NULL)
			{
				addr.s_addr = *((unsigned int *)*pp);
				//printf("address is %s\n",inet_ntoa(addr));
				sprintf(buf,"%s",inet_ntoa(addr));
				pp++;
			}
			
			break;
		}
	}

	if( is_url == 0 )
	{
		strcpy(buf,pServerIp);

		//printf("is not url ,set ip buf %s\n",buf);
	}
}


int  do_network_ntp_process()
{       
	int sockfd = 0;
	struct sockaddr_in addr_src,addr_dst;
	fd_set fds_read;
	int ret;
	int receivebytes;
	unsigned int buf[12];
	char ip_buf[200];
	int addr_len;	
       struct timeval mytime;	   
	struct timeval timeout,new_time;
	struct ntptime udp_arrival_local_timestamp;	
	struct hostent* host; 

	addr_len = sizeof(struct sockaddr_in);	
	


	DPRINTK("ntp %s:%d\n",cur_timezone.ntp_server_url,cur_timezone.ntp_server_port);

	memset(&addr_src, 0, addr_len);
	addr_src.sin_family = AF_INET;
	addr_src.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_src.sin_port = htons(0);

	memset(&addr_dst, 0, addr_len);
	addr_dst.sin_family = AF_INET;

	memset(ip_buf,0,200);
	ntp_get_host_name(ntp_server_url,ip_buf);//smtp.163.com
	if( ip_buf[0] == 0 )
	{			
		 printf("\n==================== NTP FAILED  ...................\n");
	     	goto err;
	}

/*
	host = gethostbyname(ntp_server_url);	
	if(host == NULL)
	{
             printf("\n==================== NTP FAILED  ...................\n");
	     goto err;
	}
	memcpy(&(addr_dst.sin_addr.s_addr), host->h_addr_list[0], 4);
	*/

	addr_dst.sin_addr.s_addr = inet_addr(ip_buf);
	addr_dst.sin_port = htons(ntp_port);

	 //printf("NTP CHRCK  A \n");
	
	if(-1==(sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)))
	{		
		printf("create socket error!\n");
		  goto err;
	}
    
       // printf("NTP CHRCK  B\n");


	mytime.tv_sec = 15;
	mytime.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &mytime, sizeof(mytime)) < 0)
	{
		printf("setsockopt error!\n");
		goto err;
	}
	 
	
	ret = bind(sockfd, (struct sockaddr*)&addr_src, addr_len);
	if(-1==ret)	
	{		
		printf("bind error!\n");		
		goto err;
	}

    	//printf("NTP CHRCK  C\n");	
    
		ret = connect(sockfd, (struct sockaddr*)&addr_dst, addr_len);
		if(-1==ret)	
		{		
			printf("connect error!\n");		
			
	              goto err;
		}
	
		FD_ZERO(&fds_read);
		FD_SET(sockfd, &fds_read);

		timeout.tv_sec = 6;
		timeout.tv_usec = 0;
		ret = select(sockfd + 1, &fds_read, NULL, NULL, &timeout);
		if (0 == ret || !FD_ISSET(sockfd, &fds_read))
		{
			
			//send_packet(sockfd);			
		}

		send_packet(sockfd);	
    
		
		//receivebytes = recv(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr_dst, (socklen_t *)&addr_len);
		receivebytes = recv(sockfd, buf, sizeof(buf), 0);
		if(-1==receivebytes)		
		{			
			printf("recvfrom error\n");			
			goto err;	
		}
              
		
		get_udp_arrival_local_timestamp(&udp_arrival_local_timestamp);

            
		get_new_time(buf, &new_time);	
        
		
		//set_local_time(new_time);
		ntp_new_time = new_time;

		DPRINTK("local time=>%s\n",ctime(&(new_time)));

		if (sockfd > 0)
		{
	      		close(sockfd);
			sockfd = -1;
		}
	
		DPRINTK("end close socket local time=>%s\n",ctime(&(new_time)));
	return 1;
err:

	if( sockfd > 0 )
		close(sockfd);
	
	return -1;
}


int network_ntp(void)
{ 
   	return do_network_ntp_process();   
}

static int ntp_refress = 0;

int  ntp_set_parameters(NTP_TIMEZONE_ST timezone)
{
	pthread_mutex_lock(&ntp_mutex);

	cur_timezone = timezone;

	ntp_refress = 1;

	pthread_mutex_unlock(&ntp_mutex);	
	return 1;
}


void ntp_work_thread(void * value)
{
	int time_count = 0;
	int ret = 0;

	
	while(1)
	{
		if( cur_timezone.use_ntp == 0 )
		{
			usleep(10000);
			continue;
		}

		pthread_mutex_lock(&ntp_mutex);
		if( ntp_refress == 1 )
		{
			ntp_refress = 0;
			time_count = cur_timezone.refress_time_sec;
		}
		pthread_mutex_unlock(&ntp_mutex);

		time_count++;

		if( time_count >= cur_timezone.refress_time_sec )
		{
			ret = network_ntp();
			DPRINTK("dddddtestsssssss  __start__  dddddtestsssssss\n");
			if( ret >= 0 )
			{
				ntp_new_time.tv_sec  += cur_timezone.time_zone_sec;

				if( get_time_callback )
				{
					//DPRINTK("get_time_callback sec=%ld\n",ntp_new_time.tv_sec );
					//DPRINTK("timezone time=>%s\n",ctime(&(ntp_new_time.tv_sec)));
					get_time_callback(ntp_new_time.tv_sec );
				}

				time_count = 0;
			}else
			{
				sleep(5);
			}
			
		}

		sleep(1);
	}
	return ;
}


int start_ntp_server(ntp_callback  callback)
{
	static int first = 1;
	int ret;
	pthread_t ntp_th;

	if( first != 1 )
		return 1;

	first = 0;

	get_time_callback = callback;

	pthread_mutex_init(&ntp_mutex,NULL);
	
	ret = pthread_create(&ntp_th,NULL,(void*)ntp_work_thread,NULL);
	if ( 0 != ret )
	{
		DPRINTK( "create ntp_work_thread error\n");
		goto err;
	}

	return 0;
err:
	return -1;
}



