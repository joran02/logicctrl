#include "stdio.h"
#include "string.h"
#include "cmn.h"
#include "AnalyseWifi.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WIFI_SCAN_FILE "/tmp/scan.txt"

static int FindSTR( char  * byhtml, char *cfind, int nStartPos)
{
	int i;
	long length;
	char * tmphtml;
	char * temfind ;

	
	if( byhtml == NULL || cfind == NULL)
		return -1;

	tmphtml =(char *) byhtml + nStartPos;

	temfind = cfind;

	if(nStartPos < 0)
		return -1;

	length = strlen(tmphtml); 
	
	for( i = 0 ; i < length; i++)
	{		

		if( *tmphtml != *temfind )
		{
			tmphtml++;

			continue;
		}

		while( *tmphtml == *temfind )
		{
			//printf(" %c.%c|",*tmphtml,*temfind);
			tmphtml++;
			temfind++;

			if( *temfind == (char)NULL ) // 找到了。
			{			
				return nStartPos + i;
			}
		}

		//printf("\n");	
		
		if( *temfind == (char)NULL ) // 找到了。
		{			
			return nStartPos + i;
		}
		else
		{	// 从与temfind首字节相同的那一位的后一位重新开始找，
			temfind = cfind;
			tmphtml = (char *)byhtml + nStartPos + i + 1;
		}
	}

	return -1;
}

// start 是指 '<' 所在的位置，end 指 '>'的位置，此函数取两个位置之间的那部分字符串。
static  int GetTmpLink(char * byhtml, char * temp, int start, int end,int max_num)
{
	int i;
	
	if(  byhtml == NULL ||  temp == NULL)
		return -1;

	if( end - start > max_num )
	{		
		temp[0] = (char)NULL;
		return -1;
	}	

	for(i = start + 1; i < end ; i++)
	{
		*temp = byhtml[i];

		temp++;
	}

	*temp = (char)NULL; // 结束符。

	return 1;
}

