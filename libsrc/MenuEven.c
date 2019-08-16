#include "cmn.h"

#define LOGMAXMENU 9

EVENTMENU eventmenupic;

#define START	0
#define END		1

 U8 log_cur_page=0;
 U8 log_all_page=0;
 U8 logevenpos=0;
 U8 log_color=0;
 static int bufyear[2],bufmonth[2],bufday[2];


 UC disp_tvt = 1;


 U16 menu_log_pos_ea[]=
 {
	160,66,	220,66,	256,66,	412,66,	472,66,	508,66,
										480,346,
		106,380,				334,380
 };

 U16 menu_log_pos_am[]=
 {
	160,66,	196,66,	232,66,	412,66,	448,66,	484,66,
										480,346,
		106,380,				334,380
 };

 U16* menu_log_pos[]=
 {
	menu_log_pos_ea,
	menu_log_pos_am,
	menu_log_pos_am
 };

U16 pre_log_pos_ea[]=
{
	160,66,	208,90,
	220,66,	244,90,
	256,66,	280,90,
	412,66,	460,90,
	472,66,	496,90,
	508,66,	532,90,
	480,346,	504,368,
	106,380,	266,410,
	334,380,	494,410
};

U16 pre_log_pos_am[]=
{
	160,66,	184,90,
	196,66,	220,90,
	232,66,	280,90,
	412,66,	436,90,
	448,66,	472,90,
	484,66,	532,90,
	480,346,	504,368,
	106,380,	266,410,
	334,380,	494,410
};

 U16* pre_log_pos[]=
 {
	pre_log_pos_ea,
	pre_log_pos_am,
	pre_log_pos_am
 };

void menu_log_data_disp(U8 pos,U16 color)
{
}

U8 menu_even_search(int page)
{
	return 1;
}

void menu_even_search_init()
{
}


void event_data_disp()
{
}

void menu_event_on()
{
}

void menu_event_down()
{
	menu_log_data_disp(cur_pos,WHITE);
	if (cur_pos==LOGMAXMENU-1)
		cur_pos=0;
	else
		cur_pos++;
	menu_log_data_disp(cur_pos,RED);
}
void menu_event_up()
{
	menu_log_data_disp(cur_pos,WHITE);
	if (cur_pos == 0)
		cur_pos=LOGMAXMENU-1;
	else
		cur_pos --;
	menu_log_data_disp(cur_pos,RED);
}

void menu_event_add()
{
	switch(cur_pos)
	{
		case 0:
			if (SysMenu.tmregion==0)
			{
				if (bufyear[START]>=99)
					bufyear[START] = 0x00;
				else
					bufyear[START]++;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufmonth[START]>=12)
					bufmonth[START]=1;
				else
					bufmonth[START]++;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufday[START]>=31)
					bufday[START]=1;
				else
					bufday[START]++;
			}
			break;
		case 1:
			if (SysMenu.tmregion==0)
			{
				if (bufmonth[START]>=12)
					bufmonth[START]=1;
				else
					bufmonth[START]++;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufday[START]>=31)
					bufday[START]=1;
				else
					bufday[START]++;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufmonth[START]>=12)
					bufmonth[START]=1;
				else
					bufmonth[START]++;
			}
			break;
		case 2:
			if (SysMenu.tmregion==0)
			{
				if (bufday[START]>=31)
					bufday[START]=1;
				else
					bufday[START]++;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufyear[START]>=99)
					bufyear[START] = 0x00;
				else
					bufyear[START]++;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufyear[START]>=99)
					bufyear[START] = 0x00;
				else
					bufyear[START]++;
			}
			break;
		case 3:
			if (SysMenu.tmregion==0)
			{
				if (bufyear[END]>=99)
					bufyear[END] = 0x00;
				else
					bufyear[END]++;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufmonth[END]>=12)
					bufmonth[END]=1;
				else
					bufmonth[END]++;
			}
			else if (SysMenu.tmregion)
			{
				if (bufday[END]>=31)
					bufday[END]=1;
				else
					bufday[END]++;
			}
			break;
		case 4:
			if (SysMenu.tmregion==0)
			{
				if (bufmonth[END]>=12)
					bufmonth[END]=1;
				else
					bufmonth[END]++;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufday[END]>=31)
					bufday[END]=1;
				else
					bufday[END]++;
			}
			else if (SysMenu.tmregion)
			{
				if (bufmonth[END]>=12)
					bufmonth[END]=1;
				else
					bufmonth[END]++;
			}
			break;
		case 5:
			if (SysMenu.tmregion==0)
			{
				if (bufday[END]>=31)
					bufday[END]=1;
				else
					bufday[END]++;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufyear[END]>=99)
					bufyear[END] = 0x00;
				else
					bufyear[END]++;
			}
			else if (SysMenu.tmregion)
			{
				if (bufyear[END]>=99)
					bufyear[END] = 0x00;
				else
					bufyear[END]++;
			}
			break;
		case 6:
			if (log_cur_page>=log_all_page-1)
				log_cur_page=0;
			else
				log_cur_page++;
			if (menu_even_search(log_cur_page))
				event_data_disp();
			break;
	}
	menu_log_data_disp(cur_pos,RED);
}

