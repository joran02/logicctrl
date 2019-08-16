
#include "cmn.h"
#include <dirent.h>
#include <pthread.h>
#include <net/if.h>
#include "ntp_timezone.h"
#include "Func_dst.h"

#include "IpLogic.h"
#include "cJSON.h"

#ifdef HISI_DEV 
int rgn_osd_info_w[RGN_NUM_MAX],rgn_osd_info_h[RGN_NUM_MAX];
char osd_info_buf[RGN_NUM_MAX][24*24*OSD_STR_LEN_MAX*2];
int osd_info_buf_w[RGN_NUM_MAX],osd_info_buf_h[RGN_NUM_MAX];
VI_CSC_ATTR_S stCscAttr;
RGN_HANDLE RgnHandle[RGN_NUM_MAX];
MPP_CHN_S stChn[RGN_NUM_MAX];
BITMAP_S unParamtime;
BITMAP_S unParamFace;
BITMAP_S unParamname;
BITMAP_S info_osd_str;
char iw_net_info_str[40];
IMAGE_DATA s_TimeNum[12],s_CamName[40];
IMAGE_DATA s_CharEn,s_CharEn_1;
unsigned char iw_info_osd_buf[24*24*60*2];
int iw_info_osd_buf_w,iw_info_osd_buf_h;
int g_wifi_link_flag_ex=0;
int g_is_wifi_exist = 0;//judge wifi exist or not
#endif
//========================================================
U16 byKey_val;
U16 cam_loss,loss_old;
UC menu_num,cur_pos,demo_num,demo_page,demo_num_old,demo_page_old;
UC buf_time[6];
UC pic_num;
UC buf_cam[CHANNELNUM];
U16 *scale_ptr;
U16 *picpos_ptr;
U16 *osd_ptr[5];
U16 *hdosd_ptr[5];
UC bReset;
UC bsecond_change,bminute_change,bhour_change,bday_change,bmouth_change,byear_change;
U16 cam_sensor,net_sensor;
U16 cam_motion,net_motion;
UC auto_flag;
int iw_min_cnt=0;
char iw_essid[17];
int iw_signalval=5;//offline
int wifi_use_flag=0;
int isNormalWifi = 0;
int i_is2_4G = 0;
//int isNeedScan = 0;
//int connectnocast_count = 0;
//int noconect_count = 0;
int ipcap_count = 0;

//concurrent mode
WIFIAP stwifiap_store[65];
int g_Cur_Ap_Signalch = -1;//NVR传值

//每个通道应该使用的信道号
int chanChannel[128]=
{1,  132,
2,   40,
3,   44,
4,   52,
5,   56,
6,   64,
7,   104,
8,   108,
9,   36,
10,  112,
11,  120,
12,  124,
13,  128,
14,  136,
15,  36,//140,
16,  36,//149,
17,  100,
18,  153,
19,  157,
20,  161,
21,  40,
22,  44,
23,  52,
24,  56,
25,  60,
26,  64,
27,  104,
28,  108,
29,  112,
30,  120,
31,  124,
32,  128,
33,  48,
34,  136,
35,  36,//140,
36,  36,//149,
37,  153,
38,  157,
39,  161,
40,  40,
41,  116,
42,  44,
43,  52,
44,  56,
45,  64,
46,  104,
47,  108,
48,  112,
49,  52,
50,  120,
51,  124,
52,  128,
53,  136,
54,  36,//140,
55,  36,//149,
56,  153,
57,  165,
58,  157,
59,  161,
60,  40,
61,  44,
62,  52,
63,  56,
64,  64};

UC gAlarm=0;
U8 DEMO_DISP;
int High_Resolution;
UC gtstop=0; 
//WordType SysWordType;
OSDBLANK Osdptr;
OSDSTATUS osdstatuspic;
U8 Cam_Num;
U8 ChipNum;
int WIDTH_RESOLUTION_HD;
int HIGH_RESOLUTION_HD;
int POSXRMENU_HD;
U8 HDOSDOFFSETX,HDOSDOFFSETY;

//sen_reg_t reg_info;
U8 mailsend = 0;
U8 mailevent=0;
U16 mailcam = 0;

//U8 tw2835_flag;
/////////////////////////////////////
extern pthread_mutex_t eprom_file_edit_lock;
extern int eprom_file_edit_flag;
extern int gm8126id;
extern int old_posx,pointer[4];
//=========================================================
GST_DEV_FILE_PARAM *gstCommonParam;

GST_DISKINFO * pstDiskInfo[10];


KEYBOARD_LED_INFO* net_info_ptr;

static pthread_t g_devctrlThreadpro = -1;

static pthread_t g_ThreadOSD = -1;
static pthread_t g_camwiredwirelesspro = -1;
static pthread_t g_camrecvbroadcastpro = -1;
static pthread_t g_cam2_4GWiredOrWirelesspro = -1;
//static pthread_t g_camcountpro = -1;
U8 threadcreat = 0;
//int wdgflag = 0;
////////////////

////////////////////////////////////////json/////////////////////////////////
void saveJsonTextToFile(char *buf, char *filePath, int bufLength)
{
	FILE *fp = NULL;
	int rel = 0;
	fp=fopen(filePath,"w+");
	if(fp)
	{
		printf("open ok\n");
		rel = fwrite(buf, 1, bufLength, fp);
		if( rel != bufLength )
		{
			printf(" fwrite  Error!=%d \n",rel);
		}	
		fclose(fp);
	}else
	{
		printf("open error\n");
	}
	
	printf("file:%s over\n-----%d\n", filePath, bufLength);
}

cJSON *getJsonFromFile(char *filePath)
{
	cJSON *root;
	char *out = NULL;
	int fileOffset = 0, rel = 0;

	FILE *fp = NULL;
	fp=fopen(filePath,"rb");
	if(fp)
	{
		//printf("open %s ok\n", filePath);
		rel = fseek(fp,0L,SEEK_END);
		if( rel != 0 )
		{
			printf("fseek error!!\n");
			goto GET_ERROR;
		}

		fileOffset = ftell(fp);
		if( fileOffset == -1 )
		{
			printf(" ftell error!\n");
			goto GET_ERROR;
		}

		//printf(" fileOffset = %ld\n",fileOffset);

		rewind(fp);	

		out = (char*)malloc(fileOffset);
		if( out == NULL)
		{
			printf(" malloc Error!\n");
			goto GET_ERROR;
		}

		rel = fread(out,1,fileOffset,fp);
		if( rel != fileOffset )
		{
			printf(" fread Error!\n");
			goto GET_ERROR;
		}	
		fclose(fp);
		fp = NULL;
	}else
	{
		printf("open error\n");
		goto GET_ERROR;
	}
	
GET_ERROR:

	root = cJSON_Parse(out);
	//out = cJSON_Print(root);
	//fileOffset = strlen(out);
	//printf("parse - get length:%d\n", fileOffset);
	
	if ( fp != NULL )
		fclose(fp);

	if( out != NULL )
	{
		free(out);
		out = NULL;
	}

	return root;
}

void saveJsonToFile(cJSON *root, char *filePath, int needMinify)
{
	char *out;
	out = cJSON_Print(root);
	int fileOffset = strlen(out);
	printf("get length:%d\n", fileOffset);
	if (needMinify == 1)
	{
		cJSON_Minify(out);
		fileOffset = strlen(out);
		printf("minify - get length:%d\n", fileOffset);
	}
	/*cJSON *newRoot;
	newRoot = cJSON_Parse(out);
	out = cJSON_Print(newRoot);
	fileOffset = strlen(out);
	printf("parse - get length:%d\n", fileOffset);*/
	saveJsonTextToFile(out, filePath, fileOffset);
	free(out);
}

void Sta_save_user_set_ssid_info()
{
	int iNum = 0;
	cJSON *root;
	root = cJSON_CreateObject();
	char ssidname[10] = {0};
	char pwdname[10] = {0};
	char chnname[10] = {0};

	for (iNum = 0; iNum < 6; iNum++)
	{
		sprintf(ssidname, "ssid%d", iNum);
		sprintf(pwdname, "pwd%d", iNum);
		sprintf(chnname, "chn%d", iNum);
		if (iNum == 0)
		{
			cJSON_AddItemToObject(root, ssidname, cJSON_CreateString(ipcam_AP.ssid));
			cJSON_AddItemToObject(root, pwdname, cJSON_CreateString(ipcam_AP.password));
			cJSON_AddItemToObject(root, chnname, cJSON_CreateNumber(ipcam_AP.channel));
		}else
		{
			cJSON_AddItemToObject(root, ssidname, cJSON_CreateString(netcam_AP[iNum-1].ssid));
			cJSON_AddItemToObject(root, pwdname, cJSON_CreateString(netcam_AP[iNum-1].password));
			cJSON_AddItemToObject(root, chnname, cJSON_CreateNumber(netcam_AP[iNum-1].channel));
		}
	}

	saveJsonToFile(root, "SSIDsta.json", 0);
	cJSON_Delete(root);
}

void Sta_load_user_set_ssid_info()
{
	int iNum = 0;
	cJSON *root, *rootVal;
	char *out;
//	root = cJSON_CreateObject();
	char ssidname[10] = {0};
	char pwdname[10] = {0};
	char chnname[10] = {0};
	int iCount = 0;

retry:
	root = getJsonFromFile("SSIDsta.json");
	if (root == NULL)
	{
		if (iCount < 1)
		{
			iCount++;
			printf("rebuild ssid file\n");
			Sta_save_user_set_ssid_info();
			goto retry;
		}else
			return;
	}

	for (iNum = 0; iNum < 6; iNum++)
	{
		sprintf(ssidname, "ssid%d", iNum);
		sprintf(pwdname, "pwd%d", iNum);
		sprintf(chnname, "chn%d", iNum);
		rootVal = cJSON_GetObjectItem(root, ssidname);
		
		if (iNum == 0)
		{
			if (rootVal == NULL)
			{
				strcpy(ipcam_AP.ssid, "");
			}else
			{
				out = cJSON_Print(rootVal);
				out[strlen(out)-1]='\0';
				memcpy(ipcam_AP.ssid, &out[1], strlen(out));
				free(out);
			}
		}else
		{
			if (rootVal == NULL)
			{
				strcpy(netcam_AP[iNum-1].ssid, "");
			}else
			{
				out = cJSON_Print(rootVal);
				out[strlen(out)-1]='\0';
				memcpy(netcam_AP[iNum-1].ssid, &out[1], strlen(out));
				free(out);
			}
		}

		rootVal = cJSON_GetObjectItem(root, pwdname);
		if (iNum == 0)
		{
			if (rootVal == NULL)
			{
				strcpy(ipcam_AP.password, "");
			}else
			{
				out = cJSON_Print(rootVal);
				out[strlen(out)-1]='\0';
				memcpy(ipcam_AP.password, &out[1], strlen(out));
				free(out);
			}
		}else
		{
			if (rootVal == NULL)
			{
				strcpy(netcam_AP[iNum-1].password, "");
			}else
			{
				out = cJSON_Print(rootVal);
				out[strlen(out)-1]='\0';
				memcpy(netcam_AP[iNum-1].password, &out[1], strlen(out));
				free(out);
			}
		}

		rootVal = cJSON_GetObjectItem(root, chnname);
		if (iNum == 0)
		{
			if (rootVal == NULL)
			{
				ipcam_AP.channel = 0;
			}else
			{
				out = cJSON_Print(rootVal);
				ipcam_AP.channel = atoi(out);
				free(out);
			}
		}else
		{
			if (rootVal == NULL)
			{
				netcam_AP[iNum-1].channel = 0;
			}else
			{
				out = cJSON_Print(rootVal);
				netcam_AP[iNum-1].channel = atoi(out);
				free(out);
			}
		}

#if 0
		if (iNum == 0)
			printf("ipcap:%s----%s_%d__\n", ipcam_AP.ssid, ipcam_AP.password, ipcam_AP.channel);
		else
			printf("nvrap:%s----%s_%d__\n", netcam_AP[iNum-1].ssid, netcam_AP[iNum-1].password, netcam_AP[iNum-1].channel);
#endif
	}
	cJSON_Delete(root);
}

//save nvr broadcast id msg.
void Sta_save_nvr_bc_ssid_info(WIFIAP *pNvrBcSt, char *bcNVRID)
{
	int iNum = 0;
	char ssidname[10] = {0};
	char pwdname[10] = {0};
	if (pNvrBcSt == NULL)
		return;
	cJSON *root;
	root = cJSON_CreateObject();

	if (bcNVRID != NULL)
	{
		sprintf(ssidname, "nvrssid");
		cJSON_AddItemToObject(root, ssidname, cJSON_CreateString(bcNVRID));
	}

	for (iNum = 0; iNum < 32; iNum++)
	{
		{
			sprintf(ssidname, "bcssid%d", iNum);
			cJSON_AddItemToObject(root, ssidname, cJSON_CreateString(pNvrBcSt[iNum].channelid));
		}
		
		{
			sprintf(pwdname, "bcapssid%d", iNum);
			cJSON_AddItemToObject(root, pwdname, cJSON_CreateString(pNvrBcSt[iNum].wifiapssid));
		}
	}

	saveJsonToFile(root, "BCSSIDsta.json", 0);
	cJSON_Delete(root);
}

