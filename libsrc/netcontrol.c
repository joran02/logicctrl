#include "cmn.h"
#include "ptz.h"
#include "IpcamShowMsgInfo.h"

#define SetCAM			66+100
#define GetCAM			67+100

#define CAMSET_Brightness 68+100
#define CAMSET_Hue 69+100
#define CAMSET_Contrast 70+100
#define CAMSET_Saturation 71+100
#define CAMSET_Hdelay 72+100
#define CAMSET_Vdelay 73+100
#define CAMSET_Left 74+100
#define CAMSET_Right 75+100
#define CAMSET_Up 76+100
#define CAMSET_Down 77+100
#define CAMSET_ALL 78+100



#define GetALARM 79+100
#define SetALARM 80+100

#define GetISALARM 81+100

#define GetDaySD	82+100
#define GetNightSD	83+100

#define GetNvrInfo 1+200

#define FormatTF 2+200

#define CAMSET_BACK_LIGHT 4+200

typedef struct
{
	int resolution;
	int Brightness;
	int Hue;
	int Contrast;
	int Saturation;
	int Hdelay;
	int Vdelay;
	int Left;
	int Right;
	int Up;
	int Down;
	int dayval;
	int nightval;
	int backlight;
}CamStruct;

typedef struct
{
int resolution;
int Sensitivity;
int Left;
int Right;
int Up;
int Down;
}AlarmStruct;

CamStruct netcamval;
AlarmStruct netmotionval;
NVRInfoStruct sysinfostr;
extern int fdcom;

extern unsigned char camtitle_buf[];
extern void camtitle_char_line_display(char *str);

unsigned char camtitle_bitmap_buf[100*24*2+3];

int cam_name_set_flag=0;

void rec_parameter_get(REC_PARA *para)
{
	int i;
//	for (i=0;i<Cam_Num+1;i++)
	for (i=0;i<Cam_Num;i++)
	{
		para->PicQty[i]=RecamMenu[i].PicQty;
		para->FrmRate[i]=RecamMenu[i].FrmRate;
		para->Resolution=RecamMenu[i].Reselution;
	}
}

void rec_parameter_set(REC_PARA *para)
{
	int i;
	U8 rec_succeed=0;

//	printf("rec_parameter_set s\n");
//	for (i=0;i<Cam_Num+1;i++)
	for (i=0;i<Cam_Num;i++)
	{
//		printf("i:%d\n",i);
		if(RecamMenu[i].PicQty!=para->PicQty[i])
		{
			RecamMenu[i].PicQty=para->PicQty[i];
			I_write_block_eeprom(ADDRECAM+i*ADDCOUNTRECCAM+RECPICQTY,1,&RecamMenu[i].PicQty);
		}
		if(RecamMenu[i].FrmRate!=para->FrmRate[i])
		{
			RecamMenu[i].FrmRate=para->FrmRate[i];
			I_write_block_eeprom(ADDRECAM+i*ADDCOUNTRECCAM+FRMRATE,1,&RecamMenu[i].FrmRate);
		}
		if(RecamMenu[i].Reselution!=para->Resolution)
		{
			RecamMenu[i].Reselution=para->Resolution;
			I_write_block_eeprom(ADDRECAM+i*ADDCOUNTRECCAM+RESELUTION,1,&RecamMenu[i].Reselution);
			bReset = 1;
		//reboot
		}
	}
	if(rec_on)
	{
		if (gstCommonParam->GST_FTC_StopRec()<0)
		{
//			bReset = 1;
		}
		else
		{
			RecorSet();
			while(!rec_succeed)
			{
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",reccam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",reccam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",reccam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",reccam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",reccam);
				if (gstCommonParam->GST_FTC_StartRec(reccam,100)>0) 
				{
					rec_succeed =1;
					printf("Record is sucess!\n");
				}
				else
				{
					rec_succeed = 0;
					sleep(5);
				}
			}
		}
	}
//	printf("rec_parameter_set e\n");
	
}

void ptz_parameter_get(PTZ_PARA *para)
{
	int MenuPTZNum;

	
	for(MenuPTZNum=0;MenuPTZNum<Cam_Num;MenuPTZNum++)
	{
		para->BaudRate[MenuPTZNum]=PtzMenu[MenuPTZNum].BaudRate;
		para->DeviceID[MenuPTZNum]=PtzMenu[MenuPTZNum].DeviceID;
		para->Protocol[MenuPTZNum]=PtzMenu[MenuPTZNum].Protocol;
	}
}

void ptz_parameter_set(PTZ_PARA *para)
{
	int MenuPTZNum;

//	printf("ptz_parameter_set s\n");
	for(MenuPTZNum=0;MenuPTZNum<Cam_Num;MenuPTZNum++)
	{
//		printf("MenuPTZNum:%d\n",MenuPTZNum);
		if(PtzMenu[MenuPTZNum].BaudRate!=para->BaudRate[MenuPTZNum])
		{
			PtzMenu[MenuPTZNum].BaudRate=para->BaudRate[MenuPTZNum];
			I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*MenuPTZNum+BAUDRATE,1,&PtzMenu[MenuPTZNum].BaudRate);
		}
		if(PtzMenu[MenuPTZNum].DeviceID!=para->DeviceID[MenuPTZNum])
		{
			PtzMenu[MenuPTZNum].DeviceID=para->DeviceID[MenuPTZNum];
			I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*MenuPTZNum+DEVICEID,1,&PtzMenu[MenuPTZNum].DeviceID);
		}
		if(PtzMenu[MenuPTZNum].Protocol!=para->Protocol[MenuPTZNum])
		{
			PtzMenu[MenuPTZNum].Protocol=para->Protocol[MenuPTZNum];		
			I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*MenuPTZNum+PROTOCOL,1,&PtzMenu[MenuPTZNum].Protocol);
		}
	}
//	if(PTZ_ON)
	{
		net_ptz_reconfig();
	}
//	printf("ptz_parameter_set e\n");
}

void cam_parameter_get(CAM_PARA *para)
{
	int MenuCam_Num;
	for(MenuCam_Num=0;MenuCam_Num<Cam_Num;MenuCam_Num++)
	{
		para->Brightness[MenuCam_Num]=CamMenu[MenuCam_Num].Brightness;
		para->Hue[MenuCam_Num]=CamMenu[MenuCam_Num].Hue;
		para->Contrast[MenuCam_Num]=CamMenu[MenuCam_Num].Contrast;
		para->Satutrat[MenuCam_Num]=CamMenu[MenuCam_Num].Satutrat;
		para->PicHdel[MenuCam_Num]=CamMenu[MenuCam_Num].PicHdel;
		para->PicVdel[MenuCam_Num]=CamMenu[MenuCam_Num].PicVdel;
		para->SpotOutputTime[MenuCam_Num]=CamMenu[MenuCam_Num].SpotTime;
	}
}

void cam_parameter_set(CAM_PARA *para)
{
	int MenuCam_Num;


//	printf("cam_parameter_set s\n");
	for(MenuCam_Num=0;MenuCam_Num<Cam_Num;MenuCam_Num++)
	{
//		printf("MenuCam_Num:%d\n",MenuCam_Num);
		if(CamMenu[MenuCam_Num].Brightness!=para->Brightness[MenuCam_Num])
		{
			CamMenu[MenuCam_Num].Brightness=para->Brightness[MenuCam_Num];
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*MenuCam_Num+BRIGHTNESS,1,&CamMenu[MenuCam_Num].Brightness);
			Brightness_Ajust(MenuCam_Num,CamMenu[MenuCam_Num].Brightness);
		}
		if(CamMenu[MenuCam_Num].Hue!=para->Hue[MenuCam_Num])
		{
			CamMenu[MenuCam_Num].Hue=para->Hue[MenuCam_Num];
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*MenuCam_Num+HUE,1,&CamMenu[MenuCam_Num].Hue);
			Hue_Ajust(MenuCam_Num,CamMenu[MenuCam_Num].Hue);
		}
		if(CamMenu[MenuCam_Num].Contrast!=para->Contrast[MenuCam_Num])
		{
			CamMenu[MenuCam_Num].Contrast=para->Contrast[MenuCam_Num];
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*MenuCam_Num+CONTRAST,1,&CamMenu[MenuCam_Num].Contrast);
			Contrast_Ajust(MenuCam_Num,CamMenu[MenuCam_Num].Contrast);
		}
		if(CamMenu[MenuCam_Num].Satutrat!=para->Satutrat[MenuCam_Num])
		{
			CamMenu[MenuCam_Num].Satutrat=para->Satutrat[MenuCam_Num];
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*MenuCam_Num+SATURAT,1,&CamMenu[MenuCam_Num].Satutrat);
			Stauration_Ajust(MenuCam_Num,CamMenu[MenuCam_Num].Satutrat);
		}
		if(CamMenu[MenuCam_Num].PicHdel!=para->PicHdel[MenuCam_Num])
		{
//			CamMenu[MenuCam_Num].PicHdel=para->PicHdel[MenuCam_Num];
//			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*MenuCam_Num+PICHDEL,1,&CamMenu[MenuCam_Num].PicHdel);
//			Hdelay_Ajust(MenuCam_Num,CamMenu[MenuCam_Num].PicHdel);
		}
		if(CamMenu[MenuCam_Num].PicVdel!=para->PicVdel[MenuCam_Num])
		{
//			CamMenu[MenuCam_Num].PicVdel=para->PicVdel[MenuCam_Num];
//			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*MenuCam_Num+PICVDEL,1,&CamMenu[MenuCam_Num].PicVdel);
//			Vdelay_Ajust(MenuCam_Num,CamMenu[MenuCam_Num].PicVdel);
		}
		if(CamMenu[MenuCam_Num].SpotTime!=para->SpotOutputTime[MenuCam_Num])
		{
			CamMenu[MenuCam_Num].SpotTime=para->SpotOutputTime[MenuCam_Num];
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*MenuCam_Num+SPOTTIME,1,&CamMenu[MenuCam_Num].SpotTime);
		}
	}
//	printf("cam_parameter_set e\n");
}

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


U8 netbuf[ADDCOUNTNETSTIC];

