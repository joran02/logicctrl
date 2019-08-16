#include "cmn.h"

BAKMENU bakmenupic;

static GST_FILESHOWINFO backfile_show[RecodSeaFileCount];
U8 BackType=3;

U8 courormove=0;

#define TIME_BACK_NUM 18

#define UP		0
#define DONW	1

U16 hexcam =0x0f;

static int back_time_year,back_time_month,back_time_day,back_time_hour[2],back_time_minute[2];
//static unsigned char  back_data[7][50];
static int bak_cursor = 0;

U16 TotalSize;
U16 arrowposx[2],arrowposy[2];
U16 arrowposxhd[2],arrowposyhd[2];
U8 backnum[4] = {1,0,0,0};


  U16 menu_back_pos_EA[]=
 {
// 	6,3,		13,3,	19,3,	26,3,
	226,76,
	226,102,		340,102,	486,102,	604,102,
	226,128,		286,128,	322,128,	
	226,154,		262,154,	486,154,	522,154,
	38,364,		192,364,	346,364,	500,364	
 };

  U16 menu_back_pos_AMEU[]=
 {
 //	6,3,		13,3,	19,3,	26,3,
 	226,76,
	226,102,		340,102,	486,102,	604,102,
	226,128,		262,128,	298,128,
	226,154,		262,154,	486,154,	522,154,
	38,364,		192,364,	346,364,	500,364	
 };

  U16* menu_back_pos[]=
 {
	menu_back_pos_EA,
	menu_back_pos_AMEU,
	menu_back_pos_AMEU
 };

U16 Pre_bakpos_EA[] = 
{
	226,76,		382,96,
	226,102,		250,126,
	340,102,		364,126,
	486,102,		510,126,
	604,102,		628,126,
	226,128,		274,148,
	286,128,		310,148,
	322,128,		346,148,
	226,154,		250,174,
	262,154,		286,174,
	486,154,		510,174,
	522,154,		566,174,
	38,364,		176,398,
	192,364,		330,398,
	346,364,		484,398,
	500,364,		638,398
};

U16 Pre_bakpos_AM_EU[] = 
{
	226,76,		382,96,
	226,102,		250,126,
	340,102,		364,126,
	486,102,		510,126,
	604,102,		628,126,
	226,128,		250,148,
	262,128,		286,148,
	298,128,		346,148,
	226,154,		250,174,
	262,154,		286,174,
	486,154,		510,174,
	522,154,		566,174,
	38,364,		176,398,
	192,364,		330,398,
	346,364,		484,398,
	500,364,		638,398
};

U16 *Pre_bakpos[] = 
{
	Pre_bakpos_EA,
	Pre_bakpos_AM_EU,
	Pre_bakpos_AM_EU
};


void menu_time_back_do(U8 pos);
void menu_back_data(U8 pos,U16 color);
void time_to_pos(int h,int m,U8 f);

void backupfile_size_count()
{
	time_t start_time, end_time[2];
	long long size;
	int totalsize=0,sec=0;
	char outstr[10];
	start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
	(back_time_month - 1),back_time_day,back_time_hour[UP],back_time_minute[UP],0);

	end_time[0] = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
	(back_time_month - 1),back_time_day,back_time_hour[DONW],back_time_minute[DONW],0);
	end_time[1] = end_time[0];

	if (start_time != end_time[1])
	{
		backupmenuon(FILECOUNT,BackType);
		WatchDog();
//		printf("################## backup camhex: %x\n",hexcam);
		size = gstCommonParam->GST_FS_GetDataSize(&start_time,&end_time[1],hexcam,BackType);
		if (size)
		{
			totalsize = size/1024/1024;
			if (totalsize == 0)
				totalsize = 1;
		}
		else
			totalsize = 0;
//		printf("totalsize = %d\n",totalsize);
		backupmenuoff();
		if (totalsize == 0)
		{
		}
		else 
		{
			TotalSize = totalsize;
			memset(outstr,0x00,sizeof(outstr));
			sprintf(outstr,"%04d ",totalsize);
			if (end_time[1] != end_time[0])
			{
				gstCommonParam->GST_FTC_time_tToCustomTime(end_time[1],&back_time_year,&back_time_month,
					&back_time_day,&back_time_hour[DONW],&back_time_minute[DONW],&sec);
				back_time_year += 1900;
				back_time_month += 1;
				menu_back_data(10,WHITE);
				menu_back_data(11,WHITE);
				time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			}
		}

	}
	
}

