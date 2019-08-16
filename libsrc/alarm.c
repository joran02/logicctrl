#include "cmn.h"



//*********************************************************************************************************

#define MAX_MOTION_HOLD_TIME 20
#define MAX_SENSOR_HOLD_TIME 20
#define MAX_VLOSS_HOLD_TIME 20
#define MAX_BUZZER_HOLD_TIME 16
#define MAX_RELAY_HOLD_TIME 16
#define PREALM_FRAME_RATE_D1 3
#define PREALM_FRAME_RATE_HD1 3
#define PREALM_FRAME_RATE_CIF 12

typedef struct
{
	unsigned char alm_new;
	unsigned char alm_old;
	unsigned char alm_on; 
	unsigned char alm_hold_time;
} Alarm;

Alarm motion[CHANNELNUM];
Alarm sensor[CHANNELNUM]; 
Alarm vloss[CHANNELNUM];

unsigned char buzzer_hold_time;
unsigned char relay_hold_time;
unsigned int mail_hold_time;
unsigned int recch_old;
unsigned int recch;
unsigned int motion_enable;
unsigned int sensor_enable;
unsigned int vloss_enable; //for enable loss record
unsigned int manual_enable;
unsigned int time_enable;
unsigned int cam_enable;
unsigned char frame_rate[CHANNELNUM];
unsigned char resolution;  //0_CIF 1_HD1 2_D1

unsigned char buzzer_keep_time;
unsigned char relay_keep_time;
unsigned char rec_on;
unsigned char rec_flag,rec_succeed_flag;

unsigned char rec_req;
unsigned char rec_key=0;
U8 MenuAlarm_Date[3][CHANNELNUM][3];


 UC mtdval=0;
 UC lossval=0;

 UC mtdval1=0;
 UC lossval1=0;

 UC mtdval2=0;
 UC lossval2=0;

 UC mtdval3=0;
 UC lossval3=0;

int reccam;

U8 goout = 0;

U16 motionchon=0,motionchon_old=0xffff;
 
unsigned int BITVAL[]={
	0x0001,0x0002,0x0004,0x0008,
	0x0010,0x0020,0x0040,0x0080,
	0x0100,0x0200,0x0400,0x0800,
	0x1000,0x2000,0x4000,0x8000,
};

void motion_detect(void)
{
//	static int cam_motion_tmp;
	unsigned char i;
/*
	U8 tmp1,tmp2;
	U8 tmp33,tmp66;
	tmp1=mtdval & 0x0f;
	tmp2=mtdval1& 0x0f;
	tmp33=tmp1 | (tmp2<<4);
	tmp1=mtdval2 & 0x0f;
	tmp2=mtdval3 & 0x0f;
	tmp66=tmp1 | (tmp2<<4);

	cam_motion=(tmp66<<8) | tmp33;
*/
//	printf("*\n");
	if(goout)
		return;
		
//	{
//		if(cam_motion_tmp!=cam_motion)
//		{
//			cam_motion_tmp=cam_motion;
//			printf("aaaaaaaajjjjjjjjjjjjjj 0x%x jjjjjjjjjjjjjjjjjjjjj \n",cam_motion);
//		}
//}
	for(i=0;i<Cam_Num;i++)
	{
		if(cam_enable & motion_enable & BITVAL[i])
//		if(cam_enable & BITVAL[i])
		{
///////////please add code
			if(cam_motion & BITVAL[i])			
			{
				motion[i].alm_new=1;

				if (0==(reccam & BITVAL[i]) && motion[i].alm_old == 1)
				{
					motion[i].alm_old=0;
					motion[i].alm_on =0;
				}

			}
//				motion[i].alm_new=1;
			else
				motion[i].alm_new=0;
		}
		else
		{
			motion[i].alm_new=0;
		}
	}
	net_motion = cam_motion;
	cam_motion = 0;
}
void sensor_detect(void)
{
	unsigned char i;
	for(i=0;i<Cam_Num;i++)
	{
		if(cam_enable & sensor_enable & BITVAL[i])
//		if(cam_enable & BITVAL[i])
		{
///////////please add code
			if(cam_sensor & BITVAL[i])
			{
				sensor[i].alm_new=1;  
				if (0==(reccam & BITVAL[i]) && sensor[i].alm_old == 1)
					sensor[i].alm_old=0;
				alarmch  = 1;
				net_sensor = 1;
//				alarm_sent_net();
			}
//				sensor[i].alm_new=1;  
			else
				sensor[i].alm_new=0;  
		}
		else
		{
			sensor[i].alm_new=0;
		}
	}
	net_sensor = cam_sensor;
}
void sensor_level_set(void)
{
///////////please add code
}
void vloss_enable_get(void)
{
	vloss_enable = cam_enable & (~manual_enable);//(time_enable | sensor_enable | motion_enable);
}

void vloss_detect(void)
{
	unsigned char i;
	
	U8 tmp1,tmp2;
	U8 tmp22,tmp55;
	tmp1=lossval & 0x0f;
	tmp2=lossval1 & 0x0f;
	tmp22=tmp1 | (tmp2<<4);
	tmp1=lossval2 & 0x0f;
	tmp2=lossval3 & 0x0f;
	tmp55=tmp1 | (tmp2<<4);

	cam_loss = (tmp55<<8) | tmp22;
//	loss_new= cam_loss;
	if (loss_old!=cam_loss)
	{
		loss_old=cam_loss;
//		printf("************** loss:0x%x\n",cam_loss);
	}
	for(i=0;i<Cam_Num;i++)
	{
		if(cam_enable & BITVAL[i])
		{
///////////please add code
			if(cam_loss & BITVAL[i])
				vloss[i].alm_new=1;
			else
				vloss[i].alm_new=0;
		}
		else
		{
			vloss[i].alm_new=0;
		}
	}
}