void Sta_load_nvr_bc_ssid_info(WIFIAP *pNvrBcSt, char * bcNVRID)
{
	int iNum = 0;
	cJSON *root, *rootVal;
	char *out;
//	root = cJSON_CreateObject();
	char ssidname[10] = {0};
	char pwdname[10] = {0};
	int iCount = 0;

retry:
	root = getJsonFromFile("BCSSIDsta.json");
	if (root == NULL)
	{
		if (iCount < 1)
		{
			iCount++;
			printf("rebuild ssid file\n");
			Sta_save_nvr_bc_ssid_info(pNvrBcSt, NULL);
			goto retry;
		}else
			return;
	}

	if (bcNVRID != NULL)
	{
		sprintf(ssidname, "nvrssid");
		rootVal = cJSON_GetObjectItem(root, ssidname);
		
		if (rootVal == NULL)
		{
			strcpy(bcNVRID, "");
		}else
		{
			out = cJSON_Print(rootVal);
			out[strlen(out)-1]='\0';
			memcpy(bcNVRID, &out[1], strlen(out));
			free(out);
		}
	}

	if (pNvrBcSt != NULL)
	{
		for (iNum = 0; iNum < 32; iNum++)
		{
			sprintf(ssidname, "bcssid%d", iNum);
			sprintf(pwdname, "bcapssid%d", iNum);
			rootVal = cJSON_GetObjectItem(root, ssidname);
			
			if (rootVal == NULL)
			{
				strcpy(pNvrBcSt[iNum].channelid, "");
			}else
			{
				out = cJSON_Print(rootVal);
				out[strlen(out)-1]='\0';
				memcpy(pNvrBcSt[iNum].channelid, &out[1], strlen(out));
				free(out);
			}

			rootVal = cJSON_GetObjectItem(root, pwdname);
			
			if (rootVal == NULL)
			{
				strcpy(pNvrBcSt[iNum].wifiapssid, "");
			}else
			{
				out = cJSON_Print(rootVal);
				out[strlen(out)-1]='\0';
				memcpy(pNvrBcSt[iNum].wifiapssid, &out[1], strlen(out));
				free(out);
			}
			//if (strlen(pNvrBcSt[iNum].channelid) > 0 || strlen(pNvrBcSt[iNum].wifiapssid) > 0)
				//printf("ipcap:%s----%s___\n", pNvrBcSt[iNum].channelid, pNvrBcSt[iNum].wifiapssid);
		}
	}
	cJSON_Delete(root);
}

void AP_save_ssid_info(char *ssid, char *pwd, int chn)
{
	int iNum = 0;
	cJSON *root;
	root = cJSON_CreateObject();

	cJSON_AddItemToObject(root, "apssid", cJSON_CreateString(ssid));

	cJSON_AddItemToObject(root, "appwd", cJSON_CreateString(pwd));

	cJSON_AddItemToObject(root, "chn", cJSON_CreateNumber(chn));

	saveJsonToFile(root, "APSSIDsta.json", 0);
	cJSON_Delete(root);
}

void AP_load_ssid_info(char *ssid, char *pwd, int *chn)
{
	int iNum = 0;
	cJSON *root, *rootVal;
	char *out;
//	root = cJSON_CreateObject();
	int iCount = 0;

retry:
	root = getJsonFromFile("APSSIDsta.json");
	if (root == NULL)
	{
		if (iCount < 1)
		{
			iCount++;
			printf("rebuild APssid file\n");
			AP_save_ssid_info("", "", 0);
			goto retry;
		}else
			return;
	}

	rootVal = cJSON_GetObjectItem(root, "apssid");
	
	if (rootVal == NULL)
	{
		strcpy(ssid, "");
	}else
	{
		out = cJSON_Print(rootVal);
		out[strlen(out)-1]='\0';
		memcpy(ssid, &out[1], strlen(out));
		free(out);
	}

	rootVal = cJSON_GetObjectItem(root, "appwd");
	
	if (rootVal == NULL)
	{
		strcpy(pwd, "");
	}else
	{
		out = cJSON_Print(rootVal);
		out[strlen(out)-1]='\0';
		memcpy(pwd, &out[1], strlen(out));
		free(out);
	}

	rootVal = cJSON_GetObjectItem(root, "chn");
	
	if (rootVal == NULL)
	{
		chn = 0;
	}else
	{
		out = cJSON_Print(rootVal);
		chn = atoi(out);free(out);
	}

	printf("apssid_pwd:%s----%s__chn:%d__\n", ssid, pwd, chn);
	cJSON_Delete(root);
}
////////////////////////////////////////json/////////////////////////////////

/*
int set_current_play_mode(int iPBStandard, int iPBSize)
{
	U8 tmp;
	I_read_block_eeprom(ADDSYS+VIDEOFORMAT,1,&tmp);
	tmp ^=1;
	iPBStandard = (int)tmp;
	iPBSize++;
	printf("Video Standard get succss!iPBStandard = %x\n",iPBStandard);
	return iPBStandard;
}
*/
void InitAllDevice();

/*
void picture_load(U8** ptr,char*str)
{
	char str1[100];
	memset(str1,0,sizeof(str1));
	sprintf(str1,"/mnt/mtd/%s",str);
	Load_Menu_Pic(ptr,str1);
}
*/

void iw_info_display()
{
  HI_S32 s32Ret = HI_FAILURE;
  RGN_ATTR_S stRgnAttr;
  MPP_CHN_S stChn;
  RGN_CHN_ATTR_S stChnAttr;
	static int s_w=0,s_h=0;
	
	if(s_w!=iw_info_osd_buf_w || iw_info_osd_buf_h)
	{
		s_w=iw_info_osd_buf_w;
		s_h=iw_info_osd_buf_h;
		DPRINTK("HI_MPI_RGN_Destroy [%d]\n",RgnHandle[3]);
		HI_MPI_RGN_Destroy(RgnHandle[3]);
	 		
		stRgnAttr.enType = OVERLAY_RGN;
	  stRgnAttr.unAttr.stOverlay.enPixelFmt = PIXEL_FORMAT_RGB_1555;
	  stRgnAttr.unAttr.stOverlay.stSize.u32Width  = iw_info_osd_buf_w;
	  stRgnAttr.unAttr.stOverlay.stSize.u32Height = iw_info_osd_buf_h;
	  stRgnAttr.unAttr.stOverlay.u32BgColor = 0;
	
	  s32Ret = HI_MPI_RGN_Create(RgnHandle[3], &stRgnAttr);
	  if(HI_SUCCESS != s32Ret)
	  {
	  	DBGPRINT("HI_MPI_RGN_Create (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
	  	return ;
	  }    
	  
	  stChn.enModId = HI_ID_GROUP;
	  stChn.s32DevId = 0;
	  stChn.s32ChnId = 0;
	
	  memset(&stChnAttr,0,sizeof(stChnAttr));
	  stChnAttr.bShow = HI_TRUE;
	  stChnAttr.enType = OVERLAY_RGN;
	  stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 180;
	  stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 8;
	  stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
	  stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	  stChnAttr.unChnAttr.stOverlayChn.u32Layer = 3;
	
	  stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
	  stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
	
	  s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[3], &stChn, &stChnAttr);
	  if(HI_SUCCESS != s32Ret)
	  {
	  	DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
	  	return ;
	  }
////////////////////////////////////////////	  
//	  stChn.enModId = HI_ID_GROUP;
//	  stChn.s32DevId = 1;
//	  stChn.s32ChnId = 0;
//	
//	  memset(&stChnAttr,0,sizeof(stChnAttr));
//	  stChnAttr.bShow = HI_TRUE;
//	  stChnAttr.enType = OVERLAY_RGN;
//	  stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X =  g_sub_video_w-19*8;
//	  stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 16;
//	  stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
//	  stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
//	  stChnAttr.unChnAttr.stOverlayChn.u32Layer = 3;
//	
//	  stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
//	  stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
//	
//	  s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[3], &stChn, &stChnAttr);
//	  if(HI_SUCCESS != s32Ret)
//	  {
//	  	DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
//	  	return ;
//	  }
////////////////////////////////////////////	 
	}
	info_osd_str.pData = iw_info_osd_buf;
	info_osd_str.u32Width = iw_info_osd_buf_w;
	info_osd_str.u32Height = iw_info_osd_buf_h;
	info_osd_str.enPixelFormat = PIXEL_FORMAT_RGB_1555;
  HI_MPI_RGN_SetBitMap(RgnHandle[3],&info_osd_str);
}
int iw_get_essid()
{
  FILE *fd = NULL;
  int len;
  
//		return 0;
		
  memset(iw_essid,0,sizeof(iw_essid));
	fd=fopen("/mnt/mtd/essid.txt","r");
	if(fd)
	{
		fgets(iw_essid,15,fd);
		len=strlen(iw_essid);
		iw_essid[len-1]=0;
		fclose(fd);
		return 1;
	}
	else
		return 0;
}

void iw_get_signal_level_ex()
{
  FILE *	f = fopen("/proc/net/wireless", "r");
  char	buf[256];
  char *	bp;
  char ifname[]="ra0";
  int	t;
  unsigned char tmp;

  if(f==NULL)
		return ;
  /* Loop on all devices */
  while(fgets(buf,255,f))
	{
		bp=buf;
		while(*bp&&isspace(*bp))
	  	bp++;
		/* Is it the good device ? */
		if(strncmp(bp,ifname,strlen(ifname))==0 && bp[strlen(ifname)]==':')
	  {
      /* Skip ethX: */
      bp=strchr(bp,':');
      bp++;
      /* -- status -- */
      bp = strtok(bp, " ");
      sscanf(bp, "%X", &t);
//      stats->status = (unsigned short) t;
      /* -- link quality -- */
      bp = strtok(NULL, " ");
      bp = strtok(NULL, " ");//lx 20151202
      if(strchr(bp,'.') != NULL)
				;//stats->qual.updated |= 1;
      sscanf(bp, "%d", &t);
//      stats->qual.qual = (unsigned char) t;
	    iw_signalval = (unsigned char) t;	 
			DBGPRINT("************* iw_signalval:%d \n",iw_signalval);
		  if(iw_signalval>95)
		  {
		  	if (iw_signalval <= 100 || iw_signalval >= 206)
			  	iw_signalval=4;
			else if (iw_signalval >= 196)
				iw_signalval = 3;
			else if (iw_signalval >= 171)
				iw_signalval = 2;
			else if (iw_signalval >= 157)
				iw_signalval = 1;
			else
				iw_signalval = 0;
		  }
		  else if(iw_signalval>70)
		  	iw_signalval=3;
		  else if(iw_signalval>40)
		  	iw_signalval=2;
		  else if(iw_signalval)
		  	iw_signalval=1;
		  else
		  	iw_signalval=0;
		  	
//      /* -- signal level -- */
//      bp = strtok(NULL, " ");
//      if(strchr(bp,'.') != NULL)
//				;//stats->qual.updated |= 2;
//      sscanf(bp, "%d", &t);
//	    iw_signalval = (unsigned char) t;	    
//		  if(iw_signalval >= 64)
//		    iw_signalval -= 0x100;
	    fclose(f);
	    /* No conversion needed */
	    return 0;
	  }
	}
	fclose(f);
	return -1;
}
static int iw_FindSTR( char  * byhtml, char *cfind, int nStartPos)
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
static  int iw_GetTmpLink(char * byhtml, char * temp, int start, int end,int max_num)
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
void iw_get_signal_level()
{
 	/*FILE *	fp = NULL;
	int find1 = 0, find2 = 0,find3 = 0,ret;
	char tmp1[5000],buf[1024*100];
	long fileOffset = 0;*/
	
	iw_signalval=5;//init offline
	IpLogic_Get_Rj45_Conn_Status(&g_wifi_link_flag_ex);
	if(g_wifi_link_flag_ex)
	{
		iw_get_signal_level_ex();
		return ;
	}
	/*
	memset(buf,0,sizeof(buf));
	memset(tmp1,0,sizeof(tmp1));
	if(isNormalWifi)
	{
		//sys_command("rm -f /ramdisk/iw_info.txt");
		//sys_command("iwlist ra0 scanning > /ramdisk/iw_info.txt");
	}
	
	fp = fopen("/ramdisk/iw_info.txt","rb");
	if( fp == NULL )
	{
		printf(" open test.txt error!\n");
		goto IW_GET_ERROR;
	}
	ret = fseek(fp,0L,SEEK_END);
	if( ret != 0 )
	{
		printf("fseek error!!\n");
		goto IW_GET_ERROR;
	}
	fileOffset = ftell(fp);
	if( fileOffset == -1 )
	{
		printf(" ftell error!\n");
		goto IW_GET_ERROR;
	}
	printf(" fileOffset = %ld\n",fileOffset);
	rewind(fp);	
	ret = fread(buf,1,fileOffset,fp);
	if( ret != fileOffset )
	{
		printf(" fread Error!=n");
		goto IW_GET_ERROR;
	}
	
	find1 = iw_FindSTR(buf,iw_essid,0);
	if( find1 < 0 )
		goto IW_GET_ERROR;
	find3 = iw_FindSTR(buf,"Quality:",find1);
	if( find3 < 0 )
		find3 = iw_FindSTR(buf,"Quality=",find1);	
	find2 = iw_FindSTR(buf,"/",find3 +1);
	ret = iw_GetTmpLink(buf,tmp1,find3+strlen("Quality:") -1,find2,5000);
	if(ret<0)
		goto IW_GET_ERROR;
	iw_signalval = atoi(tmp1);
	DBGPRINT("************* iw_signalval:%d \n",iw_signalval);
  if(iw_signalval>95)
  	iw_signalval=4;
  else if(iw_signalval>70)
  	iw_signalval=3;
  else if(iw_signalval>40)
  	iw_signalval=2;
  else if(iw_signalval)
  	iw_signalval=1;
  else
  	iw_signalval=0;
  	
  	if(g_wifi_link_flag_ex == 0)
		iw_signalval=5;
  	
IW_GET_ERROR:
	
	fclose(fp);*/
	
	return ;
}