void menu_back_data(U8 pos,U16 color)
{
}

void menu_arrow_time_on(time_t timebuf,U8 pos,U8 color)
{
	time_t time0;

	time0 = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),(back_time_month - 1),back_time_day,0,0,0);
//	if (MseType)
	{
		if (timebuf)
		{
			arrowposxhd[pos] = 58+offsetx_hd+(timebuf-time0)/150;
			arrowposx[pos] = 58+offsetx+(timebuf-time0)/150;
		}
		arrowposyhd[pos] = 186 + offsety_hd+pos*46;
		arrowposy[pos] = 186 + offsety+pos*46;

	}
}

void set_timearrow(U16 x,U16 widthblock, U8 flag)
{
}

void arrowmove(U16 x, U8 flag)
{
}

void time_to_pos(int h,int m,U8 f)
{
}

void menu_backvail_time_on()
{
}

void menu_time_back_init()
{
	 struct timeval tv;
	 struct tm tp;

	 gettimeofday( &tv, NULL );	
	 gmtime_r(&tv.tv_sec,&tp);


	back_time_year = tp.tm_year + 1900;
	back_time_month = tp.tm_mon + 1;
	back_time_day = tp.tm_mday;
	back_time_hour[UP] = tp.tm_hour;
	back_time_minute[UP] = tp.tm_min;

	back_time_hour[DONW] = tp.tm_hour;
	back_time_minute[DONW] = tp.tm_min;
	
	bak_cursor = 0;
	//SysMenu.Laguage = 1;
}

 void menu_time_back_on()
{	 
}

void menu_time_back_off()
{
	cur_pos = 0;
	menu_num = 0;
    	OSD_ALL_CLEAR();
	demo_osd_disp();
}

void menu_time_back_up()
{
	menu_back_data(bak_cursor,WHITE);
	bak_cursor--;

	if( bak_cursor < 0)
		bak_cursor = TIME_BACK_NUM - 3;	

	menu_back_data(bak_cursor,RED);
}

void menu_time_back_down()
{
	menu_back_data(bak_cursor,WHITE);
	bak_cursor++;

	if( bak_cursor >= TIME_BACK_NUM-2)
		bak_cursor = 0;	

	menu_back_data(bak_cursor,RED);
}