void relay_set(unsigned char onoff)
{
///////////please add code
//	printf("************************** relay_set : onoff = %d\n",onoff);
	if(onoff)
	{
//		if(relay_keep_time==0)
//		{
			RelayControl(ON);//relay on
			LedControl(LEDALARM,ON);//alarm led on
//		}

//		relay_keep_time=RELAY_TIME;
	}
	else
	{
		RelayControl(OFF);//relay off
		LedControl(LEDALARM,OFF);;//alarm led off
	}
}
void buzzer_set(unsigned char onoff)
{
///////////please add code
	if(onoff)
	{
		BuzzerControl(2);;//buzzer on
	}
	else
	{
		BuzzerControl(OFF);//buzzer off
	}
}
void frame_rate_set(unsigned char channel,unsigned char frame)
{
///////////please add code
}
void I_frame_set(unsigned char channel)
{
///////////please add code
	channel = 0;
}
void I_frame_interval_set(unsigned char channel,unsigned char frame)
{
///////////please add code
	channel = frame;
}
void log_write(unsigned char channel,unsigned char event,unsigned char num) 
//low_0_power on  low_1_power off high_*_power on reason low_2_vloss low_7_von
{
///////////please add code
	channel = channel;
	event = event;
	num = num;
}

void alarm_log_write(unsigned char channel,unsigned char event) 
//0_motion  1_sensor  2_vloss
{
	channel = 0;
	event =0;
///////////please add code
}

void record_on(int cam)
{
	int erro;
	time_t time_err;
//	printf("AAAAAAAAAAAAAAAAAAAAAAAArecord_on cam = %x,rec_flag = %x\n",cam,rec_flag);
	if (rec_succeed_flag)
		rec_succeed_flag = 0;
//	printf("record_on cam = %x,rec_flag = %x,rec_succeed_flag= %x\n",cam,rec_flag,rec_succeed_flag);
	while(!rec_succeed_flag)
	{
//		printf("&&&&&&&\n");
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",cam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",cam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",cam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",cam);
//	DBGPRINT("bbbbbbbbbbbbbbbbbbbbbbb %d \n",cam);
		if (gstCommonParam->GST_FTC_StartRec(cam,100)>0)//jlb
		{
			rec_succeed_flag = 1;
			rec_flag=1;
			reccam = cam;
//			printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&Record is sucess!cam = %x \n",cam);
		}
		else
		{
//			printf("*******\n");
			gstCommonParam->GST_FS_GetErrorMessage(&erro,&time_err);
			if (erro == ALLDISKNOTENABLETOWRITE)
			{
				rec_succeed_flag = 0;
				rec_flag=0;
				rec_on=0;
				break;
			}
			else
			{
				rec_succeed_flag = 0;
				sleep(5);
			}
		}
	}
	if (erro == ALLDISKNOTENABLETOWRITE)
	{
		InfoId = HddFullRec;
		diskstatus = 1;
		buzzer_set(1);
		MenuErrOn();
	}
	record_wait();
}

void record_set(void)
{
	if (Disk_Num)
		rec_req=1;
	motionchon_old = 0xffff;
}

void alarm_init(void)
{
	unsigned char i;
	for(i=0;i<Cam_Num;i++)
	{
		motion[i].alm_new=0;
		motion[i].alm_old=0;
		motion[i].alm_on=0;
		motion[i].alm_hold_time=0;
		sensor[i].alm_new=0;
		sensor[i].alm_old=0;
		sensor[i].alm_on=0;
		sensor[i].alm_hold_time=0;
		vloss[i].alm_new=0;
		vloss[i].alm_old=0;
		vloss[i].alm_on=0;
		vloss[i].alm_hold_time=0;
	}
	if(Cam_Num==4)
		cam_enable=0xf;
	else if(Cam_Num==8)
		cam_enable=0xff;
	else if(Cam_Num==12)
		cam_enable=0xfff;
	else
		cam_enable=0xffff;
	manual_enable=0x0;
	time_enable=0;
	vloss_enable=0;
	sensor_enable=0;
	motion_enable=0;
	recch=0;
	recch_old=0;
	buzzer_hold_time=0;
	buzzer_set(0);
	relay_hold_time=0;
	relay_set(0);
	mail_hold_time = 0;
	sensor_level_set();
	AlarmReset();
	cam_sensor=0;
	cam_motion=0;
}


