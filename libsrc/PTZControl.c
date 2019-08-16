#include "cmn.h"

#define MENUPTZMIX	3

U8 Cur_CH=0;

//U16 Key_Old;
U8 Ptz_count;

U8 PTZ_ON = 0;
//U8 Longkey_Old;
U8 NetCam=0xff;
U8 transe_count,ptz_auto_flag=0;

U8 Data_Pos=0,Data_Show=0;
U8 pointdata[2]={0,0};

int Baud[]=
{
	1200,
	2400,
	4800,
	9600
};

U16 Preptzmousepos[]=
{
	76,1,	164,21,
	240,1,	328,21,
	404,1,	492,21
};

 U8 tMENU_CAM_DATA[]=
{
	"\x10\x07 CAM01\0 CAM02\0 CAM03\0 CAM04\0 CAM05\0 CAM06\0 CAM07\0 CAM08\0 CAM09\0 CAM10\0 CAM11\0 CAM12\0 CAM13\0 CAM14\0 CAM15\0 CAM16\0"//0
};


void save_speed()
{
	PtzMenu[Cur_CH].Speed= pointdata[0];
	I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*Cur_CH+SPEED,1,&PtzMenu[Cur_CH].Speed);
}

void Ptz_Menu_Disp(U8 color, U8 pos)
{
}

void Predata_Show(U8 udata)
{
	if (Data_Show)
		pointdata[Data_Pos-1] = pointdata[Data_Pos-1]*10+udata;
	else
		pointdata[Data_Pos-1] = udata;
	if (Data_Pos==1)
	{
		if(pointdata[0]>30)
			pointdata[0]=30;
		save_speed();
	}
	else if (Data_Pos==2)
	{
		if(pointdata[1]>99)
			pointdata[1]=99;
	}
	Ptz_Menu_Disp(RED,Data_Pos);
}

void PTZ_Control_ON()
{
}

void net_ptz_reconfig()
{
	setuart(Baud[PtzMenu[Cur_CH].BaudRate]);
}


void PTZ_Control_Off()
{
	menu_num = 0;
	LedControl(LEDPTZ,0);
    	OSD_ALL_CLEAR();
	demo_osd_disp();
	byear_change = 1;
	Data_Pos = 0;
}
void set_ch_vual(U8 ch)
{
	Cur_CH = ch;
	pointdata[0] = PtzMenu[Cur_CH].Speed;


}


void PTZ_Set_CH(U8 ch)
{
	U8 i;

	alarmboardshow(2,Cur_CH,0);
	Cur_CH = ch;
	alarmboardshow(WHITE,Cur_CH,0);
	set_ch_vual(ch);
	for (i=0;i<MENUPTZMIX-1;i++)
	{
		if (i==Data_Pos)
			Ptz_Menu_Disp(RED,i);
		else
			Ptz_Menu_Disp(WHITE,i);
	}
}

void PTZ_Baud_Add()
{
	if(PtzMenu[Cur_CH].BaudRate>=3)
		PtzMenu[Cur_CH].BaudRate=0;
	else
		PtzMenu[Cur_CH].BaudRate++;
	I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*Cur_CH+BAUDRATE,1,&PtzMenu[Cur_CH].BaudRate);
}

void PTZ_Baud_Dec()
{
	if(PtzMenu[Cur_CH].BaudRate<=0)
		PtzMenu[Cur_CH].BaudRate=3;
	else
		PtzMenu[Cur_CH].BaudRate--;
	I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*Cur_CH+BAUDRATE,1,&PtzMenu[Cur_CH].BaudRate);
}

void PTZ_Protocl_Add()
{
	if (PtzMenu[Cur_CH].Protocol>=2)
		PtzMenu[Cur_CH].Protocol = 0;
	else
		PtzMenu[Cur_CH].Protocol ++;
	I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*Cur_CH+PROTOCOL,1,&PtzMenu[Cur_CH].Protocol);
}

void PTZ_Protocl_Dec()
{
	if (PtzMenu[Cur_CH].Protocol<=0)
		PtzMenu[Cur_CH].Protocol = 2;
	else
		PtzMenu[Cur_CH].Protocol --;
	I_write_block_eeprom(ADDPTZ+ADDCOUNTPTZ*Cur_CH+PROTOCOL,1,&PtzMenu[Cur_CH].Protocol);
}

