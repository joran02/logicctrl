#include "cmn.h"
#include "pciv_msg.h"




int g_sensor_id=-1;
int adc_fd=-1;
int gm8126id = -1,tw2865id=-1;
int isp_fd=-1;
static int dry_fd=-1;
unsigned char stauration_val=50;
unsigned char isp_ctrl[200];
int copyright=0;
U8 strup=0;

U8 slavego=0;

U8 ChipSelect;
U8 dev_id=0;

extern int numcharsize;
extern int recnamesize;

extern int s_antiflicker_mode;

extern unsigned char isday=DAY;
unsigned char cds_counter=0;
unsigned char cds_val=0;


int open2865_extdrv(U8 chip)
{
	char devname[20];	
	
	adc_fd=open("/dev/adc", O_RDONLY|O_NONBLOCK );
	if(adc_fd <0)
		printf("Open adc Fail! %x.\n", adc_fd);
	return 1;
}

void sensor_check()
{
	int val;
	static int s_sensorinfo=0xfff;
	char msg[10];
	
	cam_sensor = 0;
	RegisterReadWrite.value=0;
	if(gm8126id>0) ioctl(gm8126id, 10, &RegisterReadWrite);
	if(tw2865id>0) 	ioctl(tw2865id, 10, &RegisterReadWrite);
	if(adc_fd>0)
	{
		ioctl(adc_fd,0x400,&val);
		RegisterReadWrite.value=val;
	}
	if(s_sensorinfo!=RegisterReadWrite.value)
	{
		DPRINTK("*********** sensorinfo info:0x%x ********* \n",s_sensorinfo);
		s_sensorinfo=RegisterReadWrite.value;
		memset(msg,0,sizeof(msg));
		msg[0]=0x5a;
		msg[1]=s_sensorinfo;
		if( gstCommonParam->GST_DRA_Net_dvr_send_self_data != 0 )
		gstCommonParam->GST_DRA_Net_dvr_send_self_data(msg,2);
		
		usleep(5000);
		
		memset(msg,0,sizeof(msg));
		msg[0]=INIT_IPC_ORD1;
		msg[1]=INIT_IPC_ORD2;
		msg[2]=INIT_IPC_ORD3;
		msg[3]=INIT_IPC_ORD4;
		msg[4]=GET_IPC_ALARM;
		msg[5]=s_sensorinfo;
		if( gstCommonParam->GST_DRA_Net_dvr_send_self_data != 0 )
		gstCommonParam->GST_DRA_Net_dvr_send_self_data(msg,10);
	}
	cam_sensor = RegisterReadWrite.value;	
}

void chip_check(void)
{
	open2865_extdrv(0x01);
		
	ChipNum = MASTER;
	ChipSelect = HI3515;
	Cam_Num = CHANNEL1;
	printf("****************** cam:%d,ChipNum:%x,ChipSelect=%d\n",Cam_Num,ChipNum,ChipSelect);
}

void init_nvp1114(U8 system1)
{
  if(flag_480p)
  {
		if(system1==VDO_PAL )
		{
			RegisterReadWrite.addr=0x23;
			RegisterReadWrite.value=0x03;
			if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite);  
			RegisterReadWrite.addr=0x33;
			RegisterReadWrite.value=0x66;
			if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite);  
		}
		else
		{
		}		
  }
  else if(flag_576p)
  {
		if(system1==VDO_PAL)
		{
			RegisterReadWrite.addr=0;
			RegisterReadWrite.value=0;
			if(g_sensor_id>0) ioctl(g_sensor_id, 3, &RegisterReadWrite);  
			if(tw2865id>0) ioctl(tw2865id, 1, &RegisterReadWrite); 
			write_dec(0x09,0x20); 
			write_dec(0x08,0x18);
		}
		else
		{
			RegisterReadWrite.addr=0;
			RegisterReadWrite.value=0;
			if(g_sensor_id>0) ioctl(g_sensor_id, 2, &RegisterReadWrite); 
			if(tw2865id>0) ioctl(tw2865id, 0, &RegisterReadWrite);  
			write_dec(0x09,0xf0);
			write_dec(0x08,0x13); 
		}
		write_dec(0x10,0x34);		
		write_dec(0x0a,0x07);
	}
  else
  {
		if(gm8126id>0) ioctl(gm8126id, system1, &RegisterReadWrite);
  }
}