int CheckReset(void)
{
	int i,size,flag=0;
	unsigned char val[ADDCOUNTNETSTIC];
	

	memset(val,0,sizeof(val));
	I_read_byte_eeprom(ADDSYS+VIDEOFORMAT, val);
	if (SysMenu.VideoFormat != val[0])
		flag = 1;
	else
		I_read_block_eeprom(ADDSYS+TIMFORMAT,sizeof(SysMenu),&SysMenu.tmregion);
	
	{
			IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_SYS_ST *pSys_st=&pAllInfo->sys_st;
									
			if(val[0])
				pSys_st->ipcam_stardard=0;
			else 
				pSys_st->ipcam_stardard=1;
			
			gstCommonParam->GST_DRA_Hisi_save_ipcam_config_st(pAllInfo);
								
	}
	
	memset(val,0,sizeof(val));
	I_read_byte_eeprom(ADDRECAM+ADDCOUNTRECCAM,val);
	if (RecamMenu[0].Reselution != val[0])
		flag = 1;
	else
	{
		memset(val,0,sizeof(val));
		I_read_block_eeprom(ADDRECAM,16,val);
		if (val[1] != RecamMenu[0].PicQty || val[2] != RecamMenu[0].FrmRate||val[3] != RecamMenu[0].AudioOnoff)
		{
			I_read_block_eeprom(ADDRECAM,sizeof(RecamMenu[0]),&RecamMenu[0].Reselution);
			{//设置码流到公用数据中
				IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
				IPCAM_VCODE_VS_ST vcode;

				vcode = pAllInfo->onvif_st.vcode[0];

				switch(RecamMenu[0].PicQty)
				{
					case 0: vcode.BitrateLimit = 1024;
						break;
					case 1: vcode.BitrateLimit = 2048;
						break;
					case 2: vcode.BitrateLimit = 4096;
						break;
					default: vcode.BitrateLimit = 2048;
						break;
				}

				vcode.FrameRateLimit = RecamMenu[0].FrmRate;	

				if( vcode.FrameRateLimit < 5 )
					vcode.FrameRateLimit = 5;

				 pAllInfo->onvif_st.vcode[0] = vcode;

				gstCommonParam->GST_DRA_Hisi_save_ipcam_config_st(pAllInfo);				

				DPRINTK("save [0] encode bit=%d framerate=%d\n",vcode.BitrateLimit,vcode.FrameRateLimit );
				
			}


			
			RecorSet();
			
		}
	}

	MenuSCHInit();
	MenuPtzInit();
	MenuAlarm_Initial();
	MenuEmailInit();
	
	I_read_block_eeprom(ADDNSTC,ADDCOUNTNETSTIC,val);
	for (i=0;i<ADDCOUNTNETSTIC;i++)
	{
		if (val[i] != netbuf[i])
		{
			I_read_block_eeprom(ADDNSTC,sizeof(ValNetStatic),&NetStaticMenu.IPAlloction);
			if(net_cfg_save(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)<0)
				printf("net_cfg_save false\n");
			flag=1;
			break;
		}
	}
	return flag;
}

extern U8 AllCam;
extern U8 tEEPROM[0x700];
extern int g_net_motion_flag;
//extern int wdgflag;
int buflenth;
int sendalarmnet=0;
void CamArea_Setup();
void netsetmotion();
void SSet_gui_msg(char * msg)
{
	U8 tmp,buf[7];
	U16 i;
	int ret;

	if( GST_MENU_gui_msg_set_chan_bitmap(msg) > 0 )
	{
		return ;
	}
	switch (msg[0])
	{
		case mTEST:
			memcpy(msg,&SysMenu.Laguage,sizeof(U8));
			buflenth = sizeof(U8);
			break;
		case GetNetSetup:
//			wdgflag=1;
			I_read_block_eeprom(0, sizeof(tEEPROM), (U8*)msg);
			readmsg(&msg[sizeof(tEEPROM)]);
			memcpy(&msg[sizeof(tEEPROM)+sizeof(ValRecSchedule)*(CHANNELNUM+1)+1],MenuAlarm_Date,sizeof(MenuAlarm_Date));
			buflenth = sizeof(MenuAlarm_Date)+sizeof(tEEPROM)+sizeof(ValRecSchedule)*(CHANNELNUM+1)+1;
//			wdgflag=0;
			break;
		case SaveNetSetup:
//			wdgflag=1;
			memset(netbuf,0,sizeof(netbuf));
			I_read_block_eeprom(ADDNSTC,ADDCOUNTNETSTIC,netbuf);
			I_write_block_eeprom(0, sizeof(tEEPROM), (U8*)&msg[sizeof(int)+1]);
			memcpy(&NetStaticMenu.IPAlloction,&msg[ADDNSTC],sizeof(ValNetStatic));
			writemsg(&msg[sizeof(int)+1+sizeof(tEEPROM)]);
//			DBGPRINT("********** vvvvvvvvvvvvvvvvv :%d,%d\n",msg[sizeof(int)+1+sizeof(tEEPROM)],msg[sizeof(int)+1+sizeof(tEEPROM)+1]);
//			DBGPRINT("************************* kkkkkkkkkkk :%d,%d\n",SchedulMenu[0].All_Date,SchedulMenu[1].All_Date);
			memcpy(MenuAlarm_Date,&msg[sizeof(tEEPROM)+sizeof(ValRecSchedule)*(CHANNELNUM+1)+2+sizeof(int)],sizeof(MenuAlarm_Date));
			AlarmFileWrite();
			bReset=CheckReset();
			
//			wdgflag=0;
			break;
		case GetAlarm:
			memcpy(msg,&alarmch,sizeof(int));
			memcpy(&msg[sizeof(int)],&net_motion,sizeof(U16));
			memcpy(&msg[sizeof(int)+sizeof(U16)],&net_sensor,sizeof(U16));
			buflenth = sizeof(int)+sizeof(U16)+sizeof(U16);
			break;	
		case StartSendAlarmMsg:
			sendalarmnet = 1;
			break;
		case StopSendAlarmMsg:
			sendalarmnet = 0;
			break;
		case GetSysVer:
//			wdgflag=1;
			memset(msg,0x00,100);
			getsoftver(msg);
			sprintf(&msg[30],"%s0","1.011");
			sprintf(&msg[60],"%s0",gstCommonParam->GST_DRA_get_dvr_version());
			buflenth = 100;
//			wdgflag=0;
			break;
		case SetNetRec:
			if (demo_num < PBFULL && Disk_Num)
				record_key_set();
			break;
		case SetDVRTime:
//			wdgflag=1;
			buf[ADDRYEAR]= msg[sizeof(int)+1];
			buf[ADDRMONTH]= msg[sizeof(int)+2];
			buf[ADDRDAY]= msg[sizeof(int)+3];
			buf[ADDRHOUR]= msg[sizeof(int)+4];
			buf[ADDRMINUTE]= msg[sizeof(int)+5];
			buf[ADDRSECOND]= msg[sizeof(int)+6];
			tmp = rec_on;
			if (tmp)
				stop_key_set();
			setsystem(buf);
			
			
			if (tmp)
				record_key_set();
//			wdgflag=0;
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
			break;
		case 200:
			bReset = 1;
			break;
		case 198:
			MenuNetStaticInit();
			bhour_change=1;
		case 199:
//			wdgflag=1;
			MenuSysInit();
			MenuRecInit();
			MenuEmailInit();
			Menu_Load();
			MenuCamInit();
			MenuPtzInit();
			MenuSCHInit();
			MenuMotionInit();
			audioMenu_Inital();
			MenuAlarm_Initial();
			MenuHddInit();
//			wdgflag=0;
			break;
		case mRESET:
//			wdgflag=1;
//			sys_command("mount -t jffs2 /dev/mtdblock1 /mnt/nfs");
//			remove("/mnt/nfs/menu.data");
//			remove("/mnt/nfs/alarmfile.data");
//			remove("/mnt/nfs/schfile.data");
//			remove("/mnt/nfs/mac_num");
//			sys_command("umount /mnt/nfs");
			InitE2Prom();
			for (i=0;i<17;i++)
			{
				I_read_block_eeprom(ADDRECAM+ADDCOUNTRECCAM*i,sizeof(RecamMenu[i]),&RecamMenu[i].Reselution);
				RecamMenu[i].Reselution = 1;
				I_write_block_eeprom(ADDRECAM+ADDCOUNTRECCAM*i,sizeof(RecamMenu[i]),&RecamMenu[i].Reselution);
			}
//			wdgflag=0;
			bReset = 1;
			break;
		case 229:
/*
			pic = getyuvfile(cammenupic.recname,"cammenupic.recname");
			msg[0] = pic->pic_width;
			msg[1] = pic->pic_width>>8;
			msg[2] = pic->pic_height;
			msg[3] = pic->pic_height>>8;
			buflenth = 4;
*/
			break;
		case 227:
/*
			pic = getyuvfile(cammenupic.camnamelocal,"cammenupic.camnamelocal");
			msg[0] = pic->pic_width;
			msg[1] = pic->pic_width>>8;
			msg[2] = pic->pic_height;
			msg[3] = pic->pic_height>>8;
			buflenth = 4;
*/
			break;
		case 228:
/*
			pic = getyuvfile(cammenupic.recname,"cammenupic.recname");
			memcpy(msg,cammenupic.recname,pic->pic_height*pic->pic_width*2);
			buflenth = pic->pic_height*pic->pic_width*2;
*/
			break;
		case 226:
/*
			pic = getyuvfile(cammenupic.camnamelocal,"cammenupic.camnamelocal");
			memcpy(msg,cammenupic.camnamelocal,pic->pic_height*pic->pic_width*2);
			buflenth = pic->pic_height*pic->pic_width*2;
*/
			break;
		case 230:
//			picture_load(&cammenupic.recname,"recname.rgb");
//			picture_load(&cammenupic.camnamelocal,"camnamelocal.rgb");
			recpathnamedisp();
/*
			if (!menu_num && demo_num<PBFULL)
				demo_osd_disp();
			for (i=0;i<Cam_Num;i++)
				OsdY_line_display(i,0,cammenupic.recname,0);
*/
			break;
		case GetCAM:
			if (SysMenu.VideoFormat)
			{
				if (RecamMenu[0].Reselution)
					netcamval.resolution = 3;
				else
					netcamval.resolution = 2;
			}
			else
			{
				if (RecamMenu[0].Reselution)
					netcamval.resolution = 1;
				else
					netcamval.resolution = 0;
			}
			netcamval.Brightness = CamMenu[0].Brightness;
			netcamval.Hue = CamMenu[0].Hue;
			netcamval.Contrast = CamMenu[0].Contrast;
			netcamval.Saturation  = CamMenu[0].Satutrat;
			netcamval.Hdelay = CamMenu[0].PicHdel;
			netcamval.Vdelay = CamMenu[0].PicVdel;
			netcamval.Left = CamMenu[0].Starx;
			netcamval.Right = CamMenu[0].Endx;
			netcamval.Up = CamMenu[0].Stary;
			netcamval.Down = CamMenu[0].Endy;
			netcamval.dayval = CamMenu[0].dayval;
			netcamval.nightval = CamMenu[0].nightval;

			{
				IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
				IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
			
					netcamval.backlight=pIsp_st->u32BackLight;
			}

			memcpy(msg,&netcamval,sizeof(netcamval));
			DBGPRINT("dayval:0x%x \n",CamMenu[0].dayval);
			DBGPRINT("nightval:0x%x \n",CamMenu[0].nightval);
			buflenth = sizeof(netcamval);
			break;
		case SetCAM:
			memcpy(&netcamval,&msg[sizeof(int)+1],sizeof(netcamval));
			CamMenu[0].Brightness = netcamval.Brightness;
			CamMenu[0].Hue = netcamval.Hue;
			CamMenu[0].Contrast = netcamval.Contrast;
			CamMenu[0].Satutrat = netcamval.Saturation;
			CamMenu[0].PicHdel = netcamval.Hdelay;
			CamMenu[0].PicVdel = netcamval.Vdelay;
			CamMenu[0].Starx = netcamval.Left;
			CamMenu[0].Endx = netcamval.Right;
			CamMenu[0].Stary = netcamval.Up;
			CamMenu[0].Endy = netcamval.Down;
			CamMenu[0].dayval = netcamval.dayval;
			CamMenu[0].nightval = netcamval.nightval;
			CamMenu[0].backlight = netcamval.backlight;
			
	{	  
			IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_SYS_ST *pSys_st=&pAllInfo->sys_st;
			IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
			pIsp_st->u32cds[1]=255-CamMenu[0].dayval;
			pIsp_st->u32cds[0]=255-CamMenu[0].nightval;
			pIsp_st->u32BackLight= CamMenu[0].backlight;			
			gstCommonParam->GST_DRA_Hisi_save_ipcam_config_st(pAllInfo);
	}
			{
				int a,s_antiflicker_mode;
				
				if (SysMenu.VideoFormat)
					a=0;
				else
					a=1;
					s_antiflicker_mode=CamMenu[0].backlight;
					if(s_antiflicker_mode)
						s_antiflicker_mode=0;
					else
						s_antiflicker_mode=1;
				gstCommonParam->GST_DRA_Hisi_set_spi_antiflickerattr(a,s_antiflicker_mode);
			}
			
			menu_camera_do();
//			gstCommonParam->GST_DRA_Net_cam_mtd_store();
			Brightness_Ajust(0, CamMenu[0].Brightness);
			Hue_Ajust(0, CamMenu[0].Hue);
			Contrast_Ajust(0, CamMenu[0].Contrast);
			Stauration_Ajust(0, CamMenu[0].Satutrat);
			Hdelay_Ajust(0, CamMenu[0].PicHdel);
			Vdelay_Ajust(0, CamMenu[0].PicVdel);
			CamArea_Setup();
			DBGPRINT("dayval:0x%x \n",CamMenu[0].dayval);
			DBGPRINT("nightval:0x%x \n",CamMenu[0].nightval);
			break;
		case CAMSET_Brightness:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Brightness = i;
//			printf("TTTTTTTTTTTTTTTbri:%d\n",CamMenu[0].Brightness);
//			I_write_block_eeprom(ADDCAM+BRIGHTNESS,1,&CamMenu[0].Brightness);
			Brightness_Ajust(0, CamMenu[0].Brightness);
			break;
		case CAMSET_Hue:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Hue = i;
//			printf("TTTTTTTTTTTTTTTHue:%d\n",CamMenu[0].Hue);
//			I_write_block_eeprom(ADDCAM+HUE,1,&CamMenu[0].Hue);
			Hue_Ajust(0, CamMenu[0].Hue);
			break;
		case CAMSET_Contrast:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Contrast = i;
//			I_write_block_eeprom(ADDCAM+CONTRAST,1,&CamMenu[0].Contrast);
			Contrast_Ajust(0, CamMenu[0].Contrast);
			break;
		case CAMSET_Saturation:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Satutrat = i;
//			I_write_block_eeprom(ADDCAM+SATURAT,1,&CamMenu[0].Satutrat);
			Stauration_Ajust(0, CamMenu[0].Satutrat);
			break;
		case CAMSET_Hdelay:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].PicHdel = i;
//			I_write_block_eeprom(ADDCAM+PICHDEL,1,&CamMenu[0].PicHdel);
			Hdelay_Ajust(0, CamMenu[0].PicHdel);
			break;
		case CAMSET_Vdelay:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].PicVdel = i;