extern video_profile   video_setting_tw[];
void record_set1(void)
{
	U8 NorP;//,CiforD1;
//	int i,I_frame_interval;
	int iframe,framerateset;
	iframe=0;
	framerateset=0;
//	printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBb recch_old = %x,recch=%x\n",recch_old,recch);
//	if(recch_old==recch)
//	{
//		return;
//	}
	if (SysMenu.VideoFormat)
		NorP =  0;
	else
		NorP = 1;

#if 0	
	
		if((cam_enable & manual_enable) == cam_enable)
		{
			for(i=0;i<CAMNUM;i++)
			{
			//frame_rate_set(i,frame_rate[i]);
				framerateset=1;
	//			printf("AAAAAAAAAAAAAAAAA frame_rate[%d]=%d\n",i,frame_rate[i]);
				video_setting_tw[i].framerate = frame_rate[i];
			//	I_frame_set(i);
				iframe|=BITVAL[i];
			//	I_frame_interval_set(i,60);	
				video_setting_tw[i].gop_size = 60;
//				printf("@@@@@@@@@ video_setting_tw[%d].gop_size = %d\n",i,video_setting_tw[i].gop_size);
			}
		}
		else
		{
			for(i=0;i<CAMNUM;i++)
			{
				if((recch & BITVAL[i]) != 0 && (recch_old & BITVAL[i]) == 0)
				{
	//				frame_rate_set(i,frame_rate[i]);
					framerateset=1;
					video_setting_tw[i].framerate = frame_rate[i];
	//				printf("BBBBBBBBBBBBBBBBBBBBBBcjy frame_rate[%x]= %x\n",i,video_setting_tw[i].framerate);
	//				I_frame_interval_set(i,60);
	//				if(RecordMenu.Resolution==2)
	//					video_setting_tw[i].gop_size = 20;
	//				else
						video_setting_tw[i].gop_size = 60;
//					printf("*************** video_setting_tw[%d].gop_size = %d\n",i,video_setting_tw[i].gop_size);
				}
				else if((recch & BITVAL[i]) == 0 && (recch_old & BITVAL[i]) != 0)
				{
	//				frame_rate_set(i,30);//prealm_frame_rate[(CAMNUM/4)-1][resolution]);
					framerateset=1;
					video_setting_tw[i].framerate = 30;
	//				printf("CCCCCCCCCCCCCCCCCCCcjy frame_rate[%x]= %x\n",i,video_setting_tw[i].framerate);
	//				I_frame_interval_set(i,10);
					if(RecordMenu.Resolution==2)
						video_setting_tw[i].gop_size = 4;
					else
						video_setting_tw[i].gop_size = 10;
//					printf("$$$$$$$$$$$$$$$$$$$$$ video_setting_tw[%d].gop_size = %d\n",i,video_setting_tw[i].gop_size);
	//				I_frame_set(i);
					iframe|=BITVAL[i];
				}
			}

			
			
		}
	/*	
		if(iframe!=0)
		{
			printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDcjy instant I frame set= %x\n",iframe);
		}
		
		if (recch_old!=recch)
		{
			printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE recch_old = %x,recch = %x\n",recch_old,recch);
		}
	*/
		

		if(framerateset!=0)  //for frame rate and I frame interval setup
		{
			i= gstCommonParam->GST_DRA_change_device_set(&video_setting_tw[0],NorP,CiforD1);
//			if (iframe & BITVAL[3])
//				printf("############## iframe = %x,video_setting_tw[2].framerate = %x\n",iframe,video_setting_tw[2].framerate);
			if (i<0)
				printf("change_device_set error!\n");
		}

		if(iframe!=0)
		{
			gstCommonParam->GST_DRA_local_instant_i_frame(iframe);
//			if (iframe & BITVAL[3])
//				printf("***************** iframe = %x\n",iframe);
	//		printf("***************cjy instant I frame set= %x\n",iframe);
		}
	}
#endif	
	if (recch_old!=recch)
	{
//		DBGPRINT("aaaaaaaaaaaaaaaaaaaaaaa %d %d mmmmmmmmmmmmmmmmmmmmmmmmmm \n",recch,recch_old);
		recch_old=recch;
		record_on(recch);
	}
///////////please add code
	if(rec_key)
	{
		rec_key=0;
/*		
		if (demo_num == L16)
			WriteOSDBmpX_bmp(70,125+SysMenu.VideoFormat*20,WHITE,Blanking,Play_Menu);
		else
			WriteOSDBmpX_bmp(70,140+SysMenu.VideoFormat*20,WHITE,Blanking,Play_Menu);
*/
		if (demo_num<PBFULL && !menu_num)
			demo_osd_disp();
		////cjy clear record starting...
	}
	
}
extern U8 mailsend;
extern U8 mailevent;
extern U16 mailcam;

void alarm_set(unsigned int camsno,unsigned char event)  //event 2: loss 0: motion 1: senser
{
	unsigned int tmpbuzzer,tmprelay,tmpmail;
	unsigned char i;
	tmpbuzzer=0;
	tmprelay=0;
	tmpmail=0;
	switch(event)
	{
	case Emotion:
		for(i=0;i<16;i++)
		{
			tmpbuzzer |=MenuAlarm_Date[0][i][1]<<i;
			tmprelay |=MenuAlarm_Date[1][i][1]<<i;
			tmpmail |=MenuAlarm_Date[2][i][1]<<i;
//			printf("");
		}
		tmpmail=MenuAlarm_Date[0][0][1];
		break;
	case Esensor:
		for(i=0;i<16;i++)
		{
			tmpbuzzer|=MenuAlarm_Date[0][i][2]<<i;
			tmprelay|=MenuAlarm_Date[1][i][2]<<i;
			tmpmail|=MenuAlarm_Date[2][i][2]<<i;
		}
		tmpmail=MenuAlarm_Date[0][0][2];
		break;
	case Evloss:
		for(i=0;i<16;i++)
		{
			tmpbuzzer|=MenuAlarm_Date[0][i][0]<<i;
			tmprelay|=MenuAlarm_Date[1][i][0]<<i;
			tmpmail|=MenuAlarm_Date[2][i][0]<<i;
//			printf("tmpbuzzer = %x,MenuAlarm_Date[0][%d][0] = %x\n",tmpbuzzer,i,MenuAlarm_Date[0][i][0]);
		}
		break;
	default:
		break;
	}
//	printf("tmpbuzzer = %x,camsno=%x\n",tmpbuzzer,camsno);
	if(tmpbuzzer & camsno)
	{
		if(buzzer_hold_time==0)
		{
			buzzer_set(1);
		}
		buzzer_hold_time=MAX_BUZZER_HOLD_TIME;
	}

	if(tmprelay & camsno)
	{
		if(relay_hold_time==0)
		{
			relay_set(1);
		}
		relay_hold_time=MAX_RELAY_HOLD_TIME;
	}
	
	if (mail_hold_time==0 && rec_on)
	{
		mailcam = tmpmail & camsno;
		if (mailcam)
		{
			mailsend = 1;
			mailevent = event;
			mail_hold_time=EmailMenu.TimeTev*60;
		}
	}

}