void set_ircut(int t)
{
//	printf("set_ircut:%d \n",t);
//	if(t==NIGHT)
//	{
//		if(gm8126id>0)
//			ioctl(gm8126id, 9,NULL);
//	}
//	else
//	{
//		if(gm8126id>0)
//			ioctl(gm8126id, 8, NULL);
//	}
	int val;
	
	DBGPRINT("set_ircut:%d \n",t);
	if(t==NIGHT)
		val=0;
	else
		val=1;
	if(adc_fd>0)
		ioctl(adc_fd,0x100,&val);
}
void set_color(int t)
{
	int val,ret,a;
	VI_CSC_ATTR_S CscAttr;
	
	printf("set_color:%d \n",t);
	HI_MPI_VI_GetCSCAttr(0, &CscAttr); 
	if(t==NIGHT)
	{
	  if(flag_480p)
	  {
			RegisterReadWrite.addr=0xa6;
			RegisterReadWrite.value=0x24;
			if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite);
//			usleep(300000);
//			RegisterReadWrite.addr=0x13;
//			RegisterReadWrite.value=0x00;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite); 
//			usleep(300000);
//			RegisterReadWrite.addr=0x01;
//			RegisterReadWrite.value=0xc0;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite); 
//			usleep(300000);
//			RegisterReadWrite.addr=0x02;
//			RegisterReadWrite.value=0xc0;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite); 
//			usleep(300000);
//			RegisterReadWrite.addr=0x03;
//			RegisterReadWrite.value=0xc0;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite); 
	  }
	  else if(flag_576p)
	  {
			val=0;
			write_dec(0x13,val);
			write_dec(0x14,val);
			printf("Stauration value is %d\n",val);
	  }
	  else
	  {
			val=0;
			CscAttr.u32SatuVal=val;
			HI_MPI_VI_SetCSCAttr(0, &CscAttr);
	  }
	}
	else
	{
	  if(flag_480p)
	  {
			RegisterReadWrite.addr=0xa6;
			RegisterReadWrite.value=0x04;
			if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite); 
//			usleep(300000);
//			RegisterReadWrite.addr=0x01;
//			RegisterReadWrite.value=0x56;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite); 
//			usleep(300000);
//			RegisterReadWrite.addr=0x02;
//			RegisterReadWrite.value=0x40;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite); 
//			usleep(300000);
//			RegisterReadWrite.addr=0x03;
//			RegisterReadWrite.value=0x4c;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite);
//			usleep(300000);
//			RegisterReadWrite.addr=0x13;
//			RegisterReadWrite.value=0xff;
//			ioctl(g_sensor_id, 0, &RegisterReadWrite);
	  }
	  else if(flag_576p)
	  {
			val=stauration_val;
			write_dec(0x13,val);
			write_dec(0x14,val);
			printf("Stauration value is %d\n",val);
	  }
	  else
	  {
//			val=stauration_val;			
	  	if(stauration_val==50)
	  	{
		  	IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
				IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
				
	  		val=pIsp_st->u32SatuVal;
	  	}
	  	else
	  		val=stauration_val;
			
			CscAttr.u32SatuVal=val;
			HI_MPI_VI_SetCSCAttr(0, &CscAttr);
			gstCommonParam->GST_DRA_Hisi_set_spi_antiflickerattr(a,s_antiflicker_mode);
	  }	
	}
}
void set_night(void)
{	
	if(isday==DAY)
	{
		isday=NIGHT;
		set_ircut(NIGHT);
		set_color(NIGHT);
		{
		IPCAM_ALL_INFO_ST *  pIpcamInfo= gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
		 pIpcamInfo->isp_st.u32IsLightDay = 0;
		gstCommonParam->GST_DRA_Hisi_set_isp_value(pIpcamInfo,IPCAM_ISP_CMD_IS_LIGHT_DAY);
		}
		sleep(2);
	}
}
void set_day(void)
{	
	if(isday==NIGHT)
	{
		isday=DAY;
		set_ircut(DAY);
		set_color(DAY);
		{
			IPCAM_ALL_INFO_ST *  pIpcamInfo=gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			pIpcamInfo->isp_st.u32IsLightDay = 1;
			gstCommonParam->GST_DRA_Hisi_set_isp_value(pIpcamInfo,IPCAM_ISP_CMD_IS_LIGHT_DAY);
		}
		sleep(2);
	}
}