//			I_write_block_eeprom(ADDCAM+PICVDEL,1,&CamMenu[0].PicVdel);
			Vdelay_Ajust(0, CamMenu[0].PicVdel);
			break;
		case CAMSET_Left:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Starx = i;
//			buf[0]=CamMenu[0].Starx>>8;
//			buf[1]=CamMenu[0].Starx;
//			I_write_block_eeprom(ADDCAM+STARTX,2,buf);
//			printf("cmd:%d,sx:%d\n",msg[0],CamMenu[0].Starx);
			CamArea_Setup();
			break;
		case CAMSET_Right:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Endx = i;
//			buf[0]=CamMenu[0].Endx>>8;
//			buf[1]=CamMenu[0].Endx;
//			I_write_block_eeprom(ADDCAM+ENDX,2,buf);
//			printf("cmd:%d,sx:%d\n",msg[0],CamMenu[0].Endx);
			CamArea_Setup();
			break;
		case CAMSET_Up:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Stary = i;
//			buf[0]=CamMenu[0].Stary>>8;
//			buf[1]=CamMenu[0].Stary;
//			I_write_block_eeprom(ADDCAM+STARTY,2,buf);
//			printf("cmd:%d,sx:%d\n",msg[0],CamMenu[0].Stary);
			CamArea_Setup();
			break;
		case CAMSET_Down:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].Endy = i;
//			buf[0]=CamMenu[0].Endy>>8;
//			buf[1]=CamMenu[0].Endy;
//			printf("TTTTbottom:%d\n",CamMenu[0].Endy);
//			I_write_block_eeprom(ADDCAM+ENDY,2,buf);
//			printf("cmd:%d,sx:%d\n",msg[0],CamMenu[0].Endy);
			CamArea_Setup();
			break;
		case GetDaySD:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].dayval= i;
			break;
		case GetNightSD:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].nightval= i;
			break;
		case CAMSET_BACK_LIGHT:
			memcpy(&i,&msg[sizeof(int)+1],sizeof(int));
			CamMenu[0].backlight= i;
			{
				int a,s_antiflicker_mode;
				
				if (SysMenu.VideoFormat)
					a=0;
				else
					a=1;
					s_antiflicker_mode=CamMenu[0].backlight;
					if(s_antiflicker_mode)
						s_antiflicker_mode=0;
					else
						s_antiflicker_mode=1;
				gstCommonParam->GST_DRA_Hisi_set_spi_antiflickerattr(a,s_antiflicker_mode);
			}
			break;
		case GetALARM:
			if (SysMenu.VideoFormat)
			{
				if (RecamMenu[0].Reselution)
					netmotionval.resolution = 3;
				else
					netmotionval.resolution = 2;
			}
			else
			{
				if (RecamMenu[0].Reselution)
					netmotionval.resolution = 1;
				else
					netmotionval.resolution = 0;
			}
			netmotionval.Sensitivity = MotMenu[0].Sensitivity;
			netmotionval.Left = MotMenu[0].StarX_H*256 + MotMenu[0].StarX_L;
			netmotionval.Right = MotMenu[0].EndX_H*256 + MotMenu[0].EndX_L;
			netmotionval.Up = MotMenu[0].StarY_H*256 + MotMenu[0].StarY_L;
			netmotionval.Down = MotMenu[0].EndY_H*256 + MotMenu[0].EndY_L;
			memcpy(msg,&netmotionval,sizeof(netmotionval));
			buflenth = sizeof(netmotionval);
			break;
		case SetALARM:
			memcpy(&netmotionval,&msg[sizeof(int)+1],sizeof(netmotionval));
			MotMenu[0].Sensitivity = netmotionval.Sensitivity;
			MotMenu[0].StarX_H = netmotionval.Left/256;
			MotMenu[0].StarX_L = netmotionval.Left%256;
			MotMenu[0].StarY_H = netmotionval.Up/256;
			MotMenu[0].StarY_L = netmotionval.Up%256;
			MotMenu[0].EndX_H = netmotionval.Right/256;
			MotMenu[0].EndX_L = netmotionval.Right%256;
			MotMenu[0].EndY_H = netmotionval.Down/256;
			MotMenu[0].EndY_L = netmotionval.Down%256;
			netsetmotion();
			break;
		case GetISALARM:
			i = 0;
//			i = GetMotionInfo(0);
			if(g_net_motion_flag)
				i=1;
			else
				i=0;
			memcpy(msg,&i,sizeof(int));
			buflenth = sizeof(int);
			break;
		case GetNvrInfo:
			memset(&sysinfostr,0,sizeof(sysinfostr));
			Hdd_Info_Show();
			buflenth = sizeof(sysinfostr);
			memcpy(msg,&sysinfostr,sizeof(sysinfostr));
			break;
		case FormatTF:
			tmp = rec_on;
			if (tmp)
				stop_key_set();
			gstCommonParam->GST_FS_UMountAllPatition(0);
			gstCommonParam->GST_FS_PartitionDisk(0);
			gstCommonParam->GST_FS_BuildFileSystem(0);
			gstCommonParam->GST_FS_MountAllPatition(0);
					
			if (tmp)
				record_key_set();
			break;
#if 0			
		case mSYSTEM:
/*			tmp[0] = SysMenu.tmregion;
			tmp[1] = SysMenu.tmdisp;
			tmp[2] = SysMenu.bordcolor;
			tmp[4] = SysMenu.VideoFormat;
			tmp[5] = SysMenu.RemoteID;
			tmp[6] = SysMenu.spotpop;
			tmp[7] = SysMenu.mainpop;
			tmp[8] = SysMenu.osdtran;
			tmp[9] = SysMenu.Laguage;
*/
//			strcpy(msg,"jkl");
//			memcpy(msg,&SysMenu.tmregion,sizeof(ValSystem));
			break;
		case 63:
/*			
			if (SysMenu.tmregion != msg[1])
			{
				SysMenu.tmregion = msg[1];
				byear_change = 1;
			}
			if (SysMenu.tmdisp!= msg[2])
			{
				SysMenu.tmdisp = msg[2];
			    	OSD_ALL_CLEAR();
				demo_osd_disp();
			}
			if (SysMenu.bordcolor!= msg[3])
			{
				SysMenu.bordcolor = msg[3];
			    	OSD_ALL_CLEAR();
				demo_osd_disp();
			}
			if (SysMenu.VideoFormat!= msg[4])
			{
				SysMenu.VideoFormat = msg[4];
//			    	bReset = 1;
			}
			if (SysMenu.RemoteID!= msg[5])
			{
				SysMenu.RemoteID = msg[5];
			    	OSD_ALL_CLEAR();
				demo_osd_disp();
			}
			if (SysMenu.spotpop!= msg[6])
				SysMenu.spotpop = msg[6];
			if (SysMenu.mainpop!= msg[7])
				SysMenu.mainpop = msg[7];
			if (SysMenu.osdtran!= msg[8])
			{
				SysMenu.osdtran = msg[8];
			    	set_osdAlpha(SysMenu.osdtran,AD);
				set_osdAlpha(SysMenu.osdtran,HD);
			}
			if (SysMenu.Laguage!= msg[9])
			{
				tmp =SysMenu.Laguage;
				SysMenu.Laguage = msg[9];
				Check_Language();
				if (tmp != SysMenu.Laguage)
					Menu_Load();
			    	OSD_ALL_CLEAR();
				demo_osd_disp();
			}
			I_write_block_eeprom(ADDSYS+TIMFORMAT,sizeof(SysMenu),&SysMenu.tmregion);
			menu_num = 0;
*/
			break;
		case mCAMERA:
			memcpy(msg,CamMenu[0].CamTitle,sizeof(ValCammra)*CHANNELNUM);
			buflenth = sizeof(ValCammra)*CHANNELNUM;
			break;
		case 62:
			memcpy(CamMenu[0].CamTitle,&msg[1],sizeof(ValCammra)*CHANNELNUM);
			menu_camera_do();
			MenuCamInit();
		    	OSD_ALL_CLEAR();
			demo_osd_disp();
			menu_num = 0;
			break;
		case mRECORD:
			memcpy(msg,&RecamMenu[0].Reselution,sizeof(MenuRec)*(CHANNELNUM+1));
			buflenth = sizeof(MenuRec)*(CHANNELNUM+1);
			break;
		case 64:
			if (rec_on)
			{
				gstCommonParam->GST_FTC_StopRec();
			}
			memcpy(&RecamMenu[0].Reselution,&msg[1],sizeof(MenuRec)*(CHANNELNUM+1));
			menu_record_do();
//			bReset = 1;
			menu_num = 0;
			break;
		case mSCHEDULE:
//			memcpy(msg,&SchedulMenu[msg[1]].All_Date,sizeof(ValRecSchedule)*(CHANNELNUM+1));
			readmsg(msg);
			buflenth = sizeof(ValRecSchedule)*(CHANNELNUM+1)+1;
			break;
		case 41:
			msg[0]=AllCam;
			buflenth = 1;
			break;
		case 91:
			AllCam = msg[1];
			break;
		case 90:
			memcpy(&SchedulMenu[msg[1]].All_Date,&msg[2],sizeof(ValRecSchedule));
			FileWrite();
			DBGPRINT("************************* pppppppppppppp :%d\n",msg[1]);
			set_schedule();
			menu_num = 0;
			break;
		case mMOTION:
			memcpy(msg,&MotMenu[0].Sensitivity,sizeof(ValMotion)*CHANNELNUM);
			buflenth = sizeof(ValMotion)*CHANNELNUM;
			break;
		case 72:
			memcpy(&MotMenu[0].Sensitivity,&msg[1],sizeof(ValMotion)*CHANNELNUM);
			for (i=0;i<Cam_Num;i++)
			{
				I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i,sizeof(MotMenu[i]),&MotMenu[i].Sensitivity);
			}
		    	OSD_ALL_CLEAR();
			demo_osd_disp();
			menu_num = 0;
			break;
		case mPTZ:
			memcpy(msg,&PtzMenu[0].BaudRate,sizeof(ValPTZ)*CHANNELNUM);
			buflenth = sizeof(ValPTZ)*CHANNELNUM;
			break;
		case 81:
			memcpy(&PtzMenu[0].BaudRate,&msg[1],sizeof(ValPTZ)*CHANNELNUM);
			menu_ptz_do();
			menu_num = 0;
			break;
		case mSTATIC:
			setnet(msg);
			buflenth = sizeof(ValNetStatic);
			break;
		case 79:
			netsavedate(&msg[1]);
			break;
		case mMENUALARM:
			memcpy(msg,MenuAlarm_Date,sizeof(MenuAlarm_Date));
			buflenth = sizeof(MenuAlarm_Date);
			break;
		case 88:
			memcpy(MenuAlarm_Date,&msg[1],sizeof(MenuAlarm_Date));
			AlarmFileWrite();
			break;
		case mHDD:
			memcpy(msg,&HddMenu.Overwrite,sizeof(HddMenu.Overwrite));
			buflenth = sizeof(HddMenu.Overwrite);
			break;
		case 84:
			HddMenu.Overwrite = msg[1];
			I_write_byte_eeprom(ADDHDD, HddMenu.Overwrite);
			break;
		case mUSER:
			memcpy(msg,&menupass[0].UserId,sizeof(USERMAGE)*4);
			buflenth = sizeof(USERMAGE)*4;
			break;
		case 83:
			memcpy(&menupass[0].UserId,&msg[1],sizeof(USERMAGE)*4);
			user_data_save();
			net_user_set();
			break;
		case mSPOT:
			memcpy(msg,spot_cam[1],CHANNELNUM+1);
			buflenth = CHANNELNUM+1;
			break;
		case 94:
			memcpy(spot_cam[1],msg,CHANNELNUM+1);
			Set_Spot();
			break;
		case mAUDIO:
			memcpy(msg,audio_cambuf[1],18);
			buflenth = 18;
			break;
		case 97:
			memcpy(audio_cambuf[1],msg,18);
			setliveaudio();
			break;
		case Iconmenu:
			setsystem(buf);
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
				break;
			}
			for(i=0;i<7;i++)
			{
				if (i!=3)
				{
					tmp = ((buf[i]/10)*16+(buf[i]%10));
					pstCommonParam->gst_ext_write_rtc(0,ADRSECOND+i,1,&tmp);
				}
			}
			break;
#endif
		default:
			memset(msg,0x00,100);
			msg[0] = 0xfc;
			buflenth = 1;
			break;
	}
}

void Gget_gui_msg(char * msg,int *length)
{
	if( GST_MENU_gui_msg_get_chan_bitmap(msg,length) >= 0 )
		return;
	*length = buflenth;
}

void alarm_sent_net()
{
	char msg[50];
	alarm_net tmp;

	if (!sendalarmnet)
		return;
	
	memset(msg,0,sizeof(msg));

	tmp.order = 0xcdef;
	tmp.alch = alarmch;
	tmp.motioninfo = net_motion;
	tmp.sensorinfo = net_sensor;
	memcpy(msg,&tmp,sizeof(alarm_net));
	gstCommonParam->GST_DRA_Net_dvr_Net_send_self_data(msg,sizeof(alarm_net));
}

extern ValWifi Wifiinfo;
extern Valnetwifi netwifi;

int s_antiflicker_mode=1;
typedef struct {
	unsigned char Order[5];			//命令序列
	unsigned char CamNo;			//通道号：0~40
	unsigned char VideoFormat;		//制式：0--NTSC;1--PAL
	unsigned char Vbrightness;		//亮度：0~99,默认值50
	unsigned char Vhue;				//色度：0~99,默认值50
	unsigned char Vcontrast;		//对比度：0~99,默认值50
	unsigned char Vsatutrat;		//色温：0~99,默认值50
	unsigned char Vpich;			//图像水平偏移：1~30,默认值1
	unsigned char Vpicv;			//图像垂直偏移：NTSC 1~16,PAL 1~30 默认值16
	unsigned char Vday;				//滤光片白天数值：0~255,默认值234
	unsigned char Vnight;			//滤光片夜晚数值：0~255,默认值202
	unsigned char Coversx_H;		//区域覆盖起始边界x坐标高8位：0~255
	unsigned char Coversx_L;		//区域覆盖起始边界x坐标低8位：0~255
	unsigned char Coverex_H;		//区域覆盖结束边界x坐标高8位：0~255
	unsigned char Coverex_L;		//区域覆盖结束边界x坐标低8位：0~255
	unsigned char Coversy_H;		//区域覆盖起始边界y坐标高8位：0~255
	unsigned char Coversy_L;		//区域覆盖起始边界y坐标低8位：0~255
	unsigned char Coverey_H;		//区域覆盖结束边界y坐标高8位：0~255
	unsigned char Coverey_L;		//区域覆盖结束边界y坐标低8位：0~255
	unsigned char Cbaudrate;		//PTZ控制波特率：0:1200,1:2400,2:4800,4:9600
	unsigned char Cdeviceid;		//PTZ控制设备地址：1~0xff
	unsigned char Cprotocol;		//PTZ控制协议:0：Pelco_d1,1:Pelco_d2,2:Pelco_p
	unsigned char Reselution;		//录像分辨率：0：CIF，1：D1
	unsigned char Mot_sens;			//移动报警敏感度：0~50，默认值35
	unsigned char Mot_sx_H;			//移动报警区域起始边界x坐标高8位：0~255
	unsigned char Mot_sx_L;			//移动报警区域起始边界x坐标低8位：0~255
	unsigned char Mot_ex_H;			//移动报警区域结束边界x坐标高8位：0~255
	unsigned char Mot_ex_L;			//移动报警区域结束边界x坐标低8位：0~255
	unsigned char Mot_sy_H;			//移动报警区域起始边界y坐标高8位：0~255
	unsigned char Mot_sy_L;			//移动报警区域起始边界y坐标低8位：0~255
	unsigned char Mot_ey_H;			//移动报警区域结束边界y坐标高8位：0~255
	unsigned char Mot_ey_L;			//移动报警区域结束边界y坐标低8位：0~255
	unsigned char Time_Zone;		//时间格式：0 (年/月/日 时:分:秒) 1(月/日/年 时:分:秒) 2(日/月/年 时:分:秒)
	unsigned char Time_Year;		//NVR同步时间：年，例如2012则为12
	unsigned char Time_Month;		//NVR同步时间：月，例如1月则为0
	unsigned char Time_Day;			//NVR同步时间：日，例如1日则为1
	unsigned char Time_Hour;		//NVR同步时间：时，例如23点则为23
	unsigned char Time_Min;			//NVR同步时间：分，例如59分则为59
	unsigned char Time_Sec;			//NVR同步时间：秒，例如59秒则为59
	unsigned char CamTilFlag;		//通道名使用标志：0 英文字母模式;1 bimap模式
	unsigned char CamTitle[6];		//英文字母通道名：A~Z,0~9
	unsigned char ReserveDate[150];	//保留备用字段
} NVR_DATESEET;
NVR_DATESEET NvrSendDate;