void vloss_process(void)
{
	unsigned char i,almset,recset;
	unsigned int camsno;
	camsno=0;

	almset=0;
	recset=0;
	
	for(i=0;i<Cam_Num;i++)
	{
		if(vloss[i].alm_new==1 && vloss[i].alm_old==0) //new vloss
		{
			almset=1;
			camsno|=BITVAL[i];
			if(vloss[i].alm_on == 0)
			{
				if(vloss_enable & BITVAL[i])
				{//remoloss
//					if((recch & BITVAL[i]) == 0)
//					{
//						recch|=BITVAL[i];
//						recset=1;
//					}
					record_disp(i,Evloss);
					alarm_log_write(i,Evloss);
//					sensor[i].alm_on = 0;
//					sensor[i].alm_new = 0;
//					sensor[i].alm_hold_time = 0;
//					motion[i].alm_on = 0;
//					motion[i].alm_new = 0;
//					motion[i].alm_hold_time = 0;
				}
				log_write(i,Evloss,0);
				vloss[i].alm_on = 1;
				vloss[i].alm_hold_time=MAX_VLOSS_HOLD_TIME;
			}
			loss_disp(i,1);
		}
		else if(vloss[i].alm_new==0 && vloss[i].alm_old==1) //von
		{
			if(vloss[i].alm_on == 0)
			{
				if(vloss_enable & BITVAL[i])
				{//remoloss
//					if((recch & BITVAL[i]) == 0)
//					{
//						recch|=BITVAL[i];
//						recset=1;
//					}
					record_disp(i,Evloss);
				}
				log_write(i,Evon,0);
				vloss[i].alm_on = 1;
				vloss[i].alm_hold_time=MAX_VLOSS_HOLD_TIME;
			}
			loss_disp(i,0);
		}
		vloss[i].alm_old = vloss[i].alm_new;
	}

	if(recset == 1 && rec_on)
	{
		printf("vloss_process record_set **********************\n");		
		record_set();		
	}

	if(almset)
	{
		alarm_set(camsno,Evloss);
	}
}

void motion_process(void)
{
	unsigned char i,almset,recset;
	unsigned int camsno;
	camsno=0;

	almset=0;
	recset=0;
	if(goout)
		return;

	for(i=0;i<Cam_Num;i++)
	{
		if((motion_enable & BITVAL[i]))
		{
			if(motion[i].alm_new==1 && motion[i].alm_old==0) //new motion
			{
				almset=1;
				camsno|=BITVAL[i];
				if(motion[i].alm_on == 0)
				{
					if((recch & BITVAL[i]) == 0 && vloss[i].alm_new == 0)
					{
						recch|=BITVAL[i];
						recset=1;
					}
					if(vloss[i].alm_new==0 && vloss[i].alm_on==0)
					{
						record_disp(i,Emotion);
					}
					motion[i].alm_on = 1;
					alarm_log_write(i,Emotion);
				}
			}
			else if(motion[i].alm_new==0 && motion[i].alm_old==1) //motion disappear
			{
				motion[i].alm_hold_time=MAX_MOTION_HOLD_TIME;
	
			}
			motion[i].alm_old = motion[i].alm_new;
		}
	}

	if(recset == 1 && rec_on)
	{
		printf("motion_process record_set **********************\n");	
		record_set();
	}

	if(almset)
	{
		alarm_set(camsno,Emotion);
	}
}
void sensor_process(void)
{
	unsigned char i,almset,recset;
	unsigned int camsno;
	camsno=0;
	almset=0;
	recset=0;
	U8 popcam=0xff;
	if(goout)
		return;
	for(i=0;i<Cam_Num;i++)
	{
		if((sensor_enable & BITVAL[i]))
		{
			if(sensor[i].alm_new==1 && sensor[i].alm_old==0) //new sensor
			{
				almset=1;
				camsno|=BITVAL[i];
				if(sensor[i].alm_on == 0)
				{
					if((recch & BITVAL[i]) == 0 && vloss[i].alm_new == 0)
					{
						recch|=BITVAL[i];
						recset=1;
						if (popcam==0xff)
							popcam = i;
					}
					if(vloss[i].alm_new==0 && vloss[i].alm_on==0)
					{
						record_disp(i,Esensor);
					}
					sensor[i].alm_on = 1;
					alarm_log_write(i,Esensor);
				}
			}
			else if(sensor[i].alm_new==0 && sensor[i].alm_old==1) //sensor disappear
			{
				sensor[i].alm_hold_time=MAX_SENSOR_HOLD_TIME;
	
			}
			sensor[i].alm_old = sensor[i].alm_new;
		}
	}

	if(recset == 1 && rec_on)
	{
		printf("sensor_process record_set **********************\n");	
		record_set();
		if (popcam != 0xff)
		{
//			printf("&&&&&&&&&&&&&&&&&&&& spot popup setup!%x\n",popcam);
		}
	}

	if(almset)
	{
		alarm_set(camsno,Esensor);
	}
}

