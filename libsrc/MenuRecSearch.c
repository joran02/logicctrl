#include "cmn.h"

#define SEARCHMENULISTNUM		12
int time_year,time_month,time_day,time_hour,time_minute;
int cur_page_t,total_page;
GST_FILESHOWINFO file_show[RecodSeaFileCount];

time_t start_time_Whole;

U8 selectcam[4]={1,2,3,4};
U8 Play_Cam[16];

SEARCHMENU searchmenupic;
int focus_x ,focus_y,focus_type,pointx= 50;
int old_posx,pointer[4];
U8 seacourormove=0;
U16 searrowposx,searrowposy;
U16 searrowposxhd,searrowposyhd;


U16 menu_rec_search_pos_ea[]=
 {
	226,102,		340,102,	454,102,	568,102,
	226,128,		286,128,	322,128,	
	226,154,		262,154,
		84,358,		442,358
 };

U16 menu_rec_search_pos_am[]=
 {
	226,102,		340,102,	454,102,	568,102,
	226,128,		262,128,	298,128,	
	226,154,		262,154,
		84,358,		442,358
 };

U16* menu_rec_search_pos[]=
 {
	menu_rec_search_pos_ea,	
	menu_rec_search_pos_am,
	menu_rec_search_pos_am
	
 };

U16 Pre_seapos_ea[] = 
{
	226,102,		250,128,
	340,102,		364,128,
	454,102,		478,128,
	568,102,		592,128,

	226,128,		274,148,
	286,128,		310,148,
	322,128,		346,148,

	226,154,		250,174,
	262,154,		286,174,
	
	84,358,		244,382,

	442,358,		602,382
};

U16 Pre_seapos_amoreu[] = 
{
	226,102,		250,128,
	340,102,		364,128,
	454,102,		478,128,
	568,102,		592,128,

	226,128,		250,148,
	262,128,		286,148,
	298,128,		346,148,

	226,154,		250,174,
	262,154,		286,174,
	
	84,358,		244,382,

	442,358,		602,382
};

U16 *Pre_seapos[] = 
{
	Pre_seapos_ea,
	Pre_seapos_amoreu,
	Pre_seapos_amoreu
};
void SearchClick();

void PlayCam(U8 ch)
{
//	time_t start_time1;
//	int ret;
	if (ch >= Cam_Num)
		return;
//	printf("########################## %d\n",ch);
	
	PlayStatusFlag.PlayFSpeed =0;
	PlayStatusFlag.PlayBSpeed =0;
	Play_Mode =100+ch;
	Split_Play(Play_Mode, 1, 0);
//	printf("*************************************%d\n",ch);
	
/*	
	Play_Cam[0] = ch;
	Play_Cam[1] = 0xff;
	if (Mouse_PlaySta == 10)
		start_time1 = start_time_Whole;
	else
		gstCommonParam->GST_FTC_get_play_time(&start_time1);
	gstCommonParam->GST_FTC_StopPlay();
//jlb090820	gstCommonParam->GST_DRA_set_single_play_back_window(0,0,0,WIDTH_RESOLUTION,High_Resolution);
	ret = gstCommonParam->GST_FTC_SetTimeToPlay(start_time1,0,WBIT(Play_Cam[0]),1);
	if( ret < 0 )
	{
		InfoId=TimeErr;
		Play_Err_Stop();
		MenuErrOn();
		return;
	}
	OSD_ALL_CLEAR();
	Play_Ch_Show();
*/
}


void menu_search_data_disp(U8 pos,U16 color)
{
}

void menu_searrow_time_on(time_t timebuf,U8 color)
{
}

void searrowmove(U16 x)
{
}

void setime_to_pos(int h,int m)
{
}

void menu_vail_time_on()
{
}

