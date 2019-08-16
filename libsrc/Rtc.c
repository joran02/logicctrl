#include	"cmn.h"
//#include <time.h>

extern U8 CheckCodeflag;

int g_rtc_fd=-1;
rtc_time_t hi_tm;

U16 tTIME_POS[] =
{
	224,20,		//NTSC
	224,440,

	224,20,		//PAL
	224,536
};

U16 tTIME_POS_HD[] =
{
	224,30,		//NTSC
	224,984,

	224,30,		//PAL
	224,984
};


void RTCInit()
{
	{
		int ret;
		
	  g_rtc_fd=open("/dev/hi_rtc", O_RDWR);
		if(g_rtc_fd>0)
		{
			hi_tm.year=2013;
			hi_tm.month=8;
			hi_tm.date=1; 
			hi_tm.hour=12;
			hi_tm.minute=0;
			hi_tm.second=0;
			ret = ioctl(g_rtc_fd, HI_RTC_SET_TIME, &hi_tm);
			if(ret<0)
					printf("ioctl: HI_RTC_SET_TIME failed\n");
		}
		else
			printf("open hi_rtc failed\n");
		return ;
	}
	U8 buf[7];
	buf[0]= 0;
	buf[1]= 0;
	buf[2]= 0;
	buf[3]= 0;
	buf[4]= 1;
	buf[5]= 3;
	buf[6]= 12;

	setsystem(buf);
}




static int nvr_drv_get_file_data(char* file_name,char * buf,int max_len)
{
	FILE *fp=NULL;
	long fileOffset = 0;
	int rel;


	fp = fopen(file_name,"rb");
	if( fp == NULL )
	{
		//DPRINTK(" open %s error!\n",file_name);
		goto get_err;
	}

	rel = fseek(fp,0L,SEEK_END);
	if( rel != 0 )
	{
		DPRINTK("fseek error!!\n");
		goto get_err;
	}

	fileOffset = ftell(fp);
	if( fileOffset == -1 )
	{
		DPRINTK(" ftell error!\n");
		goto get_err;
	}

	DPRINTK(" fileOffset = %ld\n",fileOffset);

	rewind(fp);	

	/* if minihttp alive than kill it */
	if( fileOffset > 0 )
	{	
		rel = fread(buf,1,max_len,fp);
		if( rel <= 0 )
		{
			DPRINTK(" fread Error!\n");
			goto get_err;
		}	
		
	}

	fclose(fp);

	return rel;

get_err:
	if( fp )
	   fclose(fp);

	return -1;
}


static time_t FTC_CustomTimeTotime_t3(int year,int month, int day, int hour, int minute,int second)
{
	struct tm tp;
	time_t mtime;	
	
	
	year = (year>1900) ? year-1900 : 0;
	month = (month>0) ? month-1 : 0;

	tp.tm_year = year;
	tp.tm_mon = month;
	tp.tm_mday = day;
	tp.tm_hour  = hour;
	tp.tm_min   = minute;
	tp.tm_sec   = second;
	tp.tm_isdst = 0;

	mtime = mktime(&tp);

	//printf(" mtime = %ld\n",mtime);
	//printf("2time is %d %d %d %d %d %d  %ld\n",year,month,day,hour,minute,second,mtime);

	return mtime;	
}

static void  FTC_time_tToCustomTimet3(time_t time,int * year, int * month,int* day,int* hour,int* minute,int* second )
{	
	 struct tm tp;
	
	 gmtime_r(&time,&tp);

	 *year = tp.tm_year + 1900;
	 *month = tp.tm_mon + 1;
	 *day = tp.tm_mday;
	 *hour = tp.tm_hour;
	 *minute = tp.tm_min;
	 *second = tp.tm_sec;		
}



U8 timreadbuf[7];

