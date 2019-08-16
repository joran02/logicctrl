#include	"cmn.h"

#define MAINMAXMENU 10

U8 CopyRight[5];
MAINMENU mainmenupic;

U8 mainpos=0;

U16 offsetx,offsety;
U16 offsetx_hd,offsety_hd;


U16 menupicpos[]=
{
	28,88,	132,88,	236,88,	340,88,	444,88,
	28,216,	132,216,	236,216,	340,216,	444,216
};

U16 MainMenuPos[]=
{
	28,88,	132,216,
	132,88,	236,216,
	236,88,	340,216,
	340,88,	444,216,
	444,88,	548,216,

	28,216,	132,344,
	132,216,	236,344,
	236,216,	340,344,
	340,216,	444,344,
	444,216,	548,344,
	488,12,	556,40
};

void menu_main_data_disp(U8 pos,U16 color)
{
	switch(pos)
	{
		case 0:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.systemback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.systemfore,AD);
			}
			break;
		case 1:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.cameraback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.camerafore,AD);
			}
			break;
		case 2:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.recordback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.recordfore,AD);
			}
			break;
		case 3:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.scheduleback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.schedulefore,AD);
			}
			break;
		case 4:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.motionback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.motionfore,AD);
			}
			break;
		case 5:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.ptzback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.ptzfore,AD);
			}
			break;
		case 6:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.networkback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.networkfore,AD);
			}
			break;
		case 7:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.alarmback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.alarmfore,AD);
			}
			break;
		case 8:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.diskback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.diskfore,AD);
			}
			break;
		case 9:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.userback,AD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx,menupicpos[pos*2+1]+offsety,mainmenupic.userfore,AD);
			}
			break;
	}
	if (color==RED)
		mainpos = cur_pos;
}

void hdmenu_main_data_disp(U8 pos,U16 color)
{
	switch(pos)
	{
		case 0:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.systemback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.systemfore,HD);
			}
			break;
		case 1:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.cameraback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.camerafore,HD);
			}
			break;
		case 2:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.recordback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.recordfore,HD);
			}
			break;
		case 3:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.scheduleback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.schedulefore,HD);
			}
			break;
		case 4:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.motionback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.motionfore,HD);
			}
			break;
		case 5:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.ptzback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.ptzfore,HD);
			}
			break;
		case 6:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.networkback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.networkfore,HD);
			}
			break;
		case 7:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.alarmback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.alarmfore,HD);
			}
			break;
		case 8:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.diskback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.diskfore,HD);
			}
			break;
		case 9:
			if (color == WHITE)
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.userback,HD);
			}
			else
			{
				Bmp_All_Show(menupicpos[pos*2]+offsetx_hd,menupicpos[pos*2+1]+offsety_hd,mainmenupic.userfore,HD);
			}
			break;
	}
	if (color==RED)
		mainpos = cur_pos;
}


void menu_main_on()
{
}

void menu_main_off(void)
{
	cur_pos = 0;
	menu_num = 0;
    	OSD_ALL_CLEAR();
	alarm_minute();
	demo_osd_disp();

}

void menu_main_down()
{
	menu_main_data_disp(cur_pos,WHITE);
	hdmenu_main_data_disp(cur_pos,WHITE);
	if(cur_pos==MAINMAXMENU-1) cur_pos=0;
	else cur_pos++;
	menu_main_data_disp(cur_pos,RED);
	hdmenu_main_data_disp(cur_pos,RED);
}
void menu_main_up()
{
	menu_main_data_disp(cur_pos,WHITE);
	hdmenu_main_data_disp(cur_pos,WHITE);
	if(cur_pos==0) cur_pos=MAINMAXMENU-1;
	else cur_pos--;
	menu_main_data_disp(cur_pos,RED);
	hdmenu_main_data_disp(cur_pos,RED);
}

void menu_main_enter()
{
	switch(cur_pos)
	{
		case 0:
			cur_pos=0;
			OSD_ALL_CLEAR();
			menu_system_on();
			break;
		case 1:
			cur_pos=0;
			OSD_ALL_CLEAR();
			menu_camera_on();
			break;
		case 2:	
//			if (!rec_on)
//			{
				cur_pos=0;
				OSD_ALL_CLEAR();
				menu_record_on();
//			}
//			else if (rec_on)
//			{
//				InfoId = RecErr;
//				OSD_ALL_CLEAR();
//				MenuErrOn();
//			}
			break;
		case 3:	
			cur_pos=0;
			OSD_ALL_CLEAR();
		
			menu_schedule_on();
			break;
		case 4://9:		
			cur_pos=0;
			OSD_ALL_CLEAR();
			menu_motion_on();
			break;
		case 5:	
			cur_pos=0;
			OSD_ALL_CLEAR();
			menu_ptz_on();
			break;
		case 6://2:
//			if (!rec_on)
//			{
				cur_pos=0;
				OSD_ALL_CLEAR();
				menu_netstatic_on();
//			}
//			else if (rec_on)
//			{
//				InfoId = RecErr;
//				OSD_ALL_CLEAR();
//				MenuErrOn();
//			}
			break;
		case 7://10:	
			cur_pos=0;
			OSD_ALL_CLEAR();
			MenuAlarm_On();
			break;
		case 8:
			gstCommonParam->GST_FS_CheckUsb(&have_usb,&usb_format_is_right);
			if ((!rec_on && Disk_Num && demo_num < PBFULL)||(!rec_on && have_usb && demo_num < PBFULL))
			{
				cur_pos=0;
				OSD_ALL_CLEAR();
				menu_hdd_on();
			}
			else if (rec_on)
			{
				InfoId = RecErr;
				OSD_ALL_CLEAR();
				MenuErrOn();
			}
			break;
		case 9:
			cur_pos=0;
			OSD_ALL_CLEAR();
//			menu_user_on();
			menu_pass_on(0);		
			break;
		case 10:
			menu_main_off();
			break;
	}
}