void menu_rec_search_init()
{
	 struct timeval tv;
	 struct tm tp;

	 gettimeofday( &tv, NULL );	
	 gmtime_r(&tv.tv_sec,&tp);
	 
	if (time_year == 0)
		time_year = tp.tm_year + 1900;
	if (time_month == 0)
		time_month = tp.tm_mon + 1;
	if (time_day == 0)
		time_day = tp.tm_mday;
	if (time_hour == 0)
		time_hour = tp.tm_hour;
	if (time_minute == 0)
		time_minute = tp.tm_min;

	cur_page_t = 0;
	total_page = 0;	
	menu_vail_time_on();
}

 void menu_rec_search_on()
{	
}


void menu_rec_search_off()
{
	
	cur_pos = 0;
	menu_num = 0;
    	OSD_ALL_CLEAR();
	if (demo_num<PBFULL)
	{
		demo_osd_disp();
	}
	else
	{
		Play_Ch_Show();
		Playstatus = 0xff;
	}
}

void menu_search_down()
{
	menu_search_data_disp(cur_pos,WHITE);
	if(cur_pos==SEARCHMENULISTNUM-1) cur_pos=0;
	else cur_pos++;	
	menu_search_data_disp(cur_pos,RED);
}
void menu_search_up()
{
	menu_search_data_disp(cur_pos,WHITE);
	if(cur_pos==0) cur_pos=SEARCHMENULISTNUM-1;
	else cur_pos--;
	menu_search_data_disp(cur_pos,RED);
}

void menu_search_add()
{
	U8 tmp;
	switch(cur_pos)
	{
		case 0:
			tmp = selectcam[0];
			while (++selectcam[0])
			{
				if (selectcam[0]>Cam_Num)
				{
					selectcam[0] = 0;
					break;
				}
				else if ((selectcam[0] != selectcam[1])&&(selectcam[0] != selectcam[2])&&(selectcam[0] != selectcam[3]))
					break;
			}
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[0] = tmp;
			break;
		case 1:
			tmp = selectcam[1];
			while (++selectcam[1])
			{
				if (selectcam[1]>Cam_Num)
				{
					selectcam[1] = 0;
					break;
				}
				else if ((selectcam[1] != selectcam[0])&&(selectcam[1] != selectcam[3])&&(selectcam[1] != selectcam[2]))
				{
					break;
				}
			}
				printf("%d\n",selectcam[1]);
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[1] = tmp;
			break;
		case 2:
			tmp = selectcam[2];
			while (++selectcam[2])
			{
				if (selectcam[2]>Cam_Num)
				{
					selectcam[2] = 0;
					break;
				}
				else if ((selectcam[2] != selectcam[0])&&(selectcam[2] != selectcam[1])&&(selectcam[2] != selectcam[3]))
					break;
			}
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[2] = tmp;
			break;
		case 3:
			tmp = selectcam[3];
			while (++selectcam[3])
			{
				if (selectcam[3]>Cam_Num)
				{
					selectcam[3] = 0;
					break;
				}
				else if ((selectcam[3] != selectcam[0])&&(selectcam[3] != selectcam[1])&&(selectcam[3] != selectcam[2]))
					break;
			}
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[3] = tmp;
			break;
		case 4:
			if (SysMenu.tmregion == 0)
			{
				time_year++;
				if( time_year > 2099 )
					time_year = 1970;
			}
			else if (SysMenu.tmregion == 1)
			{
				time_month++;
				if( time_month > 12 )
					time_month = 1;
			}
			else if (SysMenu.tmregion == 2)
			{
				time_day++;
				if (time_month==2 && ((time_year%4)==0))
					tmp = 29;
				else
					tmp = Days_Mou[time_month-1];
				if( time_day > tmp )
					time_day = 1;
			}
			break;
		case 5:
			if (SysMenu.tmregion == 0)
			{
				time_month++;
				if( time_month > 12 )
					time_month = 1;
			}
			else if (SysMenu.tmregion == 1)
			{
				time_day++;
				if (time_month==2 && ((time_year%4)==0))
					tmp = 29;
				else
					tmp = Days_Mou[time_month-1];
				if( time_day > tmp )
					time_day = 1;
			}
			else if (SysMenu.tmregion == 2)
			{
				time_month++;
				if( time_month > 12 )
					time_month = 1;
			}
			break;
		case 6:
			if (SysMenu.tmregion == 0)
			{
				time_day++;
				if (time_month==2 && ((time_year%4)==0))
					tmp = 29;
				else
					tmp = Days_Mou[time_month-1];
				if( time_day > tmp )
					time_day = 1;
			}
			else if (SysMenu.tmregion == 1)
			{
				time_year++;
				if( time_year > 2099 )
					time_year = 1970;
			}
			else if (SysMenu.tmregion == 2)
			{
				time_year++;
				if( time_year > 2099 )
					time_year = 1970;
			}
			break;
		case 7:
			time_hour++;
			if( time_hour > 24)
				time_hour = 0;
			if (time_hour==24)
			{
				time_minute = 0;
				menu_search_data_disp(8,WHITE);
			}
			setime_to_pos(time_hour,time_minute);
			break;
		case 8:
			time_minute++;
			if( time_minute > 59 || time_hour==24)
				time_minute = 0;
			setime_to_pos(time_hour,time_minute);
			break;
	}
	menu_search_data_disp(cur_pos,RED);
	if (cur_pos<7)
		SearchClick();
}

