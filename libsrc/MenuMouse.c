#include "cmn.h"


MOUSEMENU mousemenupic;
U16 mouse_posx,mouse_posy;
U8 MenuStatus = 0;
U8 MOUSEMAINMENUNUM;

void mouse_list_show(U8 pos,U8 color)
{
	U8 index = 0xff;
//	BXS_PIC1* picbuf=NULL;
	
	switch(menu_num)
	{
		case mMouse:
			switch(pos)
			{
				case 1:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.quadpicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.quadpicfore,HD);
						index = 0;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.quadpicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.quadpicback,HD);
					}
					break;
				case 2:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.recordpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.recordpicfore,HD);
								index = 3;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.recordpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.recordpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8 || Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ninepicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ninepicfore,HD);
								index = 1;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ninepicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ninepicback,HD);
							}
						}
					}
					else
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpausepicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpausepicfore,HD);
								index = 11;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpausepicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpausepicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8 || Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ninepicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ninepicfore,HD);
								index = 1;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ninepicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ninepicback,HD);
							}
						}
					}
					break;
				case 3:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpicfore,HD);
								index = 4;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.recordpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.recordpicfore,HD);
								index = 3;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.recordpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.recordpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.sixtypicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.sixtypicfore,HD);
								index = 2;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.sixtypicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.sixtypicback,HD);
							}
						}
					}
					else
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicfore,HD);
								index = 5;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpausepicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpausepicfore,HD);
								index = 11;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpausepicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpausepicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.sixtypicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.sixtypicfore,HD);
								index = 2;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.sixtypicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.sixtypicback,HD);
							}
						}
					}
					break;
				case 4:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicfore,HD);
								index = 5;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpicfore,HD);
								index = 4;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.recordpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.recordpicfore,HD);
								index = 3;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.recordpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.recordpicback,HD);
							}
						}
					}
					else
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.backpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.backpicfore,HD);
								index = 12;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.backpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.backpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicfore,HD);
								index = 5;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpausepicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpausepicfore,HD);
								index = 11;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpausepicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpausepicback,HD);
							}
						}
					}
					break;
				case 5:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menupicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menupicfore,HD);
								index = 6;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menupicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menupicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicfore,HD);
								index = 5;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpicfore,HD);
								index = 4;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playpicback,HD);
							}
						}
					}
					else
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.fastpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.fastpicfore,HD);
								index = 13;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.fastpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.fastpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.backpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.backpicfore,HD);
								index = 12;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.backpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.backpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicfore,HD);
								index = 5;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicback,HD);
							}
						}
					}
					break;
				case 6:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpicfore,HD);
								index = 7;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menupicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menupicfore,HD);
								index = 6;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menupicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menupicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicfore,HD);
								index = 5;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.stoppicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.stoppicback,HD);
							}
						}
					}
					else
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playexitfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playexitfore,HD);
								index = 23;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playexitback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playexitback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.fastpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.fastpicfore,HD);
								index = 13;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.fastpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.fastpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.backpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.backpicfore,HD);
								index = 12;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.backpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.backpicback,HD);
							}
						}
					}
					break;
				case 7:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.seqpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.seqpicfore,HD);
								index = 8;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.seqpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.seqpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpicfore,HD);
								index = 7;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menupicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menupicfore,HD);
								index = 6;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menupicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menupicback,HD);
							}
						}
					}
					else
					{
						if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playexitfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playexitfore,HD);
								index = 23;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playexitback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playexitback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.fastpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.fastpicfore,HD);
								index = 13;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.fastpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.fastpicback,HD);
							}
						}
					}
					break;
				case 8:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.spotfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.spotfore,HD);
								index = 9;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.spotback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.spotback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.seqpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.seqpicfore,HD);
								index = 8;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.seqpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.seqpicback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpicfore,HD);
								index = 7;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpicback,HD);
							}
						}
					}
					break;
				case 9:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.powerfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.powerfore,HD);
								index = 10;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.powerback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.powerback,HD);
							}
						}
						else if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.spotfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.spotfore,HD);
								index = 9;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.spotback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.spotback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.seqpicfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.seqpicfore,HD);
								index = 8;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.seqpicback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.seqpicback,HD);
							}
						}
					}
					else
					{
						if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playexitfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playexitfore,HD);
								index = 23;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.playexitback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.playexitback,HD);
							}
						}
					}
					break;
				case 10:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL4)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menuexitfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menuexitfore,HD);
								index = 23;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menuexitback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menuexitback,HD);
							}
						}
						if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.powerfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.powerfore,HD);
								index = 10;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.powerback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.powerback,HD);
							}
						}
						else if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.spotfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.spotfore,HD);
								index = 9;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.spotback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.spotback,HD);
							}
						}
					}
					break;
				case 11:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL8)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menuexitfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menuexitfore,HD);
								index = 23;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menuexitback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menuexitback,HD);
							}
						}
						if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.powerfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.powerfore,HD);
								index = 10;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.powerback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.powerback,HD);
							}
						}
					}
					break;
				case 12:
					if (demo_num<PBFULL)
					{
						if (Cam_Num==CHANNEL16)
						{
							if (color==RED)
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menuexitfore,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menuexitfore,HD);
								index = 23;
							}
							else
							{
								Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.menuexitback,AD);
								Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.menuexitback,HD);
							}
						}
					}
					break;
				default:
					break;
			}
			break;
		case mMousePTZ:
			switch(pos)
			{
				case 1:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzuppicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzuppicfore,HD);
						index = 14;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzuppicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzuppicback,HD);
					}
					break;
				case 2:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzdownpicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzdownpicfore,HD);
						index = 15;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzdownpicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzdownpicback,HD);
					}
					break;
				case 3:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzleftpicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzleftpicfore,HD);
						index = 16;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzleftpicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzleftpicback,HD);
					}
					break;
				case 4:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzrightpicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzrightpicfore,HD);
						index = 17;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzrightpicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzrightpicback,HD);
					}
					break;
				case 5:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzautopicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzautopicfore,HD);
						index = 18;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzautopicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzautopicback,HD);
					}
					break;
				case 6:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptztelepicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptztelepicfore,HD);
						index = 19;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptztelepicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptztelepicback,HD);
					}
					break;
				case 7:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzwidepicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzwidepicfore,HD);
						index = 20;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzwidepicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzwidepicback,HD);
					}
					break;
				case 8:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzviewpicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzviewpicfore,HD);
						index = 21;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzviewpicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzviewpicback,HD);
					}
					break;
				case 9:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpresetpicfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpresetpicfore,HD);
						index = 22;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzpresetpicback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzpresetpicback,HD);
					}
					break;
				case 10:
					if (color==RED)
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzmenuexitfore,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzmenuexitfore,HD);
						index = 23;
					}
					else
					{
						Bmp_All_Show(pos*MouseMenuWidth, High_Resolution-MouseMenuHight*2, mousemenupic.ptzmenuexitback,AD);
						Bmp_All_Show(pos*MouseMenuWidth, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzmenuexitback,HD);
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	if (index != 0xff)
	{
		infoup(pos*MouseMenuWidth+20, High_Resolution-MouseMenuHight*2-20,index);
		infoshowhide(1);
	}
	else 
		infoshowhide(0);
}