void iw_get_netcam_ssid(char *filename, char *ssid, char *pwd)
{
 	FILE *	fp = NULL;
	int find1 = 0, find2 = 0,find3 = 0,ret;
	char tmp1[5000],buf[1024*100];
	char tmpSsid[32] = {0}, tmpPwd[32] = {0};
	long fileOffset = 0;

	memset(buf,0,sizeof(buf));
	memset(tmp1,0,sizeof(tmp1));

	strcpy(ssid, "");
	strcpy(pwd, "");
	
	fp = fopen(filename,"rb");
	if( fp == NULL )
	{
		printf(" open %s error!\n", filename);
		goto IW_GET_ERROR;
	}
	ret = fseek(fp,0L,SEEK_END);
	if( ret != 0 )
	{
		printf("fseek error!!\n");
		goto IW_GET_ERROR;
	}
	fileOffset = ftell(fp);
	if( fileOffset == -1 )
	{
		printf(" ftell error!\n");
		goto IW_GET_ERROR;
	}
	printf(" fileOffset = %ld\n",fileOffset);
	rewind(fp);	
	ret = fread(buf,1,fileOffset,fp);
	if( ret != fileOffset )
	{
		printf(" fread Error!=n");
		goto IW_GET_ERROR;
	}
	
	find1 = iw_FindSTR(buf,"SSID",0);
	if( find1 < 0 )
		goto IW_GET_ERROR;
	printf("find1:%d_s:%s_\n", find1, &buf[find1]);
	find3 = iw_FindSTR(buf,"SSID=",find1);
	find2 = iw_FindSTR(buf,"\n",find3 +1);
	ret = iw_GetTmpLink(buf,tmp1,find3+strlen("SSID=") -1,find2,5000);
	if(ret<0)
		goto IW_GET_ERROR;
	strcpy(tmpSsid, tmp1);
	DBGPRINT("************* get string ssid:%s__ \n",tmp1);

	find1 = iw_FindSTR(buf,"Key1Str",0);
	if( find1 < 0 )
		goto IW_GET_ERROR;
	printf("find1:%d_s:%s_\n", find1, &buf[find1]);
	find3 = iw_FindSTR(buf,"Key1Str=",find1);
	find2 = iw_FindSTR(buf,"\n",find3 +1);
	ret = iw_GetTmpLink(buf,tmp1,find3+strlen("Key1Str=") -1,find2,5000);
	if(ret<0)
		goto IW_GET_ERROR;
	strcpy(tmpPwd, tmp1);
	DBGPRINT("************* get string pwd:%s__ \n",tmp1);

	strcpy(ssid, tmpSsid);
	strcpy(pwd, tmpPwd);
  	
IW_GET_ERROR:
	if (fp != NULL)
		fclose(fp);
	
	return ;
}

void Menu_Load()
{
	init_nvp1114(SysMenu.VideoFormat);
}

void InitRtcTime()
{
	int ret;
	
	if(g_rtc_fd<=0)
	{
	g_rtc_fd=open("/dev/hi_rtc", O_RDWR);
	if(g_rtc_fd<=0)
		DBGPRINT("open hi_rtc false\n");
	}
			
	if(g_rtc_fd>0)
	{
		ret = ioctl(g_rtc_fd, HI_RTC_RD_TIME, &hi_tm);
		if (ret<0 || hi_tm.year >2200 )
		{
			printf("ioctl: HI_RTC_RD_TIME failed\n");
			hi_tm.year=2013;
			hi_tm.month=8;
			hi_tm.date=1; 
			hi_tm.hour=12;
			hi_tm.minute=0;
			hi_tm.second=0; 		
			ret = ioctl(g_rtc_fd, HI_RTC_SET_TIME, &hi_tm);
			if(ret<0)
					printf("ioctl: HI_RTC_SET_TIME failed\n");
		}
		else
		{
			timreadbuf[6]=hi_tm.year-2000;
			timreadbuf[5]=hi_tm.month;
			timreadbuf[4]=hi_tm.date; 
			timreadbuf[2]=hi_tm.hour;
			timreadbuf[1]=hi_tm.minute;
			timreadbuf[0]=hi_tm.second;
			setsystem(timreadbuf);
		}
	}else
	{
		timreadbuf[6]=2016-2000;
		timreadbuf[5]=hi_tm.month;
		timreadbuf[4]=hi_tm.date; 
		timreadbuf[2]=hi_tm.hour;
		timreadbuf[1]=hi_tm.minute;
		timreadbuf[0]=hi_tm.second;
		setsystem(timreadbuf);
	}
}


void InitMenu()
{
	int ret;
	U8 i;
	InitE2Prom();
	InitRtcTime();
		
	MenuSysInit();
	
	demo_page = 0x00;
	Cam_Num = CHANNEL1;
	
	High_Resolution = 480+SysMenu.VideoFormat*96;
	DEMO_DISP = LFULL;
	MOUSEMAINMENUNUM = 10;

	
	memset(&mainmenupic,0,sizeof(mainmenupic));
	memset(&systemmenupic,0,sizeof(systemmenupic));

	
	MenuRecInit();
	MenuNetStaticInit();
	Menu_Load();
	MenuCamInit();
	MenuPtzInit();
	MenuSCHInit();
	MenuEmailInit();
	SpotMenu_Inital();
	audioMenu_Inital();
	MenuAlarm_Initial();
	MenuHddInit();
	user_menu_inital();
	menu_time_back_init();

	
	InitAllDevice();
	
	set_motion(1);
	alarm_init();
	osd_init();

	sar_fun();

	LedControl(LEDPWR,1);
	WatchDog();

	memset( &PlayStatusFlag, 0x00,sizeof(ValPlay));
	if (SysMenu.RemoteID == 1)
		LedControl(LEDIR,1);
	else
		LedControl(LEDIR,0);

	demo_num = L4;
	menu_num =0x00;
	cur_pos =0;
	bReset=0;
	DiskCheck=1;
	auto_flag=0;
	remo_Led=0;
	cam_loss=0;
	loss_old=cam_loss;
	zoom_flag = 0;
	bsecond_change = 1;
	bminute_change = 1;
	bhour_change =1;
	bday_change =1;
	bmouth_change =1;
	byear_change = 1;
	mailsend = 0;
//	SearchFlag = 0;
	for (i=0;i<Cam_Num;i++)
		AutoMenu[i].KeepTime_A =5;
//	SysMenu.Laguage=0;
	if (demo_num == L4)
		Reflash_Page = FourCh1;
	else if (demo_num == L8)
		Reflash_Page = EightCh;
	else if (demo_num == L16)
		Reflash_Page = SixtyCh;
	
	LedControl(LEDUSB,ON);
	pointer[0] = 44;
	pointer[1] = 54;
	pointer[2] = 57;
	pointer[3] = 65;

}



extern U8 redpos;
int g_net_motion_flag=0;

static void s_motion_info_send(int flag)
{
	char msg[10];
	
	DBGPRINT("********* s_motion_info_send:%d \n",flag);
	memset(msg,0,sizeof(msg));	
	msg[0]=0x4a;
	msg[1]=flag;
	gstCommonParam->GST_DRA_Net_dvr_send_self_data(msg,2);
	
	usleep(8000);
	
	memset(msg,0,sizeof(msg));
	msg[0]=INIT_IPC_ORD1;
	msg[1]=INIT_IPC_ORD2;
	msg[2]=INIT_IPC_ORD3;
	msg[3]=INIT_IPC_ORD4;
	msg[4]=GET_IPC_MOTION;
	msg[5]=flag;
	gstCommonParam->GST_DRA_Net_dvr_send_self_data(msg,10);
}
void Osd_thread_pro(void)
{
	SET_PTHREAD_NAME(NULL);
//	static U8 blanktime=0,i=0;
//	int mb;
  int motion_tmp0=0,motion_tmp1=0;
  int cnt=0;
  
	while(1)
	{
		usleep(1000);
//		if (rec_on)// || !redpos)
		{
			
			//printf( "testtesttesttest\n");
			cam_motion |= GetMotionInfo(0);
			//printf("cam_motion = %x\n",cam_motion);
			motion_tmp1=cam_motion;
			if(motion_tmp1&&motion_tmp0==0)
			{
				g_net_motion_flag=1000;
			}
			if(motion_tmp1!=motion_tmp0) 
			{
					cnt=0;
				
				if(motion_tmp1)
					s_motion_info_send(1);
				else
					s_motion_info_send(0);
			}
			motion_tmp0=motion_tmp1;
			
			if(motion_tmp1&&motion_tmp0)  
			{
				cnt++;
				if(cnt>200)
				{
					cnt=0;
			//		s_motion_info_send(0);
			//		usleep(10000);
					s_motion_info_send(1);
				} 
			}
			else
			{	
				cnt=0;
			}	
				
		}
		if(g_net_motion_flag)
   	 	g_net_motion_flag--;
		if (mailsend)
		{
			mail_send(mailcam,mailevent);
			mailsend = 0;
			mailcam = 0;
			mailevent = 0;
		}
	}
}

void wtd_thread_pro(void)
{
	wdginit();
	while(1)
	{
		sleep(3);
		WatchDog();
	}
}

void mouse_osd_thread_create(void)
{
	int ret;
	if (ChipNum == SLAVE)
		return;
	
	if (threadcreat)
		return;
	ret = pthread_create(&g_ThreadOSD, NULL,(void *) Osd_thread_pro,NULL);
	if ( ret<0 )
	{
		printf( "Osd_thread_pro create error\n");
	}
	
	threadcreat = 1;
}


char channel_current_ap[5];//当前AP信道

char ipl_myid[20];//local cam ID

void set_hisi_antiflicker(void)
{
	int a,s_antiflicker_mode;
	IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
	IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;

	if (SysMenu.VideoFormat)
		a=0;
	else
		a=1;
	s_antiflicker_mode=pIsp_st->u32BackLight;
	if(s_antiflicker_mode)
		s_antiflicker_mode=0;
	else
		s_antiflicker_mode=1;
	gstCommonParam->GST_DRA_Hisi_set_spi_antiflickerattr(a,s_antiflicker_mode);
}


void dev_ctrl_thread_pro(void)
{	
	SET_PTHREAD_NAME("Tdevctrl");

	//return;
	unsigned char i,startup=0;
	char msg[20];
	int mirror_ctrl=0;
	char tempMsgLx[10];
	
	U8 Buzzer=10;
	int ret,ip_display_flag=0;
	
	pthread_mutex_init(&eprom_file_edit_lock, NULL);
	
	chip_check();
	
//all init here
	checksdcard();

	InitMenu();
	
	creatfixdiskthread();

	gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"eth0");


	
	IpLogic_Set_Dhcp_Enable((NetStaticMenu.IPAlloction&0x01));


	if (remo_Led>1)
		remo_Led = 0;
	Buzzer = remo_Led;
	remo_Led ^= 1;
	
	LedControl(LEDIR,Buzzer);
	remo_Led=Buzzer;
	Buzzer = 10;

	DiskCheck = 0;
	diskstatus = 0;
	GetHddInfo();
	AlarmReset();
	//ptz uart
	setuart(Baud[PtzMenu[0].BaudRate]);

	rec_req=0;
  	ip_display_flag=0;
	set_hisi_antiflicker();
	while(1)
	{
		usleep(100000);
		//continue;
		ircut_func();
		if(gtstop==1)
		{
			gtstop=2;
			break;
		}
		Time_Check();
		//..GetKey(); 
		if (startup==1)
		{
			startup = 3;
			CheckRecord();
			mouse_osd_thread_create();
		}

		sensor_check();
		sensor_detect();
		sensor_process();
		motion_detect();
		motion_process();
		//..vloss_detect();
		//..vloss_process();

		if(rec_req)
		{
			usleep(1000);
			printf("dev_ctrl_thread_pro record_set **********************\n");	
			record_set1();
			rec_req=0;
		}

		if (bReset)
		{
			McuReset();
			menu_num =0;
			bReset=0;
		}
		if(bsecond_change) 
		{
			Time_Date_Disp();
		}
		strup = 0x00;
		bsecond_change=0;
		bminute_change = 0;
//		bhour_change = 0;
		bday_change = 0;
		bmouth_change = 0;
		byear_change = 0;
		loss_old = cam_loss;

	}
	
}

///////////////////////////////
int  tw_init(void)
{
	return 1;	
}


int get_allfunction_ptr(GST_DEV_FILE_PARAM *filepar)
{
//	U8 tmp;
	gstCommonParam = filepar;
	netcontrolinital();
	return 1;
}

int tw_stop(void)
{
	//pthread_join( g_devctrlThreadpro, NULL );
	printf( "tw_stop\n");
	gtstop=1;
	while(gtstop==1)
	{
		usleep(100);
	}

	g_devctrlThreadpro=-1;
	return 1;
}




///////////////////////////////////////////////////自动有线无线分割线
	WIFIAP stwifiap[65];
unsigned char connect_ipcap = 0;
unsigned char connect_nvrap = 0;
unsigned char is_wire = 0;
unsigned char is_ap = 0;
unsigned char is_sta = 0;
unsigned char ap_change = 0;
//unsigned char i_haveSignal = 0;//搜索的iw_essidNvr有信号
unsigned char ap_channel = 0;
unsigned char is_ap_on = 0;
unsigned char is_ap_ch_change = 0;
unsigned char ipcap_connecting = 0;
unsigned char nvrap_connecting = 0;
int connecting_count = 0;
int count_readytoclose_ap = 0;

char myid[20];//local cam ID
char iw_essidNvr[17];
//char apsignal[100][17];直接从扫描数据中获取
char myapssid[20];//nvr 设置的SSID 
char parassid[20];//当前连接的上级AP的nvrSSID
char cmdStr[256];//构造命令字符串

char storeRX[256];
char storeTX[256];
int countUpDownModule = 0;

unsigned char threat2_start=0;
 

int set_if_ip(char *if_name, char *ip_str)  //设置接口的ip地址  
{  
    struct ifreq ifr;
    struct sockaddr_in ip_addr;  
    int sockfd;  
  
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)  
    {  
        perror("Socket error");  
	  close(sockfd);
        return -1;  
    }   // 创建用来检查网络接口的套接字  
      
    ip_addr.sin_family = AF_INET;     
    if(inet_pton(AF_INET, ip_str, &ip_addr.sin_addr) < 1)  
    {  
        perror("error ipv4 addr:");  
		close(sockfd);
        return -1;  
    }     
      
    strcpy(ifr.ifr_name, if_name);  
    memcpy(&ifr.ifr_addr, &ip_addr, sizeof(struct sockaddr_in));      
    if(ioctl(sockfd, SIOCSIFADDR, &ifr) < 0)  
    {  
        perror("ioctl SIOCSIFADDR error");  
		close(sockfd);
        return -1;  
    }  
	close(sockfd);
    return 0;     
}  

