#include "cmn.h"

#define MENULISTUNM	15

U8* passmenupic;
U8* keynumberback;
U8* keynumberfore;

UC pass_error=0;
U8 userflag,curuser;
char password[6];
U8 Passget[6];
U8 wordpos = 0;
USERMAGE menupass[4];

U16 data_list_pos[]=
{
	232,88,
	236,140,	258,140,	280,140,	302,140,
	236,162,	258,162,	280,162,	302,162,
	236,184,	258,184,	280,184,	302,184,
	
	48,220,
	224,220,
	
	232,114,
	256,114,
	280,114,
	304,114,
	328,114,
	352,114
};

U16 Pre_passpos[]=
{
	232,88,	432,112,
	236,140,	256,160,
	258,140,	278,160,
	280,140,	300,160,
	302,140,	322,160,
	236,162,	256,182,
	258,162,	278,182,
	280,162,	300,182,
	302,162,	322,182,
	236,184,	256,204,
	258,184,	278,204,
	280,184,	300,204,
	302,184,	322,204,
	
	48,220,	208,244,
	224,220,	384,244
	
};

void menu_pass_off(void)
{
	cur_pos = 0;
	menu_num = 0;
    	OSD_ALL_CLEAR();
	demo_osd_disp();
	pass_error=0;
	if (SystemLock)
		LedControl(LEDLOCK,1);
	else
		LedControl(LEDLOCK,0);
}

void pass_list_disp(U8 pos,U16 color)
{
}

void menu_pass_on(U8 flag)
{
}
//--------------------------------------

void InputPassword(U8 value)
{
	U8 tmp = wordpos;
	Passget[wordpos]=value;
	password[wordpos]=0x2A;
	pass_list_disp(wordpos+15,WHITE);
	if (wordpos<5)
	{
		wordpos++;
		password[wordpos]=0x2D;
	}
	pass_list_disp(wordpos+15,RED);
	if (tmp == 5)
	{
		pass_list_disp(cur_pos,WHITE);
		cur_pos = 13;
		pass_list_disp(cur_pos,RED);
	}
}

void breakspace()
{
	Passget[wordpos]=0xff;
	password[wordpos]=0x2D;
	pass_list_disp(wordpos+15,WHITE);
	if (wordpos)
		wordpos--;
	password[wordpos]=0x2D;
	pass_list_disp(wordpos+15,RED);
}

void clearall()
{
	U8 i;
	memset(password,0x2D,6);
	memset(Passget,0xff,6);
	wordpos = 0;
	for (i=15;i<21;i++)
	{
		if (i==15)
			pass_list_disp(i,RED);
		else
			pass_list_disp(i,WHITE);
	}
}

void confirm_password()
{
	U8 i;
	pass_error =1;
	
	for (i=0;i<6;i++)
	{
		printf("passget %d: %d,pass %d:%d\n",i,Passget[i],i,menupass[curuser].Password[i]);
		if (Passget[i] != menupass[curuser].Password[i])
		{
			pass_error = 0;
//			break;
		}
	}

//	Str_disp(150, 250+OSDOFFSET, WHITE, passdatastr[2][pass_error]);
	if (!pass_error)
	{
		cur_pos = 1;
		pass_list_disp(cur_pos,RED);
		for (i=15;i<21;i++)
		{
			password[cur_pos-1]=0x2D;
			pass_list_disp(i,WHITE);
		}
		InfoId = PASSERR;
		MenuErrOn();
	}
	else
	{
		if (!userflag)
			menu_user_on();
		else
		{
			SystemLock = 0;
			I_write_block_eeprom(ADDSYS+0x0f,1,&SystemLock);
			I_write_block_eeprom(ADDSYS+0x0e,1,&curuser);
			menu_pass_off();
		}
	}
}

void menu_pass_up()
{
	pass_list_disp(cur_pos,WHITE);
	if (userflag)
	{
		if (cur_pos==0)
			cur_pos = MENULISTUNM-1;
//		else if (cur_pos==MENULISTUNM-2)
//			cur_pos = 1;
//		else if (cur_pos>0 && cur_pos<MENULISTUNM-2)
//			cur_pos = 0;
		else 
			cur_pos--;
	}
	else
	{
		if (cur_pos == 1)
			cur_pos = MENULISTUNM-1;
//		else if (cur_pos==MENULISTUNM-2)
//			cur_pos = 1;
		else 
			cur_pos--;
	}
	pass_list_disp(cur_pos,RED);
}