unsigned char alarm_reset(unsigned char channel,unsigned char event)
//0_motion  1_sensor  2_vloss  3_time  4_manual  5_idle 6_clear 7_von
{
	unsigned char recset;
	recset = 0;
	switch(event)
	{
	case Emotion:
		if(sensor[channel].alm_on == 0 && vloss[channel].alm_on == 0)
		{
			record_disp(channel,Eidle);
			recch &= ~BITVAL[channel];
			recset = 1;
		}
		else if(vloss[channel].alm_on != 0)
		{
			record_disp(channel,Etime);
		}
		else if(sensor[channel].alm_on != 0)
		{
			record_disp(channel,Esensor);
		}
		break;
	case Esensor:
		if(motion[channel].alm_on == 0 && vloss[channel].alm_on == 0)
		{
			record_disp(channel,Eidle);
			recch &= ~BITVAL[channel];
			recset = 1;
		}
		else if(vloss[channel].alm_on != 0)
		{
			record_disp(channel,Etime);
		}
		else if(motion[channel].alm_on != 0)
		{
			record_disp(channel,Emotion);
		}
		break;
	case Evloss://remoloss
		record_disp(channel,Eidle);
//		recch &= ~BITVAL[channel];
//		recset = 1;
		break;
	case Evon:
		if(motion[channel].alm_on == 0 && sensor[channel].alm_on == 0 && (time_enable & BITVAL[channel]) == 0)
		{
			record_disp(channel,Eidle);
			recch &= ~BITVAL[channel];
			recset = 1;
		}
		else if(motion[channel].alm_on != 0)
		{
			record_disp(channel,Emotion);
		}
		else if(sensor[channel].alm_on != 0)
		{
			record_disp(channel,Esensor);
		}
		else if(time_enable & BITVAL[channel])
		{
			record_disp(channel,Etime);
		}
		break;
	case Etime:
		if(vloss[channel].alm_on)
		{
			record_disp(channel,Evloss);
			if(vloss[channel].alm_new==0)
			{
				alarm_log_write(channel,Evloss);
			}
			else
			{
				alarm_log_write(channel,Evon);
			}
		}
		else
		{
			record_disp(channel,Eidle);
			recch &= ~BITVAL[channel];
			motion_detect();
			motion_process();
			sensor_detect();
			sensor_process();
			recset = 1;
		}
		break;

	}
	return recset;
}
void alarm_second(void)
{
	unsigned char i,recset;
	recset=0;
	if(buzzer_hold_time)
	{
		buzzer_hold_time--;	
		if(buzzer_hold_time==0)
			buzzer_set(0);
	}
	if(relay_hold_time)
	{
		relay_hold_time--;	
		if(relay_hold_time==0)
			relay_set(0);
	}
	if (mail_hold_time)
		mail_hold_time--;
	for(i=0;i<Cam_Num;i++)
	{
		if(motion[i].alm_on == 1 && motion[i].alm_new == 0 && (motion_enable & BITVAL[i]))
//		if(motion[i].alm_on == 1 && motion[i].alm_new == 0)// && (motion_enable & BITVAL[i]))
		{
			if(motion[i].alm_hold_time!=0)
			{
				motion[i].alm_hold_time--;
			}
			if(motion[i].alm_hold_time==0)
			{
				motion[i].alm_on = 0;
				printf("44\n");
				if(alarm_reset(i,Emotion))
				{
					recset = 1;
				}
			}
		} 
		if(sensor[i].alm_on == 1 && sensor[i].alm_new == 0 && (sensor_enable & BITVAL[i]))
//		if(sensor[i].alm_on == 1 && sensor[i].alm_new == 0)// && (sensor_enable & BITVAL[i]))
		{
			if(sensor[i].alm_hold_time!=0)
			{
				sensor[i].alm_hold_time--;
			}
			if(sensor[i].alm_hold_time==0)
			{
				sensor[i].alm_on = 0;
				if(alarm_reset(i,Esensor))
				{
					recset = 1;
				}
			}
		}
		if(vloss[i].alm_on == 1)
		{
			if(vloss[i].alm_hold_time!=0)
			{
				vloss[i].alm_hold_time--;
			}
			if(vloss[i].alm_hold_time==0)
			{
				vloss[i].alm_on = 0;
				if(vloss_enable & BITVAL[i])
				{
					if(vloss[i].alm_new)
					{
						if(alarm_reset(i,Evloss))
						{
							recset = 1;
						}
					}
					else
					{
						if(alarm_reset(i,Evon))
						{
							recset = 1;
						}
					}
				}
			}
		}
		if(recset == 1 && rec_on)
		{
		printf("alarm_second record_set **********************\n");	
//			printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA alarmsecond\n");
			record_set();
		}
	}
}