int iw_get_nvr_set(char *fileUrl, char *conStr)
{
  FILE *fd = NULL;
  int len;
  
//		return 0;
		
  memset(conStr,0,sizeof(conStr));
	fd=fopen(fileUrl,"r");
	if(fd)
	{
		fgets(conStr,15,fd);
		len=strlen(conStr);
		printf("essidNvr: %s, len: %d-------\n", conStr, len);
		conStr[len-1]=0;
		printf("essidNvr: %s, len: %d-------\n", conStr, len);
		fclose(fd);
		if(len == 0)
			return 0;
		else
			return 1;
	}
	else
		return 0;
}

int iw_FindSSID(char *url, char *essid, int isWifiWork)
{//isWifiWork 判断当前wifi是否正常
	FILE * fp = NULL;
	int find1 = 0,ret, len = 0;
	char tmp1[5000],buf[1024*100];
	long fileOffset = 0;
	printf(" open %s ready to find %s---!\n", url, essid);
	memset(buf,0,sizeof(buf));
	memset(tmp1,0,sizeof(tmp1));

	fp = fopen(url,"rb");
	if( fp == NULL )
	{
		printf(" open %s error!\n", url);
		goto IW_GET_ERROR_2;
	}
	ret = fseek(fp,0L,SEEK_END);
	if( ret != 0 )
	{
		printf("fseek error!!\n");
		goto IW_GET_ERROR_2;
	}
	fileOffset = ftell(fp);
	if( fileOffset == -1 )
	{
		printf(" ftell error!\n");
		goto IW_GET_ERROR_2;
	}
	printf(" fileOffset = %ld\n",fileOffset);
	rewind(fp);	
	ret = fread(buf,1,fileOffset,fp);
	if( ret != fileOffset )
	{
		printf(" fread Error!=n");
		goto IW_GET_ERROR_2;
	}

	if(isWifiWork)
		{
			DBGPRINT("isWifiWork: %s\n", essid);
			if(fileOffset == 0)
				goto IW_GET_ERROR_2;
			
				DBGPRINT("To find whether ra0/ra1 is work!\n");
				find1 = iw_FindSTR(buf, "completed",0);//判断当前网卡有没在正常工作
				if(find1 < 0)
				{
					DBGPRINT("ra0/ra1 is not work! restart system begin!: %s\n", buf);
					bReset = 1;//重新启动标志
						
					goto IW_GET_ERROR_2;
				}
				DBGPRINT("ra0/ra1 is working !\n");
				DBGPRINT("ra0/ra1 is working !\n");
				DBGPRINT("ra0/ra1 is working !\n");	
		}else{
				find1 = iw_FindSTR(buf,essid,0);
				//DBGPRINT("search results: %s\n", buf);
				if( find1 < 0 )
				goto IW_GET_ERROR_2;

				DBGPRINT("Find essid:%s---------------------------ok\n", essid);
			}

	fclose(fp);
	
	return 1;
  	
IW_GET_ERROR_2:

	DBGPRINT("Find essid:%s---------------------------error\n", essid);

	fclose(fp);
	
	return 0;

}

int check_wire(void)
{
	return CheckHaveNetCable("eth0");
	
	FILE *fp = NULL;

	char cmdSteam[256];

	memset(cmdSteam, 0, sizeof(cmdSteam));
	
	fp = popen("ifconfig eth0 | grep 'RUNNING'", "r");
	if(fp != NULL)
	{
		if(fgets(cmdSteam, 256, fp) != NULL)
		{
			DBGPRINT("cmdSteam check wire status: %s\n", cmdSteam);
		}
	}

	pclose(fp);
	
	if(strlen(cmdSteam) > 0)
		{
			printf("----wire is on----:%d \n", strlen(cmdSteam));
			return 1;
		}
	else{
			printf("----wire is off----:%d \n", strlen(cmdSteam));
			return 0;
		}

}

int CheckHaveNetCable(char * pNetAdapter)
{
	struct ifreq ifr;
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	if( skfd <= 0 )
	{
		DPRINTK("Open socket err\n");
		return -1;
	}

	strcpy(ifr.ifr_name, pNetAdapter);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
	{
	    close(skfd);
	    return -1;
	}

   	close(skfd);

    	if(ifr.ifr_flags & IFF_RUNNING)
        	return 1;
    	else
		return 0;
}

int check_apsignal(char *tmpSSID)
{
	//将检测到的ssid记录在apsignal[100]
	//DBGPRINT("\nCHECK CHECK CHECK apsignal\n");
	//DBGPRINT("\nCHECK CHECK CHECK apsignal\n");

	char cmdStr_ca[256];
	char cmdSteam[256];
	
	memset(cmdStr_ca, 0, sizeof(cmdStr_ca));
	memset(cmdSteam, 0, sizeof(cmdSteam));
	
	sprintf( cmdStr_ca, "iwconfig ra0 | grep '%s'", tmpSSID);
	
	FILE *fp;
	fp = popen(cmdStr_ca, "r");
	if(fp != NULL)
	{
		if(fgets(cmdSteam, 256, fp) != NULL)
		{
			DBGPRINT("cmdSteam CHECK apsignal status: %s\n", cmdSteam);
		}
	}

	pclose(fp);
	
	if(strlen(cmdSteam) > 0)
		{
//			isNeedScan = 0;
			printf("---CHECK CHECK CHECK apsignal----ok:%d \n", strlen(cmdSteam));
			return 1;
		}
	else{
//			isNeedScan++;
			printf("---CHECK CHECK CHECK apsignal----error:%d \n", strlen(cmdSteam));
			return 0;
		}
}

int SH_GetConnStatus(char *cmdStr)
{
	char conStr[256];
	FILE *fd = NULL;
	int len;

	memset(conStr,0,sizeof(conStr));
	fd=popen(cmdStr,"r");
	if(fd)
	{
		fgets(conStr,256,fd);
		len=strlen(conStr);
		if(len > 0)
			conStr[len-1]=0;
		fclose(fd);
		if(len == 0)
			return 0;
		else
			return 1;
	}
	else
		return 0;
}



void wire_on(void)
{
	DBGPRINT("eth0 is on line.\n");
	DBGPRINT("eth0 is on line.\n");
	DBGPRINT("eth0 is on line.\n");
	is_wire = 1;
	g_wifi_link_flag_ex = 0;
	gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"eth0");
	if(is_ap == 1 && is_ap_on == 0)
	{//当作为其他枪 的上级AP，而且未开启AP时，开启AP
		ap_on();
	}
}

void wire_off(void)
{
	DBGPRINT("eth0 is off line.\n");
	DBGPRINT("eth0 is off line.\n");
	DBGPRINT("eth0 is off line.\n");
	is_wire = 0;
	g_wifi_link_flag_ex = 1;
	gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"ra0");
	if(is_ap_on ==  1)//有线断开时，AP关闭
		ap_off();
}



unsigned char is_valid_ap=0;
//unsigned char is_ssid_table=0;

unsigned char valid_ap(void)
{
	memset(iw_essidNvr, 0, sizeof(iw_essidNvr));
//	memset(iw_essid, 0, sizeof(iw_essid));
	if(iw_get_nvr_set("/mnt/mtd/essidNvr.txt", iw_essidNvr) == 0)//NVR设置的SSID
	{
		memcpy(iw_essidNvr, "00000000",sizeof("00000000"));
	}
	IPCAM_ALL_INFO_ST * pAllInfo_t = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
	if(pAllInfo_t->net_st.enable_wifi_auto_set_by_nvr == 0)
	{
		return 2;
	}
//	iw_get_nvr_set("/mnt/mtd/essid.txt", iw_essid);//netcam上设置的SSID
	if(strcmp(iw_essidNvr, "00000000") == 0 || strcmp(iw_essidNvr, "00000BNC") == 0 || strcmp(iw_essidNvr, myid) == 0)//非法ap
		return 0;
	else if(strcmp(iw_essid, iw_essidNvr) == 0)//上级ap是NVR
		return 2;
	else//上级ap不是NVR
		return 3;
}

unsigned char check_ssid_table(void)
{	 
	  //查ap信号表	  
	  //ipc ssid存在  return 1; iw_essid
	  //nvr ssid存在  return 2;iw_essidNvr
	  //都存在        return 3;	
	  //都不存在      return 0;
	  int tmp, tmp1, tmp2;
	  tmp2 = iw_FindSSID("/ramdisk/iw_info.txt", iw_essid, 0);  //查nvrssid
	  if(is_valid_ap == 3)
	  {
	  	 tmp1=iw_FindSSID("/ramdisk/iw_info.txt", iw_essidNvr, 0);  //查ipcssid	  	 
	  	 tmp=tmp1;
	  	 tmp|=tmp2<<1;
       return tmp;	  	 
	  }
	  else if(is_valid_ap==0 || is_valid_ap==2)
	  {
	  	return tmp2<<1;
	  }
}

void sta_on(void)
{
	is_sta = 1;
//	connectnocast_count = 0;//计数清零
	DBGPRINT("-----connect success!-----\n");
}

void sta_off(void)
{//结束连接AP
	{
		sys_command("killall wpa_supplicant");
		memset(parassid, 0, sizeof(parassid));

		connecting_count = 0;//正在连接状态清零
		ipcap_connecting = 0;
		nvrap_connecting = 0;
	}

	sys_command("ifconfig ra0 down");
	sys_command("ifconfig ra0 up");
	sleep(1);

//	if(connect_ipcap == 1 || connect_nvrap == 1 || is_wire == 1)
//		{//当连接成功再断开时，计数清零/当有线时清零
//			connectnocast_count = 0;//计数清零
//		}
	
	connect_nvrap = 0;
	connect_ipcap = 0;

	is_sta = 0;
	//stop ap 当转为有线时，ap信道变为自身的信道
	if(is_wire == 1)
		ap_off();
}

void ap_on(void)
{//将获取的ssid填入文件中
	sys_command("killall hostapd");//重新开启ap预处理
	sys_command("ifconfig ra1 up");

	DBGPRINT("-----start open_ap-----\n");

	int APchannel = -161, t_channel;
	//int ht_way1, ht_way2, ht_way3, ht_way4;

	char channel[5];

	{
		memset(channel, 0, sizeof(channel));		
		if(iw_get_nvr_set("/mnt/mtd/openap.txt", channel))
		{
			sscanf(channel, "%d", &APchannel);
			DBGPRINT("Current AP channel is -- %s -to-%d--\n", channel, APchannel);
		}
	}
	
	{
		FILE *hostapd_conf = NULL;
        
        hostapd_conf = fopen("/mnt/mtd/hostapd.conf","w+");

        if(hostapd_conf == NULL)
        {
        	DBGPRINT("open hostapd.conf error\n");
			return;
	 }

            fprintf(hostapd_conf, "#################### hostapd configuration file #################### \n");
            fprintf(hostapd_conf, "ctrl_interface=/var/run/hostapd\n");
            fprintf(hostapd_conf, "interface=ra1\n");			
            fprintf(hostapd_conf, "bridge=br0\n");			
            fprintf(hostapd_conf, "ssid=%s\n", myid);

		if(APchannel < 0)
			t_channel = -APchannel;
		else 
			t_channel = APchannel;


		
            fprintf(hostapd_conf, "channel=%d\n", t_channel);	

            //fprintf(hostapd_conf, "country_code=US\n");
            fprintf(hostapd_conf, "wpa=2\n");
            fprintf(hostapd_conf, "wpa_passphrase=%d\n", 88888888);
            fprintf(hostapd_conf, "\n");
            fprintf(hostapd_conf, "\n");
            fprintf(hostapd_conf, "#################### default configuration #################### \n");
            fprintf(hostapd_conf, "driver=rtl871xdrv\n");
            fprintf(hostapd_conf, "beacon_int=100\n");
            fprintf(hostapd_conf, "hw_mode=a\n");
           // fprintf(hostapd_conf, "auth_algs= 1\n");			
           // fprintf(hostapd_conf, "ieee80211n=1\n");
           // fprintf(hostapd_conf, "ieee80211d=1\n");
           // fprintf(hostapd_conf, "wme_enabled=1\n");	//省电模式

		/*ht_way1 = (t_channel - 36) % 8;
		ht_way2 = (t_channel - 40) % 8;
		ht_way3 = (t_channel - 149) % 8;
		ht_way4 = (t_channel - 153) % 8;
		
		if(t_channel == 165)
			fprintf(hostapd_conf, "ht_capab=[SHORT-GI-40][DSSS_CCK-40]\n");
		else if(ht_way1 == 0 || ht_way3 == 0)
			fprintf(hostapd_conf, "ht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]\n");
		else if(ht_way2 == 0 || ht_way4 == 0)
			fprintf(hostapd_conf, "ht_capab=[HT40-][SHORT-GI-40][DSSS_CCK-40]\n");*/
		
            fprintf(hostapd_conf, "wpa_key_mgmt=WPA-PSK\n");
            fprintf(hostapd_conf, "wpa_pairwise=CCMP\n");			
		fprintf(hostapd_conf, "ignore_broadcast_ssid=0\n");//no显示ssid

		fclose(hostapd_conf);

		sys_command("chmod 777 /mnt/mtd/hostapd.conf");
	}

	DBGPRINT("Current AP channel is --%d--\n", t_channel);
	sys_command("ifconfig ra1 up");
	sys_command("hostapd /mnt/mtd/hostapd.conf -B &");

	is_ap_on = 1;
}

void ap_off(void)
{
	DBGPRINT("-----close ap-----\n");
	sys_command("killall hostapd");
	sys_command("ifconfig ra1 up");
	is_ap_on = 0;
	is_ap_ch_change = 0;
}