void menu_time_back_add()
{
	U8 tmp;
	switch(bak_cursor)
	{
		case 0:
			if (BackType==3)
				BackType = 0;
			else
				BackType ++;
			break;
		case 1:
			tmp = backnum[bak_cursor-1];
			while (++backnum[bak_cursor-1])
			{
				if (backnum[bak_cursor-1]>Cam_Num)
				{
					backnum[bak_cursor-1] = 0;
					break;
				}
				else if ((backnum[3] != backnum[0])&&(backnum[0] != backnum[1])&&(backnum[0] != backnum[2]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 2:
			tmp = backnum[bak_cursor-1];
			while (++backnum[bak_cursor-1])
			{
				if (backnum[bak_cursor-1]>Cam_Num)
				{
					backnum[bak_cursor-1] = 0;
					break;
				}
				else if ((backnum[1] != backnum[0])&&(backnum[3] != backnum[1])&&(backnum[1] != backnum[2]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 3:
			tmp = backnum[bak_cursor-1];
			while (++backnum[bak_cursor-1])
			{
				if (backnum[bak_cursor-1]>Cam_Num)
				{
					backnum[bak_cursor-1] = 0;
					break;
				}
				else if ((backnum[2] != backnum[0])&&(backnum[2] != backnum[1])&&(backnum[3] != backnum[2]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 4:
			tmp = backnum[bak_cursor-1];
			while (++backnum[bak_cursor-1])
			{
				if (backnum[bak_cursor-1]>Cam_Num)
				{
					backnum[bak_cursor-1] = 0;
					break;
				}
				else if ((backnum[3] != backnum[0])&&(backnum[3] != backnum[1])&&(backnum[3] != backnum[2]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 5:
			if (SysMenu.tmregion == 0)
			{
				back_time_year++;
				if( back_time_year > 2099 )
					back_time_year = 1970;		
			}
			else if (SysMenu.tmregion==1)
			{
				back_time_month++;
				if( back_time_month > 12 )
					back_time_month = 1;
			}
			else if (SysMenu.tmregion==2)
			{
				back_time_day++;
				if (back_time_month==2 && ((back_time_year%4)==0))
					tmp = 29;
				else
					tmp = Days_Mou[back_time_month-1];
				if( back_time_day > tmp )
					back_time_day = 1;
			}
			break;
		case 6:
			if (SysMenu.tmregion == 0)
			{
				back_time_month++;
				if( back_time_month > 12 )
					back_time_month = 1;
			}
			else if (SysMenu.tmregion==1)
			{
				back_time_day++;
				if (back_time_month==2 && ((back_time_year%4)==0))
					tmp = 29;
				else
					tmp = Days_Mou[back_time_month-1];
				if( back_time_day > tmp )
					back_time_day = 1;
			}
			else if (SysMenu.tmregion==2)
			{
				back_time_month++;
				if( back_time_month > 12 )
					back_time_month = 1;
			}
			break;
		case 7:
			if (SysMenu.tmregion == 0)
			{
				back_time_day++;
				if (back_time_month==2 && ((back_time_year%4)==0))
					tmp = 29;
				else
					tmp = Days_Mou[back_time_month-1];
				if( back_time_day > tmp )
					back_time_day = 1;
			}
			else if (SysMenu.tmregion==1)
			{
				back_time_year++;
				if( back_time_year > 2099 )
					back_time_year = 1970;		
			}
			else if (SysMenu.tmregion==2)
			{
				back_time_year++;
				if( back_time_year > 2099 )
					back_time_year = 1970;		
			}
			break;
		case 8:
			back_time_hour[UP]++;
			if( back_time_hour[UP] > 24 )
				back_time_hour[UP] = 0;
			if (back_time_hour[UP]==24)
			{
				back_time_minute[UP] = 0;
				menu_back_data(9,WHITE);
			}
			time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[DONW] = back_time_hour[UP];
				back_time_minute[DONW] = back_time_minute[UP];
				menu_back_data(10,WHITE);
				menu_back_data(11,WHITE);
				time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			}
			break;
		case 9:
			back_time_minute[UP]++;
			if( back_time_minute[UP] > 59 || back_time_hour[UP]==24)
				back_time_minute[UP] = 0;
			time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[DONW] = back_time_hour[UP];
				back_time_minute[DONW] = back_time_minute[UP];
				menu_back_data(10,WHITE);
				menu_back_data(11,WHITE);
				time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			}
			break;
		case 10:
			back_time_hour[DONW]++;
			if( back_time_hour[DONW] > 24 )
				back_time_hour[DONW] = 0;
			if (back_time_hour[DONW]==24)
			{
				back_time_minute[DONW] = 0;
				menu_back_data(11,WHITE);
			}
			time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[UP] = back_time_hour[DONW];
				back_time_minute[UP] = back_time_minute[DONW];
				menu_back_data(8,WHITE);
				menu_back_data(9,WHITE);
				time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			}
			break;
		case 11:
			back_time_minute[DONW]++;
			if( back_time_minute[DONW] > 59 || back_time_hour[DONW]==24)
				back_time_minute[DONW] = 0;
			time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[UP] = back_time_hour[DONW];
				back_time_minute[UP] = back_time_minute[DONW];
				menu_back_data(8,WHITE);
				menu_back_data(9,WHITE);
				time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			}
			break;
		default:
			return;
	}

	menu_back_data(bak_cursor,RED);
	if ((bak_cursor>0) && (bak_cursor<8))
		menu_time_back_do(5);
}

void menu_time_back_dec()
{
	U8 tmp;
	switch(bak_cursor)
	{
		case 0:
			if (BackType==0)
				BackType = 3;
			else
				BackType --;
			break;
		case 1:
			tmp = backnum[bak_cursor-1];
			if (backnum[bak_cursor-1]==0)
				backnum[bak_cursor-1] = Cam_Num+1;
			while (backnum[bak_cursor-1])
			{
				backnum[bak_cursor-1]--;
				if (backnum[bak_cursor-1]==0)
					break;
				else if ((backnum[0] != backnum[3])&&(backnum[0] != backnum[1])&&(backnum[0] != backnum[2]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 2:
			tmp = backnum[bak_cursor-1];
			if (backnum[bak_cursor-1]==0)
				backnum[bak_cursor-1] = Cam_Num+1;
			while (backnum[bak_cursor-1])
			{
				backnum[bak_cursor-1]--;
				if (backnum[bak_cursor-1]==0)
					break;
				else if ((backnum[1] != backnum[0])&&(backnum[1] != backnum[3])&&(backnum[1] != backnum[2]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 3:
			tmp = backnum[bak_cursor-1];
			if (backnum[bak_cursor-1]==0)
				backnum[bak_cursor-1] = Cam_Num+1;
			while (backnum[bak_cursor-1])
			{
				backnum[bak_cursor-1]--;
				if (backnum[bak_cursor-1]==0)
					break;
				else if ((backnum[2] != backnum[0])&&(backnum[2] != backnum[1])&&(backnum[2] != backnum[3]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 4:
			tmp = backnum[bak_cursor-1];
			if (backnum[bak_cursor-1]==0)
				backnum[bak_cursor-1] = Cam_Num+1;
			while (backnum[bak_cursor-1])
			{
				backnum[bak_cursor-1]--;
				if (backnum[bak_cursor-1]==0)
					break;
				else if ((backnum[3] != backnum[0])&&(backnum[3] != backnum[1])&&(backnum[3] != backnum[2]))
					break;
			}
			if ((backnum[0] == 0) && (backnum[1] == 0) && (backnum[2] == 0) && (backnum[3] == 0))
				backnum[bak_cursor-1] = tmp;
			break;
		case 5:
			if (SysMenu.tmregion == 0)
			{
				back_time_year--;
				if( back_time_year < 1970 )
					back_time_year = 2099;	
			}
			else if (SysMenu.tmregion==1)
			{
				back_time_month--;
				if( back_time_month < 1 )
					back_time_month = 12;
			}
			else if (SysMenu.tmregion==2)
			{
				back_time_day--;
				if( back_time_day < 1 )
				{
					if (back_time_month==2 && ((back_time_year%4)==0))
						back_time_day = 29;
					else
						back_time_day = Days_Mou[back_time_month-1];
				}
			}
			break;
		case 6:
			if (SysMenu.tmregion == 0)
			{
				back_time_month--;
				if( back_time_month < 1 )
					back_time_month = 12;
			}
			else if (SysMenu.tmregion==1)
			{
				back_time_day--;
				if( back_time_day < 1 )
				{
					if (back_time_month==2 && ((back_time_year%4)==0))
						back_time_day = 29;
					else
						back_time_day = Days_Mou[back_time_month-1];
				}
			}
			else if (SysMenu.tmregion==2)
			{
				back_time_month--;
				if( back_time_month < 1 )
					back_time_month = 12;
			}
			break;
		case 7:
			if (SysMenu.tmregion == 0)
			{
				back_time_day--;
				if( back_time_day < 1 )
				{
					if (back_time_month==2 && ((back_time_year%4)==0))
						back_time_day = 29;
					else
						back_time_day = Days_Mou[back_time_month-1];
				}
			}
			else if (SysMenu.tmregion==1)
			{
				back_time_year--;
				if( back_time_year < 1970 )
					back_time_year = 2099;	
			}
			else if (SysMenu.tmregion==2)
			{
				back_time_year--;
				if( back_time_year < 1970 )
					back_time_year = 2099;	
			}
			break;
		case 8:
			back_time_hour[UP]--;
			if( back_time_hour[UP] < 0 )
				back_time_hour[UP] = 24;
			if (back_time_hour[UP]==24)
			{
				back_time_minute[UP] = 0;
				menu_back_data(9,WHITE);
			}
			time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[DONW] = back_time_hour[UP];
				back_time_minute[DONW] = back_time_minute[UP];
				menu_back_data(10,WHITE);
				menu_back_data(11,WHITE);
				time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			}
			break;
		case 9:
			back_time_minute[UP]--;
			if( back_time_minute[UP] < 0)
				back_time_minute[UP] = 59;
			if (back_time_hour[UP]==24)
				back_time_minute[UP] = 0;
			time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[DONW] = back_time_hour[UP];
				back_time_minute[DONW] = back_time_minute[UP];
				menu_back_data(10,WHITE);
				menu_back_data(11,WHITE);
				time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			}
			break;		
		case 10:
			back_time_hour[DONW]--;
			if( back_time_hour[DONW] < 0 )
				back_time_hour[DONW] = 24;
			if (back_time_hour[DONW]==24)
			{
				back_time_minute[DONW] = 0;
				menu_back_data(11,WHITE);
			}
			time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[UP] = back_time_hour[DONW];
				back_time_minute[UP] = back_time_minute[DONW];
				menu_back_data(8,WHITE);
				menu_back_data(9,WHITE);
				time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			}
			break;
		case 11:
			back_time_minute[DONW]--;
			if( back_time_minute[DONW] < 0)
				back_time_minute[DONW] = 59;
			if (back_time_hour[DONW]==24)
				back_time_minute[DONW] = 0;
			time_to_pos(back_time_hour[DONW],back_time_minute[DONW],2);
			if ((back_time_hour[UP]*60+back_time_minute[UP])>(back_time_hour[DONW]*60+back_time_minute[DONW]))
			{
				back_time_hour[UP] = back_time_hour[DONW];
				back_time_minute[UP] = back_time_minute[DONW];
				menu_back_data(8,WHITE);
				menu_back_data(9,WHITE);
				time_to_pos(back_time_hour[UP],back_time_minute[UP],1);
			}
			break;		
		default:
			return;
	}

	menu_back_data(bak_cursor,RED);
	if ((bak_cursor>0) && (bak_cursor<8))
		menu_time_back_do(5);
}

void back_date_show(time_t start_time,time_t end_time)
{
}

void block_show(U16 x,U16 y,U8 layer)
{
	U8 i;

	for (i=0;i<4;i++)
	{
		Bmp_All_Show(64+x,253+i*24+y,searchmenupic.block,layer);
	}
}

U8 sizeandtimecheck()
{
	U8 flag = 1;
	time_t start_time,end_time;
	start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
	(back_time_month - 1),back_time_day,back_time_hour[UP],back_time_minute[UP],0);

	end_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
	(back_time_month - 1),back_time_day,back_time_hour[DONW],back_time_minute[DONW],0);
	if(TotalSize == 0 || start_time == end_time) //给的日期出错
	{
		backupmenuon(BakTimeErr,BackType);
		flag = 0;
	}
	if (((BackType == 1) && (TotalSize> 550)) ||((BackType == 2) && (TotalSize> 180)))
	{
		backupmenuon(SIZEMORE,BackType);
		flag = 0;
	}
	return flag;
}

U8 bakdevicecheck()
{
	int ret;
	U8 flag = 1;
	if (BackType == 3)
	{
		ret = access("/proc/scsi/usb-storage",F_OK);
		if (ret<0)
		{
			backupmenuon(NOUSB,BackType);
			flag = 0;
		}
	}
	else
	{
		ret = gstCommonParam->GST_FS_scan_cdrom();
		if (!ret)
		{
			backupmenuon(NOUSB,BackType);
			flag = 0;
		}
	}
	if (flag && (menu_num == mMouseBak))
		flag=sizeandtimecheck();
	return flag;
}

void usbbakstop()
{
	time_t start_time,end_time;
	start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
	(back_time_month - 1),back_time_day,back_time_hour[UP],back_time_minute[UP],0);

	end_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
	(back_time_month - 1),back_time_day,back_time_hour[DONW],back_time_minute[DONW],0);

	gstCommonParam->GST_FS_WriteUsb(start_time,end_time,WBIT(backnum[0]),0);
}

void menu_time_back_do(U8 pos)
{
	int i,ret,page=0,totalpage;
	time_t start_time,end_time;
	U16 tmp = 0,j=0;
	switch(pos)
	{
/*	
		case 1:
		case 2:
		case 3:
		case 4:
			if (backnum[pos-1])
			{
				backnum[pos-1] = 0;
				hexcam &= (0x0f - BIT(pos-1));
			}
			else
			{
				backnum[pos-1] = 1;
				hexcam |= BIT(pos-1);
			}
			menu_back_data(pos,RED);
//			break;
*/
		case 5:
			hexcam = 0;
			for (i=0;i<4;i++)
			{
				if (backnum[i])
				{
					j=backnum[i]-1;
					hexcam |= WBIT(j);
					j=0;
				}
			}
			start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
			(back_time_month - 1),back_time_day,0,0,0);

			end_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
			(back_time_month - 1),back_time_day,23,59,59);
//			if (MseType)
			block_show(offsetx_hd,offsety_hd,HD);
//			else
			block_show(offsetx,offsety,AD);
			totalpage = gstCommonParam->GST_FTC_GetRecordlist(&backfile_show[0], 0xffff,start_time, end_time, page );
			back_date_show(start_time,end_time);
			for (i=1;i<totalpage;i++)
			{
				ret = gstCommonParam->GST_FTC_GetRecordlist(&backfile_show[0], 0xffff,start_time, end_time, i );
				back_date_show(start_time,end_time);
			}

			break;
		case 12:
			backupfile_size_count();
			break;
		case 13:
//			if (rec_on)
//			{
//				InfoId = RecErr|0x80;
//				OSD_ALL_CLEAR();
//				MenuErrOn();
//			}
//			else
			{
				if (!bakdevicecheck())
					break;
				if (BackType == 3)  //USB
				{
					start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
					(back_time_month - 1),back_time_day,back_time_hour[UP],back_time_minute[UP],0);

					end_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
					(back_time_month - 1),back_time_day,back_time_hour[DONW],back_time_minute[DONW],0);
					
					gstCommonParam->GST_FS_WriteUsb(start_time,end_time,hexcam,1);
				}
				else //DVD / CD / MiniCD
				{
					start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
					(back_time_month - 1),back_time_day,back_time_hour[UP],back_time_minute[UP],0);

					end_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
					(back_time_month - 1),back_time_day,back_time_hour[DONW],back_time_minute[DONW],0);
					
					gstCommonParam->GST_FS_WriteDataToCdrom(start_time,end_time,hexcam,0,0,0,0,0,0,BackType?1:0);
				}
				backupmenuon(BACKING,BackType);
			}
			break;
		case 14:
			start_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( (back_time_year - 1900),
			(back_time_month - 1),back_time_day,back_time_hour[UP],back_time_minute[UP],0);

			memset(Play_Cam,0,4);
			for (i=0;i<4;i++)
			{
				if (backnum[i])
				{
					j=backnum[i]-1;
					tmp |= WBIT(j);
					j=0;
				}
			}
			j=0;
			for (i = 0;i<Cam_Num;i++)
			{
				if (tmp & WBIT(i))
				{
					Play_Cam[j] = i;
					printf("********************************%d,tmp:%x\n",Play_Cam[j],tmp);
					j++;
				}
			}
//			2010-11-12 12:04:38
			if(Cam_Num==CHANNEL4&&RecamMenu[0].Reselution)
			{
				for(i=1;i<Cam_Num;i++)
					Play_Cam[i]=0;
			}
			
			start_time_Whole = start_time;

			cur_pos = 0;
			menu_num = 0;
			demo_num = PB9;
		    	OSD_ALL_CLEAR();

			Play_Mode =46;
			Split_Play(Play_Mode,2,start_time);
			break;
		case 15:
			menu_time_back_off();
		default:
			break;
	}
	
}

void MouseBakRClick(U16 x,U16 y)
{
	U16 xleft,xright,ytop,ybottom;
	if (MseType==0)
	{
		xleft = Pre_bakpos[SysMenu.tmregion][bak_cursor*4]+offsetx;
		xright = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+2]+offsetx;
		ytop = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+1]+offsety;
		ybottom = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+3]+offsety;
	}
	else
	{
		xleft = Pre_bakpos[SysMenu.tmregion][bak_cursor*4]+offsetx_hd;
		xright = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+2]+offsetx_hd;
		ytop = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+1]+offsety_hd;
		ybottom = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+3]+offsety_hd;
	}
	if ((x>=xleft) && (y>=ytop) && (x<=xright) && (y<=ybottom))
		menu_time_back_add(); 
}

