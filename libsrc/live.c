#include "cmn.h"

//U8 FirstRead=0;


U8 Audio_Char[]=
{
	"\x07      \0 AUDIO "
};

U8 Reflash_Page;

int NetStatusFlag;

U8 FullCam = 0;

extern int play_quad_flag;
extern int Baud[];

U8 SystemLock=0;

void System_Lock()
{
	
	if (SystemLock)
		menu_pass_on(1);
	else
	{
		SystemLock = 1;
		I_write_block_eeprom(ADDSYS+0x0f,1,&SystemLock);
		if (MenuStatus >= Iconmenu)
		{
		}
		else
			Icon_Menu_On(0, 0);
	}

	if (SystemLock)
		LedControl(LEDLOCK,1);
	else
		LedControl(LEDLOCK,0);

}
/*
unsigned char * get_array_data(char * file_name)
{
	FILE * fp = NULL;
	int ret;
	int fileLength;
	unsigned char * p = NULL;

	if( file_name == NULL )
		return NULL;

	fp = fopen(file_name,"rb");
	if( !fp )
	{
		printf("open file %s error!\n",file_name);
		return NULL;
	}

	ret = fseek(fp, 0, SEEK_END);
	if ( ret < 0 )
	{					
		fclose(fp);
		printf("  fseek error!\n");
		return NULL;
	}

	fileLength = ftell(fp);

	ret = fseek(fp,  0 , SEEK_SET );

	p = (unsigned char *)malloc(fileLength);

	if( !p )
	{
		fclose(fp);
		printf("malloc error!\n");
		return NULL;
	}

	ret = fread(p,1,fileLength,fp);
	if( ret != fileLength )
	{
		fclose(fp);
		printf("write file error!ret=%d len=%d\n",ret,fileLength);
		return NULL;
	}

	fclose(fp);
	fp = NULL;

	return p;

}
*/

void Hdd_Error_Check()
{
	int ret,err;
	time_t timtmp;
	ret = gstCommonParam->GST_FS_GetErrorMessage(&err,&timtmp);
	if (err==FILEERROR)
	{
		printf("Hdd_Error_Check reset1!\n");
		event_write(REC_START_BYHAND_ERROR,0,0);
		bReset = 1;
	}
	else if (err == ALLDISKNOTENABLETOWRITE && diskstatus != 1 && rec_on && (HddMenu.Overwrite == 0))
	{
		stop_key_set();
		InfoId = HddFullRec;
		diskstatus = 1;
		buzzer_set(1);
		MenuErrOn();
	}
	else if (rec_flag && reccam)
	{
		ret = gstCommonParam->GST_FTC_get_rec_cur_status();
		if (ret==0)
		{
			event_write(REC_START_ALARM_ERROR,0,0);
			bReset = 1;
			printf("Hdd_Error_Check reset2!bReset = 0\n");
		}
	}
}

void Net_Status_Check()
{
	int ret;
	ret =NetStatusFlag;
	NetStatusFlag = gstCommonParam->GST_NET_is_have_client();
	if (NetStatusFlag != 1)
		NetStatusFlag = 0;
	ret ^= NetStatusFlag;
//	printf("NetStatusFlag = %x\n",NetStatusFlag);
	if (ret)
	{
		if (NetStatusFlag)
		{
			LedControl(LEDNET,1);
			printf("Have Client!\n");
		}
		else
		{
			LedControl(LEDNET,0);
			printf("No Client!\n");
		}
	}
}


void Set_Audio(U8 bt)
{
}

void Demo_num_key_pro(U8 demonum,U8 demopage)
{
	demo_page_old=demo_page;demo_num_old=demo_num;
	demo_num=demonum;
	demo_page=demopage;
//	if (demonum == L8)
	buzzer_set(0);
	OSD_ALL_CLEAR();
//	else
//		Osd_Partion_Clear();
	get_demo_stamp(demo_num,demo_page,1);
	demo_set();  	
	demo_osd_disp();
//	mcu_disp_refresh();
//	printf("////////////////////////////demo_page_old change!\n");

}

void LFULL_cam_key_pro(U8 camno)  //cam_no start from 0
{
	if (camno>=Cam_Num||(demo_page==camno && demo_num == LFULL))
		return;
	
	demo_num_old=demo_num;demo_page_old=demo_page;
	buzzer_set(0);
	OSD_ALL_CLEAR();
	get_demo_stamp(LFULL,camno,0);
	demo_set();
	demo_osd_disp();
}