char ip_br0[20];
char ip_eth0[20];
char ip_ra0[20];
char ip_temp[20];

int ipcam_get_host_ip(char * dev,unsigned char * host_ip);//yb

void ipchange_check(void)
{//检测IP改变
	int i_temp = 0, is_ipChange = 0;
	char cmdStr_gw[256];
	int resultBR = 0, resultBE = 0, resultER = 0;
	unsigned char host_ip[4] = {0};
	struct in_addr tmp_addr;
	
//	DBGPRINT("\nInto check ip change\n");
	memset(ip_br0, 0, sizeof(ip_br0));
	//tmp_addr.s_addr = iw_ioctl_get_ipaddress("br0");
//	sprintf(ip_br0,"%s",inet_ntoa(tmp_addr));	
	if ( ipcam_get_host_ip("br0",host_ip) >= 0 )
		sprintf(ip_br0,"%d.%d.%d.%d",host_ip[0],host_ip[1],host_ip[2],host_ip[3]);
	else
		strcpy(ip_br0, "0.0.0.0");

	memset(ip_eth0, 0, sizeof(ip_eth0));
	//tmp_addr.s_addr = iw_ioctl_get_ipaddress("eth0");
	//sprintf(ip_eth0,"%s",inet_ntoa(tmp_addr));
	if ( ipcam_get_host_ip("eth0",host_ip) >= 0 )
		sprintf(ip_eth0,"%d.%d.%d.%d",host_ip[0],host_ip[1],host_ip[2],host_ip[3]);
	else
		strcpy(ip_eth0, "0.0.0.0");

	memset(ip_ra0, 0, sizeof(ip_ra0));
	//tmp_addr.s_addr = iw_ioctl_get_ipaddress("ra0");
	/*if(tmp_addr.s_addr == 0)
		{
			if(ifExistNetDevice("ra0") == 0)
			{//if Netdevice not exist, then reboot
				bReset = 1;
				return;
			}
		}*/
	//sprintf(ip_ra0,"%s",inet_ntoa(tmp_addr));
	if ( ipcam_get_host_ip("ra0",host_ip) >= 0 )
		sprintf(ip_ra0,"%d.%d.%d.%d",host_ip[0],host_ip[1],host_ip[2],host_ip[3]);
	else
		strcpy(ip_ra0, "0.0.0.0");

//	DBGPRINT("\neth0: %s, br0: %s, ra0: %s\n", ip_eth0, ip_br0, ip_ra0);
	resultBR = strcmp(ip_br0, ip_ra0);
	resultBE = strcmp(ip_br0, ip_eth0);
	resultER = strcmp(ip_eth0, ip_ra0);//防止得到新IP速度过快，造成IP紊乱
	if(is_wire == 0 && (resultBR != 0 || resultBE != 0 || resultER != 0) && bReset == 0)
	{
		gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"ra0");
		memcpy(ip_temp, ip_ra0, sizeof(ip_ra0));
		if(strcmp(ip_temp, "0.0.0.0") == 0)
		{
			if(ifExistNetDevice("ra0") == 0)
			{//if Netdevice not exist, then reboot
				bReset = 1;
				return;
			}
			memcpy(ip_temp, ip_br0, sizeof(ip_br0));
		}
		is_ipChange = 1;
	}else if(is_wire == 1 && (resultBR != 0 || resultBE != 0 || resultER != 0) && bReset == 0)
	{
		gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"eth0");
		memcpy(ip_temp, ip_eth0, sizeof(ip_eth0));
		if(strcmp(ip_temp, "0.0.0.0") == 0)
			memcpy(ip_temp, ip_br0, sizeof(ip_br0));
		is_ipChange = 2;
	}

	if(is_ipChange && bReset == 0)
	{
		DBGPRINT("\--Into start change ip--\n");

		sys_command("route del default");
		set_if_ip("br0", ip_temp);
		if(is_ipChange == 1)
			set_if_ip("ra0", "0.0.0.0");
		else if(is_ipChange == 2)
			set_if_ip("eth0", "0.0.0.0");
		set_if_ip("ra0", ip_temp);
		set_if_ip("eth0", ip_temp);
		for(i_temp = strlen(ip_temp); i_temp >= 0; i_temp--)
			{
				if(ip_temp[i_temp] == '.')
					{
						ip_temp[i_temp+1] = '1';
						ip_temp[i_temp+2] = '\0';
						break;
					}
			}
		memset(cmdStr_gw, 0, sizeof(cmdStr_gw));
		sprintf( cmdStr_gw, "route add default netmask 0.0.0.0 gw %s dev br0", ip_temp);
		sys_command(cmdStr_gw);
		is_ipChange = 0;
	}
}


void threat1(void)
{
	SET_PTHREAD_NAME(NULL);
	int i, tmp, j, num = 20, tmpInt, i_waitTime = 0;
	char mydomain[20], tmpStr[20];
	int ap_change1 = 0;

	gstCommonParam->GST_NET_get_svr_mac_id(mydomain, myid);

	for(i = 0; i < num; i++)
		{	
			if(myid[i] >= '0' && myid[i] <= '9')
				{
					continue;
				}else
					{
						for(j = i; j < num - 1; j++)
							{
								myid[j] = myid[j + 1];
							}
						i = i - 1;
						num = num - 1;
					}
		}

	sscanf(myid, "%d", &tmpInt);
	sprintf(myid, "%08X", tmpInt);
	
	DBGPRINT("\nthreat1 -%X-%s this is no.%d\n", tmpInt, myid, i);
	
	//myapssid="118QNHDS0134";
	connect_nvrap = 0;
	connect_ipcap = 0;
	memset(parassid, 0, sizeof(parassid));
	sys_command("ifconfig ra0 up");
	
	is_wire=1;

	{//检测是否为热点
		if(iw_get_nvr_set("/mnt/mtd/openap.txt", tmpStr))
			{
				is_ap = 1;//启动时判断是否需要开启AP
				//ap_on();
			}
		else{
				is_ap = 0;
				//ap_off();
			}
	}
	
	threat2_start=1;
	while(1)
		{
		      ap_change1 = ap_change;
			tmp=check_wire();			
			if(tmp==1 && is_wire==0)
				{
					countUpDownModule = 0;
					is_wire = 1;
					sta_off();
					wire_on();
					ap_change = 0;
				}else if(tmp == 0 && is_wire == 1)
					{
						countUpDownModule = 0;
						wire_off();
						ap_change = 0;
					}
				
			sleep(1);
			
			if(is_wire == 0)
			{
				if(ipcap_connecting == 0 && nvrap_connecting == 0 && connect_ipcap == 0)
					is_valid_ap = valid_ap();

				/*search_ssid_count++;
				if(search_ssid_count > 30)
					{
						search_ssid_count = 30;					
					}*/

				/*connectnocast_count++;
				if(connectnocast_count > 50)
					{
						connectnocast_count = 50;
					}
				if(connect_ipcap == 1 || connect_nvrap == 1)
				{
					if(search_ssid_count == 30 && connectnocast_count == 50) //有连接，但超过50秒未收到广播，广播接收线程对这个参数清0
					{	
						DBGPRINT("Into getbroadcast start to count--num:--%d--\n", connectnocast_count);
						DBGPRINT("--updown wifi module--!\n");
						upDown_Wifi();					
						search_ssid();	
					}
				}
				else 
					connectnocast_count = 0;*/

				if(is_valid_ap == 3 && connect_ipcap == 0)
				{
				DBGPRINT("Connectting nvr:-%d- ipc:-%d- connecting_timeout(30):-%d-\n", nvrap_connecting, ipcap_connecting, connecting_count);				
				DBGPRINT("IPC is not connected, ready count 30 to connect ipc - ipccount:--%d--\n", ipcap_count);
					if(connect_nvrap == 1)
					{
						ipcap_count++;
						if(ipcap_count > 30)
							ipcap_count = 30;
					}
				}else
					ipcap_count = 0;

				if(ipcap_count == 30 && (is_ap == 0 || (is_ap == 1 && is_ap_on == 1)) && (connect_ipcap == 0 && is_wire == 0 && ipcap_connecting == 0 && nvrap_connecting == 0))//有ipcssid
				{
					ipcap_count = 0;
					DBGPRINT("ipc try to connect!\n");
					if(is_sta == 1) 
						sta_off();

					//create wifi configure file
					memset(cmdStr, 0, sizeof(cmdStr));
					sys_command("echo 'ctrl_interface=/var/run/wpa_supplicant' > /mnt/mtd/wifi_wpa.conf");
					//sys_command("echo 'ap_scan=1' >> /mnt/mtd/wifi_wpa.conf");
					sprintf( cmdStr, "wpa_passphrase %s %d >> /mnt/mtd/wifi_wpa.conf", iw_essidNvr, 88888888);
					sys_command(cmdStr);
					//sys_command("sed -i '5a scan_ssid=1' /mnt/mtd/wifi_wpa.conf");
					sleep(1);
				
					//if(connectap(iw_essidNvr)) 
					//DBGPRINT("Start wpa_supplicant connect ipcap\n")	;				

					sys_command("wpa_supplicant -ira0 -Dwext -c/mnt/mtd/wifi_wpa.conf -b br0 -B");
					sleep(1);

					//DBGPRINT("End wpa_supplicant connect ipcap\n");
					
					ipcap_connecting = 1;
				}else if((is_ap == 0 || (is_ap == 1 && is_ap_on == 1)) && (is_wire == 0 && connect_nvrap == 0 && connect_ipcap == 0 && ipcap_connecting == 0 && nvrap_connecting == 0))
				{
					DBGPRINT("try to connect nvr -- validap:%d--\n", is_valid_ap);
					if(is_sta == 1) 
						sta_off();

					sys_command("sh /mnt/mtd/create_wpa.sh");
					sleep(1);

				//	DBGPRINT("Start wpa_supplicant connect ipcap\n");

					sys_command("wpa_supplicant -ira0 -Dwext -c/mnt/mtd/wifi_wpa.conf -b br0 -B");
					sleep(1);

					//DBGPRINT("End wpa_supplicant connect ipcap\n");
					
					nvrap_connecting = 1;
				}

				if(ipcap_connecting == 1)
				{
					if(check_apsignal(iw_essidNvr))
					{
					//	DBGPRINT("success connect ipcap.----i: %d---%s\n", noconect_count, iw_essidNvr);		
						ipcap_connecting = 0;
						
						memcpy(parassid, iw_essidNvr, sizeof(iw_essidNvr));
						sprintf(iw_net_info_str,"SSID:%s   ",parassid);

						connecting_count = 0;
						connect_ipcap = 1;
						sta_on();
					}else{
							connecting_count++;
							if(connecting_count > 30)
								{
									ipcap_connecting = 0;
									sys_command("killall wpa_supplicant");
									sleep(1);
									sys_command("ifconfig ra0 up");
									sleep(1);
									connecting_count = 0;
								}
						}
								
				}else if(nvrap_connecting == 1)
					{
						if(check_apsignal(iw_essid))
						{
							//DBGPRINT("success connect nvrap.----i: %d---%s\n", noconect_count, iw_essid);
							nvrap_connecting = 0;

							memcpy(parassid, iw_essid, sizeof(iw_essid));
							sprintf(iw_net_info_str,"SSID:%s   ",parassid);

							connecting_count = 0;
							connect_nvrap = 1;		
							sta_on();
						}else{
								connecting_count++;
								if(connecting_count > 30)
									{
										ipcap_count = 30;
										nvrap_connecting = 0;
										sys_command("killall wpa_supplicant");
										sleep(1);
										sys_command("ifconfig ra0 up");
										sleep(1);
										connecting_count = 0;
									}
							}
					}
				if(is_wire == 0 && ap_change1 == 1)//已经处于连接状态才有ap_change
					{//上级AP发生改变
						DBGPRINT("if(ap_change==1)\n");
						DBGPRINT("if(ap_change==1)\n");

						memset(iw_essidNvr, 0, sizeof(iw_essidNvr));
						if(iw_get_nvr_set("/mnt/mtd/essidNvr.txt", iw_essidNvr) == 0)
						{
							memcpy(iw_essidNvr, "00000000",sizeof("00000000"));
						}

						if(strcmp(parassid, iw_essidNvr) != 0 && strcmp(parassid, iw_essid) != 0)
							{
								DBGPRINT("--currentAP is not nvr setAP!--\n");
								sta_off();
							}
						ap_change = 0;
						ap_change1 = 0;
					}
				
			}

				if(is_ap == 1 && is_ap_on == 0)
					{//always open ap, if it need
						DBGPRINT("--wire on, open ap!--\n");					
						ap_on();
					}

				if(is_ap == 0 && is_ap_on == 1)
					{//when close ap, delay 30s to real close ap,ensure all cam receive redirectAP msg
						count_readytoclose_ap++;
						if(count_readytoclose_ap > 15)
							count_readytoclose_ap = 15;
						
						if(count_readytoclose_ap == 15)
						{
							count_readytoclose_ap = 0;
							DBGPRINT("--no need ap, close ap!--\n");
							ap_off();
						}
					}

				if(is_ap == 1 && is_ap_on == 1 && is_ap_ch_change == 1)//AP信道改变时，关闭AP
					{
						DBGPRINT("--ap_channel change, close ap!--\n");
						ap_off();
					}
				if((connect_ipcap == 1 || connect_nvrap == 1 || is_wire == 1) && bReset == 0)//有重启标志时，不change
					ipchange_check();

				if (is_wire == 1 && is_ap == 1)
				{
					receiveRX("ra1");
					if (countUpDownModule > 60)
						countUpDownModule = 60;
					if (countUpDownModule == 60)
					{
						detectNetStatus();
						ipchange_check();
					}
				}else if (is_wire == 0)
					{
						if (connect_ipcap == 1 || connect_nvrap == 1)
						{
							receiveTX("ra0");
							if (countUpDownModule > 60)
								countUpDownModule = 60;
							if (countUpDownModule == 60)
							{
								detectNetStatus();
								ipchange_check();
							}
						}
						else
						{
							receiveRX("ra0");
							if (countUpDownModule > 60)
								countUpDownModule = 60;
							if (countUpDownModule == 60)
							{
								detectNetStatus();
								ipchange_check();
							}
						}
					}

				sleep(1);
			
		}
}