#define CHINNUM			40
typedef struct {
	
	unsigned char ver0;
	unsigned char ver1;
	
	unsigned char time_zone;
	unsigned char time_pos;
	unsigned char alsrmMpop;
	unsigned char alsrmSpop;
	unsigned char userpasswork[CHINNUM][20];
	unsigned char menucfig[CHINNUM];
	unsigned char ptzcfig[CHINNUM];
	unsigned char audiocfig[CHINNUM];
	unsigned char startreccfig[CHINNUM];
	unsigned char stopreccif[CHINNUM];
	unsigned char playcfig[CHINNUM];
	unsigned char spotcfig[CHINNUM];
	unsigned char seqcfig[CHINNUM];
	unsigned char vformat;
	unsigned char remoterid;
	unsigned char hd_output;
	unsigned char menuout;
	unsigned char language_sel;
		
	unsigned char brightness[CHINNUM];
	unsigned char hue[CHINNUM];
	unsigned char contrast[CHINNUM];
	unsigned char satutrat[CHINNUM];
	unsigned char pichdel[CHINNUM];
	unsigned char picvdel[CHINNUM];
	unsigned char spottime[CHINNUM];
	unsigned short left[CHINNUM];
	unsigned short top[CHINNUM];
	unsigned short right[CHINNUM];
	unsigned short bottom[CHINNUM];
	unsigned char baudrate[CHINNUM];
	unsigned char deviceid[CHINNUM];
	unsigned char protocol[CHINNUM];
	
	unsigned char reselution[CHINNUM];
	unsigned char audioonoff[CHINNUM];
	unsigned char picqty[CHINNUM];
	unsigned char frmrate[CHINNUM];
	unsigned char hour_star[CHINNUM][7][3];
	unsigned char minute_star[CHINNUM][7][3];
	unsigned char hour_end[CHINNUM][7][3];
	unsigned char minute_end[CHINNUM][7][3];
	unsigned char rectype[CHINNUM][7][3];
	
	unsigned char motion_sens[CHINNUM];
	unsigned short motion_left[CHINNUM];
	unsigned short motion_top[CHINNUM];
	unsigned short motion_right[CHINNUM];
	unsigned short motion_bottom[CHINNUM];
	unsigned char alarm_buzzer[CHINNUM][3];
	unsigned char alarm_relay[CHINNUM][3];
	unsigned char alarm_email[CHINNUM][3];
	
	unsigned char ipget;
	unsigned char ipaddress[30];
	unsigned char cam_ipaddress[CHINNUM][30];
	unsigned char gateway[30];
	unsigned char netmask[30];
	unsigned char ddns1[30];
	unsigned char ddns2[30];
	unsigned char netprot[30];
	unsigned char	ieport[30];
	
	unsigned char upnp_use;
	
	unsigned char	wifi_use;	
	unsigned char	wifi_mode;	
	unsigned char	wifi_authmode;	
	unsigned char	wifi_encryptype;	
	unsigned char	wifi_passwork[30];	
	unsigned char	wifi_ssid[30];
	
	unsigned char eserveradd[30];
	unsigned char emailport[30];
	unsigned char epassword[30];
	unsigned char eusername[30];
	unsigned char ereciveadd[30];
	int emailsendtime;
	
	unsigned char overlayen;
	
} MMMConfig;

MMMConfig mmmConfig;

int Gget_massage(char * msg,int length)
{


#define VFORMAT_NTSC  		0
#define VFORMAT_PAL		  	1

	unsigned char* Nvr_Order=msg;
	char NetRecBuf[200],str[10];
	int ret,a,flag=0,SendLen;
	U8 tmp,buf[7],tmp0[2];
	U8 tmp_a[2],tmp_b[2];
	U16 i;
	static unsigned char s_vformat=0,s_reselution=0;
	unsigned char val[ADDCOUNTNETSTIC];

  DBGPRINT("*********** msg[0]:%d ,length:%d************ \n",msg[0],length);
  
//    return 0;
    
	if(msg[0]==INIT_IPC_ORD1)
	{
		if (Nvr_Order[0] == INIT_IPC_ORD1 && Nvr_Order[1] == INIT_IPC_ORD2 && Nvr_Order[2] == INIT_IPC_ORD3)
		{
			if (Nvr_Order[3] == INIT_IPC_ORD4)
			{
				switch (Nvr_Order[4])
				{
					case INIT_IPC_TIME:
						if(Disk_Num) break;
						memcpy(&NvrSendDate,Nvr_Order,sizeof(NVR_DATESEET));
//						timestring.tm_year=NvrSendDate.Time_Year;//msg[1];
//						timestring.tm_mon=NvrSendDate.Time_Month;//msg[2];
//						timestring.tm_mday=NvrSendDate.Time_Day;//msg[3];
//						timestring.tm_hour=NvrSendDate.Time_Hour;//msg[4];
//						timestring.tm_min=NvrSendDate.Time_Min;//msg[5];
//						timestring.tm_sec=NvrSendDate.Time_Sec;//msg[6]; 
//						sys_time_set();
						buf[ADDRYEAR]= NvrSendDate.Time_Year;
						buf[ADDRMONTH]= NvrSendDate.Time_Month+1;
						buf[ADDRDAY]= NvrSendDate.Time_Day;
						buf[ADDRHOUR]= NvrSendDate.Time_Hour;
						buf[ADDRMINUTE]= NvrSendDate.Time_Min;
						buf[ADDRSECOND]= NvrSendDate.Time_Sec;
						
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
	//					DBGPRINT("IPCam时间同步命令行:0x%02x,:0x%02x,:0x%02x,:0x%02x,:0x%02x\n",NvrSendDate.Order[0],NvrSendDate.Order[1],NvrSendDate.Order[2],NvrSendDate.Order[3],NvrSendDate.Order[4]);
	//					DBGPRINT("同步时间格式:%d (年/月/日 时:分:秒)\n",NvrSendDate.Time_Zone);
	//					DBGPRINT("同步时间:20%02d/%02d/%02d %02d:%02d:%02d\n",NvrSendDate.Time_Year,NvrSendDate.Time_Month+1,NvrSendDate.Time_Day,NvrSendDate.Time_Hour,NvrSendDate.Time_Min,NvrSendDate.Time_Sec);
						break;
					case INIT_IPC_ALL:
						memcpy(&NvrSendDate,Nvr_Order,sizeof(NVR_DATESEET));
						s_vformat=SysMenu.VideoFormat;
						if(s_vformat!=NvrSendDate.VideoFormat)
						{
//							I_write_block_eeprom(0x10,1,&NvrSendDate.VideoFormat);
							SysMenu.VideoFormat=NvrSendDate.VideoFormat;
							I_write_block_eeprom(ADDSYS+TIMFORMAT,sizeof(SysMenu),&SysMenu.tmregion);
//							I_read_block_eeprom(0x10,1,&s_vformat);
							I_read_byte_eeprom(ADDSYS+VIDEOFORMAT, val);
							s_vformat=val[0];
							DBGPRINT("*********** vformat:%d %d************ \n",s_vformat,NvrSendDate.VideoFormat);
							if(s_vformat==NvrSendDate.VideoFormat)
								flag=1;
							
							{
									IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
									IPCAM_SYS_ST *pSys_st=&pAllInfo->sys_st;
									
									if(s_vformat==VFORMAT_PAL)
										pSys_st->ipcam_stardard=TD_DRV_VIDEO_STARDARD_PAL;
									else
										pSys_st->ipcam_stardard=TD_DRV_VIDEO_STARDARD_NTSC;
										
									gstCommonParam->GST_DRA_Hisi_save_ipcam_config_st(pAllInfo);
									
							}
						}
						s_reselution=RecamMenu[0].Reselution;
						if(s_reselution!=NvrSendDate.Reselution)
						{
//							I_write_block_eeprom(0x11,1,&NvrSendDate.Reselution);	
							RecamMenu[0].Reselution=NvrSendDate.Reselution;
							I_write_block_eeprom(ADDRECAM,sizeof(RecamMenu[0]),&RecamMenu[0].Reselution);
//							I_read_block_eeprom(0x11,1,&s_reselution);	
							I_read_byte_eeprom(ADDRECAM+ADDCOUNTRECCAM,val);
							s_reselution=val[0];
							DBGPRINT("********* reselution:%d %d ********** \n",s_reselution,NvrSendDate.Reselution);
							if(s_reselution==NvrSendDate.Reselution)
								flag=1;
						}
						if(flag)
						{
							DBGPRINT("********* system reboot *************\n");
							sleep(2);
//							sys_reboot();//system("reboot");
							bReset=1;
						}
						if(fdcom)
						{
//							switch(NvrSendDate.Cbaudrate)//mmmConfig.baudrate[mmmConfig.ver1])
//							{
//								case 0:
//									portinfo.baudrate=1200;
//									break;
//								case 1:
//									portinfo.baudrate=2400;
//									break;
//								case 2:
//									portinfo.baudrate=4800;
//									break;
//								case 3:
//									portinfo.baudrate=9600;
//									break;
//							}
//							DBGPRINT("PortSet baudrates:%d\n",portinfo.baudrate);
//							PortSet(fdcom, &portinfo);
							PtzMenu[0].BaudRate=NvrSendDate.Cbaudrate;
							setuart(Baud[PtzMenu[Cur_CH].BaudRate]);
							I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*0+BAUDRATE,1,&PtzMenu[0].BaudRate);
						}
						rs232_device_id=NvrSendDate.Cdeviceid+1;
//						memset(str,0,sizeof(str));
//						sprintf(str,"CAM%02d",NvrSendDate.CamNo+1);
//						camtitle_char_line_display(str);

//						cam_name_rec_display(NvrSendDate.CamNo);	
//						brightness_ajust(0,NvrSendDate.Vbrightness);
//						hue_ajust(0,NvrSendDate.Vhue);
//						contrast_ajust(0,NvrSendDate.Vcontrast);
//						stauration_ajust(0,NvrSendDate.Vsatutrat);
//						hdelay_ajust(0,NvrSendDate.Vpich);
//						vdelay_ajust(0,NvrSendDate.Vpicv);
//						left=(NvrSendDate.Coversx_H<<8)|NvrSendDate.Coversx_L;
//						right=(NvrSendDate.Coverex_H<<8)|NvrSendDate.Coverex_L;
//						top=(NvrSendDate.Coversy_H<<8)|NvrSendDate.Coversy_L;
//						bottom=(NvrSendDate.Coverey_H<<8)|NvrSendDate.Coverey_L;
//						CoverRegionControl(left+10,top,abs(right-left),abs(bottom-top),1,0);
//						day_val=NvrSendDate.Vday;
//						night_val=NvrSendDate.Vnight;		
						
						CamMenu[0].Brightness = NvrSendDate.Vbrightness;
						CamMenu[0].Hue = NvrSendDate.Vhue;
						CamMenu[0].Contrast = NvrSendDate.Vcontrast;
						CamMenu[0].Satutrat = NvrSendDate.Vsatutrat;
						CamMenu[0].PicHdel = NvrSendDate.Vpich;
						CamMenu[0].PicVdel = NvrSendDate.Vpicv;
						CamMenu[0].Starx = (NvrSendDate.Coversx_H<<8)|NvrSendDate.Coversx_L;
						CamMenu[0].Endx = (NvrSendDate.Coverex_H<<8)|NvrSendDate.Coverex_L;
						CamMenu[0].Stary = (NvrSendDate.Coversy_H<<8)|NvrSendDate.Coversy_L;
						CamMenu[0].Endy = (NvrSendDate.Coverey_H<<8)|NvrSendDate.Coverey_L;
						CamMenu[0].dayval = NvrSendDate.Vday;
						CamMenu[0].nightval = NvrSendDate.Vnight;	
						
						Brightness_Ajust(0, CamMenu[0].Brightness);
						Hue_Ajust(0, CamMenu[0].Hue);
						Contrast_Ajust(0, CamMenu[0].Contrast);
						Stauration_Ajust(0, CamMenu[0].Satutrat);
						Hdelay_Ajust(0, CamMenu[0].PicHdel);
						Vdelay_Ajust(0, CamMenu[0].PicVdel);
						CamArea_Setup();
						
						I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+BRIGHTNESS,12,&CamMenu[0].Brightness);
						tmp0[0]=CamMenu[0].Starx>>8;
						tmp0[1]=CamMenu[0].Starx;
						I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+STARTX,2,tmp0);
						tmp0[0]=CamMenu[0].Stary>>8;
						tmp0[1]=CamMenu[0].Stary;
						I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+STARTY,2,tmp0);
						tmp0[0]=CamMenu[0].Endx>>8;
						tmp0[1]=CamMenu[0].Endx;
						I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+ENDX,2,tmp0);
						tmp0[0]=CamMenu[0].Endy>>8;
						tmp0[1]=CamMenu[0].Endy;
						I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+ENDY,2,tmp0);
						
						MotMenu[0].StarX_H = NvrSendDate.Mot_sx_H;
						MotMenu[0].StarX_L = NvrSendDate.Mot_sx_L;
						MotMenu[0].StarY_H = NvrSendDate.Mot_sy_H;
						MotMenu[0].StarY_L = NvrSendDate.Mot_sy_L;
						MotMenu[0].EndX_H = NvrSendDate.Mot_ex_H;
						MotMenu[0].EndX_L = NvrSendDate.Mot_ex_L;
						MotMenu[0].EndY_H = NvrSendDate.Mot_ey_H;
						MotMenu[0].EndY_L = NvrSendDate.Mot_ey_L;
				
						MotMenu[0].Sensitivity=NvrSendDate.Mot_sens;
						
						netsetmotion();
						
	//					DBGPRINT("IPCam参数设置命令行:0x%02x,:0x%02x,:0x%02x,:0x%02x,:0x%02x\n",NvrSendDate.Order[0],NvrSendDate.Order[1],NvrSendDate.Order[2],NvrSendDate.Order[3],NvrSendDate.Order[4]);
	//					DBGPRINT("通道:%d\n",NvrSendDate.CamNo);
	//					DBGPRINT("制式:%d(PAL)\n",NvrSendDate.VideoFormat);
	//					DBGPRINT("亮度:%d\n",NvrSendDate.Vbrightness);
	//					DBGPRINT("色度:%d\n",NvrSendDate.Vhue);
	//					DBGPRINT("对比度:%d\n",NvrSendDate.Vcontrast);
	//					DBGPRINT("色温:%d\n",NvrSendDate.Vsatutrat);
	//
	//					DBGPRINT("图像水平偏移:%d\n",NvrSendDate.CamNo);
	//					DBGPRINT("图像垂直偏移:%d(PAL)\n",NvrSendDate.VideoFormat);