unsigned char seg_process(unsigned char channel,unsigned char epval)
{
	unsigned char recset;
	recset=0;
	switch(epval)
	{
	case 0:     //motion
		motionchon |= BITVAL[channel];
		if((motion_enable & BITVAL[channel]) != 0 && (sensor_enable & BITVAL[channel]) == 0) 
			break;
		else
		{
			motion_enable |= BITVAL[channel];
			if(time_enable & BITVAL[channel])
			{
				if(alarm_reset(channel,Etime))
				{
					recset = 1;
//					printf("*************** Mtime reset channel = %x,epval = %x",channel,epval);
				}
				time_enable &= ~BITVAL[channel];
			}
			else if(sensor_enable & BITVAL[channel])
			{
//				if(sensor[channel].alm_on)
				{
					sensor[channel].alm_on = 0;
					sensor[channel].alm_new = 0;
					sensor[channel].alm_hold_time = 0;
					if(alarm_reset(channel,Esensor))
					{
						recset = 1;
//					printf("*************** Msensor reset channel = %x,epval = %x",channel,epval);
						
					}
				}
				sensor_enable &= ~BITVAL[channel];
			}						
		}
		break;
	case 1:     //sensor
		motionchon &= ~BITVAL[channel];
		if((sensor_enable & BITVAL[channel]) != 0 && (motion_enable & BITVAL[channel]) == 0) 
			break;
		else
		{
			sensor_enable |= BITVAL[channel];
			if(time_enable & BITVAL[channel])
			{
				if(alarm_reset(channel,Etime))
				{
					recset = 1;
//					printf("*************** Stime reset channel = %x,epval = %x",channel,epval);
				}
				time_enable &= ~BITVAL[channel];
			}
			else if(motion_enable & BITVAL[channel])
			{
//				if(motion[channel].alm_on)
				{
					motion[channel].alm_on = 0;
					motion[channel].alm_new = 0;
					motion[channel].alm_hold_time = 0;
					if(alarm_reset(channel,Emotion))
					{
						recset = 1;
//						printf("*************** Smotion reset channel = %x,epval = %x",channel,epval);
					}
				}
				motion_enable &= ~BITVAL[channel];
			}						
		}
		break;
	case 2:     //motion+sensor
		motionchon |= BITVAL[channel];
		if((sensor_enable & BITVAL[channel]) != 0 && (motion_enable & BITVAL[channel]) != 0) 
			break;
		else
		{
			sensor_enable |= BITVAL[channel];
			motion_enable |= BITVAL[channel];
			if(time_enable & BITVAL[channel])
			{
				if(alarm_reset(channel,Etime))
				{
					recset = 1;
//					printf("*************** MStime reset channel = %x,epval = %x",channel,epval);
				}
				time_enable &= ~BITVAL[channel];
			}						
		}
		break;
	default:    //time
		motionchon &= ~BITVAL[channel];
		if((time_enable & BITVAL[channel]) != 0) 
		{
			break;
		}
		else
		{
			time_enable |= BITVAL[channel];
			if(motion[channel].alm_on || motion_enable & BITVAL[channel])
			{
				motion[channel].alm_on = 0;
				motion[channel].alm_new = 0;
				motion[channel].alm_hold_time = 0;
				if(alarm_reset(channel,Emotion))
				{
					recset = 1;
//					printf("*************** Tmotion reset channel = %x,epval = %x",channel,epval);
				}
			}
			motion_enable &= ~BITVAL[channel];
			if(sensor[channel].alm_on || (sensor_enable & BITVAL[channel]))
			{
				sensor[channel].alm_on = 0;
				sensor[channel].alm_new = 0;
				sensor[channel].alm_hold_time = 0;
				if(alarm_reset(channel,Esensor))
				{
					recset = 1;
//					printf("*************** Tsensor reset channel = %x,epval = %x",channel,epval);
				}
			}
			sensor_enable &= ~BITVAL[channel];
//			if(vloss[channel].alm_new==0 && vloss[channel].alm_on==0)
			{
				record_disp(channel,Etime);
			}
			if((recch & BITVAL[channel])==0)
			{
				recch|=BITVAL[channel];
				recset = 1;
			}
		}
		break;
	}
	return recset;
}

void alarm_minute(void)
{
	time_t now_time,seg1_start,seg1_end;
	struct timeval tv;
	struct tm tp;

	time_t seg2_start,seg2_end,seg3_start,seg3_end,seg4_start,seg4_end;
	unsigned char i,recset,epval;
	unsigned int manual_enable_new;
	if(rec_on==0) return;
	recset=0;
	epval=0;

	gettimeofday( &tv, NULL );	
	gmtime_r(&tv.tv_sec,&tp);
	
	now_time=0;
	seg1_start=0;
	seg1_end=0;
	seg2_start=0;
	seg2_end=0;
	seg3_start=0;
	seg3_end=0;
	seg4_start=0;
	seg4_end=0;
	manual_enable_new=0;
	goout = 1;

	for (i=0;i<Cam_Num;i++)
	{
		if (SchedulMenu[i].Record_Stytle[tp.tm_wday+1] == 0)
			manual_enable_new |= BITVAL[i];
	}
	printf("hhhhhhhhhhhhhhhhh2014/12/27 15:48:26 hhhhhhhh manual_enable_new = %x\n",manual_enable_new);
	manual_enable_new&=cam_enable;
	printf("manual_enable_new = %x\n",manual_enable_new);
	now_time = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,buf_time[2],buf_time[1],0);

	if(manual_enable_new != manual_enable)
	{
		for(i=0;i<Cam_Num;i++)
		{
			seg1_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][0],SchedulMenu[i].Minute_Star[tp.tm_wday+1][0],0);
			seg1_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][0],SchedulMenu[i].Minute_End[tp.tm_wday+1][0],0);
			
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][0],SchedulMenu[i].Minute_Star[tp.tm_wday+1][0]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][0],SchedulMenu[i].Minute_End[tp.tm_wday+1][0]);
			seg2_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][1],SchedulMenu[i].Minute_Star[tp.tm_wday+1][1],0);
			seg2_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][1],SchedulMenu[i].Minute_End[tp.tm_wday+1][1],0);
			
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][1],SchedulMenu[i].Minute_Star[tp.tm_wday+1][1]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][1],SchedulMenu[i].Minute_End[tp.tm_wday+1][1]);

			seg3_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][2],SchedulMenu[i].Minute_Star[tp.tm_wday+1][2],0);
			seg3_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][2],SchedulMenu[i].Minute_End[tp.tm_wday+1][2],0);
			
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][2],SchedulMenu[i].Minute_Star[tp.tm_wday+1][2]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][2],SchedulMenu[i].Minute_End[tp.tm_wday+1][2]);
			
			seg4_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][3],SchedulMenu[i].Minute_Star[tp.tm_wday+1][3],0);
			seg4_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][3],SchedulMenu[i].Minute_End[tp.tm_wday+1][3],0);
			
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][3],SchedulMenu[i].Minute_Star[tp.tm_wday+1][3]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][3],SchedulMenu[i].Minute_End[tp.tm_wday+1][3]);
			if((manual_enable & BITVAL[i]) == 0 && (manual_enable_new & BITVAL[i]) != 0)
			{
				motion[i].alm_on = 0;
				motion[i].alm_new = 0;
				motion[i].alm_hold_time = 0;
				motion_enable &= ~BITVAL[i];
				sensor[i].alm_on = 0;
				sensor[i].alm_new = 0;
				sensor[i].alm_hold_time = 0;				
				sensor_enable &= ~BITVAL[i];
				time_enable &= ~BITVAL[i];
				vloss_enable &= ~BITVAL[i];	
				if((recch & BITVAL[i]) == 0)
				{
					recch|=BITVAL[i];
//					printf("manual trigger!!\n");
					recset=1;
				}
				record_disp(i,Emanual);
			}
			else if((manual_enable & BITVAL[i]) != 0 && (manual_enable_new & BITVAL[i]) == 0)
			{
				recch &= ~BITVAL[i];
				recset=1;
				record_disp(i,Eidle);
//				printf("manual trigger_1!!\n");
			}
		}

		manual_enable = manual_enable_new;
	}