int cam_wired_wireless_connect_thread_create(void)
{
	int ret;
	ret = pthread_create(&g_camwiredwirelesspro, NULL,(void *) threat1,NULL);
	if ( ret < 0 )
	{
		printf("cam_wired_wireless_connect_thread create error\n");
		return -1;
	}
	return 1;
}

int check_broadcast(void)
{//接收AP 广播
	char channel_broadcast_ap[5];
	char tmpStr_chanChannel[5];
	char nvr_id[20];
	char cmdStr_bro[256];
	int broadcastCount = 0;
	int currentChannelNum;//record cam current channel
	
	DBGPRINT("\nrecv  ap broadcast\n");
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);
	addrto.sin_port = htons(12301);

	struct sockaddr_in from;
	bzero(&from, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	from.sin_port = htons(12301);
	
	int sock = -1;
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("socket error.\n");
		return 0;
	}

	const int opt = 1;
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		printf("set socket error...\n");
		return 0;
	}

	if(bind(sock, (struct sockaddr *)&addrto, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind error.\n");
		return 0;
	}

	int len = sizeof(struct sockaddr_in);
	char smsg[1400] = {0};
	char judgeOK[20];

	const char *token = "\r\n";
	char *p;

	//memset(stwifiap_store, 0, sizeof(stwifiap_store));
	//Sta_load_nvr_bc_ssid_info(&stwifiap_store);
		
	while(1)
	{	
		len = sizeof(from);
		memset(&from, 0, len);
		memset(channel_broadcast_ap, 0, sizeof(channel_broadcast_ap));
		memset(tmpStr_chanChannel, 0, sizeof(tmpStr_chanChannel));
		memset(nvr_id, 0, sizeof(nvr_id));
		memset(smsg, 0, sizeof(smsg));
		broadcastCount = 0;
		
		memset(myapssid, 0, sizeof(myapssid));
		memset(stwifiap, 0, sizeof(stwifiap));

		if(iw_get_nvr_set("/mnt/mtd/essidNvr.txt", myapssid) == 0)//刚开启时赋值
		{
			memcpy(iw_essidNvr, "00000000",sizeof("00000000"));
		}
		//DBGPRINT("(Devctrl.c(2774):during while(1) : myapssid:----- %s----\n", myapssid);
		
		int ret = recvfrom(sock, smsg, 1400, 0, (struct sockaddr*)&from, (int*)&len);

		if(ret <= 0)
		{
			printf("read error...\n");
		}else
		{
			int i = 0;
			DBGPRINT("recv  broadcast ok.-----------------\n");		
			//printf("Receive : \n%s\n", smsg);
			/*sscanf(smsg, "%s%s%s%s%s%s%s%s", 
				stwifiap[0].channelid, stwifiap[0].wifiapssid, 
				stwifiap[1].channelid, stwifiap[1].wifiapssid, 
				stwifiap[2].channelid, stwifiap[2].wifiapssid, 
				stwifiap[3].channelid, stwifiap[3].wifiapssid);*/
			p = strtok(smsg, token);
			memcpy(channel_broadcast_ap, p, strlen(p));

			p = strtok(NULL, token);
			memcpy(nvr_id, p, strlen(p));

			p = strtok(NULL, token);
			while(p)
				{
					if(i%2 == 0)
						{
							memcpy(stwifiap[i/2].channelid, p, strlen(p));
							//DBGPRINT("set channelid %s location 1.-----------------\n", stwifiap[i/2].channelid);
						}
					else
						{
							if(strcmp("nvr", p) == 0)
								memcpy(stwifiap[i/2].wifiapssid, nvr_id, strlen(nvr_id));
							else
								memcpy(stwifiap[i/2].wifiapssid, p, strlen(p));
							//DBGPRINT("set channelid %s location 1.-----------------\n", stwifiap[i/2].wifiapssid);
						}

					if(i == 128)//限制最高为64路
						break;
					
					i++;
					p = strtok(NULL, token);
				}

			broadcastCount = i/2;

		/*if(is_wire == 0)
			{
				//DBGPRINT("--broadcast before :  - count:%d--", connectnocast_count);
					connectnocast_count = 0;
				//DBGPRINT("--broadcast after :  - count:%d--", connectnocast_count);					
			}*/

		if(i <= 128)
			{//avoid loss data
				memset(judgeOK, 0, sizeof(judgeOK));
				memcpy(judgeOK, stwifiap[broadcastCount].channelid,strlen(stwifiap[broadcastCount].channelid));
				if(strcmp(judgeOK, "BROADCASTEND") == 0)
					DBGPRINT("recev completely ok\n");
				else{
					DBGPRINT("recev completely fail!continue, %s\n", judgeOK);
					continue;
					}
			}

		if (memcmp(stwifiap_store, stwifiap, sizeof(stwifiap)) != 0)
		{
			memcpy(stwifiap_store, stwifiap, sizeof(stwifiap));
			Sta_save_nvr_bc_ssid_info(&stwifiap_store, NULL);
		/*	if (i_is2_4G)
				IpLogic_Set_hot_SSID(nvr_id, "88888888", 1, 2);
			else
				IpLogic_Set_hot_SSID(nvr_id, "88888888", 161, 2);*/
		}
		
		{//开启ap
			DBGPRINT("recv  the broadcast id:%s--num:%d.-----------------\n", nvr_id, broadcastCount);
						for(i = 0; i < broadcastCount; i++)
						{
							if(strcmp(stwifiap[i].channelid, myid) == 0)
								{
									currentChannelNum = i;//记录ipc在NVR通道号
									if(strcmp(myapssid, stwifiap[i].wifiapssid) != 0)
										{
											memset(myapssid, 0, sizeof(myapssid));
											memcpy(myapssid, stwifiap[i].wifiapssid, sizeof(stwifiap[i].wifiapssid));
											memset(cmdStr_bro, 0, sizeof(cmdStr_bro));
											sprintf( cmdStr_bro, "echo %s > /mnt/mtd/essidNvr.txt", myapssid);
											DBGPRINT("myapssid:-----%s\n", myapssid);
											sys_command(cmdStr_bro);
											sys_command("chmod 777 /mnt/mtd/essidNvr.txt");

											if(is_wire == 0)
											{//当设备在操作中时不改变
											
												//sleep(12);
												
												ap_change = 1;							
											}
										}

									break;
								}
						}

					if(i == broadcastCount)
						{
							continue;
						}

					for(i = 0; i < broadcastCount; i++)
						{
							if(strcmp(stwifiap[i].wifiapssid, myid) == 0)
								{
									sprintf(tmpStr_chanChannel, "%d", chanChannel[currentChannelNum*2 + 1]);
									if(is_ap == 0)
										{
											count_readytoclose_ap = 0;
											FILE *fp = NULL;
											fp = fopen("/mnt/mtd/openap.txt","w+");
											//fprintf(fp, "%s\n", channel_broadcast_ap);
											fprintf(fp, "%s\n", tmpStr_chanChannel);//i为当前通道对应信道表里的信道号
											fclose(fp);
											sys_command("chmod 777 /mnt/mtd/openap.txt");

											is_ap = 1;
											
											//ap_on();
											//memcpy(channel_current_ap, channel_broadcast_ap, sizeof(channel_broadcast_ap));
											memcpy(channel_current_ap, tmpStr_chanChannel, sizeof(tmpStr_chanChannel));

											DBGPRINT("open the ap, channel:%s--\n", channel_current_ap);
										}

									if(is_ap_on == 1 && strcmp(channel_current_ap, tmpStr_chanChannel) != 0)
									{//判断信道改变时重新开启AP
										//ap_on();
														  
										FILE *fp = NULL;
										fp = fopen("/mnt/mtd/openap.txt","w+");
										fprintf(fp, "%s\n", tmpStr_chanChannel);
										fclose(fp);
										
										DBGPRINT("restart the ap, current channel:%s--now :%s--\n", channel_current_ap, tmpStr_chanChannel);

										memcpy(channel_current_ap, tmpStr_chanChannel, sizeof(tmpStr_chanChannel));
										
										DBGPRINT("restart the ap, current channel:%s--now :%s--\n", channel_current_ap, tmpStr_chanChannel);

										//sleep(3);//直接重启
										//bReset = 1;
										//ap_off();

										is_ap_ch_change = 1;
										
									}
									break;
								}
						}
					if(i == broadcastCount && is_ap == 1)
						{
							DBGPRINT("check_broadcast: - close the ap -\n");
							is_ap = 0;
							//ap_off();
							sys_command("rm -f /mnt/mtd/openap.txt");
							if ( isNormalWifi == 2 )
							{//when camera connect to NVR
								sleep(2);
								return 0;
							}
						}
			sleep(2);
		}//end ap
		
		}

		sleep(1);	
	}
	
	return 0;
}

void threat2(void)
{//开启ap 线程
	SET_PTHREAD_NAME(NULL);
	int a;

	IPCAM_ALL_INFO_ST * pAllInfo_t = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
	if(pAllInfo_t->net_st.enable_wifi_auto_set_by_nvr == 0)
	{
		return;
	}

	while(threat2_start == 0)
		{
			a = 1;
			sleep(1);
			DBGPRINT("\nduring threat2 loopssssssssssssss\n");
		}
	check_broadcast();
}

int cam_recv_broadcast_thread_create(void)
{
	int ret = 0;
	ret = pthread_create(&g_camrecvbroadcastpro, NULL,(void *) threat2,NULL);
	if ( ret < 0 )
	{
		DBGPRINT("cam_recv_broadcast_thread create error\n");
		return -1;
	}
	return 1;
}

void receiveRX(char *wlanX)
{//detect wlanX
	char strRX[256];
	FILE *fp = NULL;

	char cmdStream[256];

	memset(cmdStream, 0, sizeof(cmdStream));
	memset(strRX, 0, sizeof(strRX));
	sprintf( strRX, "ifconfig %s | grep 'RX bytes' | awk -F ' ' '{print $2}'| awk -F : '{print $2}'", wlanX);
	
	fp = popen(strRX, "r");
	if(fp != NULL)
	{
		if(fgets(cmdStream, 256, fp) != NULL)
		{
			DBGPRINT("currentCount:%d - check device %s status: %s.\n", countUpDownModule, wlanX, cmdStream);
		}
	}
	
	pclose(fp);
	
	if(strlen(cmdStream) > 0 && strlen(storeRX) > 0)
		{
			if (strcmp(storeRX, cmdStream) == 0)
				{
					countUpDownModule = countUpDownModule + 2;
				}else{
					countUpDownModule = 0;

					memset(storeRX, 0, sizeof(storeRX));
					memcpy(storeRX, cmdStream, strlen(cmdStream));
				}
		}
	else{
			if (strlen(storeRX) > 0)
			{	
				DBGPRINT("Cannot get %s data.\n", wlanX);
				if (ifExistNetDevice(wlanX) == 0)
					bReset = 1;
			}else{
				memset(storeRX, 0, sizeof(storeRX));
				memcpy(storeRX, cmdStream, strlen(cmdStream));
				}
		}
}

void receiveTX(char *wlanX)
{//detect wlanX
	char strRX[256];
	unsigned long long valueStore;
	unsigned long long valueGet;
	FILE *fp = NULL;

	char cmdStream[256];

	memset(cmdStream, 0, sizeof(cmdStream));
	memset(strRX, 0, sizeof(strRX));
	sprintf( strRX, "ifconfig %s | grep 'TX bytes' | awk -F ' ' '{print $6}'| awk -F : '{print $2}'", wlanX);
	
	fp = popen(strRX, "r");
	if(fp != NULL)
	{
		if(fgets(cmdStream, 256, fp) != NULL)
		{
			DBGPRINT("currentCount:%d - check device %s status: %s.\n", countUpDownModule, wlanX, cmdStream);
		}
	}
	
	pclose(fp);
	
	if(strlen(cmdStream) > 0 && strlen(storeRX) > 0)
		{
			valueGet = strtoull(cmdStream, (char**)NULL, 10);//十进制
			valueStore = strtoull(storeRX, (char**)NULL, 10);
			if (valueGet - valueStore < 60000)// 如果2s内传的数据小于60k
				{
					countUpDownModule = countUpDownModule + 3;//40秒左右
				}else{
					countUpDownModule = 0;

					memset(storeRX, 0, sizeof(storeRX));
					memcpy(storeRX, cmdStream, strlen(cmdStream));
				}
		}
	else{
			if (strlen(storeRX) > 0)
			{	
				DBGPRINT("Cannot get %s data.\n", wlanX);
				if (ifExistNetDevice(wlanX) == 0)
					bReset = 1;
			}else{
				memset(storeRX, 0, sizeof(storeRX));
				memcpy(storeRX, cmdStream, strlen(cmdStream));
				}
		}
}

void detectNetStatus()
{
	countUpDownModule = 0;

	if(is_sta == 1)
		sta_off();
	if(is_ap == 1)
		ap_off();
	
	DBGPRINT("---close ra0 down---");
	sys_command("killall hostapd");
	sys_command("killall wpa_supplicant");
	sys_command("ifconfig ra0 down");
	sys_command("ifconfig ra1 down");
	sys_command("rmmod 8192du");
	sys_command("rmmod 8821au");
	sleep(3);
	sys_command("insmod /mnt/mtd/8192du.ko");
	sys_command("insmod /mnt/mtd/8821au.ko");
	sleep(1);
	sys_command("ifconfig ra0 up");
	sys_command("ifconfig ra1 up");		
	sys_command("brctl addif br0 ra0");
	sys_command("brctl addif br0 ra1");		
	sleep(1);
//	sys_command("hostapd /mnt/mtd/hostapd.conf -B &");
}