void MainLClick(U16 x,U16 y)
{
	unsigned char i;
	unsigned int picleft,picright,pictop,picbottom;
	
	if (MseType == 0)
	{
		for (i=0;i<MAINMAXMENU+1;i++)
		{
			picleft = MainMenuPos[i*4]+offsetx;
			picright = MainMenuPos[i*4+2]+offsetx;
			pictop = MainMenuPos[i*4+1]+offsety;
			picbottom = MainMenuPos[i*4+3]+offsety;
			if((x > picleft && y > pictop) &&
				(x < picright && y < picbottom))
			{
				cur_pos = i;
				menu_main_enter();
				break;
			}
		}
	}
	else
	{
		for (i=0;i<MAINMAXMENU+1;i++)
		{
			picleft = MainMenuPos[i*4]+offsetx_hd;
			picright = MainMenuPos[i*4+2]+offsetx_hd;
			pictop = MainMenuPos[i*4+1]+offsety_hd;
			picbottom = MainMenuPos[i*4+3]+offsety_hd;
			if((x > picleft && y > pictop) &&
				(x < picright && y < picbottom))
			{
				cur_pos = i;
				menu_main_enter();
				break;
			}
		}
	}
}

void Mouse_MainMenu_Fun(U16 x,U16 y)
{
	unsigned char i;
	unsigned int picleft,picright,pictop,picbottom;
	if (MseType == 0)
	{
		for (i=0;i<MAINMAXMENU;i++)
		{
			picleft = MainMenuPos[i*4]+offsetx;
			picright = MainMenuPos[i*4+2]+offsetx;
			pictop = MainMenuPos[i*4+1]+offsety;
			picbottom = MainMenuPos[i*4+3]+offsety;
			if((x > picleft && y > pictop) &&
				(x < picright && y < picbottom) && (cur_pos != i))
			{
				menu_main_data_disp(cur_pos,WHITE);
				hdmenu_main_data_disp(cur_pos,WHITE);
				cur_pos=i;
				menu_main_data_disp(cur_pos,RED);
				hdmenu_main_data_disp(cur_pos,RED);
				break;
			}
		}
	}
	else
	{
		for (i=0;i<MAINMAXMENU;i++)
		{
			picleft = MainMenuPos[i*4]+offsetx_hd;
			picright = MainMenuPos[i*4+2]+offsetx_hd;
			pictop = MainMenuPos[i*4+1]+offsety_hd;
			picbottom = MainMenuPos[i*4+3]+offsety_hd;
			if((x > picleft && y > pictop) &&
				(x < picright && y < picbottom) && (cur_pos != i))
			{
				menu_main_data_disp(cur_pos,WHITE);
				hdmenu_main_data_disp(cur_pos,WHITE);
				cur_pos=i;
				menu_main_data_disp(cur_pos,RED);
				hdmenu_main_data_disp(cur_pos,RED);
				break;
			}
		}
	}
}


//******************************key func*************************************
void mMAIN_key_func()
{
	U8 i;
	if(byKey_val==0) 
	{
//		menu_main_off();
		return;
	}
	printf("mMAIN_key_func,byKey_val = %x\n",byKey_val);
	switch(byKey_val)
	{
		case kESC:	//menu        exit
			CopyRight[0] = 0;
			menu_main_off();
			break;
		case kUP:	
		case kLT:
			CopyRight[0] = 0;
			menu_main_up();
		    	break;
		case kDN:                  
		case kRH:
			CopyRight[0] = 0;
			menu_main_down();
			break;
		case kET:                //call
			CopyRight[0] = 0;
			menu_main_enter();
			break;
		case kPTZ:
			CopyRight[0] = 0;
	//		PTZ_Control_ON();
			break;
		case kREC:
			CopyRight[0]=0xaa;
			CopyRight[4] = 0;
			break;
		case kPLAY:
			CopyRight[1]=0xaa;
			CopyRight[4] = 0;
			break;
		case kSTOP:
			CopyRight[2]=0xaa;
			CopyRight[4] = 0;
			break;
		case kSEA:
			CopyRight[3]=0xaa;
			for (i=0;i<4;i++)
			{
				if (CopyRight[i] != 0xaa)
					break;
			}
			if (i>3)
			{
//				Bmp_Port_Show(Err_Title.CopyRight_Show,3,13,0,0,0,0,WHITE);
			}
			break;
		default:
			CopyRight[0] = 0;
			break;
	}
		
	byKey_val=0;
}