char * AnalyseData(int * wifi_num)
{
	WifiInfo *stWifi;
	int i  = 0;
	FILE *fp=NULL;
	char * fileBuf = NULL;
	int fileOffset;
	char buf[MN_MAX_BUF_SIZE];
	//char buf[1024];
	int ret;
	int num_sell = 0;

	fp = fopen(WIFI_SCAN_FILE,"rb");
	if( fp == NULL )
	{
		DBGPRINT("open file error!");
		return NULL;
	}

	ret = fseek(fp,0L,SEEK_END);
	if( ret != 0 )
	{
		DBGPRINT("fseek error!");
		goto FILESENDERROR;
	}

	fileOffset = ftell(fp);
	if( fileOffset == -1 )
	{
		DBGPRINT("ftell error!");
		goto FILESENDERROR;
	}

	if( fileOffset >  0 )
	{
		rewind(fp);

		fileBuf = (char *)malloc(fileOffset);
		if( fileBuf == NULL )
		{
			goto FILESENDERROR;
		}

		ret = fread(fileBuf,1,fileOffset,fp);
		if( ret != fileOffset )
		{
			DBGPRINT("fread error!");
			goto FILESENDERROR;
		}

	}

	fclose(fp);
	fp = NULL;

	int find_pos = 0;
	while(1)
	{
		find_pos = FindSTR(fileBuf,"Cell",find_pos);
		if( find_pos > 0 )
		{
			num_sell++;
			find_pos = find_pos + 1;
		}else
		{
			break;
		}
	}

	if( num_sell == 0 )
		return NULL;

	stWifi = (WifiInfo*)malloc(sizeof(WifiInfo)*num_sell);

	int pos1 = 0,pos2 = 0;
	int find1 = 0, find2 = 0;
	char tmp1[5000];

	for(i = 0;i < num_sell;i++)
	{
		pos1 = FindSTR(fileBuf,"Cell",pos1);
		pos2 = FindSTR(fileBuf,"Cell",pos1+1);

		if( pos2 < 0 )				
			pos2 = fileOffset;		

		if( pos2 > 0 )
		{		

			ret = GetTmpLink(fileBuf,buf,pos1,pos2,5000);
			if( ret < 0)			
				goto FILESENDERROR;

			find1 = FindSTR(buf,"Protocol:",0);
			find2 = FindSTR(buf,"\x0A",find1 +1);

			ret = GetTmpLink(buf,tmp1,find1+strlen("Protocol:")-1,find2,5000);
			if( ret < 0)			
				goto FILESENDERROR;

			if( strcmp(tmp1,"802.11b/g/n") == 0 )
			{
				strcpy(stWifi[i].WirelessMode, "9");
			}else if( strcmp(tmp1,"802.11b/g") == 0 )
			{
				strcpy(stWifi[i].WirelessMode, "0");
			}else if( strcmp(tmp1,"802.11b") == 0 )
			{
				strcpy(stWifi[i].WirelessMode, "1");
			}else if( strcmp(tmp1,"802.11g") == 0 )
			{
				strcpy(stWifi[i].WirelessMode, "2");
			}else 
			{
				strcpy(stWifi[i].WirelessMode, "9");
			}



			find1 = FindSTR(buf,"ESSID:\"",0);
			find2 = FindSTR(buf,"\"\x0A",find1 +1);

			ret = GetTmpLink(buf,tmp1,find1+strlen("ESSID:\"") -1,find2,5000);
			if( ret < 0)			
				goto FILESENDERROR;			

			strcpy(stWifi[i].Essid, tmp1);


			find1 = FindSTR(buf,"(Channel ",0);
			find2 = FindSTR(buf,")\x0A",find1 +1);

			ret = GetTmpLink(buf,tmp1,find1+strlen("(Channel ") -1,find2,5000);
			if( ret < 0)			
				goto FILESENDERROR;

			strcpy(stWifi[i].Channel, tmp1);


			find1 = FindSTR(buf,"Quality:",0);
			if( find1 < 0 )
				find1 = FindSTR(buf,"Quality=",0);

			find2 = FindSTR(buf,"/",find1 +1);

			ret = GetTmpLink(buf,tmp1,find1+strlen("Quality:") -1,find2,5000);
			if( ret < 0)			
				goto FILESENDERROR;

			if( atoi(tmp1) == 0 )
			{
				find1 = FindSTR(buf,"level=",0);				

				find2 = FindSTR(buf,"/",find1 +1);

				ret = GetTmpLink(buf,tmp1,find1+strlen("level=") -1,find2,5000);
				if( ret < 0)			
					goto FILESENDERROR;
			}

			strcpy(stWifi[i].Quality, tmp1);

			//检查OPEN模式
			find1 = FindSTR(buf,"Encryption key:off",0);
			if( find1> 0)
			{				
				strcpy(stWifi[i].AuthMode, "OPEN");
				strcpy(stWifi[i].EncrypType, "NONE");

				goto check_end;
			}


			find1 = FindSTR(buf,"IE:",0);
			if( find1< 0)
			{	
				strcpy(stWifi[i].AuthMode, "SHARED");
				strcpy(stWifi[i].EncrypType, "WEP");

				goto check_end;
			}

			if( (FindSTR(buf,"WPA2 Version 1",0) > 0) &&  (FindSTR(buf,"TKIP",0) > 0)  )
			{
				strcpy(stWifi[i].AuthMode, "WPA2");
				strcpy(stWifi[i].EncrypType, "TKIP");


				//if( FindSTR(buf,"Preauthentication Supported",0) > 0 )
				//	stWifi[i].AuthMode.Format(_T("WPA2PSK");				

				goto check_end;
			}		

			if( (FindSTR(buf,"WPA2 Version 1",0) > 0)  &&  (FindSTR(buf,"CCMP",0) > 0)  )
			{
				strcpy(stWifi[i].AuthMode, "WPA2");
				strcpy(stWifi[i].EncrypType, "AES");

				//if( FindSTR(buf,"Preauthentication Supported",0) > 0 )
				//	stWifi[i].AuthMode.Format(_T("WPA2PSK");		

				goto check_end;
			}

			if( (FindSTR(buf,"WPA Version 1",0) > 0) &&  (FindSTR(buf,"TKIP",0) > 0)  )
			{
				strcpy(stWifi[i].AuthMode, "WPA");
				strcpy(stWifi[i].EncrypType, "TKIP");

				//if( FindSTR(buf,"Preauthentication Supported",0) > 0 )
				//	stWifi[i].AuthMode.Format(_T("WPA2PSK");		

				goto check_end;
			}		

			if( (FindSTR(buf,"WPA Version 1",0) > 0)  &&  (FindSTR(buf,"CCMP",0) > 0)  )
			{
				strcpy(stWifi[i].AuthMode, "WPA");
				strcpy(stWifi[i].EncrypType, "AES");

				//if( FindSTR(buf,"Preauthentication Supported",0) > 0 )
				//	stWifi[i].AuthMode.Format(_T("WPA2PSK");		

				goto check_end;
			}		

check_end:
			pos1 = pos2;


			DBGPRINT("%s %s %s %s %s %s\n",stWifi[i].Essid,stWifi[i].WirelessMode,stWifi[i].Channel,
				stWifi[i].Quality,stWifi[i].AuthMode,stWifi[i].EncrypType);
		}
	}

	*wifi_num = num_sell;

	return (char*)stWifi;

FILESENDERROR:
	return NULL;
}

int SortScanData(SORT_LIST *stList)
{
	if (stList == NULL)
		return -1;
	int i = 0, j = 0, ret = 0;
	WifiInfo *stWifi = NULL;
	//SORT_LIST stList;
	int tVal = 0;
	char tSsid[32]={0};
	int wifi_num = 0;

	memset(stList, 0, sizeof(SORT_LIST));
	stWifi = (WifiInfo *)AnalyseData(&wifi_num);

	if (stWifi == NULL)
	{
		ret = -1;
		goto err;
	}

	for (i = 0; i < wifi_num; i++)
	{
		if (strlen(stWifi[i].Quality) > 0)
		{
			tVal = atoi(stWifi[i].Quality);
			if (tVal < 0 && tVal > 100)
				continue;
			else
			{
				strcpy(stList->member[stList->num].name, stWifi[i].Essid);
				stList->member[stList->num].val = tVal;
				stList->num++;
			}
		}
	}

	for (i = 0; i < stList->num-1; i++)
	{
		for (j = i+1; j < stList->num; j++)
		{
			if (stList->member[i].val < stList->member[j].val)
			{
				tVal = stList->member[i].val;
				stList->member[i].val = stList->member[j].val;
				stList->member[j].val = tVal;
				strcpy(tSsid, stList->member[i].name);
				strcpy(stList->member[i].name, stList->member[j].name);
				strcpy(stList->member[j].name, tSsid);
			}
		}
	}

	DBGPRINT("\n");
err:
	if( stWifi )
	{
		free(stWifi);
		stWifi = NULL;
	}
	return ret;
}

#ifdef __cplusplus
}
#endif