int ifExistNetDevice(char *wlanX)
{//detect wlanX
	char strRX[256];
	FILE *fp = NULL;

	char cmdStream[256];

	memset(cmdStream, 0, sizeof(cmdStream));
	memset(strRX, 0, sizeof(strRX));
	sprintf( strRX, "cat /proc/net/dev | grep '%s'", wlanX);
	
	fp = popen(strRX, "r");
	if(fp != NULL)
	{
		if(fgets(cmdStream, 256, fp) != NULL)
		{
			DBGPRINT("Check Device %s status .\n", wlanX);
		}
	}
	
	pclose(fp);
	
	if(strlen(cmdStream) > 0)
		{
			DBGPRINT("Device %s exist .\n", wlanX);
			return 1;
		}
	else{
			DBGPRINT("Device %s not exist .\n", wlanX);
			return 0;
		}
}

//2.4G logic
int check_Wifi_Status();
int kill_Wpa_Supplicant();
int start_Wifi_Connection();

void cam_2_4G_Wired_Or_Wireless_Logic_Func()
{
	SET_PTHREAD_NAME(NULL);
	int tmp = 0;
	int countNum = 0;
	int countRaNum = 0;
	int connectOk = 0;
	int connectOkBefore = 0;
	int tmmp = 0;
	while(1)
	{
		tmp = check_wire();
		if ( bReset == 0 )
		{
			if ( g_wifi_link_flag_ex == 0 && tmp == 0)
			{
				countNum++;
				if (countNum > 1)
				{
					DBGPRINT("**eth0 is offline---eth0 is offline**\n");
					countNum = 0;
					if (g_is_wifi_exist = 1)
					{
						Net_dvr_stop_client();
						sys_command("ifconfig eth0 0.0.0.0 up");
						g_wifi_link_flag_ex = 1;
						gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"ra0");
						MenuNetStaticInit();
						sys_command("ifconfig eth0 0.0.0.0 up");
					}
				}
			}else if ( g_wifi_link_flag_ex == 1 && tmp == 1)
			{
				countNum++;
				if (countNum > 1)
				{
					DBGPRINT("**eth0 is online---eth0 is online**\n");
					Net_dvr_stop_client();
					countNum = 0;
					g_wifi_link_flag_ex = 0;
					sys_command("killall wpa_supplicant");
					memset(parassid, 0, sizeof(parassid));
					
					sys_command("ifconfig ra0 down");
					sys_command("ifconfig ra0 0.0.0.0 up");

					gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"eth0");
					MenuNetStaticInit();
				}
			}

		if ( tmp == 0 &&  g_wifi_link_flag_ex == 1 )
		{
			if ( i_is2_4G == 1)
			{//7601
				tmmp = check_Wifi_Status();
//				tmmp = SH_GetConnStatus("iwpriv ra0 connStatus | grep 'Connected'");
				DBGPRINT("----------ra0 connect result:%d---------\n", tmmp);
				if (tmmp)
				{
					DBGPRINT("----------ra0 connect ok:%d---------\n", tmmp);
					connectOk = 1;
					countRaNum = 0;
				}
				else
				{
					DBGPRINT("----------ra0 connect error:%d---------\n", tmmp);
					connectOk = 0;
				}

				if ( connectOk == 1 && connectOkBefore == 0 )
					connectOkBefore = 1;
					
				if (connectOkBefore == 1 )
				{
					if ( connectOk == 0 )
					{
						countRaNum++;
						if ( countRaNum > 0 )
						{
						DBGPRINT("----------ra0 killall---------\n");
						connectOkBefore = 0;
						//bReset = 1;
						/*sys_command("killall wpa_supplicant");
						sys_command("ifconfig ra0 up");
						MenuNetStaticInit();
						sys_command("wpa_supplicant -Dwext -ira0 -c /mnt/mtd/pc/wifi_wpa.conf -d &");*/
						if ( kill_Wpa_Supplicant() == 1 )
						{
							MenuNetStaticInit();
							//if ( start_Wifi_Connection() == 0 )
							//	bReset = 1;
						}else{
							bReset = 1;
						}
						}
					}
				}
			}else if ( i_is2_4G == 2 )
			{//8188
				connectOk = CheckHaveNetCable("ra0");

				if ( connectOk == 1 && connectOkBefore == 0 )
				{
					connectOkBefore = 1;
					countRaNum = 0;
				}
					
				if (connectOkBefore == 1 )
				{
					if ( connectOk == 0 )
					{
						countRaNum++;
						if ( countRaNum > 0 )
						{
						DBGPRINT("----------ra0 killall---------\n");
						connectOkBefore = 0;
						//bReset = 1;
						/*sys_command("killall wpa_supplicant");
						sys_command("ifconfig ra0 up");
						MenuNetStaticInit();
						sys_command("wpa_supplicant -Dwext -ira0 -c /mnt/mtd/pc/wifi_wpa.conf -d &");*/
						if ( kill_Wpa_Supplicant() == 1 )
						{
							MenuNetStaticInit();
							//if ( start_Wifi_Connection() == 0 )
							//	bReset = 1;
						}else{
							bReset = 1;
						}
						}
					}
				}
			}
		}else
			{
				connectOk = 0;
				connectOkBefore = 0;
			}
		
		}
		sleep(2);
	}
}

void cam_wired_or_wireless_thread_create()
{
	int ret = 0;
	ret = pthread_create(&g_cam2_4GWiredOrWirelesspro, NULL,(void *) cam_2_4G_Wired_Or_Wireless_Logic_Func,NULL);
	if ( ret < 0 )
	{
		DBGPRINT("cam_2_4G_Wired_Or_Wireless_Logic_Func create error\n");
		return -1;
	}
	return 1;
}

//default mode is AP mode

void cam_2_4G_ap_on(void)
{//kill all hostapd relative
	sys_command("killall hostapd");
	sys_command("ifconfig ra0 up");//8188 only have one interface ra0

//get local device ID
	int i, j, num = 20, tmpInt;
	char mydomain[20];

	gstCommonParam->GST_NET_get_svr_mac_id(mydomain, myid);

	for(i = 0; i < num; i++)
		{	
			if(myid[i] >= '0' && myid[i] <= '9')
				{
					continue;
				}else
					{
						for(j = i; j < num - 1; j++)
							{
								myid[j] = myid[j + 1];
							}
						i = i - 1;
						num = num - 1;
					}
		}

	sscanf(myid, "%d", &tmpInt);
	sprintf(myid, "%08X", tmpInt);
	
	DBGPRINT("\nthreat1 -%X-%s this is no.%d\n", tmpInt, myid, i);
//get local device ID end
	
	DBGPRINT("-----start open_test_ap-----\n");

	int APchannel = -11, t_channel;
	//int ht_way1, ht_way2, ht_way3, ht_way4;

	char channel[4];

	/*{
		memset(channel, 0, sizeof(channel));		
		if(iw_get_nvr_set("/mnt/mtd/openap.txt", channel))
		{
			sscanf(channel, "%d", &APchannel);
			DBGPRINT("Current AP channel is -- %s -to-%d--\n", channel, APchannel);
		}
	}*/
	
	{
		FILE *hostapd_conf = NULL;
        
        hostapd_conf = fopen("/mnt/mtd/hostapd.conf","w+");

        if(hostapd_conf == NULL)
        {
        	DBGPRINT("open hostapd.conf error\n");
			return;
	 }

            fprintf(hostapd_conf, "#################### hostapd configuration file #################### \n");
            fprintf(hostapd_conf, "ctrl_interface=/var/run/hostapd\n");
            fprintf(hostapd_conf, "interface=ra0\n");			
            //fprintf(hostapd_conf, "bridge=br0\n");			
            fprintf(hostapd_conf, "ssid=%s\n", myid);

		if(APchannel < 0)
			t_channel = -APchannel;
		else 
			t_channel = APchannel;

		/*if(APchannel == 24)
			t_channel = 165;
		else if(APchannel >= 0 && APchannel < 8)
		{
			t_channel = 36 + APchannel*4;
		}else if(APchannel >=8 && APchannel < 20)
		{
			t_channel = 100 + (APchannel - 8)*4;
		}else if(APchannel >= 20 && APchannel < 24)
		{
			t_channel = 149 + (APchannel - 20)*4;
		}*/
		
            fprintf(hostapd_conf, "channel=%d\n", t_channel);	

            //fprintf(hostapd_conf, "country_code=US\n");
            fprintf(hostapd_conf, "wpa=2\n");
            fprintf(hostapd_conf, "wpa_passphrase=%d\n", 88888888);
            fprintf(hostapd_conf, "\n");
            fprintf(hostapd_conf, "\n");
            fprintf(hostapd_conf, "#################### default configuration #################### \n");
            fprintf(hostapd_conf, "driver=rtl871xdrv\n");
            fprintf(hostapd_conf, "beacon_int=100\n");
            fprintf(hostapd_conf, "hw_mode=g\n");
           // fprintf(hostapd_conf, "auth_algs= 1\n");			
           // fprintf(hostapd_conf, "ieee80211n=1\n");
           // fprintf(hostapd_conf, "ieee80211d=1\n");
           // fprintf(hostapd_conf, "wme_enabled=1\n");	

		/*ht_way1 = (t_channel - 36) % 8;
		ht_way2 = (t_channel - 40) % 8;
		ht_way3 = (t_channel - 149) % 8;
		ht_way4 = (t_channel - 153) % 8;
		
		if(t_channel == 165)
			fprintf(hostapd_conf, "ht_capab=[SHORT-GI-40][DSSS_CCK-40]\n");
		else if(ht_way1 == 0 || ht_way3 == 0)
			fprintf(hostapd_conf, "ht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]\n");
		else if(ht_way2 == 0 || ht_way4 == 0)
			fprintf(hostapd_conf, "ht_capab=[HT40-][SHORT-GI-40][DSSS_CCK-40]\n");*/
		
            fprintf(hostapd_conf, "wpa_key_mgmt=WPA-PSK\n");
            fprintf(hostapd_conf, "wpa_pairwise=CCMP\n");			
		fprintf(hostapd_conf, "ignore_broadcast_ssid=0\n");//no显示ssid

		fclose(hostapd_conf);

		sys_command("chmod 777 /mnt/mtd/hostapd.conf");
	}

	DBGPRINT("Current AP channel is --%d--\n", t_channel);
	sys_command("ifconfig ra0 up");
	sys_command("hostapd /mnt/mtd/hostapd.conf -B &");
	is_ap_on = 1;
	threat2_start=1;
}

void cam_new_mode()
{//if module is 2.4G
	int tmp = 0;

	char tmpStr[20];
	//检测是否为热点
	if ( iw_get_nvr_set("/mnt/mtd/openap.txt", tmpStr) )
	{
		is_ap = 1;//启动时判断是否需要开启AP
	}
	else{
		is_ap = 0;
		}
	if ( is_ap == 1)
	{
		isNormalWifi = 2;//8188eu		

		//open ap
		cam_2_4G_ap_on();
		//open check broadcast
		cam_recv_broadcast_thread_create();
	}
}

//DST---START
META_DST_ELEMENT g_dstSt;
void InitDstParams()
{
	int needSave = 1;
	int tmp = 0, ret = 0, port = 0;
	unsigned long timeL = 0;
	U8 tmp1=0;
	char str[100]={0};
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTON",str,1);
	if (!ret)
	{
		tmp = atoi(str);
//		DPRINTK("NTPON:%s,%d\n",str,tmp);
		if (tmp == 1)
			g_dstSt.m_DST_On= 1;
		else
			g_dstSt.m_DST_On = 0;
	}
	else
	{
		g_dstSt.m_DST_On = 0;
		DPRINTK("Read [DSTMENU] DSTON fail!\n");
	}

	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTTYPE",str,1);
	if (!ret)
	{
		tmp = atoi(str);
//		DPRINTK("DSTTYPE:%s,%d\n",str,tmp);
		if (tmp == 1)
			g_dstSt.m_DST_Type = 1;
		else
			g_dstSt.m_DST_Type = 0;
	}
	else
	{
		g_dstSt.m_DST_Type = 0;
		DPRINTK("Read [DSTMENU] DSTTYPE fail!\n");
	}

	if (1)//g_dstSt.m_DST_Type)
	{
		ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEYEARS",str,10);
		if (!ret)
		{
			timeL = strtoul(str, NULL, 0);
			DPRINTK("DSTDATEYEARS:%s,%d\n",str,tmp);

			struct tm tp;
			gmtime_r(&timeL,&tp);

			g_dstSt.m_DST_Date_StartTime.u16Year = tp.tm_year;
			g_dstSt.m_DST_Date_StartTime.u8Month = tp.tm_mon;
			g_dstSt.m_DST_Date_StartTime.u8Day = tp.tm_mday;
			g_dstSt.m_DST_Date_StartTime.u8Hour = tp.tm_hour;
			g_dstSt.m_DST_Date_StartTime.u8Minute = tp.tm_min;
			g_dstSt.m_DST_Date_StartTime.u8Second = tp.tm_sec;	
		}
		else
		{
			g_dstSt.m_DST_Date_StartTime.u16Year = 70;
			g_dstSt.m_DST_Date_StartTime.u8Month = 0;
			g_dstSt.m_DST_Date_StartTime.u8Day = 1;
			g_dstSt.m_DST_Date_StartTime.u8Hour = 1;
			g_dstSt.m_DST_Date_StartTime.u8Minute = 1;
			g_dstSt.m_DST_Date_StartTime.u8Second = 0;	
			DPRINTK("Read [DSTMENU] DSTDATEYEARS fail!\n");
		}

		ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEYEARE",str,10);
		if (!ret)
		{
			timeL = strtoul(str, NULL, 0);
			DPRINTK("DSTDATEYEARS:%s,%d\n",str,tmp);

			struct tm tp;
			gmtime_r(&timeL,&tp);

			g_dstSt.m_DST_Date_EndTime.u16Year = tp.tm_year;
			g_dstSt.m_DST_Date_EndTime.u8Month = tp.tm_mon;
			g_dstSt.m_DST_Date_EndTime.u8Day = tp.tm_mday;
			g_dstSt.m_DST_Date_EndTime.u8Hour = tp.tm_hour;
			g_dstSt.m_DST_Date_EndTime.u8Minute = tp.tm_min;
			g_dstSt.m_DST_Date_EndTime.u8Second = tp.tm_sec;	
		}
		else
		{
			g_dstSt.m_DST_Date_EndTime.u16Year = 70;//1970
			g_dstSt.m_DST_Date_EndTime.u8Month = 0;
			g_dstSt.m_DST_Date_EndTime.u8Day = 1;
			g_dstSt.m_DST_Date_EndTime.u8Hour = 1;
			g_dstSt.m_DST_Date_EndTime.u8Minute = 1;
			g_dstSt.m_DST_Date_EndTime.u8Second = 0;	
			DPRINTK("Read [DSTMENU] DSTDATEYEARE fail!\n");
		}
	}

	if (needSave)
	{
		SaveDstParams();
	}
}