DBGPRINT("bbbbbbbbbbbbbbbbbbbb aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \n");
DBGPRINT("bbbbbbbbbbbbbbbbbbbb aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \n");
DBGPRINT("bbbbbbbbbbbbbbbbbbbb aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \n");
DBGPRINT("bbbbbbbbbbbbbbbbbbbb aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \n");
DBGPRINT("bbbbbbbbbbbbbbbbbbbb aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \n");
DBGPRINT("bbbbbbbbbbbbbbbbbbbb aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa \n");

for(i=0;i<8;i++)
{
	DBGPRINT("i:%d,start:%d,%d,end:%d,%d \n",i,SchedulMenu[0].Hour_Star[i][0],SchedulMenu[0].Minute_Star[i][0],SchedulMenu[0].Hour_End[i][0],SchedulMenu[0].Minute_End[i][0]);
}

	for(i=0;i<Cam_Num;i++)
	{
		seg1_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][0],SchedulMenu[i].Minute_Star[tp.tm_wday+1][0],0);
		seg1_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][0],SchedulMenu[i].Minute_End[tp.tm_wday+1][0],0);
		
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][0],SchedulMenu[i].Minute_Star[tp.tm_wday+1][0]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][0],SchedulMenu[i].Minute_End[tp.tm_wday+1][0]);
			DBGPRINT(" ****************** now_time:%d ,seg1_start:%d,seg1_end:%d \n",now_time,seg1_start,seg1_end);
		seg2_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][1],SchedulMenu[i].Minute_Star[tp.tm_wday+1][1],0);
		seg2_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][1],SchedulMenu[i].Minute_End[tp.tm_wday+1][1],0);

			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][1],SchedulMenu[i].Minute_Star[tp.tm_wday+1][1]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][1],SchedulMenu[i].Minute_End[tp.tm_wday+1][1]);
			DBGPRINT(" ****************** now_time:%d ,seg2_start:%d,seg2_end:%d \n",now_time,seg2_start,seg2_end);
			
		seg3_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][2],SchedulMenu[i].Minute_Star[tp.tm_wday+1][2],0);
		seg3_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][2],SchedulMenu[i].Minute_End[tp.tm_wday+1][2],0);
		
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][2],SchedulMenu[i].Minute_Star[tp.tm_wday+1][2]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][2],SchedulMenu[i].Minute_End[tp.tm_wday+1][2]);
			DBGPRINT(" ****************** now_time:%d ,seg3_start:%d,seg3_end:%d \n",now_time,seg3_start,seg3_end);
			
		seg4_start = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_Star[tp.tm_wday+1][3],SchedulMenu[i].Minute_Star[tp.tm_wday+1][3],0);
		seg4_end = gstCommonParam->GST_FTC_CustomTimeTotime_t( 100,3,1,SchedulMenu[i].Hour_End[tp.tm_wday+1][3],SchedulMenu[i].Minute_End[tp.tm_wday+1][3],0);