//						DBGPRINT("滤光片白天数值:%d\n",NvrSendDate.Vday);
//						DBGPRINT("滤光片夜晚数值:%d\n",NvrSendDate.Vnight);
	//					DBGPRINT("区域覆盖起始边界x坐标:%d\n",(NvrSendDate.Coversx_H<<8)|NvrSendDate.Coversx_L);
	//					DBGPRINT("区域覆盖结束边界x坐标:%d\n",(NvrSendDate.Coverex_H<<8)|NvrSendDate.Coverex_L);
	//					DBGPRINT("区域覆盖起始边界y坐标:%d\n",(NvrSendDate.Coversy_H<<8)|NvrSendDate.Coversy_L);
	//					DBGPRINT("区域覆盖结束边界y坐标:%d\n",(NvrSendDate.Coverey_H<<8)|NvrSendDate.Coverey_L);
	//
	//					DBGPRINT("PTZ控制波特率:%d(1200)\n",NvrSendDate.Cbaudrate);
	//					DBGPRINT("PTZ控制设备地址:0x%02x\n",NvrSendDate.Cdeviceid);
	//					DBGPRINT("PTZ控制协议:%d(PELCO_D1)\n",NvrSendDate.Cprotocol);
	//					DBGPRINT("录像分辨率:%d\n",NvrSendDate.Reselution);
	//
	//					DBGPRINT("移动报警敏感度:%d\n",NvrSendDate.Mot_sens);
	//					DBGPRINT("移动报警区域起始边界x坐标:%d\n",(NvrSendDate.Mot_sx_H<<8)|NvrSendDate.Mot_sx_L);
	//					DBGPRINT("移动报警区域结束边界x坐标:%d\n",(NvrSendDate.Mot_ex_H<<8)|NvrSendDate.Mot_ex_L);
	//					DBGPRINT("移动报警区域起始边界y坐标:%d\n",(NvrSendDate.Mot_sy_H<<8)|NvrSendDate.Mot_sy_L);
	//					DBGPRINT("移动报警区域结束边界y坐标:%d\n",(NvrSendDate.Mot_ey_H<<8)|NvrSendDate.Mot_ey_L);
	//
	//					DBGPRINT("通道名标志:%d(0 英文字母模式;1 图片模式)\n",NvrSendDate.CamTilFlag);
	//					DBGPRINT("英文通道名:%s\n",NvrSendDate.CamTitle);
						break;
					case INIT_IPC_TITLE:
	//					DBGPRINT("通道名设置命令行:0x%02x,:0x%02x,:0x%02x,:0x%02x,:0x%02x\n",Nvr_Order[0],Nvr_Order[1],Nvr_Order[2],Nvr_Order[3],Nvr_Order[4]);
	//					DBGPRINT("通道名标志:%d(0 英文字母模式;1 图片模式)\n",NvrSendDate.CamTilFlag);
						break;
					case SET_IPC_RELAY:
