#include "cmn.h"

U8* autoicon;

U8 AutoTime;
U8 automode=0xff,autoch=0xff;
ValAuto AutoMenu[CHANNELNUM];
U8 auto_old_ch;

U8 olddem,oldchan;

void autochnamedisp(U8 ch);

void sequence_title_on(U8 dc)
{
}

void autochset(U8 ch,U8 qumode)
{
//	U8 i;
	if (automode != qumode)
	{
		if (qumode==LFULL)
		{
			LFULL_cam_key_pro(ch);
			auto_old_ch = ch;
		}
		else if (qumode==L4)
		{
			Demo_num_key_pro(L4,ch);
		}
		sequence_title_on(1);
		automode = qumode;	
	}
	else
	{
		if (qumode==LFULL)
		{
			gstCommonParam->GST_DRA_Hisi_window_vo(1,ch,0,0,0,0);
			auto_old_ch = ch;
			autochnamedisp(ch);
			get_demo_stamp(LFULL,ch,0);
		 	if(demo_num<PBFULL || demo_num==RECORD)
				record_disp_refresh();
		}
	}
}

U8 threadselect=0;

void auto_set()
{
//	 U8 i,temp;
//	 U8 wtemp,wtemp1;
//	sequence_title_on(1);
//	printf("********** Reflash_Page = %d\n",Reflash_Page);
	threadselect=1;
	switch(Reflash_Page)
	{
		case FourCh1:
			if (Cam_Num>4)
			{
				demo_page=1;
				Demo_num_key_pro(L4,demo_page);
				sequence_title_on(1);
				AutoTime=AutoMenu[0].KeepTime_A ;
			}
			break;			
		case FourCh2:
			if (Cam_Num>8)
				demo_page=2;
			else
				demo_page=0;
			Demo_num_key_pro(L4,demo_page);
			sequence_title_on(1);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case FourCh3:
			demo_page=3;
			Demo_num_key_pro(L4,demo_page);
			sequence_title_on(1);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case FourCh4:
			demo_page=0;
			Demo_num_key_pro(L4,demo_page);
			sequence_title_on(1);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case EightCh1:
			demo_page = 0;
			Demo_num_key_pro(L8,demo_page);
			sequence_title_on(1);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case SixtyCh:
//			LFULL_cam_key_pro(0);
//			sequence_title_on(1);
//			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case EightCh:
			if (Cam_Num==16)
			{
				demo_page = 1;
				Demo_num_key_pro(L8,demo_page);
				sequence_title_on(1);
				AutoTime=AutoMenu[0].KeepTime_A ;
			}
//			else			
//				autochset(0,LFULL);
			break;
		case Full1:
//			LFULL_cam_key_pro(1);
			autochset(1,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full2:
//			LFULL_cam_key_pro(2);
			autochset(2,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full3:
//			LFULL_cam_key_pro(3);
			autochset(3,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full4:
			if (Cam_Num>4)
			{
				autochset(4,LFULL);
				AutoTime=AutoMenu[0].KeepTime_A ;
			}
			else
			{
				demo_page=0;
				autochset(demo_page,LFULL);
				AutoTime=AutoMenu[0].KeepTime_A ;
			}
			break;
		case Full5:
			autochset(5,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full6:
			autochset(6,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full7:
			autochset(7,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full8:
			if (Cam_Num>8)
			{
				autochset(8,LFULL);
				AutoTime=AutoMenu[0].KeepTime_A ;
			}
			else 
			{
				demo_page=0;
				autochset(demo_page,LFULL);
				AutoTime=AutoMenu[0].KeepTime_A ;
			}
			break;
		case Full9:
			autochset(9,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full10:
			autochset(10,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full11:
			autochset(11,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full12:
			autochset(12,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full13:
			autochset(13,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full14:
			autochset(14,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full15:
			autochset(15,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
		case Full16:
			demo_page=0;
			autochset(demo_page,LFULL);
			AutoTime=AutoMenu[0].KeepTime_A ;
			break;
			
	}
	threadselect=0;
}

void auto_relate()
{
	if(AutoTime && auto_flag && valid_dispmode_change())
	{
		AutoTime--;
		if(AutoTime==0) 
			auto_set();
	}
}



void auto_on()
{
#if 0
	auto_flag=1;
/*	
	if (Cam_Num == 16)
		Reflash_Page = SixtyCh;
	else if (Cam_Num == 8)
		Reflash_Page = EightCh;
	else if (Cam_Num == 4)
		Reflash_Page = FourCh1;
*/
	automode=0xff;
	autoch=0xff;
	auto_set();
//	autochset(0,L4);
	sequence_title_on(1);
#endif	
}

void auto_func()
{
}

void auto_cam_func(U8 cam_no)
{
//	alm_disp_rst(cam_no);
	if (cam_no>=Cam_Num)
		return;
	auto_flag=0;
	LedControl(LEDSEQ,0);
	OSD_ALL_CLEAR();
//	sequence_title_on(0);
//	if(demo_num!=LFULL || demo_page!=cam_no)
	{
		get_demo_stamp(LFULL,cam_no,0);
		demo_set();
		demo_osd_disp();
	}
}

void AUTOSEQ_key_func()
{
	if((byKey_val==0) || (threadselect==1)) return;

	printf("AUTOSEQ_key_func byKey_val = %x\n",byKey_val);
	
	switch(byKey_val)
	{
		case kESC:
	   	case kSQ:	//AUTO     
			auto_flag=0;
			sequence_title_on(0);
			LedControl(LEDSEQ,0);
			if(demo_num!=olddem || demo_page!=oldchan)
			{
//				WriteOSDBmpX_bmp(80,8,WHITE,Blanking,Play_Menu);
				OSD_ALL_CLEAR();
				get_demo_stamp(olddem,oldchan,0);
				demo_set();
				demo_osd_disp();
			}
			else if (MenuStatus == Iconmenu)
				Icon_Menu_On(0, 0);
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
			auto_cam_func((U8)(byKey_val-k01));
			break;
		case k10:
			auto_cam_func(9);
			break;
		case k11:
		case k12:
		case k13:
		case k14:
		case k15:
		case k16:
			auto_cam_func((U8)(byKey_val-k11));
			break;
	}
	byKey_val=0;
}

void MouseAutoClick(U8 flag)
{
	if ((flag == 25 || flag == 26) && (threadselect==0))
	{
		auto_flag=0;
		sequence_title_on(0);
		LedControl(LEDSEQ,0);
		if(demo_num!=olddem || demo_page!=oldchan)
		{
			OSD_ALL_CLEAR();
			get_demo_stamp(olddem,oldchan,0);
			demo_set();
			demo_osd_disp();
		}
		else if (MenuStatus == Iconmenu)
			Icon_Menu_On(0, 0);
	}
}