void menu_event_dec()
{
	switch(cur_pos)
	{
		case 0:
			if (SysMenu.tmregion==0)
			{
				if (bufyear[START]<=0)
					bufyear[START] = 99;
				else
					bufyear[START]--;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufmonth[START]<=1)
					bufmonth[START]=12;
				else
					bufmonth[START]--;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufday[START]<=1)
					bufday[START]=31;
				else
					bufday[START]--;
			}
			break;
		case 1:
			if (SysMenu.tmregion==0)
			{
				if (bufmonth[START]<=1)
					bufmonth[START]=12;
				else
					bufmonth[START]--;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufday[START]<=1)
					bufday[START]=31;
				else
					bufday[START]--;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufmonth[START]<=1)
					bufmonth[START]=12;
				else
					bufmonth[START]--;
			}
			break;
		case 2:
			if (SysMenu.tmregion==0)
			{
				if (bufday[START]<=1)
					bufday[START]=31;
				else
					bufday[START]--;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufyear[START]<=0)
					bufyear[START] = 99;
				else
					bufyear[START]--;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufyear[START]<=0)
					bufyear[START] = 99;
				else
					bufyear[START]--;
			}
			break;
		case 3:
			if (SysMenu.tmregion==0)
			{
				if (bufyear[END]<=0)
					bufyear[END] = 99;
				else
					bufyear[END]--;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufmonth[END]<=1)
					bufmonth[END]=12;
				else
					bufmonth[END]--;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufday[END]<=1)
					bufday[END]=31;
				else
					bufday[END]--;
			}
			break;
		case 4:
			if (SysMenu.tmregion==0)
			{
				if (bufmonth[END]<=1)
					bufmonth[END]=12;
				else
					bufmonth[END]--;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufday[END]<=1)
					bufday[END]=31;
				else
					bufday[END]--;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufmonth[END]<=1)
					bufmonth[END]=12;
				else
					bufmonth[END]--;
			}
			break;
		case 5:
			if (SysMenu.tmregion==0)
			{
				if (bufday[END]<=1)
					bufday[END]=31;
				else
					bufday[END]--;
			}
			else if (SysMenu.tmregion==1)
			{
				if (bufyear[END]<=0)
					bufyear[END] = 99;
				else
					bufyear[END]--;
			}
			else if (SysMenu.tmregion==2)
			{
				if (bufyear[END]<=0)
					bufyear[END] = 99;
				else
					bufyear[END]--;
			}
			break;
		case 6:
			if (log_cur_page<=0)
			{
				if (log_all_page>=1)
					log_cur_page=log_all_page-1;
				else
					log_cur_page=0;
			}
			else
				log_cur_page--;
			if (menu_even_search(log_cur_page))
				event_data_disp();
			break;
	}
	menu_log_data_disp(cur_pos,RED);
}

void menu_event_enter()
{
	if (cur_pos == 7)
	{	
		log_cur_page = 0;
		menu_even_search(log_cur_page);
		event_data_disp();
		if(cur_pos != 7)menu_log_data_disp(7,WHITE);
		else menu_log_data_disp(7,RED);

	}
	else if (cur_pos == 8)
		menu_main_off();
}

void MouseEventRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = pre_log_pos[SysMenu.tmregion][cur_pos*4]+offsetx_hd;
		miny = pre_log_pos[SysMenu.tmregion][cur_pos*4+1]+offsety_hd;
		maxx = pre_log_pos[SysMenu.tmregion][cur_pos*4+2]+offsetx_hd;
		maxy = pre_log_pos[SysMenu.tmregion][cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = pre_log_pos[SysMenu.tmregion][cur_pos*4]+offsetx;
		miny = pre_log_pos[SysMenu.tmregion][cur_pos*4+1]+offsety;
		maxx = pre_log_pos[SysMenu.tmregion][cur_pos*4+2]+offsetx;
		maxy = pre_log_pos[SysMenu.tmregion][cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		menu_event_add(); 
}

void MouseEventLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = pre_log_pos[SysMenu.tmregion][cur_pos*4]+offsetx_hd;
		miny = pre_log_pos[SysMenu.tmregion][cur_pos*4+1]+offsety_hd;
		maxx = pre_log_pos[SysMenu.tmregion][cur_pos*4+2]+offsetx_hd;
		maxy = pre_log_pos[SysMenu.tmregion][cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = pre_log_pos[SysMenu.tmregion][cur_pos*4]+offsetx;
		miny = pre_log_pos[SysMenu.tmregion][cur_pos*4+1]+offsety;
		maxx = pre_log_pos[SysMenu.tmregion][cur_pos*4+2]+offsetx;
		maxy = pre_log_pos[SysMenu.tmregion][cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos<LOGMAXMENU-2)
			menu_event_dec(); 
		else
			menu_event_enter();
	}
}

void Mouse_Event_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<LOGMAXMENU ;i++)
	{
		if (MseType)
		{
			minx = pre_log_pos[SysMenu.tmregion][i*4]+offsetx_hd;
			miny = pre_log_pos[SysMenu.tmregion][i*4+1]+offsety_hd;
			maxx = pre_log_pos[SysMenu.tmregion][i*4+2]+offsetx_hd;
			maxy = pre_log_pos[SysMenu.tmregion][i*4+3]+offsety_hd;
		}
		else
		{
			minx = pre_log_pos[SysMenu.tmregion][i*4]+offsetx;
			miny = pre_log_pos[SysMenu.tmregion][i*4+1]+offsety;
			maxx = pre_log_pos[SysMenu.tmregion][i*4+2]+offsetx;
			maxy = pre_log_pos[SysMenu.tmregion][i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
			menu_log_data_disp(cur_pos,WHITE);
			cur_pos=i;
			menu_log_data_disp(cur_pos,RED);
			break;
		}
	}
}

void mEVENT_key_func()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
	case kESC:
		menu_main_off();
		break;
	case kUP:
		menu_event_up();
   		break;
	case kDN:
		menu_event_down();
		break;
	case kLT:
		menu_event_dec();
		break;
	case kRH:
		menu_event_add();
		break;
	case kET:
		menu_event_enter();
		break;
	default:
		break;
    }
	byKey_val=0;
}