//						RegisterReadWrite.value=Nvr_Order[5];
//						if(gm8126id>0) ioctl(gm8126id, 11, &RegisterReadWrite);
//						if(tw2865id>0) ioctl(tw2865id, 11, &RegisterReadWrite);							
						RelayControl(Nvr_Order[5]);
						DBGPRINT(" SETALARM %d \n",Nvr_Order[5]);
	//					DBGPRINT("报警继电器输出命令行:0x%02x,:0x%02x,:0x%02x,:0x%02x,:0x%02x\n",Nvr_Order[0],Nvr_Order[1],Nvr_Order[2],Nvr_Order[3],Nvr_Order[4]);
	//					DBGPRINT("报警继电器输出！%d\n",Nvr_Order[5]);
						break;
					default:
	
						break;
				}
			}
			else if (Nvr_Order[3] == SET_IPC_PTZ)
			{
//				rs232_device_id=NvrSendDate.Cdeviceid+1;
				switch(Nvr_Order[4])
				{
					case SET_PTZ_LEFT:
						ptz_h_speed=ptz_v_speed=Nvr_Order[5];
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_LEFT);
	//					DBGPRINT("PTZ左转！转速：%d\n",Nvr_Order[5]);
						break;
					case SET_PTZ_RIGHT:
						ptz_h_speed=ptz_v_speed=Nvr_Order[5];
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_RIGHT);
	//					DBGPRINT("PTZ右转！转速：%d\n",Nvr_Order[5]);
						break;
					case SET_PTZ_UP:
						ptz_h_speed=ptz_v_speed=Nvr_Order[5];
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_UP);
	//					DBGPRINT("PTZ上转！转速：%d\n",Nvr_Order[5]);
						break;
					case SET_PTZ_DOWN:
						ptz_h_speed=ptz_v_speed=Nvr_Order[5];
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_DOWN);
	//					DBGPRINT("PTZ下转！转速：%d\n",Nvr_Order[5]);
						break;
					case SET_PTZ_FAR:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_FAR);
	//					DBGPRINT("PTZ Far！\n");
						break;
					case SET_PTZ_NEAR:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_NEAR);
	//					DBGPRINT("PTZ Near！\n");
						break;
					case SET_PTZ_ZOOMOUT:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_ZOOMOUT);
	//					DBGPRINT("PTZ Zoomout！\n");
						break;
					case SET_PTZ_ZOOMIN:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_ZOOMIN);
	//					DBGPRINT("PTZ Zoomin！\n");
						break;
					case SET_PTZ_IROPEN:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_IRISOPEN);
	//					DBGPRINT("PTZ Iropen！\n");
						break;
					case SET_PTZ_IRCLOSE:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_IRISCLOSE);
	//					DBGPRINT("PTZ Irclose！\n");
						break;
					case SET_PTZ_AUTO:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_AUTO);
	//					DBGPRINT("PTZ auto！\n");
						break;
					case SET_PTZ_AUTO1:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_AUTO);
	//					DBGPRINT("PTZ Auto1！\n");
						break;
					case SET_PTZ_STOP:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_STOP);
	//					DBGPRINT("PTZ Stop！\n");
						break;
					case SET_PTZ_STOP1:
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_STOP);
	//					DBGPRINT("PTZ Stop1！\n");
						break;
					case SET_PTZ_PRESET:
						ptz_point=Nvr_Order[5];
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_PRESET);
	//					DBGPRINT("PTZ Preset position:%d！\n",Nvr_Order[5]);
						break;
					case SET_PTZ_PREVIEW:
						ptz_point=Nvr_Order[5];
						ptz_buf_get(NvrSendDate.Cprotocol,PTZ_PREVIEW);
	//					DBGPRINT("PTZ Preview position:%d！\n",Nvr_Order[5]);
						break;
					default:
						break;
				}
				DBGPRINT("****rs232_device_id:%d \n",rs232_device_id);
				SendLen = PortSend(fdcom,bySbuf_232, rs232_send_len); 
		 		if(SendLen==0 || SendLen<0) 
		 		  printf("Error: send2 failed.\n"); 
				else
				{
					printf("send2 ok!!\n"); 
				}
			}
		}
	}
	else
	{
		DBGPRINT("msg[0]:0x%x,length:%d \n",msg[0],length);
		switch(msg[0])
		{
			case 0x0a://sys time set
				if(Disk_Num) break;
	//			timestring.tm_year=msg[1];
	//			timestring.tm_mon=msg[2];
	//			timestring.tm_mday=msg[3];
	//			timestring.tm_hour=msg[4];
	//			timestring.tm_min=msg[5];
	//			timestring.tm_sec=msg[6]; 
	//			sys_time_set(); 
	//			g_NetCmd=msg[0];
				buf[ADDRYEAR]= msg[1];
				buf[ADDRMONTH]= msg[2]+1;
				buf[ADDRDAY]= msg[3];
				buf[ADDRHOUR]= msg[4];
				buf[ADDRMINUTE]= msg[5];
				buf[ADDRSECOND]= msg[6];
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
				break;
			case 0x19:	
				if (s_vformat==VFORMAT_PAL)
					a=TD_DRV_VIDEO_STARDARD_PAL;
				else
					a=TD_DRV_VIDEO_STARDARD_NTSC;
				s_antiflicker_mode=msg[1];
				DBGPRINT("****************** s_antiflicker_mode:%d \n",s_antiflicker_mode);
				if(s_antiflicker_mode)
					s_antiflicker_mode=0;
				else
					s_antiflicker_mode=1;
				gstCommonParam->GST_DRA_Hisi_set_spi_antiflickerattr(a,s_antiflicker_mode);
				break;
			case 0x1a://sys menu set
			case 0x2a://motion menu set
			case 0x3a://cam menu set
				flag=0;
				memcpy(&mmmConfig,msg,sizeof(mmmConfig));
				s_vformat=SysMenu.VideoFormat;
				if(s_vformat!=mmmConfig.vformat)
				{
	//				I_write_block_eeprom(0x10,1,&mmmConfig.vformat);
					SysMenu.VideoFormat=mmmConfig.vformat;
					I_write_block_eeprom(ADDSYS+TIMFORMAT,sizeof(SysMenu),&SysMenu.tmregion);
	//				I_read_block_eeprom(0x10,1,&s_vformat);
					I_read_byte_eeprom(ADDSYS+VIDEOFORMAT, val);
					s_vformat=val[0];
					DBGPRINT("*********** vformat:%d %d************ \n",s_vformat,mmmConfig.vformat);
					if(s_vformat==mmmConfig.vformat)
						flag=1;
						
					{
							IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
							IPCAM_SYS_ST *pSys_st=&pAllInfo->sys_st;
							
							if(s_vformat==VFORMAT_PAL)
								pSys_st->ipcam_stardard=TD_DRV_VIDEO_STARDARD_PAL;
							else
								pSys_st->ipcam_stardard=TD_DRV_VIDEO_STARDARD_NTSC;
								
							gstCommonParam->GST_DRA_Hisi_save_ipcam_config_st(pAllInfo);							
					}
				}
	
				s_reselution=RecamMenu[0].Reselution;
				if(s_reselution!=mmmConfig.reselution[0])
				{
	//				I_write_block_eeprom(0x11,1,&mmmConfig.reselution[0]);	
					RecamMenu[0].Reselution=NvrSendDate.Reselution;
					I_write_block_eeprom(ADDRECAM,sizeof(RecamMenu[0]),&RecamMenu[0].Reselution);
//					I_read_block_eeprom(0x11,1,&s_reselution);	
					I_read_byte_eeprom(ADDRECAM+ADDCOUNTRECCAM,val);
					s_reselution=val[0];
					DBGPRINT("********* reselution:%d %d ********** \n",s_reselution,mmmConfig.reselution[0]);
					if(s_reselution==mmmConfig.reselution[0])
						flag=1;
				}
				printf("ch:%d,sens:%d,left:%d,top:%d,right:%d,bottom:%d\n",mmmConfig.ver1,mmmConfig.motion_sens[mmmConfig.ver1],
				mmmConfig.motion_left[mmmConfig.ver1],mmmConfig.motion_top[mmmConfig.ver1],mmmConfig.motion_right[mmmConfig.ver1],
				mmmConfig.motion_bottom[mmmConfig.ver1]);
				if(flag)
				{
					DBGPRINT("********* system reboot *************\n");
					sleep(2);
					bReset=1;
				}
				if(fdcom)
				{
//					switch(mmmConfig.baudrate[mmmConfig.ver1])
//					{
//						case 0:
//							portinfo.baudrate=1200;
//							break;
//						case 1:
//							portinfo.baudrate=2400;
//							break;
//						case 2:
//							portinfo.baudrate=4800;
//							break;
//						case 3:
//							portinfo.baudrate=9600;
//							break;
//					}				
//					DBGPRINT("PortSet baudrates:%d\n",portinfo.baudrate);
//					PortSet(fdcom, &portinfo);
					PtzMenu[0].BaudRate=mmmConfig.baudrate[mmmConfig.ver1];
					setuart(Baud[PtzMenu[Cur_CH].BaudRate]);
					I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*0+BAUDRATE,1,&PtzMenu[0].BaudRate);
				}
				memset(str,0,sizeof(str));
				sprintf(str,"CAM%02d",mmmConfig.ver1);
				camtitle_char_line_display(str);
//			cam_name_rec_display(mmmConfig.ver1);		
				if(msg[0]==0x3a)
				{
//				brightness_ajust(0,mmmConfig.brightness[mmmConfig.ver1]);
//				hue_ajust(0,mmmConfig.hue[mmmConfig.ver1]);
//				contrast_ajust(0,mmmConfig.contrast[mmmConfig.ver1]);
//				stauration_ajust(0,mmmConfig.satutrat[mmmConfig.ver1]);
//				hdelay_ajust(0,mmmConfig.pichdel[mmmConfig.ver1]);
//				vdelay_ajust(0,mmmConfig.picvdel[mmmConfig.ver1]);
//				CoverRegionControl(mmmConfig.left[mmmConfig.ver1]+10,mmmConfig.top[mmmConfig.ver1],abs(mmmConfig.right[mmmConfig.ver1]-mmmConfig.left[mmmConfig.ver1]),abs(mmmConfig.bottom[mmmConfig.ver1]-mmmConfig.top[mmmConfig.ver1]),1,0);
					CamMenu[0].Brightness = mmmConfig.brightness[mmmConfig.ver1];
					CamMenu[0].Hue = mmmConfig.hue[mmmConfig.ver1];
					CamMenu[0].Contrast = mmmConfig.contrast[mmmConfig.ver1];
					CamMenu[0].Satutrat = mmmConfig.satutrat[mmmConfig.ver1];
					CamMenu[0].PicHdel = mmmConfig.pichdel[mmmConfig.ver1];
					CamMenu[0].PicVdel = mmmConfig.picvdel[mmmConfig.ver1];
					CamMenu[0].Starx = mmmConfig.left[mmmConfig.ver1];
					CamMenu[0].Endx = mmmConfig.right[mmmConfig.ver1];
					CamMenu[0].Stary = mmmConfig.top[mmmConfig.ver1];
					CamMenu[0].Endy = mmmConfig.bottom[mmmConfig.ver1];
					Brightness_Ajust(0, CamMenu[0].Brightness);
					Hue_Ajust(0, CamMenu[0].Hue);
					Contrast_Ajust(0, CamMenu[0].Contrast);
					Stauration_Ajust(0, CamMenu[0].Satutrat);
					Hdelay_Ajust(0, CamMenu[0].PicHdel);
					Vdelay_Ajust(0, CamMenu[0].PicVdel);
					CamArea_Setup();
					
					I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+BRIGHTNESS,12,&CamMenu[0].Brightness);
					tmp0[0]=CamMenu[0].Starx>>8;
					tmp0[1]=CamMenu[0].Starx;
					I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+STARTX,2,tmp0);
					tmp0[0]=CamMenu[0].Stary>>8;
					tmp0[1]=CamMenu[0].Stary;
					I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+STARTY,2,tmp0);
					tmp0[0]=CamMenu[0].Endx>>8;
					tmp0[1]=CamMenu[0].Endx;
					I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+ENDX,2,tmp0);
					tmp0[0]=CamMenu[0].Endy>>8;
					tmp0[1]=CamMenu[0].Endy;
					I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+ENDY,2,tmp0);
				}
				
				MotMenu[0].StarX_H = mmmConfig.motion_left[mmmConfig.ver1]/256;
				MotMenu[0].StarX_L = mmmConfig.motion_left[mmmConfig.ver1]%256;
				MotMenu[0].StarY_H = mmmConfig.motion_top[mmmConfig.ver1]/256;
				MotMenu[0].StarY_L = mmmConfig.motion_top[mmmConfig.ver1]%256;
				MotMenu[0].EndX_H = mmmConfig.motion_right[mmmConfig.ver1]/256;
				MotMenu[0].EndX_L = mmmConfig.motion_right[mmmConfig.ver1]%256;
				MotMenu[0].EndY_H = mmmConfig.motion_bottom[mmmConfig.ver1]/256;
				MotMenu[0].EndY_L = mmmConfig.motion_bottom[mmmConfig.ver1]%256;
				
				MotMenu[0].Sensitivity=mmmConfig.motion_sens[mmmConfig.ver1];

				netsetmotion();
				break;
			case 0x5a:
//			RegisterReadWrite.value=msg[1];
//			if(gm8126id>0) ioctl(gm8126id, 11, &RegisterReadWrite);
//			if(tw2865id>0) ioctl(tw2865id, 11, &RegisterReadWrite);
//			g_NetCmd=msg[0];
				RelayControl(msg[1]);
				DBGPRINT(" SETALARM %d \n",msg[1]);
				break;
			case 0x6e:
				memcpy(camtitle_bitmap_buf,msg,sizeof(camtitle_bitmap_buf));
				cam_name_set_flag=1;
				switch(g_record_display_info)
				{
					case 'W':
						cam_name_rec_display(1);
						break;
					case 'M':
						cam_name_rec_display(2);
						break;
					case 'S':
						cam_name_rec_display(3);
						break;
					case 'T':
						cam_name_rec_display(4);
						break;
					case 'N':
						cam_name_rec_display(5);
						break;
				}
				break;
			case 0x6f:
				cam_name_set_flag=0;
//			cam_name_rec_display(msg[1]);
				memset(str,0,sizeof(str));
				sprintf(str,"CAM%02d",msg[1]);
				camtitle_char_line_display(str);
				break;
			case 0x6b:
				cam_name_set_flag=0;
				memset(str,0,sizeof(str));
				memcpy(str,&msg[1],5);
				DBGPRINT("********* str:%s *************\n",str);
//			char_line_display(str);
				camtitle_char_line_display(str);
				break;
			case 0x6c:
//			I_read_block_eeprom(0x0b,1,&by_tmp);
//			if(by_tmp!=msg[1])
//			{				
//				I_write_block_eeprom(0x0b,1,&msg[1]);
//				I_read_block_eeprom(0x0b,1,&by_tmp);
//				if(by_tmp==msg[1])
//				{
//					DBGPRINT("********* system reboot *************\n");
//					sleep(2);
//					sys_reboot();
//				}
//			}
				break;
			case 0x6a:
//			RegisterReadWrite.value=1;
//			if(gm8126id>0) ioctl(gm8126id, 12, &RegisterReadWrite);
//			usleep(5000);
//			usleep(5000);
				DBGPRINT("****msg[1]:%d,msg[2]:%d,msg[3]:%d,msg[4]:%d\n",msg[1],msg[2],msg[3],msg[4]);
				SendLen = PortSend(fdcom,&msg[1], length-1); 
		 		if(SendLen==0 || SendLen<0) 
		 		  printf("Error: send2 failed.\n"); 
				else
				{
					printf("send2 ok!!\n"); 
				}
//			usleep(5000);
//			usleep(5000);
//			usleep(5000);
//			usleep(5000);
//			usleep(5000);
//			usleep(5000);
//			usleep(5000);
//			usleep(5000);
//			usleep(5000);
//			RegisterReadWrite.value=0;
//			if(gm8126id>0) ioctl(gm8126id, 12, &RegisterReadWrite);
//			g_NetCmd=msg[0]; 
				break;
			case 0x7a://get info
//			g_NetCmd=msg[0];
					break;
			case 0x8a:
				memset(NetRecBuf,0,200);
				NetRecBuf[0]=0x6a;
				memcpy(&NetRecBuf[1],&netwifi,sizeof(netwifi));
				gstCommonParam->GST_DRA_Net_dvr_send_self_data(NetRecBuf,200);
				break;
			case 0x9a:
//			wdgflag=1;
				memcpy(&Wifiinfo.Wififlag,&msg[1],sizeof(Wifiinfo));
				I_write_block_eeprom(ADDWIFI,sizeof(Wifiinfo),&Wifiinfo.Wififlag);
				memcpy(&NetStaticMenu.IPAlloction,&msg[1+sizeof(Wifiinfo)],sizeof(NetStaticMenu));
				I_write_block_eeprom(ADDNSTC,sizeof(ValNetStatic),&NetStaticMenu.IPAlloction);
				if(net_cfg_save(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)<0)
					printf("net_cfg_save false\n");
//			gstCommonParam->GST_DRA_Net_cam_mtd_store();
//			wdgflag=0;
				tmp_a[0]=NetStaticMenu.Port>>8;
				tmp_a[1]=NetStaticMenu.Port;
				tmp_b[0]=NetStaticMenu.IEPort>>8;
				tmp_b[1]=NetStaticMenu.IEPort;
				I_write_block_eeprom(ADDNSTC+PORT,2,tmp_a);
				I_write_block_eeprom(ADDNSTC+IEPORT,2,tmp_b);

				bReset = 1;
				break;

			case 0xa1:
				menupass[0].UserName[0] = 'A';
				menupass[0].UserName[1] = 'D';
				menupass[0].UserName[2] = 'M';
				menupass[0].UserName[3] = 'I';
				menupass[0].UserName[4] = 'N';
				menupass[0].UserName[5] = '0';
				menupass[0].Password[0] = 0;
				menupass[0].Password[1] = 0;
				menupass[0].Password[2] = 0;
				menupass[0].Password[3] = 0;
				menupass[0].Password[4] = 0;
				menupass[0].Password[5] = 0;

				menupass[1].UserName[0] = 'U';
				menupass[1].UserName[1] = 'S';
				menupass[1].UserName[2] = 'E';
				menupass[1].UserName[3] = 'R';
				menupass[1].UserName[4] = '0';
				menupass[1].UserName[5] = '1';
				menupass[1].Password[0] = 1;
				menupass[1].Password[1] = 1;
				menupass[1].Password[2] = 1;
				menupass[1].Password[3] = 1;
				menupass[1].Password[4] = 1;
				menupass[1].Password[5] = 1;

				menupass[2].UserName[0] = 'U';
				menupass[2].UserName[1] = 'S';
				menupass[2].UserName[2] = 'E';
				menupass[2].UserName[3] = 'R';
				menupass[2].UserName[4] = '0';
				menupass[2].UserName[5] = '2';
				menupass[2].Password[0] = 2;
				menupass[2].Password[1] = 2;
				menupass[2].Password[2] = 2;
				menupass[2].Password[3] = 2;
				menupass[2].Password[4] = 2;
				menupass[2].Password[5] = 2;

				menupass[3].UserName[0] = 'U';
				menupass[3].UserName[1] = 'S';
				menupass[3].UserName[2] = 'E';
				menupass[3].UserName[3] = 'R';
				menupass[3].UserName[4] = '0';
				menupass[3].UserName[5] = '3';
				menupass[3].Password[0] = 3;
				menupass[3].Password[1] = 3;
				menupass[3].Password[2] = 3;
				menupass[3].Password[3] = 3;
				menupass[3].Password[4] = 3;
				menupass[3].Password[5] = 3;
				
				for (i=0;i<4;i++)
				{
				//DBGPRINT("\nddddddddddjjjjjjjjjjjjjjjddddddddddd---%s--%s\n", menupass[i].UserName, menupass[i].Password);
				I_write_block_eeprom(ADDPAS+ADDCOUNTPAS*i+1,sizeof(menupass[i].Password),&menupass[i].Password);
				I_write_block_eeprom(ADDPAS+ADDCOUNTPAS*i+16,sizeof(menupass[i].UserName),&menupass[i].UserName);
				}
				break;

			case 0xa3:
				{
					IPCAM_SHOW_MSG_INFO msg_info;
					memcpy(&msg_info,&msg[1],sizeof(IPCAM_SHOW_MSG_INFO));
					for(i=0;i<5;i++)
					{
						set_osd_display(i+4,msg_info.show_msg_st[i].show_mode,msg_info.show_msg_st[i].show_pos_x,msg_info.show_msg_st[i].show_pos_y,msg_info.show_msg_st[i].show_pos_x1,msg_info.show_msg_st[i].show_pos_y1,msg_info.show_msg_st[i].show_on_screen,msg_info.show_msg_st[i].show_msg);
					}
				}
					break;
			case 0xf0:
//			day_val=msg[1];
//			night_val=msg[2];		
				CamMenu[0].dayval = msg[1];
				CamMenu[0].nightval = msg[2];		
//			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+DAYV,1,&CamMenu[0].dayval);
//			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+NIGHTV,1,&CamMenu[0].nightval);
				printf("day_val:0x%x,night_val:0x%x \n",CamMenu[0].dayval,CamMenu[0].nightval);
				break;
		}
	}
	return 0;
}

void net_key(int iKeyval)
{
	switch(iKeyval)
	{
		case NET_KEY_VAL_ESC:
			byKey_val=kESC;
			break;
		case NET_KEY_VAL_REC:
			byKey_val=kREC;
			break;
	}
}

int get_ipcam_net_info(void * buf)
{
	memcpy(buf,&NetStaticMenu.IPAlloction,sizeof(ValNetStatic));
	return 0;
}
int set_ipcam_net_info(void * buf)
{
	memcpy(&NetStaticMenu.IPAlloction,buf,sizeof(ValNetStatic));
	if(net_cfg_save(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)<0)
	{
		DBGPRINT("net_cfg_save false\n");
		return -1;
	}
	return 0;
}
void netcontrolinital(void)
{
	gstCommonParam->GST_MENU_set_current_play_mode = set_current_play_mode;
	gstCommonParam->GST_MENU_cam_parameter_get = cam_parameter_get;
	gstCommonParam->GST_MENU_cam_parameter_set = cam_parameter_set;
	gstCommonParam->GST_MENU_ptz_parameter_get = ptz_parameter_get;
	gstCommonParam->GST_MENU_ptz_parameter_set = ptz_parameter_set;
	gstCommonParam->GST_MENU_rec_parameter_get = rec_parameter_get;
	gstCommonParam->GST_MENU_rec_parameter_set = rec_parameter_set;
	gstCommonParam->GST_MENU_net_key = net_key;
	gstCommonParam->GST_MENU_gui_msg_get = Gget_gui_msg;
	gstCommonParam->GST_MENU_gui_msg_set = SSet_gui_msg;
	gstCommonParam->GST_DRA_Net_dvr_recv_self_data=Gget_massage;
	
	gstCommonParam->GSt_NET_get_ipcam_net_info=get_ipcam_net_info;
	gstCommonParam->GSt_NET_set_ipcam_net_info=set_ipcam_net_info;
}