U8 demo_tmp;

void Menu_Mouse_On()
{
	if (demo_num<PBFULL)
	{
		demo_tmp = demo_num;
		if (Cam_Num==CHANNEL4)
		{
			MOUSEMAINMENUNUM = 10+1;
			Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.mainmenuallpic4,AD);
			Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.mainmenuallpic4,HD);
		}
		else if (Cam_Num==CHANNEL8)
		{
			MOUSEMAINMENUNUM = 11+1;
			Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.mainmenuallpic8,AD);
			Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.mainmenuallpic8,HD);
		}
		else if (Cam_Num==CHANNEL16)
		{
			MOUSEMAINMENUNUM = 12+1;
			Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.mainmenuallpic16,AD);
			Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.mainmenuallpic16,HD);
		}
	}
	else
	{
		if (Cam_Num==CHANNEL4)
		{
			MOUSEMAINMENUNUM = 7;
			Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.mainmenuplaypic4,AD);
			Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.mainmenuplaypic4,HD);
		}
		else if (Cam_Num==CHANNEL8)
		{
			MOUSEMAINMENUNUM = 8;
			Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.mainmenuplaypic8,AD);
			Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.mainmenuplaypic8,HD);
		}
		else if (Cam_Num==CHANNEL16)
		{
			if (ChipSelect == HI3515)
			{
				MOUSEMAINMENUNUM = 8;
				Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.mainmenuplaypic16,AD);
				Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.mainmenuplaypic16,HD);
			}
			else
			{
				MOUSEMAINMENUNUM = 9;
				Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.mainmenuplaypic16_20,AD);
				Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.mainmenuplaypic16_20,HD);
			}
		}
	}
	if ((MenuStatus != mMouse) && (MenuStatus != (Iconmenu+mMouse)))
		MenuStatus += mMouse;
