#include "cmn.h"

U8 RecFlag[CHANNELNUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},Cam_Rec=0xff;  //0 off 1 by manual  2 by sensor  3  motion 4  time
U8 RecFlagOld[CHANNELNUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},RecStatus=0x00;

U8* recordico;

void Record_Stop_Function()
{
	U8 i;
	printf("Record End!\n");
	i= RECOFF;
	if (gstCommonParam->GST_FTC_StopRec()<0)
	{
		bReset = 1;
		printf("StopRec err!");
		return;
	}
	else
	{
		for (i=0;i<Cam_Num;i++)
			RecFlag[i]=RECOFF;
//		record_disp();
		demo_num = L4;
		LedControl(LEDREC,0);
		event_write(REC_STOP_BYHAND,0,0);
	}
	printf("Record End!\n");
}

extern U8 CheckCodeflag;

void CheckRecord()
{
	U8 tmp1=1;
	MenuMotionInit();
	WatchDog();
	gstCommonParam->GST_FTC_PreviewRecordStart(5);
//	if(CheckCodeflag)
//		tmp1 = 0;

	if (Disk_Num && tmp1 && (demo_num < PBFULL))
	{
		record_key_set();
	
//		InfoId = ConRec;
//		MenuErrOn();
	}

	
}

void Rec_key_func()
{
//	record_disp();
	if(byKey_val==0 || (SystemLock && byKey_val != kLOCK && byKey_val != kESC)) 
	{
		byKey_val = 0;
		return;
	}

	switch(byKey_val)
	{
		case kLOCK:
//		case kESC:
			System_Lock();
			break;
/*
		case kMU:
			if (tmp == 0x23)
			{
				demo_num=L4;
				demo_page=demo_page;
				get_demo_stamp(demo_num,demo_page,0);
				demo_set();  	
				OSD_ALL_CLEAR();
				menu_pass_on();
			}
			break;
*/
		case kMU:
			buzzer_set(0);
			OSD_ALL_CLEAR();
			menu_main_on();
			break;
		case kESC:
			if (SystemLock)
				System_Lock();
			else if (MenuStatus==mMouse)
				menu_mouse_off();
			else if (MenuStatus==Iconmenu)
				Icon_Menu_Off();
			else if (MenuStatus==(Iconmenu+mMouse))
			{
				menu_mouse_off();
				Icon_Menu_Off();
			}
			else if (CheckUser(menupass[curuser].Stopcfig) == 0)
			{
				InfoId = StopRecord;
				MenuErrOn();
			}
			break;
		case kSTOP:
			if (CheckUser(menupass[curuser].Stopcfig) == 0)
			{
				InfoId = StopRecord;
				MenuErrOn();
			}
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
			break;
		case kAUDIO:
			audio_ch_on();
			break;
		case kPTZ:
			PTZ_Control_ON();
			break;
		case kPLAY:
			menu_time_back_on();
			break;
		case kSQ:
			auto_func();
			break;
		case kFFAST:
		case kSpot:
			menu_spot_on();
			break;
		case kSEA:
			if (!CheckUser(menupass[curuser].Playcfig))
			{
				buzzer_set(0);
				OSD_ALL_CLEAR();
	//			menu_search_on();
				menu_rec_search_on();
			}
			break;
		case kBAK:
			buzzer_set(0);
			menu_time_back_on();
			break;
		case kPW:
			Menu_Power_On();
			break;
		case kCHAD:
			FullCamAdjust(1);
			break;
		case kCHDE:
			FullCamAdjust(0);
			break;
		default:
			break;
    }
	byKey_val=0;
}