//		printf("DDDDDDDDDDDDDDDDDDDDDDDSchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][0]:%d\n",SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][0]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_Star[tp.tm_wday+1][3],SchedulMenu[i].Minute_Star[tp.tm_wday+1][3]);
			DBGPRINT("i:%d,wday:%d,%d,%d, \n",i,(tp.tm_wday+1),SchedulMenu[i].Hour_End[tp.tm_wday+1][3],SchedulMenu[i].Minute_End[tp.tm_wday+1][3]);
			DBGPRINT(" ****************** now_time:%d ,seg4_start:%d,seg4_end:%d \n",now_time,seg4_start,seg4_end);
			
		if((manual_enable & BITVAL[i]) == 0)
		{
			if(now_time>=seg1_start && now_time<seg1_end && seg1_start!=seg1_end)
			{
				DBGPRINT("aaaaaaaaaaaaaaaaaa \n");
				if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][0] == 2)  //motion
					epval= 0;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][0] == 1)  //sensor
					epval= 1;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][0] == 3)  //motion+sensor
					epval= 2;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][0] == 0)                                  //time
					epval= 3;
				if(seg_process(i,epval))
				{
					recset = 1;
//					printf("seg1 trigger!!\n");
//					printf("*************** i = %x,epval = %x",i,epval);
				}
			}
			else if(now_time>=seg2_start && now_time<seg2_end && seg1_start!=seg1_end)
			{
				DBGPRINT("nnnbbbbbbbbbbbbbbbbbbb \n");
				if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][1] == 2)  //motion
					epval= 0;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][1] == 1)  //sensor
					epval= 1;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][1] == 3)  //motion+sensor
					epval= 2;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][1] == 0)                                  //time
					epval= 3;
				if(seg_process(i,epval))
				{
					recset = 1;
					DBGPRINT("seg2 trigger!!\n");
				}
			}
			else if(now_time>=seg3_start && now_time<seg3_end && seg1_start!=seg1_end)
			{
				DBGPRINT("ccccccccccccccccccccccccccc \n");
				if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][2] == 2)  //motion
					epval= 0;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][2] == 1)  //sensor
					epval= 1;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][2] == 3)  //motion+sensor
					epval= 2;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][2] == 0)                                  //time
					epval= 3;
				if(seg_process(i,epval))
				{
					recset = 1;
					DBGPRINT("seg3 trigger!!\n");
				}
			}
			else if(now_time>=seg4_start && now_time<seg4_end && seg1_start!=seg1_end)
			{
				DBGPRINT("dddddddddddddddddddddddddd \n");
				if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][3] == 2)  //motion
					epval= 0;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][3] == 1)  //sensor
					epval= 1;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][3] == 3)  //motion+sensor
					epval= 2;
				else if (SchedulMenu[i].Alarm_Stytle[tp.tm_wday+1][3] == 0)                                  //time
					epval= 3;
				if(seg_process(i,epval))
				{
					recset = 1;
					DBGPRINT("seg4 trigger!!\n");
				}
			}
			else
			{
				sensor_enable &= ~BITVAL[i];
				motion_enable &= ~BITVAL[i];
				time_enable &= ~BITVAL[i];
				motion[i].alm_on = 0;
				motion[i].alm_new = 0;
				motion[i].alm_hold_time = 0;
				sensor[i].alm_on = 0;
				sensor[i].alm_new = 0;
				sensor[i].alm_hold_time = 0;				
				if(vloss[i].alm_on == 0) 
				{
					record_disp(i,Eidle);
					if((recch & BITVAL[i])!=0)
					{
						recch&=~BITVAL[i];
						recset = 1;
					}
				}
				
			}
		}
		vloss_enable_get();
	}
	if(recset == 1)
	{
		printf("alarm_minute record_set **********************\n");	
		record_set();
	}
	if (rec_req)
	{
		if (motionchon_old != motionchon)
		{
			for (i=0;i<Cam_Num;i++)
			{
				epval = motionchon & BITVAL[i];
				if (epval)
					epval = 1;
				else
					epval = 0;
//jlb090820				gstCommonParam->GST_DRA_set_motion_detect_info(i,epval,&mval[i]);
//				printf("###############ch:%d, epval : %d,motionchon=%d\n",i,epval,motionchon);
			}
			motionchon_old = motionchon;
		}
	}
	goout = 0;
}

void record_disp_refresh(void)
{
	unsigned char i;
	for(i=0;i<Cam_Num;i++)
	{
		if(vloss[i].alm_new)
			loss_disp(i,1);
		if(manual_enable & BITVAL[i])
			record_disp(i,Emanual);
		else if(vloss[i].alm_on)
			record_disp(i,Evloss);
		else if(vloss[i].alm_on == 0 && vloss[i].alm_new == 1)
			record_disp(i,Eidle);
		else if(motion[i].alm_on)
			record_disp(i,Emotion);
		else if(sensor[i].alm_on)
			record_disp(i,Esensor);
		else if(time_enable & BITVAL[i])
			record_disp(i,Etime);
		else
			record_disp(i,Eidle);		
	}
}

U8 CheckUser(U8 UserId)
{
//	UserId = 1;
	if (UserId==0)
	{
		InfoId = PermissonErr;
		MenuErrOn();
		return 1;
	}

	return 0;
}


void record_key_set(void)
{

//	U8 i;
	int ret,tmp;
	tmp=0;
	if (CheckUser(menupass[curuser].Reccfig))
	{
		return;
	}
	
	if(rec_on) return;

	rec_key=1;
/*	
	if (demo_num == L16)
		WriteOSDBmpX_bmp(70,125+SysMenu.VideoFormat*20,WHITE,Recording,Play_Menu);
	else
		WriteOSDBmpX_bmp(70,140+SysMenu.VideoFormat*20,WHITE,Recording,Play_Menu);
*/
//	tmp = Get_Fream_Setup();
//	gstCommonParam->GST_FTC_set_count_create_keyframe(tmp*5);

	ret = gstCommonParam->GST_FTC_CheckPreviewStatus();
	while(!ret)
	{
		
		gstCommonParam->GST_FTC_PreviewRecordStart(5);
		ret = gstCommonParam->GST_FTC_CheckPreviewStatus();
		if(ret)
			printf("prerecord ok!! \n" );
		usleep(50000);
	}
	///cjy prerecord end

	if (CheckUser(menupass[curuser].Reccfig))
		return;
	alarm_init();
	rec_on=1;
//	set_motion(rec_on);
	alarm_minute();
	event_write(REC_START_BYHAND,0,0);

//	printf("NNNNNNNNNNNNNNNNNNNNNNNNNNN rec_succeed_flag = %d,manual_enable=%x\n",rec_succeed_flag,manual_enable);

		printf("record_key_set record_set **********************\n");	
	record_set();
//	if (rec_succeed_flag)
//	{
		record_disp_refresh();
		LedControl(LEDREC,1);
//	}
//	else
//		alarm_init();
}

void record_off(void)
{
	
	if(rec_flag==0) return;
	rec_flag=0;
	
	if (gstCommonParam->GST_FTC_StopRec()<0)
	{
		bReset = 1;
		printf("bReset is %x\n",bReset);
		return;
	}
	else
	{
		printf("Record off is sucess!\n");
	}

}

void stop_key_set(void)
{
	U8 i;
	int ret;
	if(rec_on==0) return;
	for (i=0;i<Cam_Num;i++)
		record_disp(i,Eclear);	
	rec_on=0;
	record_off();
//	record_disp_clear();
//	record_disp();
//	demo_num = L4;
	LedControl(LEDREC,0);
//	event_write(REC_STOP_BYHAND,0,4);
	alarm_init();
	ret=gstCommonParam->GST_FTC_StopRec(0);
	if(ret)
		printf("prerecord stop!!!\n");
	
	alarmch = 0;
	net_motion = 0;
	net_sensor = 0;
}