//	menu_num = mMouse;
	cur_pos=0xff;
}


void menu_mouse_off()
{
}

void Stop_Click()
{
	if ((!SystemLock) && (!rec_key))
	{
		if (demo_num > L16)
		{
//			menu_mouse_off();
			Play_Stop();
			if (!SystemLock && !CheckUser(menupass[curuser].Playcfig))
			{
				menu_time_back_on();
			}
		}
		else if (rec_on)
		{
			if (CheckUser(menupass[curuser].Stopcfig) == 0)
			{
				InfoId = StopRecord;
				MenuErrOn();
			}
		}
	}
}

void Fast_Click()
{
	if (!SystemLock && demo_num > L16)
	{
//		menu_mouse_off();
		Play_Fast();
	}
}

void Back_Click()
{
	if (!SystemLock && demo_num > L16)
	{
//		menu_mouse_off();
		Play_Back();
	}
}

void Play_Click()
{
/*
	if ((!SystemLock) && (!rec_key))
	{
		menu_mouse_off();
		if (demo_num < PBFULL)
			Play_Start();
		else
			Rplay_Start();
//		menu_num = 0;
	}
*/
	if (!SystemLock && Disk_Num)
	{
		if (demo_num < PBFULL)
			menu_time_back_on();
		else
			Rplay_Start();
	}

}

void Rec_Click()
{
	if (!SystemLock && Disk_Num && !rec_on && demo_num < PBFULL)
	{
//		menu_num = 0;
//		OSD_ALL_CLEAR();
		menu_mouse_off();
		record_key_set();
	}
}

void Menu_Click()
{
	if (!SystemLock && demo_num < PBFULL)
	{
		menu_main_on();
	}
}

void PTZ_Click()
{
	if (!SystemLock && demo_num < PBFULL)
	{
		menu_num = 0;
//		OSD_ALL_CLEAR();
//		demo_osd_disp();
		menu_mouse_off();
		Icon_Menu_Off();
		PTZ_Control_ON();
		
	}
}

void Seq_Click()
{
	printf(" Seq_Click\n");
	if (!SystemLock && demo_num < PBFULL)
	{
		menu_num = 0;
		menu_mouse_off();
		Icon_Menu_Off();
		auto_func();
	}
}

void SPower_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL8)
	{	
		menu_mouse_off();
	}
	else if (Cam_Num == CHANNEL16)
	{
		if (curuser==0)
		{
			buzzer_set(0);
			Menu_Power_On();
		}
		else
		{
			InfoId = PermissonErr;
			MenuErrOn();
		}
	}
}

void NPower_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
	{	
		menu_mouse_off();
	}
	else if (Cam_Num == CHANNEL8)
	{	
		if (curuser==0)
		{
			buzzer_set(0);
			Menu_Power_On();
		}
		else
		{
			InfoId = PermissonErr;
			MenuErrOn();
		}
	}
	else if (Cam_Num == CHANNEL16)
		menu_spot_on();
}
void C16Power_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL16)
	{	
		menu_mouse_off();
	}
}
void Power_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
	{	
		if (curuser==0)
		{
			buzzer_set(0);
			Menu_Power_On();
		}
		else
		{
			InfoId = PermissonErr;
			MenuErrOn();
		}
	}
	else if (Cam_Num == CHANNEL8)
		menu_spot_on();
	else if (Cam_Num == CHANNEL16)
		Seq_Click();
}

void XorSpot_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
		menu_spot_on();
	else if (Cam_Num == CHANNEL8)
		Seq_Click();
	else if (Cam_Num == CHANNEL16)
	{
		if (demo_num>L16 && ChipSelect == HI3520)
			menu_mouse_off();
		else
			PTZ_Click();
	}
}

