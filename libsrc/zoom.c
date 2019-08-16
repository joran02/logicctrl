#include "cmn.h"

U8 zoom_flag;


typedef enum
{
	CH01,
	CH02,
	CH03,
	CH04
} ZoomArea;

U8 Zoom_Area_Pos_PAL[]=
{
	0x23,0x2e,
	0x01,0x00,		0x5a,0x00,
	0x01,0x48,		0x5a,0x48
};

U8 Zoom_Area_Pos_NTSC[]=
{
	0x2d,0x25,
	0x00,0x00,		0x5a,0x00,
	0x00,0x3a,		0x5a,0x3a
};

U8* Zoom_Area_Pos[]=
{
	Zoom_Area_Pos_NTSC,
	Zoom_Area_Pos_PAL
};
void Area_Zoom_Func(U8 pos)
{
	zoom_flag = 1;
}

void Zoom_Off_Func()
{

	zoom_flag = 0;
}
void ZoomFun()
{
	if (zoom_flag)
		Area_Zoom_Func(0);
	else
		Zoom_Off_Func();

}


