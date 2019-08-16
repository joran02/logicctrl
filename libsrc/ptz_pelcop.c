#include "ptz.h"

static unsigned char peclop_getchecksum(unsigned char *pData)
{
	unsigned char CheckSum=0;
	unsigned char i;
	
	for(i=0;i<7;i++)
	{
		CheckSum ^= pData[i];
	}
	return CheckSum;
}

unsigned char lib_ptzpelcop_stop()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_stop1()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x60;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_left()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x04;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_right()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x02;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_up()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x08;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_down()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x10;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_near()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x02;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_up_left()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x08|0x04;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_up_right()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x08|0x02;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_down_left()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x10|0x04;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_down_right()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x10|0x02;
	bySbuf_232[4] = ptz_h_speed;
	bySbuf_232[5] = ptz_v_speed;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_far()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x01;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_zoomout()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x40;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_zoomin()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x20;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_irisopen()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x04;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_irisclose()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x08;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_preset()
{
	if (ptz_point==0)
		return 0;
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x03;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_point;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);
	
	return 8;
	
}

unsigned char lib_ptzpelcop_preview()
{
	if (ptz_point==0)
		return 0;
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_point;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);
	
	return 8;
}

unsigned char lib_ptzpelcop_predel()
{
	if (ptz_point==0)
		return 0;
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x05;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = ptz_point;
	bySbuf_232[6] = 0xAF;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);
	
	return 8;
}

unsigned char lib_ptzpelcop_auto()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0x90;
	bySbuf_232[3] = 0;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x00;
	bySbuf_232[6] = 0xaf;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_auto1()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x3f;
	bySbuf_232[6] = 0xaf;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_right_limit()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x5d;
	bySbuf_232[6] = 0xaf;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}

unsigned char lib_ptzpelcop_left_limit()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x5c;
	bySbuf_232[6] = 0xaf;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_run_group()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x4c;
	bySbuf_232[6] = 0xaf;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
unsigned char lib_ptzpelcop_stop_group()
{
	bySbuf_232[0] = 0xA0;
	bySbuf_232[1] = rs232_device_id;//rs232_device_id-1;
	bySbuf_232[2] = 0;
	bySbuf_232[3] = 0x07;
	bySbuf_232[4] = 0;
	bySbuf_232[5] = 0x60;
	bySbuf_232[6] = 0xaf;
	bySbuf_232[7] = peclop_getchecksum(bySbuf_232);

	return 8;
}