void XorSeq_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
	{
		Seq_Click();
	}
	else if (Cam_Num == CHANNEL8)
	{
		if (demo_num>L16)
			menu_mouse_off();
		else
			PTZ_Click();
	}
	else if (Cam_Num == CHANNEL16)
	{
		if (demo_num>L16)
		{
			if (ChipSelect == HI3515)
				menu_mouse_off();
			else
				Fast_Click();
		}
		else
			Menu_Click();
	}
}

void XorPTZ_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
	{
		if (demo_num>L16)
			menu_mouse_off();
		else
			PTZ_Click();
	}
	else if (Cam_Num == CHANNEL8)
	{
		if (demo_num>L16)
			Fast_Click();
		else
			Menu_Click();
	}
	else if (Cam_Num == CHANNEL16)
	{
		if (demo_num>L16)
		{
			if (ChipSelect == HI3515)
				Fast_Click();
			else
				Back_Click();
		}
		else
			Stop_Click();
	}
}

void XorMenu_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
	{
		if (demo_num>L16)
			Fast_Click();
		else
			Menu_Click();
	}
	else if (Cam_Num == CHANNEL8)
	{
		if (demo_num>L16)
			Back_Click();
		else
			Stop_Click();
	}
	else if (Cam_Num == CHANNEL16)
	{
		if (demo_num>L16)
		{
			if (ChipSelect==HI3515)
				Back_Click();
			else
				Stop_Click();
		}
		else
			Play_Click();
	}
}

void XorRec_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
	{
		if (demo_num>L16)
			Back_Click();
		else
			Stop_Click();
	}
	else if (Cam_Num == CHANNEL8)
	{
		if (demo_num>L16)
			Stop_Click();
		else
			Play_Click();
	}
	else if (Cam_Num == CHANNEL16)
	{
		if (demo_num>L16)
		{
			if (ChipSelect == HI3515)
				Stop_Click();
			else
				Play_Click();
		}
		else
			Rec_Click();
	}
}

void Sixty_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num == CHANNEL4)
	{
		if (demo_num>L16)
			Stop_Click();
		else
			Play_Click();
	}
	else if (Cam_Num == CHANNEL8)
	{
		if (demo_num>L16)
			Play_Click();
		else
			Rec_Click();
	}
	else if (Cam_Num == CHANNEL16)
	{
		if (demo_num<L16)
		{
			menu_num = 0;
			OSD_ALL_CLEAR();
			demo_page = 0;
			Demo_num_key_pro(L16,demo_page);
		}
		else if (demo_num>L16)
		{
			if (ChipSelect == HI3515)
				Play_Click();
			else
			{
				if (16!=Play_Mode)
				{
					OSD_ALL_CLEAR();
					Play_Mode = 16;
					Split_Play(Play_Mode,1,0);
				}
			}
		}
	}
}

void Nine_Click()
{
	if (SystemLock)
		return;
	if (Cam_Num > CHANNEL4)
	{
		if (demo_num>L16)
		{
			menu_num = 0;
			if (Cam_Num>CHANNEL8)
			{
				OSD_ALL_CLEAR();
				if (8!=(Play_Mode/10))
					Play_Mode = 80;
				else if(Play_Mode<82)
				{
					Play_Mode++;
					if (Play_Mode>=82)
						Play_Mode = 80;
				}
				Split_Play(Play_Mode,1,0);
			}
			else if (Cam_Num==CHANNEL8)
			{
				if (8!=(Play_Mode/10))
				{
					OSD_ALL_CLEAR();
					Play_Mode = 80;
					Split_Play(Play_Mode,1,0);
				}
			}
//			if(play_menu_display_flag==0&&menu_num==0)
//				play_menu_mouse_display();
/*
			if (8!=(Play_Mode/10))
				Play_Mode = 80;
			else
			{
				Play_Mode++;
				if (Play_Mode>=(80+Cam_Num/8))
					Play_Mode = 80;
			}
			Split_Play(Play_Mode,1,0);
*/
		}
		else if (Cam_Num > 8 || demo_num!=L8)
		{
			menu_num = 0;
			OSD_ALL_CLEAR();
			if(demo_num!=L8) 
				demo_page=0;
			else
			{
				demo_page++;
				if(demo_page>1) demo_page=0;
			}
			Demo_num_key_pro(L8,demo_page);
		}
	}
	else
	{
		if (demo_num>L16)
			Play_Click();	
		else
			Rec_Click();
	}
}

