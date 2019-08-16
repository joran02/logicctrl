#include "cmn.h"

U8* menupoweroffpic;

U16 buttonpos[]=
{
	70,72,	242,72
};

U16 Pre_powerpos[] = 
{
	70,72,		230,102,
	242,72,		402,102
};


void Menu_Power_On()
{
}

void menu_power_off()
{
	OSD_ALL_CLEAR();
	menu_main_off();
}

void menu_power_move()
{
	if (cur_pos)
		cur_pos = 0;
	else
		cur_pos = 1;
	if (cur_pos)
	{
		Bmp_All_Show(buttonpos[0]+offsetx, buttonpos[1]+offsety, systemmenupic.okback,AD);
		Bmp_All_Show(buttonpos[0]+offsetx_hd, buttonpos[1]+offsety_hd, systemmenupic.okback,HD);
		Bmp_All_Show(buttonpos[2]+offsetx, buttonpos[3]+offsety, systemmenupic.exitfore,AD);
		Bmp_All_Show(buttonpos[2]+offsetx_hd, buttonpos[3]+offsety_hd, systemmenupic.exitfore,HD);
	}
	else
	{
		Bmp_All_Show(buttonpos[0]+offsetx, buttonpos[1]+offsety, systemmenupic.okfore,AD);
		Bmp_All_Show(buttonpos[0]+offsetx_hd, buttonpos[1]+offsety_hd, systemmenupic.okfore,HD);
		Bmp_All_Show(buttonpos[2]+offsetx, buttonpos[3]+offsety, systemmenupic.exitback,AD);
		Bmp_All_Show(buttonpos[2]+offsetx_hd, buttonpos[3]+offsety_hd, systemmenupic.exitback,HD);
	}
}

void menu_power_enter()
{
	if (!cur_pos)
	{
		LedControl(LEDPWR,0);
//		gstCommonParam->GST_FTC_StopSavePlay();
		TurnOnOff();
	}
	else
		menu_power_off();
}

void PowerLClick(U16 x,U16 y)
{
	U8 i;
	if (MseType == 0)
	{
		for (i=0;i<2;i++)
		{
			if (x>Pre_powerpos[i*4]+offsetx && x<Pre_powerpos[i*4+2]+offsetx && y>Pre_powerpos[i*4+1]+offsety && y<Pre_powerpos[i*4+3]+offsety)
				menu_power_enter();
		}	
	}
	else
	{
		for (i=0;i<2;i++)
		{
			if (x>Pre_powerpos[i*4]+offsetx_hd&& x<Pre_powerpos[i*4+2]+offsetx_hd&& y>Pre_powerpos[i*4+1]+offsety_hd&& y<Pre_powerpos[i*4+3]+offsety_hd)
				menu_power_enter();
		}	
	}
}

void Mouse_Power_Move_Func(U16 x,U16 y)
{
	U8 i;
	if (MseType == 0)
	{
		for (i=0;i<2;i++)
		{
			if (x>Pre_powerpos[i*4]+offsetx && x<Pre_powerpos[i*4+2]+offsetx && y>Pre_powerpos[i*4+1]+offsety && y<Pre_powerpos[i*4+3]+offsety && cur_pos !=i)
				menu_power_move();
		}		
	}
	else
	{
		for (i=0;i<2;i++)
		{
			if (x>Pre_powerpos[i*4]+offsetx_hd&& x<Pre_powerpos[i*4+2]+offsetx_hd&& y>Pre_powerpos[i*4+1]+offsety_hd&& y<Pre_powerpos[i*4+3]+offsety_hd&& cur_pos !=i)
				menu_power_move();
		}		
	}
}

void mMENUPOWER_Keyfun()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
		case kESC:
			menu_power_off();
			break;
		case kUP:
		case kLT:
		case kRH:
		case kDN:
			menu_power_move();
			break;
		case kET:
			menu_power_enter();
			break;
		default:
			break;
    }
	byKey_val=0;
}


