
#include "cmn.h"

ALARMMENU alarmmenupic;

U8 Alarm_flag,Alarm_Page,CurMenu_Page,cams;


static FILE  * alarm_file = NULL;

U8 MenuAlarmMax;//	14


U16 MenuAlarm_Datapos[]=
{
	40,42,	162,42, 	284,42,
	208,104,		346,104,	484,104,
	208,128,		346,128,	484,128,
	208,152,		346,152,	484,152,
	208,176,		346,176,	484,176,
	208,200,		346,200,	484,200,
	208,224,		346,224,	484,224,
	208,248,		346,248,	484,248,
	208,272,		346,272,	484,272,

					544,302,
			130,342,	312,342
	
};

U16 Pre_alarmpos[] = 
{
	40,42,		160,72,
	162,42,		282,72,
	284,42,		444,72,

	208,104,		232,124,
	346,104,		370,124,
	484,104,		508,124,

	208,128,		232,148,
	346,128,		370,148,
	484,128,		508,148,

	208,152,		232,172,
	346,152,		370,172,
	484,152,		508,172,

	208,176,		232,196,
	346,176,		370,196,
	484,176,		508,196,

	208,200,		232,220,
	346,200,		370,220,
	484,200,		508,220,

	208,224,		232,244,
	346,224,		370,244,
	484,224,		508,244,

	208,248,		232,268,
	346,248,		370,268,
	484,248,		508,268,

	208,272,		232,292,
	346,272,		370,292,
	484,272,		508,292,

	544,302,		578,336,

	130,342,		290,366,
	312,342,		472,366
};

int  AlarmFileOpen()
{
	alarm_file = fopen("/mnt/mtd/alarmfile.data","r+b");
	if( alarm_file == NULL )
	{
//		printf("*************************No File*********************************\n");
		alarm_file = fopen("/mnt/mtd/alarmfile.data","wb");
		if( alarm_file == NULL )
			return -1;
		else
			return 2;
	}

	return 1;
}

void AlarmFileClose()
{
	if( alarm_file )
	{
		fclose(alarm_file);
		alarm_file = NULL;
	}
}

int AlarmFileWrite()
{
	int ret = 0;
	
	ret = AlarmFileOpen();

	if( ret > 0 )
	{
		rewind(alarm_file);
//		printf("******************File Size %x *****************************\n",17*sizeof(ValRecSchedule));
		ret = fwrite(&MenuAlarm_Date[0][0][0],1,sizeof(MenuAlarm_Date),alarm_file);
		if( ret != sizeof(MenuAlarm_Date))
		{
			printf(" epprom data write error!\n");
			return -1;
		}

		AlarmFileClose();

		return 1;
	}else
		return -1;
}

int  AlarmFileRead()
{
	int ret = 0;

	ret = AlarmFileOpen();

	if( ret == 1 )
	{
		rewind(alarm_file);
		ret = fread(&MenuAlarm_Date[0][0][0],1,sizeof(MenuAlarm_Date),alarm_file);
		if( ret != sizeof(MenuAlarm_Date))
		{
			printf(" epprom data read error!\n");
			return -1;
		}

		AlarmFileClose();

		return 1;
	}else if (ret == 2)
	{
		AlarmFileWrite();
		return ret;
	}
	else
		return ret;
}

void MenuAlarm_Initial()
{
//	U8 i;
	AlarmFileRead();
/*
	for (i=0;i<2;i++)
	{
		for (j=0;j<16;j++)
		{
			MenuAlarm_Date[i][j][k] = 0;
			for (k=0;k<3;k++)
			{
				if (MenuAlarm_Date[i][j][k]>1)
				{
					MenuAlarm_Date[i][j][k] = 0;
					tmp = 1;
				}
			}
		}
	}
	if (tmp == 1)
		AlarmFileWrite();
*/
}

void Menu_Alarm_disp(U8 pos, U16 color)
{
}

