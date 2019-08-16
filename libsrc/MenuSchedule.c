
#include "cmn.h"

#define MENUSCHEDULENUM		26
ValRecSchedule SchedulMenu[CHANNELNUM+1];
SCHMENU schmenupic;
U8 Camsch,SchWeek,AllCam;
static FILE  * epprom_file = NULL;

 U16 tSchedule_data_pos[]=
{
	154,50,								438,50,
	154,76,								438,76,
	
	180,180,		216,180,		290,180,	326,180,	390,180,
	180,206,		216,206,		290,206,	326,206,	390,206,
	180,232,		216,232,		290,232,	326,232,	390,232,
	180,258,		216,258,		290,258,	326,258,	390,258,
				128,304,		316,304
};

U16 Pre_schpos[] = 
{
	154,50,		280,70,
	438,50,		462,70,
	154,76,		280,96,
	438,76,		462,96,

	180,180,		204,204,
	216,180,		240,204,
	290,180,		314,204,
	326,180,		350,204,
	390,180,		560,204,
	
	180,206,		204,230,
	216,206,		240,230,
	290,206,		314,230,
	326,206,		350,230,
	390,206,		560,230,

	180,232,		204,256,
	216,232,		240,256,
	290,232,		314,256,
	326,232,		350,256,
	390,232,		560,256,

	180,258,		204,282,
	216,258,		240,282,
	290,258,		314,282,
	326,258,		350,282,
	390,258,		560,282,

	128,304,		298,330,
	316,304,		486,330
};


int  FileOpen()
{
	epprom_file = fopen("/mnt/mtd/schfile.data","r+b");
	if( epprom_file == NULL )
	{
//		printf("*************************No File*********************************\n");
		epprom_file = fopen("/mnt/mtd/schfile.data","wb");
		if( epprom_file == NULL )
			return -1;
		else
			return 2;
	}

	return 1;
}

void FileClose()
{
	if( epprom_file )
	{
		fclose(epprom_file);
		epprom_file = NULL;
	}
}

int FileWrite()
{
	int ret = 0;
	
	ret = FileOpen();

	if( ret > 0 )
	{
		rewind(epprom_file);
//		printf("******************File Size %x *****************************\n",17*sizeof(ValRecSchedule));
		ret = fwrite(&SchedulMenu[0],1,17*sizeof(ValRecSchedule),epprom_file);
		if( ret != 17*sizeof(ValRecSchedule))
		{
			printf(" epprom data write error!\n");
			return -1;
		}

		ret = fwrite(&AllCam,1,1,epprom_file);
		if( ret != 1)
		{
			printf(" epprom data write error!\n");
			return -1;
		}
		
		FileClose();

		return 1;
	}else
		return -1;
}

void setendhour()
{
	U8 i,j;
	for (i=0;i<17;i++)
	{
		for (j=0;j<8;j++)
			SchedulMenu[i].Hour_End[j][0] = 24;
	}
}


int  FileRead()
{
	int ret = 0;

	ret = FileOpen();

	if( ret == 1 )
	{
		rewind(epprom_file);
		ret = fread(&SchedulMenu[0],1,17*sizeof(ValRecSchedule),epprom_file);
		if( ret != 17*sizeof(ValRecSchedule))
		{
			printf(" epprom data read error!\n");
			return -1;
		}

		ret = fread(&AllCam,1,1,epprom_file);
		if( ret != 1)
		{
			printf(" epprom data read error!\n");
			return -1;
		}

		FileClose();

		return 1;
	}else if (ret == 2)
	{
		setendhour();
		FileWrite();
		return ret;
	}
	else
		return ret;
}

void readmsg(char *  buf)
{
	int ret = 0;
	ret = FileOpen();
	if(1==ret)
	{
		rewind(epprom_file);
		ret = fread(buf,1,17*sizeof(ValRecSchedule)+1,epprom_file);
		if( ret != (17*sizeof(ValRecSchedule)+1))
			printf(" epprom data read error!\n");

		FileClose();

	}
	else
		printf("epprom data open error!\n");
}