void Time_Check()
{
	struct timeval tv;
	struct tm tp;
	char cmd[100];
	int read_num = 0;
	static int _Year;
	static int _Month;
	static int _Day;
	static int _Hour;
	static int _Minute;
	static int _Second;
	static time_t delay_time = 0;
	static int use_change_time = 0;
	int i  =0;

	gettimeofday( &tv, NULL );	
	
	
	read_num = nvr_drv_get_file_data("/tmp/onvif_set_time",cmd,100);
	if( read_num > 0 )
	{
		//system("rm -rf /tmp/onvif_set_time");
		SendM("rm -rf /tmp/onvif_set_time");
		if(( i= sscanf(cmd,"%d-%d-%d %d:%d:%d",&_Year,&_Month,&_Day,&_Hour,&_Minute,&_Second)) == 6 )
		{
			IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			time_t tmp_time = 0;	

			tmp_time = FTC_CustomTimeTotime_t3(_Year,_Month,_Day,_Hour,_Minute,_Second);
			tmp_time += pAllInfo->sys_st.time_zone*3600;
			
			sprintf(cmd,"date %d-%d-%d %d:%d:%d  zone=%d",
				_Year,_Month,_Day,_Hour,_Minute,_Second,pAllInfo->sys_st.time_zone);
			
			DPRINTK("%s\n",cmd);

			
			if(0 && gstCommonParam->GST_DRA_Net_dvr_have_client() > 0)
			{							
				delay_time = tmp_time - tv.tv_sec;		
				DBGPRINT("tmp_time=%ld  tv.tv_sec=%ld delay_time=%ld\n",tmp_time,tv.tv_sec,delay_time);
				use_change_time = 1;

				DBGPRINT("have client: onvif set time %s delay_time=%ld \n",cmd,delay_time);
			}else
			{		
				FTC_time_tToCustomTimet3(tmp_time,&_Year,&_Month,&_Day,&_Hour,&_Minute,&_Second);			

				sprintf(cmd,"date %0.2d%0.2d%0.2d%0.2d%0.4d.%0.2d",_Month,_Day,_Hour,_Minute,_Year,_Second);
				//system(cmd);	
				SendM(cmd);
				
				DBGPRINT("not have client: onvif set time %s ",cmd);				
			}
		}else
		{
			DBGPRINT("sscanf = %d err\n",i);	
		}
		
	}

	if( use_change_time == 1 )
	{	
		
		tv.tv_sec =  tv.tv_sec + delay_time;	

		if(gstCommonParam->GST_DRA_Net_dvr_have_client() == 0)
		{
			use_change_time = 0;
			delay_time = 0;
			FTC_time_tToCustomTimet3(tv.tv_sec,&_Year,&_Month,&_Day,&_Hour,&_Minute,&_Second);			

			sprintf(cmd,"date %0.2d%0.2d%0.2d%0.2d%0.4d.%0.2d",_Month,_Day,_Hour,_Minute,_Year,_Second);
			//system(cmd);
			SendM(cmd);
			DBGPRINT("not have client: onvif set time %s ",cmd);		
		}		
	}

	
	
	
	gmtime_r(&tv.tv_sec,&tp);

	if ((tp.tm_year-100)!=buf_time[5])
	{
		buf_time[5] = tp.tm_year-100;
		byear_change = 1;
//		pstCommonParam->gst_ext_write_rtc(0,0x15,1,&buf_time[5]);
	}
	if (tp.tm_mon!=(buf_time[4]-1))
	{
		buf_time[4]=tp.tm_mon+1;
		bmouth_change = 1;
//		pstCommonParam->gst_ext_write_rtc(0,0x14,1,&buf_time[4]);
	}
	if (tp.tm_mday!=buf_time[3])
	{
		buf_time[3]=tp.tm_mday;
		bday_change = 1;
//		pstCommonParam->gst_ext_write_rtc(0,0x13,1,&buf_time[3]);
	}
	if (tp.tm_hour!=buf_time[2])
	{
		buf_time[2]=tp.tm_hour;
		bhour_change = 1;
//		pstCommonParam->gst_ext_write_rtc(0,0x12,1,&buf_time[2]);
	}
	if (tp.tm_min!=buf_time[1])
	{
		buf_time[1]=tp.tm_min;
		bminute_change = 1;
//		pstCommonParam->gst_ext_write_rtc(0,0x11,1,&buf_time[1]);
//		printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA :%d,eventset:%d\n",buf_time[1],eventset);
	}
	if (tp.tm_sec!=buf_time[0])
	{
		buf_time[0]=tp.tm_sec;
		bsecond_change=1;
	}

}
#ifdef GM8126_DEV
void gettimestr()
{
	int ret;
	fiosd_string_t string;

	string.windex   = 1;
	string.start    = 32;
	string.fg_color = 1;
	string.bg_color = 0;
	if (CheckCodeflag)
		sprintf(&string.string[0],"MACHINE INIT MODE    ");
	else
	{
		if (SysMenu.tmregion == 0)
			sprintf(string.string,"%d-%02d-%02d %02d:%02d:%02d\n",buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
		else if (SysMenu.tmregion == 2)
			sprintf(string.string,"%02d-%02d-%d %02d:%02d:%02d\n",buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
		else if (SysMenu.tmregion == 1)
			sprintf(string.string,"%02d-%02d-%d %02d:%02d:%02d\n",buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
	}
	ret = ioctl(fosd_fd[0], FIOSDS_STRING, &string);
	if(ret < 0)
		printf("FIOSDS 0 STRING Fail!");

	string.windex   = 1;
	string.start    = 32;
	string.fg_color = 1;
	string.bg_color = 0;
	if (CheckCodeflag)
		sprintf(&string.string[0],"MACHINE INIT MODE    ");
	else
	{
		if (SysMenu.tmregion == 0)
			sprintf(string.string,"%d-%02d-%02d %02d:%02d:%02d\n",buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
		else if (SysMenu.tmregion == 2)
			sprintf(string.string,"%02d-%02d-%d %02d:%02d:%02d\n",buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
		else if (SysMenu.tmregion == 1)
			sprintf(string.string,"%02d-%02d-%d %02d:%02d:%02d\n",buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
	}
	ret = ioctl(fosd_fd[1], FIOSDS_STRING, &string);
	if(ret < 0)
		printf("FIOSDS 1 STRING Fail!");
}
#endif

void SDDisplayTimechange()
{
}
#ifdef HISI_DEV



int real_time_display()
{
#if 1//#ifndef NETDVR

    HI_S32 s32Ret;

	if( unParamtime.pData == NULL )
	{
		DBGPRINT("real_time_display .pData NULL \n");
        return -1;
	}

  s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle[1],&unParamtime);
  if(s32Ret != HI_SUCCESS)
  {
  	DBGPRINT("HI_MPI_RGN_SetBitMap failed with  %x %#x   %d  %d %d!\n",RgnHandle[1], s32Ret,unParamtime.u32Width,unParamtime.u32Height,unParamtime.enPixelFormat);
    return HI_FAILURE;
  }
  printf("real_time_display ...\n");

	
#endif


	return 0;
}


int rec_time_display(int x,int y,int image_width,int image_height,unsigned char *image_data,int flag)
{
#if 1//#ifndef NETDVR
    int j;
    HI_CHAR *pDst = NULL;
    HI_S32 s32Ret;

	
    if(image_data==NULL)
    {
        DBGPRINT("image_data NULL \n");
        return -1;
    }

	if( unParamtime.pData == NULL )
	{
		DBGPRINT("unParamtime.pData NULL \n");
        return -1;
	}


    pDst = (HI_CHAR *)(unParamtime.pData+x*2+y*s_TimeNum[0].w*19*2);
//  pDst = (HI_CHAR *)(unParamtime.stBitmap.pData+x*2+y*8*21*2);
    for(j=0; j<image_height; j++)
    {
        if(flag)
            memcpy(pDst,&image_data[image_width*2*j],image_width*2);
        else
            memset(pDst,0,image_width*2);

        pDst += s_TimeNum[0].w*19*2;
//      pDst += 8*21*2;
    }

  //	DBGPRINT("HI_MPI_RGN_SetBitMap 1\n");


	/*

  s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle[1],&unParamtime);
  if(s32Ret != HI_SUCCESS)
  {
  	DBGPRINT("HI_MPI_RGN_SetBitMap failed with  %x %#x   %d  %d %d!\n",RgnHandle[1], s32Ret,unParamtime.u32Width,unParamtime.u32Height,unParamtime.enPixelFormat);
    return HI_FAILURE;
  }



	rect_t myTestRect[10];

	memset(myTestRect, 0, sizeof(rect_t)*10);

static int mytestcount = 0;
	mytestcount+=10;

	myTestRect[0].left = 10+mytestcount;
	myTestRect[0].right = 100+mytestcount;
	myTestRect[0].top = 10+mytestcount;
	myTestRect[0].bottom = 100+mytestcount;

	if(mytestcount > 300)
		mytestcount = 3;
	

  showRectOnVenc(myTestRect,1);*/
	
#endif


	return 0;
}
void time_display(int flag)
{
	static char s_record_display_info='0';
	static GST_TIME_TM s_TimeStr={0xff,0xff,0xff,0xff,0xff,0xff,0xff};
	int layer=0,x=0,y=0;
	int rec_x=0,rec_y=0;
	int tmp,i,flag1=0,iTmp;
	int char_size=s_TimeNum[0].w;

	iTmp=SysMenu.tmregion;
	for(i=0;i<2;i++)
	{
		if(i==0)
		{
			flag1=0;
			layer=0;
		}
		else if(i==1)
		{
			layer=2;
			flag1=1;
		}
		switch(1)
		{
			case 0:
				return ;
				break;
			case 1:
				x=720;
				y=16+4;
				x=x/2;
				x=x-13*char_size;
				rec_x=x;
				rec_y=y;
				break;
			case 2:
				x=720;
				if(SysMenu.VideoFormat==VDO_PAL)
					y=576-16-4;
				else
					y=480-16-4;
				x=x/2;
				x=x-13*char_size;
				rec_x=x;
				rec_y=y;
				break;
		}
		switch(iTmp)
		{
			case 0:
	/////////////////////year////////	
				if(flag||s_TimeStr.tm_year!=buf_time[5])
				{
					//osd_image_display(layer,x,y,s_TimeNum[2].w,s_TimeNum[2].h,s_TimeNum[2].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[2].w,s_TimeNum[2].h,s_TimeNum[2].buf,1);
					x+=char_size;
					//osd_image_display(layer,x,y,s_TimeNum[0].w,s_TimeNum[10].h,s_TimeNum[0].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[0].w,s_TimeNum[10].h,s_TimeNum[0].buf,1);
					x+=char_size;
					tmp=buf_time[5]/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=buf_time[5]%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_year=buf_time[5];
				}
				else
				{
					x=x+char_size*3;
				}
	////////////////////  "/ "//////////				
				x+=char_size;
//				if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
				if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
	//////////////////// month //////////				
				if(flag||s_TimeStr.tm_mon!=buf_time[4])
				{
					x+=char_size;
					tmp=(buf_time[4])/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=(buf_time[4])%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_mon=buf_time[4];
				}
				else
				{
					x=x+char_size*2;
				}
	////////////////// "/ "/////////////					
				x+=char_size;
//				if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
				if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
	/////////////////  date /////////////				
				if(flag||s_TimeStr.tm_mday!=buf_time[3])
				{
					x+=char_size;
					tmp=buf_time[3]/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=buf_time[3]%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_mday=buf_time[3];
				}
				else
				{
					x=x+char_size*2;
				}
				break;
			case 2:
	/////////////////  date /////////////				
				if(flag||s_TimeStr.tm_mday!=buf_time[3])
				{
//					x+=char_size;
					tmp=buf_time[3]/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=buf_time[3]%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_mday=buf_time[3];
				}
				else
				{
					x=x+char_size*2;
				}
	////////////////// "/ "/////////////					
				x+=char_size;
//				if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
				if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
	//////////////////// month //////////				
				if(flag||s_TimeStr.tm_mon!=buf_time[4])
				{
					x+=char_size;
					tmp=(buf_time[4])/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=(buf_time[4])%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_mon=buf_time[4];
				}
				else
				{
					x=x+char_size*2;
				}
	////////////////// "/ "/////////////					
				x+=char_size;
//				if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
				if(flag1)
					rec_time_display(x-rec_x,y-rec_y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
	/////////////////////year////////	
				if(flag||s_TimeStr.tm_year!=buf_time[5])
				{		
					x+=char_size;
					//osd_image_display(layer,x,y,s_TimeNum[2].w,s_TimeNum[2].h,s_TimeNum[2].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[2].w,s_TimeNum[2].h,s_TimeNum[2].buf,1);
					x+=char_size;
					//osd_image_display(layer,x,y,s_TimeNum[0].w,s_TimeNum[10].h,s_TimeNum[0].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[0].w,s_TimeNum[10].h,s_TimeNum[0].buf,1);
					x+=char_size;
					tmp=buf_time[5]/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=buf_time[5]%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_year=buf_time[5];
				}
				else
				{
					x=x+char_size*4;
				}
				break;		
			case 1:
	//////////////////// month //////////				
				if(flag||s_TimeStr.tm_mon!=buf_time[4])
				{
//					x+=char_size;
					tmp=(buf_time[4])/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=(buf_time[4])%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_mon=buf_time[4];
				}
				else
				{
					x=x+char_size*2;
				}
	////////////////// "/ "/////////////					
				x+=char_size;
//				if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
				if(flag1)
					rec_time_display(x-rec_x,y-rec_y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
	/////////////////  date /////////////				
				if(flag||s_TimeStr.tm_mday!=buf_time[3])
				{
					x+=char_size;
					tmp=buf_time[3]/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=buf_time[3]%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_mday=buf_time[3];
				}
				else
				{
					x=x+char_size*2;
				}
	////////////////// "/ "/////////////					
				x+=char_size;
//				if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
				if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[11].w,s_TimeNum[11].h,s_TimeNum[11].buf,1);
	/////////////////////year////////	
				if(flag||s_TimeStr.tm_year!=buf_time[5])
				{		
					x+=char_size;
					//osd_image_display(layer,x,y,s_TimeNum[2].w,s_TimeNum[2].h,s_TimeNum[2].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[2].w,s_TimeNum[2].h,s_TimeNum[2].buf,1);
					x+=char_size;
					//osd_image_display(layer,x,y,s_TimeNum[0].w,s_TimeNum[10].h,s_TimeNum[0].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[0].w,s_TimeNum[10].h,s_TimeNum[0].buf,1);
					x+=char_size;
					tmp=buf_time[5]/10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					x+=char_size;
					tmp=buf_time[5]%10;
					//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
					if(flag1)
						s_TimeStr.tm_year=buf_time[5];
				}
				else
				{
					x=x+char_size*4;
				}
				break;
		}
	//////////////////////////////				
		x+=char_size*3;
		rec_x+=char_size;
	//////////////////////////////	
		if(flag||s_TimeStr.tm_hour!=buf_time[2])
		{
			tmp=buf_time[2]/10;
			//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			x+=char_size;
			tmp=buf_time[2]%10;
			//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				s_TimeStr.tm_hour=buf_time[2];
		}
		else
		{
			x+=char_size;
		}
	//////////////////////////////	
		x+=char_size;
//		if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[10].w,s_TimeNum[10].h,s_TimeNum[10].buf,1);
		if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[10].w,s_TimeNum[10].h,s_TimeNum[10].buf,1);
	//////////////////////////////	
		if(flag||s_TimeStr.tm_min!=buf_time[1])
		{
			x+=char_size;
			tmp=buf_time[1]/10;
			//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			x+=char_size;
			tmp=buf_time[1]%10;
			//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				s_TimeStr.tm_min=buf_time[1];
		}
		else
		{
			x=x+char_size*2;
		}
	//////////////////////////////	
		x+=char_size;
//		if(flag)
					//osd_image_display(layer,x,y,s_TimeNum[10].w,s_TimeNum[10].h,s_TimeNum[10].buf,1);
		if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[10].w,s_TimeNum[10].h,s_TimeNum[10].buf,1);
	//////////////////////////////	
		if(flag||s_TimeStr.tm_sec!=buf_time[0])
		{
			x+=char_size;
			tmp=buf_time[0]/10;
			//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			x+=char_size;
			tmp=buf_time[0]%10;
			//osd_image_display(layer,x,y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				rec_time_display(x-rec_x,y-rec_y,s_TimeNum[tmp].w,s_TimeNum[tmp].h,s_TimeNum[tmp].buf,1);
			if(flag1)
				s_TimeStr.tm_sec=buf_time[0];
		}
	}
	
  	//DBGPRINT("HI_MPI_RGN_SetB");
//	DBGPRINT("aaaaaaaaaaaaaaaaa \n");
//////////////////////////////	
}
#endif
void Time_Date_Disp()//selet :YEAR,MONTH,DAY,HOUR,MINUTE,SECOND
{
#ifdef HISI_DEV
	time_display(1);
	real_time_display();

#endif
}