void SetDstParams(META_DST_ELEMENT *pDstSt)
{
	memcpy(&g_dstSt, pDstSt, sizeof(META_DST_ELEMENT));
	SaveDstParams();
	return;
}

int SaveDstParams()
{
	time_t timet;
	char str[50] = {0};
	if (g_dstSt.m_DST_On)
		str[0] = '1';
	else
		str[0] = '0';
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTON",str);

	if (g_dstSt.m_DST_Type)
		str[0] = '1';
	else
		str[0] = '0';
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTTYPE",str);

	memset(str,0,sizeof(str));
	timet = gstCommonParam->GST_FTC_CustomTimeTotime_t(g_dstSt.m_DST_Date_StartTime.u16Year,//1970
			g_dstSt.m_DST_Date_StartTime.u8Month,
			g_dstSt.m_DST_Date_StartTime.u8Day,
			g_dstSt.m_DST_Date_StartTime.u8Hour,
			g_dstSt.m_DST_Date_StartTime.u8Minute,
			g_dstSt.m_DST_Date_StartTime.u8Second);
	sprintf(str, "%lu", timet);
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEYEARS",str);
	
	memset(str,0,sizeof(str));
	timet = gstCommonParam->GST_FTC_CustomTimeTotime_t(g_dstSt.m_DST_Date_EndTime.u16Year,//1970
			g_dstSt.m_DST_Date_EndTime.u8Month,
			g_dstSt.m_DST_Date_EndTime.u8Day,
			g_dstSt.m_DST_Date_EndTime.u8Hour,
			g_dstSt.m_DST_Date_EndTime.u8Minute,
			g_dstSt.m_DST_Date_EndTime.u8Second);
	sprintf(str, "%lu", timet);
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEYEARE",str);

	return 1;
}
//DST---END

//NTP---START
ValNtp g_ntpSt;
void InitNtpParams()
{
	int needSave = 0;
	int tmp = 0, ret = 0, port = 0;
	U8 tmp1=0;
	char str[100]={0};
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPON",str,1);
	if (!ret)
	{
		tmp = atoi(str);
//		DPRINTK("NTPON:%s,%d\n",str,tmp);
		if (tmp == 1)
			g_ntpSt.NtpOn = 1;
		else
			g_ntpSt.NtpOn = 0;
	}
	else
	{
		g_ntpSt.NtpOn = 0;
		DPRINTK("Read [NTPMENU] NTPON fail!\n");
	}

	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPADDR",str,30);
	if (!ret)
	{
//		DPRINTK("NTPADDR:%s\n",str);
		strcpy(g_ntpSt.ServerAddr, str);
	}
	else
		strcpy(g_ntpSt.ServerAddr, "time.windows.com");

	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPPORT",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("NTPPORT:%s,%d\n",str,port);
		g_ntpSt.NtpPort = port;
	}
	else
	{
		g_ntpSt.NtpPort = 123;
		DPRINTK("Read [NTPMENU] NTPPORT fail!\n");
	}

	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPTIMEZONEPOS",str,8);
	if (!ret)
	{
		tmp = atoi(str);
//		DPRINTK("NTPTIMEZONEPOS:%s,%d\n",str,tmp);
		if (tmp<35)
			g_ntpSt.TimeZonePos = tmp;
		else
			g_ntpSt.TimeZonePos = 13;
			
	}
	else
	{
		g_ntpSt.TimeZonePos = 13;
		DPRINTK("Read [NTPMENU] NTPTIMEZONEPOS fail!\n");
	}

	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPTIMETEV",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("NTPTIMETEV:%s,%d\n",str,port);
		g_ntpSt.TimeTev = port;
	}
	else
	{
		g_ntpSt.TimeTev = 10*60;
		DPRINTK("Read [NTPMENU] NTPTIMETEV fail!\n");
	}

	if (g_ntpSt.NtpPort < 0 || g_ntpSt.NtpPort > 65535)
	{
		g_ntpSt.NtpPort = 123;
		needSave = 1;
	}

	if (g_ntpSt.TimeZonePos < 0 || g_ntpSt.TimeZonePos > 34)
	{
		g_ntpSt.TimeZonePos = 13;//东八区
		needSave = 1;
	}
	if (g_ntpSt.NtpSec < 0)
	{
		g_ntpSt.NtpSec = 8*3600;
		needSave = 1;
	}

	if (strlen(g_ntpSt.ServerAddr) <= 0)
	{
		strcpy(g_ntpSt.ServerAddr, "time.windows.com");
		needSave = 1;
	}

	if (g_ntpSt.TimeTev < 0 || g_ntpSt.TimeTev > 65535)
	{
		g_ntpSt.TimeTev = 10*60;
		needSave = 1;
	}

	//get time diff.
	g_ntpSt.NtpSec = GetNtpTimeDiffSec(g_ntpSt.TimeZonePos);

	InitDstParams();

	Xdvr_StartNtpFunc(g_ntpSt);

	if (needSave)
	{
		SaveNtpParams();
	}
}

void SetNtpParams(ValNtp *pNtpSt)
{
	memcpy(&g_ntpSt, pNtpSt, sizeof(ValNtp));

	if (g_ntpSt.NtpPort < 0 || g_ntpSt.NtpPort > 65535)
		g_ntpSt.NtpPort = 123;

	if (g_ntpSt.TimeZonePos < 0 || g_ntpSt.TimeZonePos > 34)
		g_ntpSt.TimeZonePos = 13;//东八区
	if (g_ntpSt.NtpSec < 0)
		g_ntpSt.NtpSec = 8*3600;

	if (strlen(g_ntpSt.ServerAddr) <= 0)
		strcpy(g_ntpSt.ServerAddr, "time.windows.com");

	if (g_ntpSt.TimeTev < 0 || g_ntpSt.TimeTev > 65535)
		g_ntpSt.TimeTev = 10*60;

	SaveNtpParams();
	Xdvr_StartNtpFunc(g_ntpSt);
}

int SaveNtpParams()
{
	char str[50] = {0};
	if (g_ntpSt.NtpOn)
		str[0] = '1';
	else
		str[0] = '0';
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPON",str);

	memset(str,0,sizeof(str));
	sprintf(str, "%s", g_ntpSt.ServerAddr);
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPADDR",str);

	memset(str,0,sizeof(str));
	sprintf(str, "%d", g_ntpSt.NtpPort);
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPPORT",str);

	memset(str,0,sizeof(str));
	sprintf(str, "%d", g_ntpSt.TimeZonePos);
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPTIMEZONEPOS",str);

	memset(str,0,sizeof(str));
	sprintf(str, "%d", g_ntpSt.TimeTev);
	SetValueToEtcFile("/mnt/mtd/ntp_dst.txt","NTPMENU","NTPTIMETEV",str);

	return 1;
}

void  DevCtrl_Time_tToCustomTime(time_t time,int * year, int * month,int* day,int* hour,int* minute,int* second )
{	
	 struct tm tp;
	
	 gmtime_r(&time,&tp);

	 *year = tp.tm_year + 1900-2000;
	 *month = tp.tm_mon + 1;
	 *day = tp.tm_mday;
	 *hour = tp.tm_hour;
	 *minute = tp.tm_min;
	 *second = tp.tm_sec;		
}

void ntp_test_callback(unsigned long ntp_timezone_time)
{
	int year,month,day,hour,minute,second;
	int recFlag = 0, iCount = 0;
	//time_t timet;
 	struct timeval tv;

	U8 i = 0, ret = 0, tmp;
	U8 buf[7] = {0};
	U8 tBuf[7] = {0};
	
	DevCtrl_Time_tToCustomTime(ntp_timezone_time,&year,&month,&day,&hour,&minute,&second);

	gettimeofday( &tv, NULL );

	printf("now:%ld__callback ntp_timezone_time=%ld_  %d-%d-%d %d:%d:%d\n",tv.tv_sec,ntp_timezone_time,year,month,day,hour,minute,second);
	if (abs(ntp_timezone_time - tv.tv_sec) < 20)
	{
		printf("____interval is in tolerant!____\n");
		return;
	}

	buf[6] = year;
	buf[5] = month;
	buf[4] = day;
	buf[2] = hour;
	buf[1] = minute;
	buf[0] = second;

	tBuf[6] = buf[6]+100;
	tBuf[5] = buf[5]-1;
	tBuf[4] = buf[4];
	tBuf[2] = buf[2];
	tBuf[1] = buf[1];
	tBuf[0] = buf[0];

	func_Dst_Check(ntp_timezone_time, tBuf);
	printf("after_dst_change__  %d-%d-%d %d:%d:%d\n",buf[6],buf[5],buf[4],buf[2],buf[1],buf[0]);

	tmp = rec_on;
	if (tmp)
		stop_key_set();
	setsystem(buf);

	if (tmp)
		record_key_set();
	{
		if(g_rtc_fd>0)
		{
			for(i=0;i<3;i++)
			{
				hi_tm.year=2000+buf[6];
				hi_tm.month=buf[5];
				hi_tm.date=buf[4]; 
				hi_tm.hour=buf[2];
				hi_tm.minute=buf[1];
				hi_tm.second=buf[0];		
				ret = ioctl(g_rtc_fd, HI_RTC_SET_TIME, &hi_tm);
				if(ret<0)
						printf("ioctl: HI_RTC_SET_TIME failed\n");
				usleep(5000);
			}
		}
	}
}

int Xdvr_StartNtpFunc(ValNtp ntpSt)
{//timezone 单位为小时
	printf("test start !\n");
	
	int ret = 0;
	NTP_TIMEZONE_ST timezone;

	memset(&timezone, 0, sizeof(timezone));
	
	timezone.use_ntp = ntpSt.NtpOn;
	timezone.refress_time_sec = ntpSt.TimeTev;//24*60*60;// 1天自动对时一次
	timezone.time_zone_sec = ntpSt.NtpSec;//时区偏移
	strcpy(timezone.ntp_server_url,"time.windows.com");
	printf("****timezone url = %s******\n", timezone.ntp_server_url);
	timezone.ntp_server_port = ntpSt.NtpPort;

	ret = start_ntp_server(ntp_test_callback);
	if (ret < 0)
	{
		printf("---start ntp thread error!---\n");
	}else if (ret == 1)
	{
		printf("---start ntp thread already!---\n");
	}

	ntp_set_parameters(timezone);//?′ê±??ê±μ?ê±oòμ÷ó?′?oˉêy

	/*目测不用等待。
	if (ret < 0 || ret == 1)
		return -1;
	else
	{
		while(1)
			sleep(2);
	}*/
	return 0;
}

int GetNtpTimeDiffSec(int zonePos)
{
	int tDiffSec = 0;
	switch(zonePos)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			tDiffSec = zonePos*3600;
			break;
		case 4:
			tDiffSec = (zonePos-1)*3600+1800;
			break;
		case 5:
			tDiffSec = (zonePos-1)*3600;
			break;
		case 6:
			tDiffSec = (zonePos-2)*3600+1800;
			break;
		case 7:
			tDiffSec = (zonePos-2)*3600;
			break;
		case 8:
			tDiffSec = (zonePos-3)*3600+1800;
			break;
		case 9:
			tDiffSec = (zonePos-4)*3600+2700;
			break;
		case 10:
			tDiffSec = (zonePos-4)*3600;
			break;
		case 11:
			tDiffSec = (zonePos-5)*3600+1800;
			break;
		case 12:
		case 13:
		case 14:
			tDiffSec = (zonePos-5)*3600;
			break;
		case 15:
			tDiffSec = (zonePos-6)*3600+1800;
			break;
		case 16:
		case 17:
		case 18:
		case 19:
			tDiffSec = (zonePos-6)*3600;
			break;
		case 20:
		case 21:
		case 22:
			tDiffSec = -(zonePos-19)*3600;
			break;
		case 23:
			tDiffSec = -(zonePos-20)*3600-1800;
			break;
		case 24:
			tDiffSec = -(zonePos-20)*3600;
			break;
		case 25:
			tDiffSec = -(zonePos-21)*3600-1800;
			break;
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
			tDiffSec = -(zonePos-21)*3600;
			break;
		default:
			tDiffSec = 0;
			break;
	}

	return tDiffSec;
}
//NTP---END

void getSelfID(char *selfID)
{
	int i = 0, j = 0, num = 20, tmpInt = 0;
	char tIDStr[20], mydomain[20];
	gstCommonParam->GST_NET_get_svr_mac_id(mydomain, tIDStr);

	for(i = 0; i < num; i++)
		{	
			if(tIDStr[i] >= '0' && tIDStr[i] <= '9')
				{
					continue;
				}else
					{
						for(j = i; j < num - 1; j++)
							{
								tIDStr[j] = tIDStr[j + 1];
							}
						i = i - 1;
						num = num - 1;
					}
		}

	sscanf(tIDStr, "%d", &tmpInt);
	sprintf(selfID, "%08X", tmpInt);
	return;
}