void writemsg(char * bufll)
{
	int ret = 0;
	
	ret = FileOpen();

	if( ret > 0 )
	{
		rewind(epprom_file);
//		printf("******************File Size %x *****************************\n",17*sizeof(ValRecSchedule));
		ret = fwrite(bufll,1,17*sizeof(ValRecSchedule)+1,epprom_file);
		if( ret != 17*sizeof(ValRecSchedule)+1)
			printf(" epprom data write error!\n");

		FileClose();
	}
	else
		printf("epprom data open error!\n");
}

void set_schedule()
{
	U8 i,j,k;

	if (AllCam)
	{
		for (i=0;i<Cam_Num;i++)
		{
			if (SchedulMenu[16].All_Date == 0)
			{
//					DBGPRINT("************************* AAAAAAAAAAAAAA\n");
				for (j=1;j<8;j++)
				{
					SchedulMenu[i].Record_Stytle[j] = SchedulMenu[16].Record_Stytle[j];
					for (k=0;k<4;k++)
					{
						SchedulMenu[i].Hour_Star[j][k] = SchedulMenu[16].Hour_Star[j][k];
						SchedulMenu[i].Hour_End[j][k] = SchedulMenu[16].Hour_End[j][k];
						SchedulMenu[i].Minute_Star[j][k] = SchedulMenu[16].Minute_Star[j][k];
						SchedulMenu[i].Minute_End[j][k] = SchedulMenu[16].Minute_End[j][k];
						SchedulMenu[i].Alarm_Stytle[j][k] = SchedulMenu[16].Alarm_Stytle[j][k];
					}
				}
			}
			else
			{
//					DBGPRINT("************************* BBBBBBBBBBBBBBBB\n");
				for (j=1;j<8;j++)
				{
					SchedulMenu[i].Record_Stytle[j] = SchedulMenu[16].Record_Stytle[0];
					for (k=0;k<4;k++)
					{
						SchedulMenu[i].Hour_Star[j][k] = SchedulMenu[16].Hour_Star[0][k];
						SchedulMenu[i].Hour_End[j][k] = SchedulMenu[16].Hour_End[0][k];
						SchedulMenu[i].Minute_Star[j][k] = SchedulMenu[16].Minute_Star[0][k];
						SchedulMenu[i].Minute_End[j][k] = SchedulMenu[16].Minute_End[0][k];
						SchedulMenu[i].Alarm_Stytle[j][k] = SchedulMenu[16].Alarm_Stytle[0][k];
					} 
				}
			}
		}
	}
	else
	{
		for (i=0;i<Cam_Num;i++)
		{
//			DBGPRINT("************************* kkkkkkkkkkk :%d,%d,%d\n",i,SchedulMenu[i].All_Date,SchedulMenu[1].All_Date);
			if (SchedulMenu[i].All_Date)
			{
//					DBGPRINT("************************* CCCCCCCCCC\n");
				for (j=1;j<8;j++)
				{
					SchedulMenu[i].Record_Stytle[j] = SchedulMenu[i].Record_Stytle[0];
					for (k=0;k<4;k++)
					{
//						DBGPRINT("************************* i:%d,j:%d,%d,%d\n",i,j,SchedulMenu[i].Hour_Star[j][k],SchedulMenu[i].Hour_Star[0][k]);
						SchedulMenu[i].Hour_Star[j][k] = SchedulMenu[i].Hour_Star[0][k];
//						DBGPRINT("************************* i:%d,j:%d,%d,%d\n",i,j,SchedulMenu[i].Hour_End[j][k],SchedulMenu[i].Hour_End[0][k]);
						SchedulMenu[i].Hour_End[j][k] = SchedulMenu[i].Hour_End[0][k];
//						DBGPRINT("************************* i:%d,j:%d,%d,%d\n",i,j,SchedulMenu[i].Minute_Star[j][k],SchedulMenu[i].Minute_Star[0][k]);
						SchedulMenu[i].Minute_Star[j][k] = SchedulMenu[i].Minute_Star[0][k];
//						DBGPRINT("************************* i:%d,j:%d,%d,%d\n",i,j,SchedulMenu[i].Minute_End[j][k],SchedulMenu[i].Minute_End[0][k]);
						SchedulMenu[i].Minute_End[j][k] = SchedulMenu[i].Minute_End[0][k];
						SchedulMenu[i].Alarm_Stytle[j][k] = SchedulMenu[i].Alarm_Stytle[0][k];
//						DBGPRINT("\n\n");
					}
				}
			}
		}
	}
	for (i=0;i<17;i++)
	{
		for (j=0;j<8;j++)
			SchedulMenu[i].Record_Stytle[j] = 1;
	}
}