void Quad_Click()
{
	if (SystemLock)
		return;
	if (demo_num>L16)
	{
		menu_num = 0;
		if (Cam_Num>CHANNEL4)
		{
			OSD_ALL_CLEAR();
			if (4!=(Play_Mode/10))
				Play_Mode = 40;
			else if(Play_Mode<45)
			{
				Play_Mode++;
				if (Play_Mode>=(40+Cam_Num/4))
					Play_Mode = 40;
			}
			Split_Play(Play_Mode,1,0);
		}
		else if (Cam_Num==CHANNEL4)
		{
			if (4!=(Play_Mode/10))
			{
				OSD_ALL_CLEAR();
				Play_Mode = 40;
				Split_Play(Play_Mode,1,0);
			}
		}
/*		
		if (Play_Mode == 45)
		{}
		else if (4!=(Play_Mode/10))
			Play_Mode = 40;
		else
		{
			Play_Mode++;
			if (Play_Mode>=(40+Cam_Num/4))
				Play_Mode = 40;
		}
		Split_Play(Play_Mode,1,0);
*/
	}
	else if (Cam_Num>4 || demo_num!=L4)
	{
		menu_num = 0;
		OSD_ALL_CLEAR();
		if(demo_num!=L4) 
			demo_page=0;
		else
		{
			demo_page++;
			if(demo_page>3) demo_page=0;
		}
		Demo_num_key_pro(L4,demo_page);
	}

//	if (demo_num < PBFULL)
//	MouseQuadMenuShow();
//	else
//		Split_Play();

}

typedef void (FUNC)();
FUNC *fun[]=
{
	Quad_Click,
	Nine_Click,
	Sixty_Click,
	XorRec_Click,
	XorMenu_Click,
	XorPTZ_Click,
	XorSeq_Click,
	XorSpot_Click,
	Power_Click,
	NPower_Click,
	SPower_Click,
	C16Power_Click
} ;


void MouseMainLClick(U16 x,U16 y)
{
	U16 miny,maxy;
	
	if (MseType)
	{
		miny = HIGH_RESOLUTION_HD-MouseMenuHight*2;
		maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
	}
	else
	{
		miny = High_Resolution-MouseMenuHight*2;
		maxy = High_Resolution-MouseMenuHight;
	}
	if (x>MouseMenuWidth*cur_pos && x<MouseMenuWidth*(cur_pos+1) && y > miny && y < maxy)
	{
		if (cur_pos && cur_pos<MOUSEMAINMENUNUM)
		{
			infoshowhide(0);
			fun[cur_pos-1]();
		}
	}
}

void Mouse_LiveMenu_Fun(U16 x,U16 y)
{
	unsigned char i;
		
	U16 maxy,miny;
	if (MseType)
	{
		maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
		miny = HIGH_RESOLUTION_HD-MouseMenuHight*2;
	}
	else
	{
		maxy = High_Resolution-MouseMenuHight;
		miny = High_Resolution-MouseMenuHight*2;
	}
	if(y < miny || y > maxy)
	{
		menu_num = 0;
		infoshowhide(0);
	}
	else
	{
		for (i=1;i<MOUSEMAINMENUNUM;i++)
		{
			if (x > MouseMenuWidth*i && x < MouseMenuWidth*(i+1) && cur_pos != i)
			{
				mouse_list_show(cur_pos,WHITE);
//				peng 2010-11-12 10:26:48
				if(demo_num==PB9&&Cam_Num==CHANNEL4&&i==1&&RecamMenu[0].Reselution)
					break;
					
				cur_pos=i;
				mouse_list_show(cur_pos,RED);
				break;
			}
		}
	}
}


void PtzMenu_Mouse_On()
{
	menu_num = mMousePTZ;
	cur_pos=0;
	Bmp_All_Show(0, High_Resolution-MouseMenuHight*2, mousemenupic.ptzmenuallpic,AD);
	Bmp_All_Show(0, HIGH_RESOLUTION_HD-MouseMenuHight*2, mousemenupic.ptzmenuallpic,HD);
}
extern U8 ptz_auto_flag;
void ptzmenu_mouse_off()
{
}

