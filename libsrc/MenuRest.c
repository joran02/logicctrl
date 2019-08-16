#include "cmn.h"

#define RESETMAXMENU 2


void menu_reset_curdata_disp(U8 pos,U16 color)
{

}

void menu_reset_on()
{
	 U8 i;

	menu_num=mRESET;
 	OSD_ALL_CLEAR();

	cur_pos=1;
	for(i=0;i<RESETMAXMENU;i++)
	{
		if(i!=cur_pos)menu_reset_curdata_disp(i,WHITE);
		else menu_reset_curdata_disp(cur_pos,RED);
	}
}

void menu_reset_up()
{
	menu_reset_curdata_disp(cur_pos,WHITE);
	cur_pos=1-cur_pos;
	menu_reset_curdata_disp(cur_pos,RED);
}

void menu_reset_off()
{
	OSD_ALL_CLEAR();
	menu_main_on();
}

void menu_reset_enter()
{
//	 U8 temp;
	switch(cur_pos)
	{
	case 0:
//		sys_command("mount -t jffs2 /dev/mtdblock1 /mnt/nfs");
//		remove("/mnt/nfs/menu.data");
//		remove("/mnt/nfs/alarmfile.data");
//		remove("/mnt/nfs/schfile.data");
//		remove("/mnt/nfs/mac_num");
//		sys_command("umount /mnt/nfs");
		bReset = 1;
		break;
	case 1:
		menu_reset_off();
		break;
	}
}

//******************************key func*************************************
int Pre_retpos[] = 
{
	110,154,		148,164,
	195,154,		238,164
};

int Pre_ret_Posy;

void Mouse_Ret_Move_Func()
{
	U8 i;
	if (Pre_ret_Posy != (mouse_posy+mouse_posx))
	for (i=0;i<RESETMAXMENU;i++)
	{
		if ((mouse_posx>=Pre_retpos[i*4]) && (mouse_posy>=Pre_retpos[i*4+1]) && (mouse_posx<=Pre_retpos[i*4+2]) && (mouse_posy<=Pre_retpos[i*4+3]) && (cur_pos != i))
		{
//		printf("posx:%d,posy:%d,minx:%d,miny:%d,maxx:%d,maxy:%d\n",mouse_posx,mouse_posy,Pre_retpos[i*4],Pre_retpos[i*4+1],Pre_retpos[i*4+2],Pre_retpos[i*4+3]);
//			printf("************* into ******************\n");
			printf("cur_pos = %d,i=%d\n",cur_pos,i);
			Pre_ret_Posy = mouse_posy+mouse_posx;
			menu_reset_curdata_disp(cur_pos,WHITE);
			cur_pos=i;
			menu_reset_curdata_disp(cur_pos,RED);
			break;
		}
	}
}

void mRESET_key_func()
{
	Mouse_Ret_Move_Func();
	if(byKey_val==0) return;
	printf("mRESET_key_func\n");
	switch(byKey_val)                   
	{
		case kESC:	//menu        exit
			menu_reset_off();
			break;
		case kLT:	//menu       left
		case kRH:	//menu       right
			menu_reset_up();
			break;
		case kET:	     //16sp        Enter  
			menu_reset_enter();
			break;
		case kMLeftup:
			if ((mouse_posx>244) && (mouse_posx<255) && (mouse_posy>106) && (mouse_posy<117))
				menu_reset_off();
			break;
		case kMLeftdn:
			if ((mouse_posx>=Pre_retpos[cur_pos*4]) && (mouse_posy>=Pre_retpos[cur_pos*4+1]) && (mouse_posx<=Pre_retpos[cur_pos*4+2]) && (mouse_posy<=Pre_retpos[cur_pos*4+3]))
				menu_reset_enter();
			break;
		default:
			break;
       }
	byKey_val=0;
}