void FullCamAdjust(U8 flag)
{
	if (flag==0)
	{
		if (FullCam==0)
			FullCam = Cam_Num-1;
		else
			FullCam --;
	}
	else 
	{
		if (FullCam == Cam_Num-1)
			FullCam = 0;
		else
			FullCam ++;
	}
	LFULL_cam_key_pro(FullCam);
}

void Mouse_Live_Fun(U16 x,U16 y,U8 type)
{
}

void LIVE_key_func(void)
{

	if(byKey_val==0 || (SystemLock && byKey_val != kLOCK && byKey_val != kESC)) 
	{
		return;
	}
	printf("LIVE_key_func,byKey_val = %x\n",byKey_val);
	
	switch(byKey_val)
	{
	case kLOCK:
		System_Lock();
		break;
	case kESC:
		if (SystemLock)
		{
			LedControl(LEDUSB, ON);
			buzzer_set(0);
			System_Lock();
		}
		else if (MenuStatus==mMouse)
			menu_mouse_off();
		else if (MenuStatus==Iconmenu)
			Icon_Menu_Off();
		else if (MenuStatus==(Iconmenu+mMouse))
		{
			menu_mouse_off();
			Icon_Menu_Off();
		}
		break;
	
	case kMU:
		buzzer_set(0);
		menu_main_on();
		break;
	case k4q:	
	case kUP:
		if(demo_num!=L4) 
			demo_page=0;
		else
		{
			if (Cam_Num>CHANNEL4)
			{
				demo_page++;
				if(demo_page>3) demo_page=0;
			}
			else
				break;
		}
		Demo_num_key_pro(L4,demo_page);
		break;
	case k8q:
	case kET:
		if (Cam_Num>CHANNEL4)
		{
			if(demo_num!=L8) 
				demo_page=0;
			else
			{
				demo_page++;
				if(demo_page>1) demo_page=0;
			}
			Demo_num_key_pro(L8,demo_page);
		}
		break;
	case k16q:
	case kDN:
		if (Cam_Num==CHANNEL16)
		{
			if(demo_num!=L16) 
				demo_page=0;
			else
			{
				demo_page++;
				if(demo_page>1) demo_page=0;
			}
			Demo_num_key_pro(L16,demo_page);
		}
		break;
	case kIN:
		if (MenuStatus<=mMouse)
			Icon_Menu_On(0,0);
		else
		{
			buzzer_set(0);
			OSD_ALL_CLEAR();
			Hdd_Info_Show();
		}
/*
		setled(m1,m2);
		if (m1==0x80)
		{
			if (m2==0)
				m2 = 1;
			else
				m2 *= 2;
		}
		else if (m1==0)
			m1=1;
		else
			m1*=2;
*/
		break;
	case kSQ:
		auto_func();
		break;
	case kSpot:
		menu_spot_on();
		break;
	case kSTOP:
/*		
		setled(m1,m2);
		if (m1==0x80)
		{
			if (m2==0)
				m2 = 1;
			else
				m2 *= 2;
		}
		else if (m1==0)
			m1=1;
		else
			m1*=2;
*/
//		menu_event_on();
		break;
	case k01:
	case k02:
	case k03:
	case k04:
	case k05:
	case k06:
	case k07:
	case k08:
	case k09:
		LFULL_cam_key_pro((U8)(byKey_val-k01));
		break;
	case k10:
		LFULL_cam_key_pro(9);
		break;
	case k11:
	case k12:
	case k13:
	case k14:
	case k15:
	case k16:
		LFULL_cam_key_pro((U8)(byKey_val-0xf7));
		break;
	case kREC:
		if (demo_num < PBFULL && Disk_Num)
		{
			record_key_set();
		}
		break;
	case kPLAY:
		if (rec_on==RECOFF&& Disk_Num)
		{
			menu_time_back_on();
		}
		break;
	case kSEA:
	case kBAK:
		printf("DiskNum=%x\n",Disk_Num);
		if (Disk_Num&& !CheckUser(menupass[curuser].Playcfig))
		{
			buzzer_set(0);
			OSD_ALL_CLEAR();
			menu_time_back_on();
		}
		break;
		
	case kPW:
		if (curuser==0)
		{
			buzzer_set(0);
			Menu_Power_On();
		}
		else
		{
			InfoId = PermissonErr;
			MenuErrOn();
		}
		break;

	case kAUDIO:
		audio_ch_on();
		break;
	case kPTZ:
		PTZ_Control_ON();
		break;
	case kCHAD:
		FullCamAdjust(1);
		break;
	case kCHDE:
		FullCamAdjust(0);
		break;
/*		
	case kBAK:
		if (Disk_Num)
		{
			buzzer_set(0);
			menu_time_back_on();
		}
		break;
*/
	default:
		break;
	}
	byKey_val=0;
}
