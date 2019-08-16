#include "cmn.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

U8 mouse_type,mouse_button;

U8 mouseflash=1;

U8 MseType;


extern U8 Ptz_key_long;


void mouse_handle()//24 leftdown,25rightup,26 leftup,27 rightdown
{
	if (menu_num)
	{
		switch(menu_num)
		{
			case mMAIN:
				if (mouse_type==26)
					MainLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_MainMenu_Fun(mouse_posx,mouse_posy);
				break;
			case mSYSTEM:
				if (mouse_type==26 || (mouse_type==31 && cur_pos<16))
					MouseLClick(mouse_posx,mouse_posy);
				else if (mouse_type == 25 || mouse_type==30)
					MouseRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Sys_Move_Func(mouse_posx,mouse_posy);
				break;
			case mCAMERA:
				Mouse_Cam_Move_Func(mouse_posx,mouse_posy,mouse_type);
				break;
			case mRECORD:
				if (mouse_type==26 || mouse_type==31)
					MouseRecLClick(mouse_posx,mouse_posy,mouse_type);
				else if (mouse_type == 25 || mouse_type==30)
					MouseRecRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Rec_Move_Func(mouse_posx,mouse_posy);
				break;
			case mSCHEDULE:
				if (mouse_type==26 || (mouse_type==31 && cur_pos != 1 && cur_pos != 3 && cur_pos != 24 && cur_pos != 25))
					MouseSchLClick(mouse_posx,mouse_posy);
				else if (mouse_type == 25 || mouse_type==30)
					MouseSchRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Sch_Move_Func(mouse_posx,mouse_posy);
				break;
			case mMOTION:
				Mouse_Mot_Move_Func(mouse_posx,mouse_posy,mouse_type);
				break;
			case mPTZ:
				if (mouse_type==26 || (mouse_type==31 && cur_pos < 4))
					MousePTZLClick(mouse_posx,mouse_posy);
				else if (mouse_type == 25 || mouse_type==30)
					MousePTZRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Ptz_Move_Func(mouse_posx,mouse_posy);
				break;
			case mEVENT:
				if (mouse_type == 25 || mouse_type==30)
					MouseEventRClick(mouse_posx,mouse_posy);
				else if (mouse_type==26 || (mouse_type==31 && cur_pos < 45))
					MouseEventLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Event_Move_Func(mouse_posx,mouse_posy);
				break;
			case mSTATIC:
				if (mouse_type==26 || (mouse_type==31 && cur_pos < 70))
					MouseNETLClick(mouse_posx,mouse_posy);
				else if (mouse_type == 25 || mouse_type==30)
					MouseNETRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Net_Move_Func(mouse_posx,mouse_posy);
				break;
			case mEMAIL:
				if (mouse_type==26)
					MouseEmailLClick(mouse_posx,mouse_posy);
				else if (mouse_type == 25)
					MouseEmailRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Email_Move_Func(mouse_posx,mouse_posy);
				break;
			case mMENUALARM:
				if (mouse_type==26)
					MouseAlarmLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Alarm_Move_Func(mouse_posx,mouse_posy);
				break;
			case mHDD:
				if (mouse_type==26 || (mouse_type==31 && cur_pos == 1))
					MouseHddLClick(mouse_posx,mouse_posy);
				else if (mouse_type == 25 || mouse_type==30)
					MouseHddRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Hdd_Move_Func(mouse_posx,mouse_posy);
				break;
				
			case mHDDINFO:
				if (mouse_type==26)
					HddinfoLClick(mouse_posx,mouse_posy);
//				else if (mouse_type == 25)
//					HddinfoRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_HddInfo_Move_Func(mouse_posx,mouse_posy);
				break;
			case mUSER:
				if (mouse_type==26 || mouse_type==31)
					MouseUserLClick(mouse_posx,mouse_posy,mouse_type);
				else if (mouse_type == 25 || mouse_type==30)
					MouseUserRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_User_Move_Func(mouse_posx,mouse_posy);
				break;
			case mPASS:
				if (mouse_type==26 || (mouse_type==31 && cur_pos==0))
					MousePassLClick(mouse_posx,mouse_posy);
				else if (mouse_type == 25 || mouse_type==30)
					MousePassRClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Pass_Move_Func(mouse_posx,mouse_posy);
				break;
			case mSPOT:
				if (mouse_type==26)
					SpotMouseLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_spot_Move_Func(mouse_posx,mouse_posy);
				break;
			case mAUDIO:
				if (mouse_type==26)
					AudioMouseLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_audio_Move_Func(mouse_posx,mouse_posy);
				break;
			case mMouseBak:
				Back_Key_func(mouse_posx,mouse_posy,mouse_type);
				break;
			case mRECSEARCH:
				Mouse_Sea_Move_Func(mouse_posx,mouse_posy,mouse_type);
				break;
			case mMouse:
				if (mouse_type==26)
					MouseMainLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_LiveMenu_Fun(mouse_posx,mouse_posy);
				break;
			case mMousePTZ:
				if (mouse_type==24)
					PTZMouseMainLClick(mouse_posx,mouse_posy);
				else if (mouse_type==26)
					PTZMouseMainLUClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_PTZMenu_Fun(mouse_posx,mouse_posy);
				break;
			case mERRINFO:
				if (mouse_type==26)
					ErrMouseLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Err_Move_Func(mouse_posx,mouse_posy);
				break;
			case mPTZControl:
				ptzmousekeyfun(mouse_posx,mouse_posy,mouse_type);
				break;
			case mBAKSTAU:
				if (mouse_type==26)
					BackupLclick(mouse_posx,mouse_posy);
				break;
			case mPOWER:
				if (mouse_type==26)
					PowerLClick(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Power_Move_Func(mouse_posx,mouse_posy);
				break;
			case mDISKFORMAT:
				if (mouse_type==26)
					Format_LClick_Fun(mouse_posx,mouse_posy);
				else if (mouse_type==0)
					Mouse_Format_Move_Func(mouse_posx,mouse_posy);
				break;
			case Iconmenu:
				if (mouse_type==26)
					Icon_LClick_Fun(mouse_posx,mouse_posy);
				else if (mouse_type==0) 
					Icon_Move_Fun(mouse_posx,mouse_posy);
				break;
			default:
				break;
		}
	}
	else if  (demo_num > L16)
		Mouse_Play_Func(mouse_posx,mouse_posy,mouse_type);
	else if (auto_flag)
		MouseAutoClick(mouse_type);
	else 
		Mouse_Live_Fun(mouse_posx,mouse_posy,mouse_type);
	
	if  (demo_num > L16 && menu_num >mPTZControl &&  menu_num <mMousePTZ)
		Play_Status_Show();
	mouse_type = 0;
//	printf("mouse_posx = %d,mouse_posy=%d,byKey_val=%d\n",mouse_posx,mouse_posy,byKey_val);

	
}

int my_handler (int x,int y,int button,U8 wheel)
{
	U8 tmp;
	U8 upf=0;
	if (button > 200)
		return 0;
	tmp = mouse_button  & 0x0f;
	mouse_button = button ;
	if ((tmp == 8) && ((mouse_button & 0x0f)== 9) )//leftup
		mouse_type = 24;
	else if ((tmp == 9) && ((mouse_button & 0x0f)== 8) )//lefdown
	{
		if (menu_num == mCAMERA)
			upf = getmousestatus();
		else if (menu_num == mMOTION)
			upf = getmousestatus2();
		else 
			upf = 0;
		mouse_type = 26;
	}
	else if ((tmp == 10) && ((mouse_button  & 0x0f)== 8) )//rightup
		mouse_type = 25;
	else if ((tmp == 8) && ((mouse_button  & 0x0f)== 10) )//rightdown
		mouse_type = 27;
	else if ((tmp == 8) && ((mouse_button  & 0x0f)== 12) )//wheelddown
		mouse_type = 28;
	else if ((tmp == 12) && ((mouse_button  & 0x0f)== 8) )//wheeldup
		mouse_type = 29;
	else if (wheel && wheel<10)//wheelnext
		mouse_type = 30;
	else if (wheel && wheel>250)//wheelpre
		mouse_type = 31;

	mouse_posx = x;
	mouse_posy = y;
	mouse_handle();
	return 0;       
}