void MouseBakLClick(U16 x,U16 y)
{
	U16 xleft,xright,ytop,ybottom;
	if (courormove)
	{
		arrowmove(x,courormove);
		return;
	}
	if (MseType==0)
	{
		xleft = Pre_bakpos[SysMenu.tmregion][bak_cursor*4]+offsetx;
		xright = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+2]+offsetx;
		ytop = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+1]+offsety;
		ybottom = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+3]+offsety;
	}
	else
	{
		xleft = Pre_bakpos[SysMenu.tmregion][bak_cursor*4]+offsetx_hd;
		xright = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+2]+offsetx_hd;
		ytop = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+1]+offsety_hd;
		ybottom = Pre_bakpos[SysMenu.tmregion][bak_cursor*4+3]+offsety_hd;
	}
	if ((x>=xleft) && (y>=ytop) && (x<=xright) && (y<=ybottom))
	{
//		if ((bak_cursor<TIME_BACK_NUM-5 && bak_cursor>4) || bak_cursor==0)
		if (bak_cursor<TIME_BACK_NUM-6)
			menu_time_back_dec();
		else
			menu_time_back_do(bak_cursor);
		if(bak_cursor==14&&menu_num==0)
			Menu_Mouse_On();
	}
}


void Back_Key_func(U16 x,U16 y,U8 type)
{
	U8 i;
	U16 xleft,xright,ytop,ybottom;
	switch(type)
	{
		case 0:
			for (i=0;i<TIME_BACK_NUM;i++)
			{
				if (i<TIME_BACK_NUM-2)
				{
					if (MseType==0)
					{
						xleft = Pre_bakpos[SysMenu.tmregion][i*4]+offsetx;
						xright = Pre_bakpos[SysMenu.tmregion][i*4+2]+offsetx;
						ytop = Pre_bakpos[SysMenu.tmregion][i*4+1]+offsety;
						ybottom = Pre_bakpos[SysMenu.tmregion][i*4+3]+offsety;
					}
					else
					{
						xleft = Pre_bakpos[SysMenu.tmregion][i*4]+offsetx_hd;
						xright = Pre_bakpos[SysMenu.tmregion][i*4+2]+offsetx_hd;
						ytop = Pre_bakpos[SysMenu.tmregion][i*4+1]+offsety_hd;
						ybottom = Pre_bakpos[SysMenu.tmregion][i*4+3]+offsety_hd;
					}
				}
				else
				{
					if (MseType == 0)
					{
						xleft = arrowposx[i-16];
						xright = arrowposx[i-16]+12;
						ytop = arrowposy[i-16];
						ybottom = arrowposy[i-16]+12;
					}
					else
					{
						xleft = arrowposxhd[i-16];
						xright = arrowposxhd[i-16]+12;
						ytop = arrowposyhd[i-16];
						ybottom = arrowposyhd[i-16]+12;
					}
				}
				if ((x>=xleft) && (y>=ytop) && (x<=xright) && (y<=ybottom) && (bak_cursor != i))
				{
					menu_back_data(bak_cursor,WHITE);
					bak_cursor=i;
					menu_back_data(bak_cursor,RED);
					return;
				}
			}
			if (courormove<3  && courormove>0)
				arrowmove(x,courormove);			
			break;
		case 24:
			if (MseType)
			{
				if (y>arrowposyhd[0] && y<arrowposyhd[0]+12 && x>arrowposxhd[0] && x<arrowposxhd[0]+12)
					courormove =1;
				else if (y>arrowposyhd[1] && y<arrowposyhd[1]+12 && x>arrowposxhd[1] && x<arrowposxhd[1]+12)
					courormove =2;
				else if (y>arrowposyhd[0] && y<arrowposyhd[0]+12)
					courormove =3;
				else if (y>arrowposyhd[1] && y<arrowposyhd[1]+12)
					courormove =4;
			}
			else
			{
				if (y>arrowposy[0] && y<arrowposy[0]+12 && x>arrowposx[0] && x<arrowposx[0]+12)
					courormove =1;
				else if (y>arrowposy[1] && y<arrowposy[1]+12 && x>arrowposx[1] && x<arrowposx[1]+12)
					courormove =2;
				else if (y>arrowposy[0] && y<arrowposy[0]+12)
					courormove =3;
				else if (y>arrowposy[1] && y<arrowposy[1]+12)
					courormove =4;
			}
			break;
		case 26:
			MouseBakLClick(x,y);
			courormove = 0;
			break;
		case 25:
		case 30:
			MouseBakRClick(x,y);
			break;
		case 31:
			if (bak_cursor<TIME_BACK_NUM-5)
				menu_time_back_dec();
			break;
		default:
			break;
	}
}


void mBACKUP_key_func()
{
	if(byKey_val==0 ) return;
	printf("************ mBACKUP_key_func\n");
	switch(byKey_val)
	{
	case kESC:
		menu_time_back_off();
		break;
	case kUP:
		menu_time_back_up();
   		break;
	case kDN:
		menu_time_back_down();
		break;
	case kRH:
	case kAD:
 		menu_time_back_add();
		break;
   	case kLT:
	case kDE:
		menu_time_back_dec();
		break;
	case kET:
		menu_time_back_do(bak_cursor);
		break;
	case kSTOP:
//		usbbakstop();
		break;
	case kPLAY:
		menu_time_back_do(14);
//		menu_time_dircback();
		break;
	default: 
		break;
    }
	byKey_val=0;
}