void Ptz_Data_Do(U8 pos)
{
	U8 i;
	switch(pos)
	{
		case 0:
			set_ch_vual(Cur_CH);
			for (i=1;i<MENUPTZMIX-1;i++)
			{
				Ptz_Menu_Disp(WHITE,i);
			}
			break;
		case 1:
			save_speed();
			break;
		case 2:

			break;
		default:
			break;
	}
	Ptz_Menu_Disp(RED,pos);
}

void Ptz_Data_Add(U8 pos)
{
//	printf("********************** pos:%d\n",pos);
	switch(pos)
	{
		case 0:
			alarmboardshow(2,Cur_CH,0);
			if (Cur_CH>= Cam_Num-1)
				Cur_CH = 0;
			else
				Cur_CH ++;
			alarmboardshow(WHITE,Cur_CH,0);
			pointdata[1] = 0;
			break;
		case 1:
//			if (pointdata[0]>=30)
			if (pointdata[0]>=63)
				pointdata[0] = 0;
			else
				pointdata[0] ++;
			break;
		case 2:
			if (pointdata[1]>=99)
				pointdata[1] = 0;
			else
				pointdata[1] ++;
			break;
		default:
			break;
	}
	Ptz_Data_Do(pos);
}

void Ptz_Data_Dec(U8 pos)
{
//	printf("###################### pos:%d\n",pos);
	switch(pos)
	{
		case 0:
			alarmboardshow(2,Cur_CH,0);
			if (Cur_CH<= 0)
				Cur_CH = Cam_Num-1;
			else
				Cur_CH --;
			alarmboardshow(WHITE,Cur_CH,0);
			pointdata[1] = 0;
			break;
		case 1:
			if (pointdata[0]<=0)
//				pointdata[0] = 30;
				pointdata[0] = 63;
			else
				pointdata[0] --;
			break;
		case 2:
			if (pointdata[1]<=0)
				pointdata[1] = 99;
			else
				pointdata[1] --;
			
			break;
		default:
			break;
	}
	Ptz_Data_Do(pos);
}

void Ptz_Menu_Pos_Up()
{
	Ptz_Menu_Disp(WHITE,Data_Pos);
	if (Data_Pos<=0)
		Data_Pos = 2;
	else
		Data_Pos--;
	Ptz_Menu_Disp(RED,Data_Pos);
}

void Ptz_Menu_Pos_Down()
{
	Ptz_Menu_Disp(WHITE,Data_Pos);
	if (Data_Pos>=2)
		Data_Pos = 0;
	else
		Data_Pos++;
	Ptz_Menu_Disp(RED,Data_Pos);
}

static unsigned char checksum(char *pData)
{
	int i;
	unsigned int CheckSum=0;
	for(i=1;i<6;i++)
	{
		CheckSum += pData[i];
	}
	return CheckSum;
}

void ptzpelcod_stop1(U8 ch)
{	
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x07;
	msg[4] = 0;
	msg[5] = 0x60;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_stop(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_irisopen(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0x02;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}
void ptzpelcod_irisclose(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0x04;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
	
}
void ptzpelcod_left(U8 ch,U8 speed)
{ 
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x04;
	msg[4] = speed;//PtzMenu[ch].Speed*8;
	msg[5] = 0;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));

}
void ptzpelcod_right(U8 ch,U8 speed)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x02;
	msg[4] = speed;//PtzMenu[ch].Speed*8;
	msg[5] = 0;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}
void ptzpelcod_down(U8 ch,U8 speed)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x10;
	msg[4] = 0;//PtzMenu[ch].Speed*8;
	msg[5] = speed;//PtzMenu[ch].Speed*8;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}
void ptzpelcod_up(U8 ch,U8 speed)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x08;
	msg[4] = 0;//PtzMenu[ch].Speed*8;
	msg[5] = speed;//PtzMenu[ch].Speed*8;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}