void PTZMouseMainLClick(U16 x,U16 y)
{
	U16 maxy,miny;
	if (MseType)
	{
		maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
		miny = HIGH_RESOLUTION_HD-MouseMenuHight*2;
	}
	else
	{
		maxy = High_Resolution-MouseMenuHight;
		miny = High_Resolution-MouseMenuHight*2;
	}
	if (x>MouseMenuWidth*cur_pos && x<MouseMenuWidth*(cur_pos+1) && y > miny && y < maxy)
	{
		switch(cur_pos)
		{
			case 1:
				ptzupclick();
				break;
			case 2:
				ptzdownclick();
				break;
			case 3:
				ptzleftclick();
				break;
			case 4:
				ptzrightclick();
				break;
			case 5:
				ptzautoclick();
				break;
			case 6:
				ptzteleclick();
				break;
			case 7:
				ptzwideclick();
				break;
			case 8:
				ptzviewclick();
				break;
			case 9:
				ptzpresetclick();
				break;
		}
	}
}

void PTZMouseMainLUClick(U16 x,U16 y)
{
	U16 maxy,miny;
	if (MseType)
	{
		maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
		miny = HIGH_RESOLUTION_HD-MouseMenuHight*2;
	}
	else
	{
		maxy = High_Resolution-MouseMenuHight;
		miny = High_Resolution-MouseMenuHight*2;
	}
	if (x>MouseMenuWidth*cur_pos && x<MouseMenuWidth*(cur_pos+1) && y > miny && y < maxy)
	{
		switch(cur_pos)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 6:
			case 7:
				ptzmousestop();
				break;
			case 10:
				ptzmenuexitclick();
				break;
		}
	}
}
extern U8 infoshowflag;
void Mouse_PTZMenu_Fun(U16 x,U16 y)
{
	unsigned char i;
	U16 maxy,miny;
	if (MseType)
	{
		maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
		miny = HIGH_RESOLUTION_HD-MouseMenuHight*2;
	}
	else
	{
		maxy = High_Resolution-MouseMenuHight;
		miny = High_Resolution-MouseMenuHight*2;
	}
	if(y < miny || y > maxy)
	{
		menu_num = mPTZControl;
		infoshowhide(0);
	}
	else
	{
		for (i=1;i<MOUSEPTZMENUNUN;i++)
		{
			if (x > MouseMenuWidth*i && x < MouseMenuWidth*(i+1) && cur_pos != i)
			{
				mouse_list_show(cur_pos,WHITE);
				cur_pos=i;
				mouse_list_show(cur_pos,RED);
				break;
			}
		}
	}
}

void Icon_disp(U8 pos,U8 color)
{
}

U8 checkpos(U16 x,U16 y)
{
	return 0;
}

void Icon_Menu_Off()
{
}

void Icon_Menu_On(U16 x,U16 y)
{
}

int Icon_LClick_Fun(U16 x,U16 y)
{
	U8 tmp = checkpos(x, y);
	infoshowhide(0);
	switch (tmp)
	{
		case 0:
			if (!SystemLock)
				Icon_disp(tmp,RED);
			System_Lock();
			break;
		case 1:
			if (!SystemLock && demo_num < PBFULL)
			{
				if (!SysMenu.RemoteID)
				{
					if (remo_Led)
					{
						LedControl(LEDIR,0);
					}
					else
					{
						SysMenu.RemoteID = 1;
						LedControl(LEDIR,1);
					}
				}
				else
				{
					if (remo_Led)
					{
						LedControl(LEDIR,0);
					}
					else
					{
						LedControl(LEDIR,1);
					}
				}
				Icon_disp(tmp,RED);
				remote_state_save();
			}
			break;
		case 2:
			if (!SystemLock && demo_num < PBFULL)
			{
				Icon_disp(tmp,WHITE);
				audio_ch_on();
			}
			break;
		case 3:
			if (!SystemLock&& demo_num < PBFULL)
			{
				Icon_disp(tmp,WHITE);
				Hdd_Info_Show();
			}
			break;
		case 4:
			if (!SystemLock)
			{
				Icon_Menu_Off();
			 }
			break;
		default:
			break;
	}
	return 1;
}

void Icon_Move_Fun(U16 x,U16 y)
{
	U8 tmp = checkpos(x, y);
	if (tmp>4)
	{
		menu_num = 0;
		cur_pos = 0;
		infoshowhide(0);
		return;
	}
	if (cur_pos != tmp)
	{
		Icon_disp(cur_pos,WHITE);
		cur_pos = tmp;
		Icon_disp(cur_pos,RED);
	}
}