void menu_pass_down()
{
	pass_list_disp(cur_pos,WHITE);
	if (userflag)
	{
		if (cur_pos==MENULISTUNM-1)
			cur_pos = 0;
//		else if (cur_pos>0 && cur_pos<MENULISTUNM-2)
//			cur_pos = MENULISTUNM-2;
		else 
			cur_pos ++;
	}
	else
	{
//		if (cur_pos>0 && cur_pos<MENULISTUNM-2)
//			cur_pos = MENULISTUNM-2;
		if (cur_pos==MENULISTUNM-1)
			cur_pos = 1;
		else 
			cur_pos ++;
	}
	pass_list_disp(cur_pos,RED);
}

void menu_pass_enter()
{
	switch(cur_pos)
	{
		case 1:
		case 2:
		case 3:
		case 4:
			InputPassword(cur_pos+5);
			break;
		case 5:
		case 6:
		case 7:
		case 8:
			InputPassword(cur_pos-3);
			break;
		case 9:
		case 10:
			InputPassword(10-cur_pos);
			break;
		case 11:
			breakspace();
			break;
		case 12:
			clearall();
			break;
		case 13:
			confirm_password();
			break;
		case 14:
			menu_pass_off();
			break;
	}
}

void menu_pass_add()
{
	if (cur_pos==0)
	{
		if (curuser>=3)
			curuser = 0;
		else
			curuser++;
		pass_list_disp(cur_pos,RED);
	}
}

void menu_pass_dec()
{
	if (cur_pos==0)
	{
		if (curuser<=0)
			curuser = 3;
		else
			curuser--;
		pass_list_disp(cur_pos,RED);
	}
}

void MousePassLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_passpos[cur_pos*4]+offsetx_hd;
		miny = Pre_passpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_passpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_passpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_passpos[cur_pos*4]+offsetx;
		miny = Pre_passpos[cur_pos*4+1]+offsety;
		maxx = Pre_passpos[cur_pos*4+2]+offsetx;
		maxy = Pre_passpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos==0)
		{
			if (userflag)
				menu_pass_dec();
		}
		else
			menu_pass_enter(); 
	}
}

void MousePassRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_passpos[cur_pos*4]+offsetx_hd;
		miny = Pre_passpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_passpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_passpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_passpos[cur_pos*4]+offsetx;
		miny = Pre_passpos[cur_pos*4+1]+offsety;
		maxx = Pre_passpos[cur_pos*4+2]+offsetx;
		maxy = Pre_passpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (userflag && cur_pos == 0)
			menu_pass_add(); 
	}
}

void Mouse_Pass_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<MENULISTUNM;i++)
	{
		if (MseType)
		{
			minx = Pre_passpos[i*4]+offsetx_hd;
			miny = Pre_passpos[i*4+1]+offsety_hd;
			maxx = Pre_passpos[i*4+2]+offsetx_hd;
			maxy = Pre_passpos[i*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_passpos[i*4]+offsetx;
			miny = Pre_passpos[i*4+1]+offsety;
			maxx = Pre_passpos[i*4+2]+offsetx;
			maxy = Pre_passpos[i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
			if (userflag ||(!userflag&& i>0))
			{
				pass_list_disp(cur_pos,WHITE);
				cur_pos=i;
				pass_list_disp(cur_pos,RED);
				break;
			}
		}
	}
}


void mPASS_key_func()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
	case kESC:
		menu_pass_off();
		break;
	case kUP:
		menu_pass_up();
		break;
	case kDN:
		menu_pass_down();
		break;
	case kET:
		menu_pass_enter();
		break;
	case kRH:
	case kAD:
		menu_pass_add();
		break;
	case kLT:
	case kDE:
		menu_pass_dec();
		break;
	case kCR:
		clearall();
		break;
	case k01:
	case k02:
	case k03:
	case k04:
	case k05:
	case k06:
	case k07:
	case k08:
	case k09:
	case k10:	//4  
		InputPassword((U8)(byKey_val-k10));
		break;
    }
   byKey_val=0;
}	    


