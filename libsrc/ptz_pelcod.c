#include "ptz.h"

static unsigned char pelcod_checksum(unsigned char *pData)
{
	unsigned char i;
	unsigned char pelcod_checksum=0;
	for(i=1;i<6;i++)
	{
		pelcod_checksum += pData[i];
	}
	return (pelcod_checksum);
}

unsigned char lib_ptzpelcod_stop()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_stop1()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x60;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_irisopen()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x02;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_irisclose()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x04;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
	
}

unsigned char lib_ptzpelcod_left()
{
	bySbuf_232[0] = 0xFF; 
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x04;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_right()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x02;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_down()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x10;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_up()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x08;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_far()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x00;
	bySbuf_232[3] = 0x80;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_near()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x01;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_zoomin()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x40;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_zoomout()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x20;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_auto1()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x63;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}
unsigned char lib_ptzpelcod_up_left()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x08|0x04;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}
unsigned char lib_ptzpelcod_up_right()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x08|0x02;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}
unsigned char lib_ptzpelcod_down_left()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x10|0x04;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}
unsigned char lib_ptzpelcod_down_right()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x10|0x02;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}
unsigned char lib_ptzpelcod_preset()
{
	if (ptz_point==0)
		return 0;
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x03;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_point;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_preview()
{
	if (ptz_point==0)
		return 0;
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_point;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}

unsigned char lib_ptzpelcod_predel()
{
	if (ptz_point==0)
		return 0;
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x05;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_point;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}
unsigned char lib_ptzpelcod_auto()
{
	bySbuf_232[0] = 0xFF;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x90;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = pelcod_checksum(bySbuf_232);
	
	return 7;
}
