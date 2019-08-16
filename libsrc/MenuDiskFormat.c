#include "cmn.h"

#define DISKFORMATMAXMENU	2

void menu_diskformat_disp(U8 pos,U16 color)
{
}

void menu_diskformat_on()
{
}

void menu_diskformat_up()
{
	menu_diskformat_disp(cur_pos,RED);
	cur_pos=1-cur_pos;
	menu_diskformat_disp(cur_pos,WHITE);
}

void menu_diskformat_off()
{
	cur_pos = 0;
	menu_num = 0;
    	OSD_ALL_CLEAR();
}

void menu_diskformat_enter()
{
	switch(cur_pos)
	{
		case 0:
			DISK_Format(40+offsetx,40+offsety);
		case 1:
			menu_diskformat_off();
			break;
	}
}

//******************************key func*************************************

U16 Pre_formatpos[] = 
{
	50,75,		210,105,
	260,75,		420,105
};

void Format_LClick_Fun(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_formatpos[cur_pos*4]+offsetx_hd;
		miny = Pre_formatpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_formatpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_formatpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_formatpos[cur_pos*4]+offsetx;
		miny = Pre_formatpos[cur_pos*4+1]+offsety;
		maxx = Pre_formatpos[cur_pos*4+2]+offsetx;
		maxy = Pre_formatpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		menu_diskformat_enter();
	}
}

void Mouse_Format_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<DISKFORMATMAXMENU;i++)
	{
		if (MseType)
		{
			minx = Pre_formatpos[i*4]+offsetx_hd;
			miny = Pre_formatpos[i*4+1]+offsety_hd;
			maxx = Pre_formatpos[i*4+2]+offsetx_hd;
			maxy = Pre_formatpos[i*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_formatpos[i*4]+offsetx;
			miny = Pre_formatpos[i*4+1]+offsety;
			maxx = Pre_formatpos[i*4+2]+offsetx;
			maxy = Pre_formatpos[i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
			menu_diskformat_disp(cur_pos,RED);
			cur_pos=i;
			menu_diskformat_disp(cur_pos,WHITE);
			break;
		}
	}
}

void mDISKFORMAT_key_func()
{
	if(byKey_val==0) return;
	switch(byKey_val)                   
	{
		case kESC:	//menu        exit
			menu_diskformat_off();
			break;
		case kUP:
		case kDN:
		case kLT:	//menu       left
		case kRH:	//menu       right
			menu_diskformat_up();
			break;
		case kET:	     //16sp        Enter  
			menu_diskformat_enter();
			break;
	default:
		break;
       }
	byKey_val=0;
}