void menu_search_dec()
{
	U8 tmp;
	switch(cur_pos)
	{
		case 0:
			tmp = selectcam[0];
			if (selectcam[0]==0)
				selectcam[0] = Cam_Num+1;
			while (selectcam[0])
			{
				selectcam[0]--;
				if (selectcam[0]==0)
					break;
				else if ((selectcam[0] != selectcam[1])&&(selectcam[0] != selectcam[2])&&(selectcam[0] != selectcam[3]))
					break;
			}
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[0] = tmp;
			break;
		case 1:
			tmp = selectcam[1];
			if (selectcam[1]==0)
				selectcam[1] = Cam_Num+1;
			while (selectcam[1])
			{
				selectcam[1]--;
				if (selectcam[1]==0)
					break;
				else if ((selectcam[1] != selectcam[0])&&(selectcam[1] != selectcam[2])&&(selectcam[1] != selectcam[3]))
					break;
			}
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[1] = tmp;
			break;
		case 2:
			tmp = selectcam[2];
			if (selectcam[2]==0)
				selectcam[2] = Cam_Num+1;
			while (selectcam[2])
			{
				selectcam[2]--;
				if (selectcam[2]==0)
					break;
				else if ((selectcam[2] != selectcam[0])&&(selectcam[2] != selectcam[1])&&(selectcam[2] != selectcam[3]))
					break;
			}
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[2] = tmp;
			break;
		case 3:
			tmp = selectcam[3];
			if (selectcam[3]==0)
				selectcam[3] = Cam_Num+1;
			while (selectcam[3])
			{
				selectcam[3]--;
				if (selectcam[3]==0)
					break;
				else if ((selectcam[3] != selectcam[0])&&(selectcam[3] != selectcam[1])&&(selectcam[3] != selectcam[2]))
					break;
			}
			if ((selectcam[0] == 0) && (selectcam[1] == 0) && (selectcam[2] == 0) && (selectcam[3] == 0))
				selectcam[3] = tmp;
			break;
		case 4:
			if (SysMenu.tmregion == 0)
			{
				time_year--;
				if( time_year < 1970 )
					time_year = 2099;	
			}
			else if (SysMenu.tmregion == 1)
			{
				time_month--;
				if( time_month < 1 )
					time_month = 12;
			}
			else if (SysMenu.tmregion == 2)
			{
				time_day--;
				if( time_day < 1 )
				{
					if (time_month==2 && ((time_year%4)==0))
						time_day = 29;
					else
						time_day = Days_Mou[time_month-1];
				}
			}
			break;
		case 5:
			if (SysMenu.tmregion == 0)
			{
				time_month--;
				if( time_month < 1 )
					time_month = 12;
			}
			else if (SysMenu.tmregion == 1)
			{
				time_day--;
				if( time_day < 1 )
				{
					if (time_month==2 && ((time_year%4)==0))
						time_day = 29;
					else
						time_day = Days_Mou[time_month-1];
				}
			}
			else if (SysMenu.tmregion == 2)
			{
				time_month--;
				if( time_month < 1 )
					time_month = 12;
			}
			break;
		case 6:
			if (SysMenu.tmregion == 0)
			{
				time_day--;
				if( time_day < 1 )
				{
					if (time_month==2 && ((time_year%4)==0))
						time_day = 29;
					else
						time_day = Days_Mou[time_month-1];
				}
			}
			else if (SysMenu.tmregion == 1)
			{
				time_year--;
				if( time_year < 1970 )
					time_year = 2099;	
			}
			else if (SysMenu.tmregion == 2)
			{
				time_year--;
				if( time_year < 1970 )
					time_year = 2099;	
			}
			break;
		case 7:
			time_hour--;
			if( time_hour < 0 )
				time_hour = 24;
			if (time_hour==24)
			{
				time_minute = 0;
				menu_search_data_disp(8,WHITE);
			}
			setime_to_pos(time_hour,time_minute);
			break;
		case 8:
			time_minute--;
			if( time_minute < 0 )
				time_minute = 59;
			if (time_hour==24)
				time_minute = 0;
			setime_to_pos(time_hour,time_minute);
			break;
	}
	menu_search_data_disp(cur_pos,RED);
	if (cur_pos<7)
		SearchClick();
}
//
void PlayClick()
{
	time_t start_time;
	U8 i;
	start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (time_year - 1900),
	(time_month - 1),time_day,time_hour,time_minute,0);

	memset(Play_Cam,0,4);
	for (i=0;i<4;i++)
	{
		if (selectcam[i])
			Play_Cam[i] = selectcam[i]-1;
		else
			Play_Cam[i]=selectcam[i];
		printf("%d\n",Play_Cam[i]);
	}

	start_time_Whole = start_time;

	cur_pos = 0;
	menu_num = 0;
	demo_num = PB9;
    	OSD_ALL_CLEAR();

	Play_Mode =46;
	Split_Play(Play_Mode,2,start_time);
}