void MenuSCHInit()
{
	FileRead();
	set_schedule();
}


void menu_schedule_disp(U8 pos,U16 color)
{
}

void menu_schedule_on()
{
}

U8 time_check(U8 i,U8 k)
{
	U8 j,l;
	time_t start_time[4],end_time[4],base_time;
	for (j=0;j<4;j++)
	{
		base_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,0,0,0);
		start_time[j] = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[k][j],SchedulMenu[i].Minute_Star[k][j],0);
		end_time[j] = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[k][j],SchedulMenu[i].Minute_End[k][j],0);
		if (end_time[j] < start_time[j])
			return 0;
	}

	for (j=0;j<4;j++)
	{
		for (l=j+1;l<4;l++)
		{
			if (start_time[j]<start_time[l]  && start_time[j] > end_time[l]) return 0;
			if (end_time[j]>start_time[l]  && end_time[j] < end_time[l]) return 0;
			if (start_time[l]<start_time[j]  && start_time[l] > end_time[j]) return 0;
			if (end_time[l]>start_time[j]  && end_time[l] < end_time[j]) return 0;

			if (end_time[j] == start_time[j] && start_time[j] == base_time) 
			{
				if (start_time[l]!=base_time || end_time[l] != base_time)
				{
					return 0;
				}
			}
		}
	}
	if((end_time[0] > start_time[1]) && (end_time[1]!=start_time[1]) && (end_time[0]!=start_time[0])) return 0;
	if((end_time[1] > start_time[2]) && (end_time[2]!=start_time[2]) && (end_time[1]!=start_time[1])) return 0;
	if((end_time[2] > start_time[3]) && (end_time[3]!=start_time[3]) && (end_time[2]!=start_time[2])) return 0;

	return 1;
}

U8 menu_schedule_check()
{
	U8 i,j,k;

	if (AllCam == 0)
	{
		for (i=0;i<Cam_Num;i++)
		{
			if (SchedulMenu[i].All_Date)
			{
				j=time_check(i,0);
				if (!j)
					return j;
			}
			else
			{
				for (k=1;k<8;k++)
				{
					j=time_check(i,k);
					if (!j)
						return j;
				}
			}
		}
	}
	else
	{
		if (SchedulMenu[16].All_Date)
		{
			j=time_check(16,0);
			if (!j)
				return j;
		}
		else
		{
			for (k=1;k<8;k++)
			{
				j=time_check(16,k);
				if (!j)
					return j;
			}
		}
	}
	
	return 1;
}


void menu_schedule_off()
{
	OSD_ALL_CLEAR();
	MenuSCHInit();
    	menu_main_on();
}

void menu_schedule_do()
{
	U8 i;
	if (cur_pos < 4)
	{
		for(i=0;i<MENUSCHEDULENUM;i++)
		{
			if(i!=cur_pos)menu_schedule_disp(i,WHITE);
			else menu_schedule_disp(cur_pos,RED);
		}
	}
	else
		menu_schedule_disp(cur_pos,RED);

}

void menu_schedule_up()
{
	menu_schedule_disp(cur_pos,WHITE);
	if (cur_pos==0)
		cur_pos = MENUSCHEDULENUM-1;
	else
		cur_pos--;

	if (cur_pos==0 && AllCam)
		cur_pos = MENUSCHEDULENUM-1;
	else if (cur_pos == 2 && SchedulMenu[Camsch].All_Date)
		cur_pos = 1;
	
	menu_schedule_disp(cur_pos,RED);
}

void menu_schedule_down()
{
	menu_schedule_disp(cur_pos,WHITE);
	
	if (cur_pos==MENUSCHEDULENUM-1)
		cur_pos = 0;
	else
		cur_pos++;

	if (cur_pos==0 && AllCam)
		cur_pos = 1;
	else if (cur_pos == 2 && SchedulMenu[Camsch].All_Date)
		cur_pos = 3;
	
	menu_schedule_disp(cur_pos,RED);
}