void ptzpelcod_far(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x80;
	msg[4] = 0;//PtzMenu[ch].Speed*8;
	msg[5] = 0;//PtzMenu[ch].Speed*8;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}
void ptzpelcod_near(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 1;
	msg[3] = 0;
	msg[4] = 0;//PtzMenu[ch].Speed*8;
	msg[5] = 0;//PtzMenu[ch].Speed*8;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}
void ptzpelcod_zoomout(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x40;
	msg[4] = 0;//PtzMenu[ch].Speed*8;
	msg[5] = 0;//PtzMenu[ch].Speed*8;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}
void ptzpelcod_zoomin(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x20;
	msg[4] = 0;//PtzMenu[ch].Speed*8;
	msg[5] = 0;//PtzMenu[ch].Speed*8;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_auto(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0x90;
	msg[3] = 0;
	msg[4] = 0;//PtzMenu[ch].Speed*8;
	msg[5] = 0;//PtzMenu[ch].Speed*8;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_auto1(U8 ch)
{
	char msg[7];

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x07;
	msg[4] = 0;
	msg[5] = 0x63;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_preset(U8 ch,U8 posion)
{
	char msg[7];
	if (posion==0)
		return;

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x03;
	msg[4] = 0;
	msg[5] = posion;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_preview(U8 ch,U8 posion)
{
	char msg[7];
	if (posion==0)
		return;

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x07;
	msg[4] = 0;
	msg[5] = posion;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_predel(U8 ch,U8 posion)
{
	char msg[7];
	if (posion==0)
		return;

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x05;
	msg[4] = 0;
	msg[5] = posion;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_groupsetstar(U8 ch,U8 track)
{
	char msg[7];
	if (track==0)
		return;
	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x47;
	msg[4] = 0;
	msg[5] = track-1;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_groupsetend(U8 ch)
{
	char msg[7];
	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x4b;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}


void ptzpelcod_groupset(U8 ch,U8 track,U8 point,U8 speed,U8 time)
{
	char msg[7];
	if (point==0 || track == 0 || speed == 0 || time ==0)
		return;

	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = point;
	msg[3] = 0x49;
	msg[4] = speed-1;
	msg[5] = time;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcod_rungroup(U8 ch,U8 posion)
{
	char msg[7];
	if (posion==0 || posion>6)
		return;
	msg[0] = 0xFF;
	msg[1] = PtzMenu[ch].DeviceID+1;
	msg[2] = 0;
	msg[3] = 0x4f;
	msg[4] = 1;
	msg[5] = posion-1;
	msg[6] = checksum(msg);
	uartsend(msg,sizeof(msg));
}


static unsigned char getchecksum(char *pData)
{
	unsigned int CheckSum=0;
	int i;
	
	for(i=0;i<7;i++)
	{
		CheckSum ^= pData[i];
	}
	return CheckSum;
}
void ptzpelcop_stop1(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x07;
	msg[4] = 0;
	msg[5] = 0x60;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}


void ptzpelcop_stop(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcop_left(U8 ch,U8 speed)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x04;
	msg[4] = speed;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_right(U8 ch,U8 speed)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x02;
	msg[4] = speed;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_up(U8 ch,U8 speed)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x02;
	msg[4] = 0;
	msg[5] = speed;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_down(U8 ch,U8 speed)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x10;
	msg[4] = 0;
	msg[5] = speed;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_near(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0x02;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_far(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0x01;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_zoomout(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x40;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_zoomin(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x20;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_irisopen(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0x04;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
};
void ptzpelcop_irisclose(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0x08;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcop_auto(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0x90;
	msg[3] = 0;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}


void ptzpelcop_auto1(U8 ch)
{
	char msg[8];
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x07;
	msg[4] = 0;
	msg[5] = 0x3f;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcop_preset(U8 ch,U8 posion)
{
	char msg[8];
	if (posion==0)
		return;
	
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x03;
	msg[4] = 0;
	msg[5] = posion;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcop_preview(U8 ch,U8 posion)
{
	char msg[8];
	if (posion==0)
		return;
	
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x07;
	msg[4] = 0;
	msg[5] = posion;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcop_predel(U8 ch,U8 posion)
{
	char msg[8];
	if (posion==0)
		return;
	
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x05;
	msg[4] = 0;
	msg[5] = posion;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcop_groupsetstar(U8 ch,U8 track)
{
	char msg[8];
	if (track==0)
		return;
	
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x47;
	msg[4] = 0;
	msg[5] = track-1;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}

void ptzpelcop_groupsetend(U8 ch)
{
	char msg[8];
	
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x4b;
	msg[4] = 0;
	msg[5] = 0;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}


void ptzpelcop_groupset(U8 ch,U8 track,U8 point,U8 speed,U8 time)
{
	char msg[8];
	if (point==0 || track == 0 || speed == 0 || time ==0)
		return;

	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = point;
	msg[3] = 0x49;
	msg[4] = speed-1;
	msg[5] = time;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}



void ptzpelcop_rungroup(U8 ch,U8 posion)
{
	char msg[8];
	if (posion==0 || posion>6)
		return;
	msg[0] = 0xA0;
	msg[1] = PtzMenu[ch].DeviceID;
	msg[2] = 0;
	msg[3] = 0x4f;
	msg[4] = 1;
	msg[5] = posion-1;
	msg[6] = 0xAF;
	msg[7] = getchecksum(msg);
	uartsend(msg,sizeof(msg));
}


void ptz_stop_func()
{
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_stop(Cur_CH);
	else if (PtzMenu[Cur_CH].Protocol==1 && ptz_auto_flag)
		ptzpelcod_stop1(Cur_CH);
	else
		ptzpelcod_stop(Cur_CH);
}


void NETPTZ_Control_Fun(U8 ch,U8 ptzspeed,U8 ptzdata)
{

	U8 tmp;
	static unsigned char  Longkey_Old=0xff;
	
//	DBGPRINT("Net ptz control err! ch:%d,ptzspeed:%d,ptzdata:%d\n",ch,ptzspeed,ptzdata);
	if (ch >= Cam_Num)
	{
		DBGPRINT("Net ptz control err! ch:%d,ptzspeed:%d,ptzdata:%d\n",ch,ptzspeed,ptzdata);
		return;
	}
	if(Longkey_Old!=LongKey)
	{
		Longkey_Old=LongKey;
		if(LongKey==0)
			byKey_val= 0x0c;
	}
//	tmp = Longkey_Old ^ LongKey;
//	Longkey_Old = LongKey;
//	if (tmp)
//	{
//		if (LongKey)
//		{
//			byKey_val=Key_Old;
//		}
//		else
//		{
//			Key_Old =0xffff;
//			byKey_val= 0x0c;
//		}
//	}
//	else if (byKey_val)
//		Key_Old=byKey_val;
	if (ptz_auto_flag)
	{
/*
		if (transe_count>2)
		{
//			DBGPRINT("B,transe_count = %d\n",transe_count);
			ptz_auto_func();
			transe_count = 0;
		}
		else
			transe_count++;
*/			
	}
	
	if(byKey_val==0) return;
	if (NetCam != ch)
	{
		NetCam = ch;
#ifdef UART		
	if (SysMenu.RemoteID>1)
		pstCommonParam->gst_ext_set_uart_pam1(SysMenu.RemoteID-1,Baud[PtzMenu[NetCam].BaudRate]);
	else
		pstCommonParam->gst_ext_set_uart_pam1(0,Baud[PtzMenu[NetCam].BaudRate]);
#endif
	}

// peng 2010-12-6 1:53:09
#ifdef UART	
	if(g_cur_baudrate!=Baud[PtzMenu[NetCam].BaudRate])
	{
		if (SysMenu.RemoteID>1)
			pstCommonParam->gst_ext_set_uart_pam1(SysMenu.RemoteID-1,Baud[PtzMenu[NetCam].BaudRate]);
		else
			pstCommonParam->gst_ext_set_uart_pam1(0,Baud[PtzMenu[NetCam].BaudRate]);
		g_cur_baudrate=Baud[PtzMenu[NetCam].BaudRate];
	}
#endif

	switch(byKey_val)
	{
		case kNUP:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_up(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_up(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
		case kNDN:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_down(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_down(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
		case kNLT:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_left(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_left(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
		case kNRH:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_right(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_right(ch,ptzspeed);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
		case 0x0c:

			if (PtzMenu[ch].Protocol==2)
				ptzpelcop_stop(ch);
			else if (PtzMenu[ch].Protocol==1 && ptz_auto_flag)
				ptzpelcod_stop1(ch);
			else
				ptzpelcod_stop(ch);
			ptz_auto_flag = 0;
/*			
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_stop(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_stop(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop1(ch);
				}
			}
*/
			break;
		case 0x0013:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_near(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_near(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
		case 0x0014:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_far(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_far(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
	   		break;
		case 0x0011:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_irisopen(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_irisopen(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
		case 0x0012:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_irisclose(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
	 			ptzpelcod_irisclose(ch); 
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
	   	case 0x0015:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_zoomout(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_zoomout(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
	   	case 0x0016:
			ptz_auto_flag = 0;
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_zoomin(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcop_stop(ch);
				}
			}
			else
			{
				ptzpelcod_zoomin(ch);
				if (!LongKey)
				{
					usleep(80000);
					ptzpelcod_stop(ch);
				}
			}
			break;
	   	case 0x0017:
			ptz_auto_flag = 1;
/*			
			if (PtzMenu[ch].Protocol==2)
			{
				ptzpelcop_auto(ch);
				usleep(80000);
				ptzpelcop_auto1(ch);
			}
			else
			{
				ptzpelcod_auto(ch);
				usleep(80000);
				ptzpelcod_auto1(ch);
			}
*/
			if (PtzMenu[ch].Protocol==0)
				ptzpelcod_auto(ch);
			else if (PtzMenu[ch].Protocol==1)
				ptzpelcod_auto1(ch);
			else
				ptzpelcop_auto(ch);
		
			break;
		case 0x0030:
			DBGPRINT("ch:%d,data:%d\n",ch,ptzdata);
			if (PtzMenu[ch].Protocol==2)
				ptzpelcop_preset(ch,ptzdata);
			else
				ptzpelcod_preset(ch,ptzdata);
			break;
		case 0x0031:
			DBGPRINT("ch:%d,data:%d\n",ch,ptzdata);
			if (PtzMenu[ch].Protocol==2)
				ptzpelcop_preview(ch,ptzdata);
			else
				ptzpelcod_preview(ch,ptzdata);
			break;
		default:
			break;
    }
	byKey_val=0;
}

void PTZ_Control_KeyFun()
{
}

void ptzmousestop()
{
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_stop(Cur_CH);
	else if (PtzMenu[Cur_CH].Protocol==1 && ptz_auto_flag)
		ptzpelcod_stop1(Cur_CH);
	else
		ptzpelcod_stop(Cur_CH);
}

void ptzupclick()
{
	ptz_auto_flag = 0;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_up(Cur_CH,PtzMenu[Cur_CH].Speed);
	else
		ptzpelcod_up(Cur_CH,PtzMenu[Cur_CH].Speed);
}

void ptzdownclick()
{
	ptz_auto_flag = 0;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_down(Cur_CH,PtzMenu[Cur_CH].Speed);
	else
		ptzpelcod_down(Cur_CH,PtzMenu[Cur_CH].Speed);
}

void ptzleftclick()
{
	ptz_auto_flag = 0;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_left(Cur_CH,PtzMenu[Cur_CH].Speed);
	else
		ptzpelcod_left(Cur_CH,PtzMenu[Cur_CH].Speed);
}

void ptzrightclick()
{
	ptz_auto_flag = 0;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_right(Cur_CH,PtzMenu[Cur_CH].Speed);
	else
		ptzpelcod_right(Cur_CH,PtzMenu[Cur_CH].Speed);
}

void ptzautoclick()
{
	if (ptz_auto_flag)
	{
		if (PtzMenu[Cur_CH].Protocol==2)
			ptzpelcop_stop(Cur_CH);
		else if (PtzMenu[Cur_CH].Protocol==1 && ptz_auto_flag)
			ptzpelcod_stop1(Cur_CH);
		else
			ptzpelcod_stop(Cur_CH);
		ptz_auto_flag = 0;
	}
	else
	{
		ptz_auto_flag = 1;
		if (PtzMenu[Cur_CH].Protocol==0)
			ptzpelcod_auto(Cur_CH);
		else if (PtzMenu[Cur_CH].Protocol==1)
			ptzpelcod_auto1(Cur_CH);
		else
			ptzpelcop_auto(Cur_CH);
	}
}

void ptzteleclick()
{
	ptz_auto_flag = 0;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_zoomin(Cur_CH);
	else
		ptzpelcod_zoomin(Cur_CH);
}

void ptzwideclick()
{
	ptz_auto_flag = 0;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_zoomout(Cur_CH);
	else
		ptzpelcod_zoomout(Cur_CH);
}

void ptzviewclick()
{
	printf("ch:%d,data:%d\n",Cur_CH,pointdata[1]);
	ptz_auto_flag = 1;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_preview(Cur_CH,pointdata[1]);
	else
		ptzpelcod_preview(Cur_CH,pointdata[1]);
}

void ptzpresetclick()
{
	printf("ch:%d,data:%d\n",Cur_CH,pointdata[1]);
	ptz_auto_flag = 1;
	if (PtzMenu[Cur_CH].Protocol==2)
		ptzpelcop_preset(Cur_CH,pointdata[1]);
	else
		ptzpelcod_preset(Cur_CH,pointdata[1]);
}

void ptzmenuexitclick()
{
	ptzmenu_mouse_off();
	PTZ_Control_Off();
}

void ptzLclick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Preptzmousepos[Data_Pos*4]+offsetx_hd;
		maxx = Preptzmousepos[Data_Pos*4+2]+offsetx_hd;
		miny= Preptzmousepos[Data_Pos*4+1]+offsety_hd;
		maxy = Preptzmousepos[Data_Pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Preptzmousepos[Data_Pos*4]+offsetx;
		maxx = Preptzmousepos[Data_Pos*4+2]+offsetx;
		miny= Preptzmousepos[Data_Pos*4+1]+offsety;
		maxy = Preptzmousepos[Data_Pos*4+3]+offsety;
	}
	if (x>minx && x<maxx && y>miny && y<maxy)
	{
		ptz_auto_flag = 1;
		Ptz_Data_Dec(Data_Pos);
	}
}

void ptzRclick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Preptzmousepos[Data_Pos*4]+offsetx_hd;
		maxx = Preptzmousepos[Data_Pos*4+2]+offsetx_hd;
		miny= Preptzmousepos[Data_Pos*4+1]+offsety_hd;
		maxy = Preptzmousepos[Data_Pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Preptzmousepos[Data_Pos*4]+offsetx;
		maxx = Preptzmousepos[Data_Pos*4+2]+offsetx;
		miny= Preptzmousepos[Data_Pos*4+1]+offsety;
		maxy = Preptzmousepos[Data_Pos*4+3]+offsety;
	}
	if (x>minx && x<maxx && y>miny && y<maxy)
	{
		ptz_auto_flag = 1;
		Ptz_Data_Add(Data_Pos);
	}
}

void ptzmousekeyfun(U16 x, U16 y, U8 type)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	switch(type)
	{
		case 0:
			if (MseType)
			{
				minx = 0;
				maxx = HIGH_RESOLUTION_HD-MouseMenuHight*2;
				miny= WIDTH_RESOLUTION_HD;
				maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
			}
			else
			{
				minx = 0;
				maxx = High_Resolution-MouseMenuHight*2;
				miny= WIDTH_RESOLUTION;
				maxy = High_Resolution-MouseMenuHight;
			}
			if((x > minx && y > maxx) && (x < miny && y < maxy))
				menu_num = mMousePTZ;
			else
			{
				for (i=0;i<3;i++)
				{
					if (MseType)
					{
						minx = Preptzmousepos[i*4]+offsetx_hd;
						maxx = Preptzmousepos[i*4+2]+offsetx_hd;
						miny= Preptzmousepos[i*4+1]+offsety_hd;
						maxy = Preptzmousepos[i*4+3]+offsety_hd;
					}
					else
					{
						minx = Preptzmousepos[i*4]+offsetx;
						maxx = Preptzmousepos[i*4+2]+offsetx;
						miny= Preptzmousepos[i*4+1]+offsety;
						maxy = Preptzmousepos[i*4+3]+offsety;
					}
					if (x>minx && x<maxx && y>miny && y<maxy && (Data_Pos!= i))
					{
						Ptz_Menu_Disp(WHITE,Data_Pos);
						Data_Pos=i;
						Ptz_Menu_Disp(RED,Data_Pos);
						break;
					}
				}
			}
			break;
		case 26:
		case 31:
			ptzLclick(x,y);
			break;
		case 25:
		case 30:
			ptzRclick(x,y);
			break;
		default:
			break;
	}
}