void record_date_show(time_t start_time,time_t end_time)
{
}


void SearchClick()
{
	int i,ret,page=0,totalpage;
	time_t start_time,end_time;

	start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (time_year - 1900),
	(time_month - 1),time_day,0,0,0);

	end_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (time_year - 1900),
	(time_month - 1),time_day,23,59,59);
//	if (MseType)
	block_show(offsetx_hd,offsety_hd,HD);
//	else
	block_show(offsetx,offsety,AD);
	totalpage = gstCommonParam->GST_FTC_GetRecordlist(file_show, 0xffff,start_time, end_time, page );

	for (i=0;i<totalpage;i++)
	{
		ret = gstCommonParam->GST_FTC_GetRecordlist(file_show, 0xffff,start_time, end_time, i );
		
		record_date_show(start_time,end_time);
	}

}

void menu_search_enter()
{
//	U8 i,tmp=0;
	switch(cur_pos)
	{
/*	
		case 0:
		case 1:
		case 2:
		case 3:
			if (selectcam[cur_pos])
				selectcam[cur_pos]=0;
			else
				selectcam[cur_pos]=1;
			for (i=0;i<4;i++)
			{
				if (selectcam[i])
					tmp++;
			}
			if (tmp>2 && selectcam[cur_pos])
				selectcam[cur_pos] = 0;
			else
			{
				menu_search_data_disp(cur_pos,RED);
				SearchClick();
			}
			break;
*/			
		case 9:
			PlayClick();
			break;
		case 10:
			menu_rec_search_off();
			break;
		default:
			break;
	}
}

void MouseSearchRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_seapos[SysMenu.tmregion][cur_pos*4]+offsetx_hd;
		miny = Pre_seapos[SysMenu.tmregion][cur_pos*4+1]+offsety_hd;
		maxx = Pre_seapos[SysMenu.tmregion][cur_pos*4+2]+offsetx_hd;
		maxy = Pre_seapos[SysMenu.tmregion][cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_seapos[SysMenu.tmregion][cur_pos*4]+offsetx;
		miny = Pre_seapos[SysMenu.tmregion][cur_pos*4+1]+offsety;
		maxx = Pre_seapos[SysMenu.tmregion][cur_pos*4+2]+offsetx;
		maxy = Pre_seapos[SysMenu.tmregion][cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		menu_search_add(); 
	}
}

void MouseSearchLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_seapos[SysMenu.tmregion][cur_pos*4]+offsetx_hd;
		miny = Pre_seapos[SysMenu.tmregion][cur_pos*4+1]+offsety_hd;
		maxx = Pre_seapos[SysMenu.tmregion][cur_pos*4+2]+offsetx_hd;
		maxy = Pre_seapos[SysMenu.tmregion][cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_seapos[SysMenu.tmregion][cur_pos*4]+offsetx;
		miny = Pre_seapos[SysMenu.tmregion][cur_pos*4+1]+offsety;
		maxx = Pre_seapos[SysMenu.tmregion][cur_pos*4+2]+offsetx;
		maxy = Pre_seapos[SysMenu.tmregion][cur_pos*4+3]+offsety;
	}
	if (seacourormove)
	{
		searrowmove(x);
		return;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos<9)
			menu_search_dec();
		else
			menu_search_enter();
	}
}
void Mouse_Sea_Move_Func(U16 x,U16 y,U8 type)
{
	U8 i;
	U16 xleft,xright,ytop,ybottom;
	switch(type)
	{
		case 0:
			for (i=0;i<SEARCHMENULISTNUM;i++)
			{
				if (i<SEARCHMENULISTNUM-1)
				{
					if (MseType)
					{
						xleft = Pre_seapos[SysMenu.tmregion][i*4]+offsetx_hd;
						ytop = Pre_seapos[SysMenu.tmregion][i*4+1]+offsety_hd;
						xright = Pre_seapos[SysMenu.tmregion][i*4+2]+offsetx_hd;
						ybottom = Pre_seapos[SysMenu.tmregion][i*4+3]+offsety_hd;
					}
					else
					{
						xleft = Pre_seapos[SysMenu.tmregion][i*4]+offsetx;
						xright = Pre_seapos[SysMenu.tmregion][i*4+2]+offsetx;
						ytop = Pre_seapos[SysMenu.tmregion][i*4+1]+offsety;
						ybottom = Pre_seapos[SysMenu.tmregion][i*4+3]+offsety;
					}
				}
				else
				{
					xleft = searrowposx;
					xright = searrowposx+12;
					ytop = searrowposy;
					ybottom = searrowposy+12;
				}
				if ((x>=xleft) && (y>=ytop) && (x<=xright) && (y<=ybottom) && (cur_pos != i))
				{
					menu_search_data_disp(cur_pos,WHITE);
					cur_pos=i;					
					menu_search_data_disp(cur_pos,RED);
					break;
				}
			}
			if (seacourormove==1)
				searrowmove(x);			
			break;
		case 24:
			if (y>searrowposy && y<searrowposy+12 && x>searrowposx && x<searrowposx+12)
				seacourormove =1;
			else if (y>searrowposy && y<searrowposy+12)
				seacourormove =2;
//			printf("******* mousedown\n");
			break;
		case 26:
			MouseSearchLClick(x,y);
			seacourormove = 0;
			break;
		case 25:
			MouseSearchRClick(x,y);
			break;
		default:
			break;
	}
}
void mRECSEARCH_key_func()
{
	if(byKey_val==0 ) return;

	switch(byKey_val)
	{
		case kESC:	//menu/exit   full/zoom      
			menu_rec_search_off();
			break;
		case kUP:	   //PIP        up
			menu_search_up();
	   		break;
		case kDN:        //4SP        down
			menu_search_down();
			break;
		case kRH:         //live   add
		case kAD:
	 		menu_search_add();
			break;
	   	case kLT:	          //VCR   dec     
		case kDE:
			menu_search_dec();
			break;
		case kPLAY:
			break;
		case kET:
			menu_search_enter();
			break;
		default:
			break;
    }
	byKey_val=0;

}