void menu_schedule_add()
{
	switch(cur_pos)
	{
		case 0:
			if (Camsch>=Cam_Num-1)
				Camsch = 0;
			else
				Camsch++;
			SchWeek = 1-SchedulMenu[Camsch].All_Date;
			break;
		case 2:
			if (SchWeek>=7)
				SchWeek=1;
			else
				SchWeek++;
 			break;
		case 4:
		case 9:
		case 14:
		case 19:
			if (SchedulMenu[Camsch].Hour_Star[SchWeek][cur_pos/5]>=24)
				SchedulMenu[Camsch].Hour_Star[SchWeek][cur_pos/5]=0;
			else 
				SchedulMenu[Camsch].Hour_Star[SchWeek][cur_pos/5]++;
			if (SchedulMenu[Camsch].Hour_Star[SchWeek][(cur_pos)/5]==24)
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos)/5] = 0;
			break;
		case 5:
		case 10:
		case 15:
		case 20:
			if (SchedulMenu[Camsch].Hour_Star[SchWeek][(cur_pos-1)/5]==24)
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5] = 0;
			else if (SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5]>=59)
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5] = 0;
			else
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5]++;
			break;
		case 6:
		case 11:
		case 16:
		case 21:
			if (SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5]>=24)
				SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5] = 0;
			else
				SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5]++;
			if (SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5]==24)
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-2)/5] = 0;
			break;
		case 7:
		case 12:
		case 17:
		case 22:
			if (SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-3)/5]==24)
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5] = 0;
			else if (SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5]>=59)
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5] = 0;
			else
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5] ++;
			break;
		case 8:
		case 13:
		case 18:
		case 23:
			if (SchedulMenu[Camsch].Alarm_Stytle[SchWeek][(cur_pos-4)/5]>=3)
				SchedulMenu[Camsch].Alarm_Stytle[SchWeek][(cur_pos-4)/5] = 0;
			else
				SchedulMenu[Camsch].Alarm_Stytle[SchWeek][(cur_pos-4)/5]++;
			break;
		default:
			break;
	}
	menu_schedule_do();
}

void menu_schedule_dec()
{
	switch(cur_pos)
	{
		case 0:
			if (Camsch<=0)
				Camsch = Cam_Num-1;
			else
				Camsch--;
			
			SchWeek = 1-SchedulMenu[Camsch].All_Date;
			break;
		case 2:
			if (SchWeek<=1)
				SchWeek=7;
			else
				SchWeek--;
 			break;
		case 4:
		case 9:
		case 14:
		case 19:
			if (SchedulMenu[Camsch].Hour_Star[SchWeek][cur_pos/5]<=0)
				SchedulMenu[Camsch].Hour_Star[SchWeek][cur_pos/5]=24;
			else 
				SchedulMenu[Camsch].Hour_Star[SchWeek][cur_pos/5]--;
			if (SchedulMenu[Camsch].Hour_Star[SchWeek][(cur_pos)/5]==24)
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos)/5] = 0;
			break;
		case 5:
		case 10:
		case 15:
		case 20:
			if (SchedulMenu[Camsch].Hour_Star[SchWeek][(cur_pos-1)/5]==24)
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5] = 0;
			else if (SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5]<=0)
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5] = 59;
			else
				SchedulMenu[Camsch].Minute_Star[SchWeek][(cur_pos-1)/5]--;
			break;
		case 6:
		case 11:
		case 16:
		case 21:
			if (SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5]<=0)
				SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5] = 24;
			else
				SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5]--;
			if (SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-2)/5]==24)
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-2)/5] = 0;
			break;
		case 7:
		case 12:
		case 17:
		case 22:
			if (SchedulMenu[Camsch].Hour_End[SchWeek][(cur_pos-3)/5]==24)
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5] = 0;
			else if (SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5]<=0)
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5] = 59;
			else
				SchedulMenu[Camsch].Minute_End[SchWeek][(cur_pos-3)/5] --;
			break;
		case 8:
		case 13:
		case 18:
		case 23:
			if (SchedulMenu[Camsch].Alarm_Stytle[SchWeek][(cur_pos-4)/5]<=0)
				SchedulMenu[Camsch].Alarm_Stytle[SchWeek][(cur_pos-4)/5] = 3;
			else
				SchedulMenu[Camsch].Alarm_Stytle[SchWeek][(cur_pos-4)/5]--;
			break;
		default:
			break;
	}
	menu_schedule_do();
}