void MenuAlarm_On()
{
	U8 i,k;
	cur_pos=0;
	Alarm_flag = 0;
	Alarm_Page = 0;
	
	if (Cam_Num == 4)
	{
		MenuAlarmMax = 17;
	}
	else 
	{
		if (Cam_Num == 8)
			MenuAlarmMax = 29;
		else if (Cam_Num == 16)
			MenuAlarmMax = 30;
	}

	for(i=0;i<MenuAlarmMax;i++)
	{
		if (i==MenuAlarmMax-1)
			k=29;
		else if (i==MenuAlarmMax-2)
			k=28;
		else k=i;
		if(k!=cur_pos)Menu_Alarm_disp(k,WHITE);
		else Menu_Alarm_disp(cur_pos,RED);
	}
}

void MenuAlarm_off()
{
	OSD_ALL_CLEAR();
	MenuAlarm_Initial();
    	menu_main_on();
}

void MenuAlarm_up()
{
	Menu_Alarm_disp(cur_pos,WHITE);

	if(cur_pos<=0) 
		cur_pos=29;
	else if (cur_pos==28)
		cur_pos=MenuAlarmMax-3;
	else
		cur_pos--;

	Menu_Alarm_disp(cur_pos,RED);
}

void MenuAlarm_down()
{
	Menu_Alarm_disp(cur_pos,WHITE);

	if(cur_pos==29) 
		cur_pos=0;
	else
		cur_pos++;
	
	if ((Cam_Num < 16) && (cur_pos==MenuAlarmMax-2))
		cur_pos = 28;

	Menu_Alarm_disp(cur_pos,RED);
}

void MenuAlarm_enter()
{
	U8 i;
	if(cur_pos<3)
	{
		if (Alarm_Page != cur_pos)
		{
			Alarm_Page = cur_pos;
		}
	}
	else if (cur_pos<27) 
	{
		MenuAlarm_Date[Alarm_Page][(cur_pos-3)/3+CurMenu_Page*8][(cur_pos-3)%3] ^= 1;
		Menu_Alarm_disp(cur_pos,RED);
	}	
	else if (cur_pos==27) 
	{
		CurMenu_Page ^= 1;
		for(i=2;i<(MenuAlarmMax-1);i++)
		{
			if (i!=27)
				Menu_Alarm_disp(i,WHITE);
			else
				Menu_Alarm_disp(i,RED);
		}
	}
	else if (cur_pos==28)
	{
		AlarmFileWrite();
		MenuAlarm_off();
	}
	else if (cur_pos==29)
		MenuAlarm_off();
}

void MouseAlarmLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_alarmpos[cur_pos*4]+offsetx_hd;
		miny = Pre_alarmpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_alarmpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_alarmpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_alarmpos[cur_pos*4]+offsetx;
		miny = Pre_alarmpos[cur_pos*4+1]+offsety;
		maxx = Pre_alarmpos[cur_pos*4+2]+offsetx;
		maxy = Pre_alarmpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		MenuAlarm_enter(); 
	}
}

void Mouse_Alarm_Move_Func(U16 x,U16 y)
{
	U8 i,k;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<MenuAlarmMax;i++)
	{
		if (i==MenuAlarmMax-1)
			k=29;
		else if (i==MenuAlarmMax-2)
			k=28;
		else k=i;
		if (MseType)
		{
			minx = Pre_alarmpos[k*4]+offsetx_hd;
			miny = Pre_alarmpos[k*4+1]+offsety_hd;
			maxx = Pre_alarmpos[k*4+2]+offsetx_hd;
			maxy = Pre_alarmpos[k*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_alarmpos[k*4]+offsetx;
			miny = Pre_alarmpos[k*4+1]+offsety;
			maxx = Pre_alarmpos[k*4+2]+offsetx;
			maxy = Pre_alarmpos[k*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != k))
		{
			Menu_Alarm_disp(cur_pos,WHITE);
			cur_pos=k;
			Menu_Alarm_disp(cur_pos,RED);
			break;
		}
	}
}

void mMenuAlarm_KeyFun()
{
	if(byKey_val==0) return;
	printf("mMenuAlarm_KeyFun\n");
	switch(byKey_val)                   
	{
	case kESC:
		MenuAlarm_off();
		break;
	case kLT:
	case kUP:
		MenuAlarm_up();
		break;
	case kRH:
	case kDN:
		MenuAlarm_down();
		break;
	case kET:
		MenuAlarm_enter();
		break;
	default:
		break;
       }
	byKey_val=0;
}