void ircut_check()
{
	static int cnt=0;
	VI_CSC_ATTR_S CscAttr;
	
	cnt++;
	if(cnt>=800)
	{
		cnt=0;
		printf("hour:%d,min:%d,sec:%d \n",buf_time[2],buf_time[1],buf_time[0]);
		
		HI_MPI_VI_GetCSCAttr(0, &CscAttr); 
		if(isday==NIGHT&&CscAttr.u32SatuVal!=0)
		{
			CscAttr.u32SatuVal=0;
			HI_MPI_VI_SetCSCAttr(0, &CscAttr);
			printf("HI_MPI_VI_SetCSCAttr :%d \n",CscAttr.u32SatuVal);
		}
	}
}
void ircut_func(void)
{
	unsigned char tmp;	
	int val;
	int isp_gain_value;
	int light_control_add_value;
	int isp_check_count = 100;
	static int cnt=0;
	
	if(ioctl(adc_fd, 0x300,&val))
		return;

	//printf("get adc_fd %d\n",val);
	//set color bw
	ircut_check();

	isp_gain_value = gstCommonParam->GSt_NET_get_isp_light_dark_value();

	//printf("get isp_gain_value %d\n",isp_gain_value);

	if( isp_gain_value > 999)
	{
		light_control_add_value =  isp_gain_value / 1000 ;
		isp_gain_value = isp_gain_value % 1000;
	

		if( light_control_add_value == IPCAM_LIGHT_CONTROL_MANUAL_DAY_ENFORCE )
		{		
			if(isday==NIGHT )
			{
				set_day();
			}
		}else if( light_control_add_value == IPCAM_LIGHT_CONTROL_MANUAL_NIGHT_ENFORCE )
		{		
			if(isday==DAY )
			{
				set_night();
			}
		}else if(light_control_add_value == IPCAM_LIGHT_CONTROL_RESISTANCE_GAIN_CHECK_WORK_AUTO)
		{
			cnt++;
			if(cnt>200)
			{
				cnt=0;
				DBGPRINT("mode:%d day_val:%d,night_val:%d,val:%d,isp_gain_value:%d\n",light_control_add_value,CamMenu[0].dayval,CamMenu[0].nightval,tmp,isp_gain_value);
			}

			tmp = isp_gain_value;

			if(isday==NIGHT && tmp<CamMenu[0].dayval)
			{
			    if(cds_val<CamMenu[0].dayval)
			    {
			    	cds_counter++;
			      if(cds_counter>isp_check_count)
			      {
				      	printf("dayval:0x%x \n",CamMenu[0].dayval);
			          cds_counter=0;
			          set_day();
			      }
			    }  
			    else
			    	cds_counter=1;
			}
			else if(isday==DAY && tmp>CamMenu[0].nightval)	
			{
			    if(cds_val>CamMenu[0].nightval)
			    {
			    	cds_counter++;
			      if(cds_counter>isp_check_count)
			      {
				      	printf("nightval:0x%x \n",CamMenu[0].nightval);
			          cds_counter=0;
			          set_night();
			      }
			    }  
			    else
			    	cds_counter=1;
			}
			else
			{
				cds_counter=0;
			}
			cds_val=tmp;
		}


		return ;
	}
	
	tmp=val>>2;	
	
	cnt++;
	if(cnt>200)
	{
		cnt=0;
		DBGPRINT("day_val:%d,night_val:%d,val:%d,isp_gain_value:%d\n",CamMenu[0].dayval,CamMenu[0].nightval,tmp,isp_gain_value);
	}
	
//	printf("adc_val tmp:0x%x ,cds_counter:0x%x,day_val:0x%x, night_val:0x%x\n",tmp,cds_counter,day_val,night_val);	
	if(isday==NIGHT && tmp<CamMenu[0].dayval)
	{
		if(cds_val<CamMenu[0].dayval)
	    {
	    	cds_counter++;
	      if(cds_counter>MAXCOUNT)
	      {
		      	printf("dayval:0x%x \n",CamMenu[0].dayval);
	          cds_counter=0;
	          set_day();
	      }
	    }  
	    else
	    	cds_counter=1;
	}
	else if(isday==DAY && tmp>CamMenu[0].nightval)	
	{
	    if(cds_val>CamMenu[0].nightval)
	    {
	    	cds_counter++;
	      if(cds_counter>MAXCOUNT)
	      {
		      	printf("nightval:0x%x \n",CamMenu[0].nightval);
	          cds_counter=0;
	          set_night();
	      }
	    }  
	    else
	    	cds_counter=1;
	}
	else
	{
		cds_counter=0;
	}
	cds_val=tmp;
}

void sar_fun(void)
{
	
	dry_fd=open("/dev/ftsar_adc010_0_dry", O_RDONLY|O_NONBLOCK );
	if(dry_fd <0)
		printf("Open dry_fd Fail! %x.\n", dry_fd);

	isp_fd = open("/dev/isp0", O_RDWR); 
	if(isp_fd<0) 
		DPRINTK("Open ISP fail\n");

	isday=DAY;
	set_night();
	set_day();
}