void menu_schedule_enter()
{
	U8 i;
	if (cur_pos == 1)
	{
		if (AllCam)
		{
			Camsch = 0;
			AllCam = 0;
		}
		else
		{
			AllCam = 1;
			Camsch = 16;
		}
		SchWeek = 1-SchedulMenu[Camsch].All_Date;
		menu_schedule_do();
	}
	else if (cur_pos == 3)
	{
		if (SchedulMenu[Camsch].All_Date)
		{
			if (AllCam)
			{
				for (i=0;i<Cam_Num;i++)
				{
					SchedulMenu[i].All_Date = 0;
				}
				SchedulMenu[Camsch].All_Date = 0;
			}
			else
				SchedulMenu[Camsch].All_Date = 0;
			SchWeek = 1;
		}
		else
		{
			if (AllCam)
			{
				for (i=0;i<Cam_Num;i++)
				{
					SchedulMenu[i].All_Date = 1;
				}
				SchedulMenu[Camsch].All_Date = 1;
			}
			else
				SchedulMenu[Camsch].All_Date = 1;
			SchWeek = 0;
		}
		menu_schedule_do();
	}
	else if (cur_pos==24)
	{
		if (!menu_schedule_check())
		{
			InfoId=TimeSetErr;
			MenuErrOn();
			return ;
		}
		FileWrite();
		set_schedule();
		menu_schedule_off();
	}
	else if (cur_pos==25)
		menu_schedule_off();

}

void MouseSchRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_schpos[cur_pos*4]+offsetx_hd;
		miny = Pre_schpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_schpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_schpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_schpos[cur_pos*4]+offsetx;
		miny = Pre_schpos[cur_pos*4+1]+offsety;
		maxx = Pre_schpos[cur_pos*4+2]+offsetx;
		maxy = Pre_schpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		menu_schedule_add(); 
}

void MouseSchLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_schpos[cur_pos*4]+offsetx_hd;
		miny = Pre_schpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_schpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_schpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_schpos[cur_pos*4]+offsetx;
		miny = Pre_schpos[cur_pos*4+1]+offsety;
		maxx = Pre_schpos[cur_pos*4+2]+offsetx;
		maxy = Pre_schpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos == 1 || cur_pos == 3 || cur_pos == 24 || cur_pos == 25)
			menu_schedule_enter();
		else
			menu_schedule_dec(); 
	}
}

void Mouse_Sch_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<MENUSCHEDULENUM;i++)
	{
		if (MseType)
		{
			minx = Pre_schpos[i*4]+offsetx_hd;
			miny = Pre_schpos[i*4+1]+offsety_hd;
			maxx = Pre_schpos[i*4+2]+offsetx_hd;
			maxy = Pre_schpos[i*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_schpos[i*4]+offsetx;
			miny = Pre_schpos[i*4+1]+offsety;
			maxx = Pre_schpos[i*4+2]+offsetx;
			maxy = Pre_schpos[i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
			if ((i==0 && AllCam) || (i == 2 && SchedulMenu[Camsch].All_Date))
				return;
			menu_schedule_disp(cur_pos,WHITE);
			cur_pos=i;
			menu_schedule_disp(cur_pos,RED);
			break;
		}
	}
}


void mSCHEDUL_key_func()
{
	if(byKey_val==0) return;
	printf("mSCHEDUL_key_func\n");
	switch(byKey_val)                   
	{
	case kESC:
		menu_schedule_off();
		break;
	case kUP:
		menu_schedule_up();
		break;
	case kDN:
		menu_schedule_down();
		break;
	case kLT:
	case kDE:
		menu_schedule_dec();
		break;
	case kRH:
	case kAD:
		menu_schedule_add();
		break;
	case kET:
		menu_schedule_enter();
		break;
	default:
		break;
       }
	byKey_val=0;
}

