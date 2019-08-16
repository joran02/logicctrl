#include "cmn.h"
ValPTZ PtzMenu[CHANNELNUM];
PTZMENU ptzmenupic;

#define PTZFIXEDMENU 6

U8 MenuPTZNum=0;

 U8 Menu_PTZ_DataPos[]=
{
	236,74,
	236,100,
	236,126,
	236,152,
	50,220,
	224,220
};

U16 Pre_ptzpos[] = 
{
	236,74,		362,98,
	236,100,		362,124,
	236,126,		362,150,
	236,152,		362,176,
	50,220,		210,244,
	224,220,		384,244
};

extern U8 NetCam;

void MenuPtzInit()
{
	U8 i;
	for (i=0;i<Cam_Num;i++)
	{
		I_read_block_eeprom(ADDPTZ+i*ADDCOUNTPTZ,sizeof(PtzMenu[i]),&PtzMenu[i].BaudRate);
	}
	NetCam = 0xff;
}

void menu_ptz_data_disp(U8 pos,U16 color)
{
}

void menu_ptz_on()
{
}

void menu_ptz_down()
{
	menu_ptz_data_disp(cur_pos,WHITE);
	if(cur_pos>=PTZFIXEDMENU-1)	
		cur_pos = 0;  
	else
		cur_pos++;
	menu_ptz_data_disp(cur_pos,RED);
}
void menu_ptz_up()
{
	menu_ptz_data_disp(cur_pos,WHITE);
	if(cur_pos<=0)
		cur_pos = PTZFIXEDMENU-1;         
	else
		cur_pos--;
	menu_ptz_data_disp(cur_pos,RED);
}

void menu_ptz_do()
{
	U8 i;
	for (i=0;i<Cam_Num;i++)
	{
		I_write_block_eeprom(ADDPTZ+i*ADDCOUNTPTZ,sizeof(PtzMenu[i]),&PtzMenu[i].BaudRate);
	}
}

void menu_ptz_add()
{
	 U8 i;
	switch(cur_pos)
	{
	case 0:
		MenuPTZNum++;
		if(MenuPTZNum>(Cam_Num-1)) MenuPTZNum=0;
		for(i=1;i<PTZFIXEDMENU;i++)
		{
			menu_ptz_data_disp(i,WHITE);
		}
		break;
	case 1:
		if (PtzMenu[MenuPTZNum].BaudRate>=3)
			PtzMenu[MenuPTZNum].BaudRate=0;
		else
			PtzMenu[MenuPTZNum].BaudRate++;
		break;
	case 2:
		if (PtzMenu[MenuPTZNum].DeviceID>=0xfe)
			PtzMenu[MenuPTZNum].DeviceID=0;
		else
			PtzMenu[MenuPTZNum].DeviceID++;
		break;
	case 3:
		if (PtzMenu[MenuPTZNum].Protocol>=2)
			PtzMenu[MenuPTZNum].Protocol=0;
		else
			PtzMenu[MenuPTZNum].Protocol++;
		break;
	}
	menu_ptz_data_disp(cur_pos,RED);

}
void menu_ptz_dec()
{
	 U8 i;
	switch(cur_pos)
	{
	case 0:
		MenuPTZNum--;
		if(MenuPTZNum>(Cam_Num-1))MenuPTZNum=Cam_Num-1;
		for(i=1;i<PTZFIXEDMENU;i++)
		{
			menu_ptz_data_disp(i,WHITE);
		}
		break;
	case 1:
		if (PtzMenu[MenuPTZNum].BaudRate<=0)
			PtzMenu[MenuPTZNum].BaudRate=3;
		else
			PtzMenu[MenuPTZNum].BaudRate--;
		break;
	case 2:
		if (PtzMenu[MenuPTZNum].DeviceID<=0)
			PtzMenu[MenuPTZNum].DeviceID=0xfe;
		else
			PtzMenu[MenuPTZNum].DeviceID--;
		break;
	case 3:
		if (PtzMenu[MenuPTZNum].Protocol<=0)
			PtzMenu[MenuPTZNum].Protocol=2;
		else
			PtzMenu[MenuPTZNum].Protocol--;
		break;
		break;
	}
	menu_ptz_data_disp(cur_pos,RED);
}


void menu_ptz_off()
{
    	OSD_ALL_CLEAR();
	MenuPtzInit();
	menu_main_on();
}

void menu_ptz_enter()
{
	if (cur_pos==PTZFIXEDMENU-1)
		menu_ptz_off();
	else if (cur_pos==PTZFIXEDMENU-2)
	{
		menu_ptz_do();
		menu_ptz_off();
	}
}

void MousePTZRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_ptzpos[cur_pos*4]+offsetx_hd;
		miny = Pre_ptzpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_ptzpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_ptzpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_ptzpos[cur_pos*4]+offsetx;
		miny = Pre_ptzpos[cur_pos*4+1]+offsety;
		maxx = Pre_ptzpos[cur_pos*4+2]+offsetx;
		maxy = Pre_ptzpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		menu_ptz_add(); 
}

void MousePTZLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_ptzpos[cur_pos*4]+offsetx_hd;
		miny = Pre_ptzpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_ptzpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_ptzpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_ptzpos[cur_pos*4]+offsetx;
		miny = Pre_ptzpos[cur_pos*4+1]+offsety;
		maxx = Pre_ptzpos[cur_pos*4+2]+offsetx;
		maxy = Pre_ptzpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos<PTZFIXEDMENU-2)
			menu_ptz_dec(); 
		else
			menu_ptz_enter();
	}
}

void Mouse_Ptz_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<PTZFIXEDMENU;i++)
	{
		if (MseType)
		{
			minx = Pre_ptzpos[i*4]+offsetx_hd;
			miny = Pre_ptzpos[i*4+1]+offsety_hd;
			maxx = Pre_ptzpos[i*4+2]+offsetx_hd;
			maxy = Pre_ptzpos[i*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_ptzpos[i*4]+offsetx;
			miny = Pre_ptzpos[i*4+1]+offsety;
			maxx = Pre_ptzpos[i*4+2]+offsetx;
			maxy = Pre_ptzpos[i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
			menu_ptz_data_disp(cur_pos,WHITE);
			cur_pos=i;
			menu_ptz_data_disp(cur_pos,RED);
			break;
		}
	}
}


void mPTZ_key_func()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
	case kESC:
		menu_ptz_off();
		break;
	case kUP:
		menu_ptz_up();
   		break;
	case kDN:
		menu_ptz_down();
		break;
	case kRH:
	case kAD:
 		menu_ptz_add(); 
		break;
   	case kLT:
	case kDE:
 		menu_ptz_dec(); 
		break;
   	case kET:
		menu_ptz_enter();
		break;
	default:
		break;
    }
	byKey_val=0;
}


