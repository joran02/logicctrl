#include "cmn.h"

#define USERMENULISTNUM	24

USERMENU usermenupic;

U8 curusers;


U16 userdata_list_pos[]=
{
	280,70,	304,70,	328,70,	352,70,	376,70,	400,70,
	426,70,	426,80,
	280,96,	304,96,	328,96,	352,96,	376,96,	400,96,
	220,156,	440,156, 	
	220,182,	440,182,
	220,208,	440,208,
	220,234,	440,234,
//	220,260,

	112,308,	308,308	
};

U16 Pre_userpos[]=
{
	280,70,	292,90,
	304,70,	316,90,
	328,70,	340,90,
	352,70,	364,90,
	376,70,	388,90,
	400,70,	412,90,
	426,70,	454,80,
	426,80,	454,90,
	280,96,	292,116,
	304,96,	316,116,
	328,96,	340,116,
	352,96,	364,116,
	376,96,	388,116,
	400,96,	412,116,
	220,156,	244,180,
	440,156,	464,180,
	220,182,	244,206,
	440,182,	464,206,
	220,208,	244,232,
	440,208,	464,232,
	220,234,	244,258,
	440,234,	464,258,
//	220,260,	244,284,

	112,308,	272,332,
	308,308,	468,332
};


void user_menu_inital()
{
	U8 i;
	for (i=0;i<4;i++)
	{
		I_read_block_eeprom(ADDPAS+ADDCOUNTPAS*i,sizeof(menupass[i]),&menupass[i].UserId);
	}

}

void net_user_set()
{
	char data_Buf1[15],data_Buf2[15],data_Buf3[15],data_Buf4[15];
	char data_name0[7],data_name1[7],data_name2[7],data_name3[7];

	memset(data_Buf1,0x00,sizeof(data_Buf1));
	memset(data_Buf2,0x00,sizeof(data_Buf2));
	memset(data_Buf3,0x00,sizeof(data_Buf3));
	memset(data_Buf4,0x00,sizeof(data_Buf4));
	sprintf(data_Buf1,"%d%d%d%d%d%d",
					menupass[0].Password[0],menupass[0].Password[1],menupass[0].Password[2],menupass[0].Password[3],menupass[0].Password[4],menupass[0].Password[5]);
	sprintf(data_Buf2,"%d%d%d%d%d%d",
					menupass[1].Password[0],menupass[1].Password[1],menupass[1].Password[2],menupass[1].Password[3],menupass[1].Password[4],menupass[1].Password[5]);
	sprintf(data_Buf3,"%d%d%d%d%d%d",
					menupass[2].Password[0],menupass[2].Password[1],menupass[2].Password[2],menupass[2].Password[3],menupass[2].Password[4],menupass[2].Password[5]);
	sprintf(data_Buf4,"%d%d%d%d%d%d",
					menupass[3].Password[0],menupass[3].Password[1],menupass[3].Password[2],menupass[3].Password[3],menupass[3].Password[4],menupass[3].Password[5]);
	memset(data_name0,0x00,sizeof(data_name0));
	memset(data_name1,0x00,sizeof(data_name1));
	memset(data_name2,0x00,sizeof(data_name2));
	memset(data_name3,0x00,sizeof(data_name3));
	sprintf(data_name0,"%s",menupass[0].UserName);
	sprintf(data_name1,"%s",menupass[1].UserName);
	sprintf(data_name2,"%s",menupass[2].UserName);
	sprintf(data_name3,"%s",menupass[3].UserName);
	printf("data_buf1=%s,name1 = %s\n",data_Buf1,data_name0);
	printf("data_buf2=%s,name2 = %s\n",data_Buf2,data_name1);
	printf("data_buf3=%s,name3 = %s\n",data_Buf3,data_name2);
	printf("data_buf4=%s,name4 = %s\n",data_Buf4,data_name3);

	gstCommonParam->GST_NET_get_admin_info(data_name0,data_Buf1,data_name1,data_Buf2,data_name2,data_Buf3,data_name3,data_Buf4);
}

void menu_user_off(void)
{

	OSD_ALL_CLEAR();
	user_menu_inital();
	net_user_set();
    	menu_main_on();
}

void user_list_disp(U8 pos,U16 color)
{
}

void menu_user_on()
{
}

void menu_user_up()
{
	user_list_disp(cur_pos,WHITE);
	if (cur_pos==0)
		cur_pos = USERMENULISTNUM-1;
/*
	else if (cur_pos == 6)
	{
		if (menupass[curusers].UserId)
			cur_pos--;
		else
			cur_pos = 0;
	}
*/
	else
		cur_pos--;
	user_list_disp(cur_pos,RED);
}

void menu_user_down()
{
	user_list_disp(cur_pos,WHITE);
	if (cur_pos==USERMENULISTNUM-1)
		cur_pos = 0;
/*	
	else if (cur_pos == 0)
	{
		if (menupass[curusers].UserId)
			cur_pos++;
		else
			cur_pos = 6;
	}
*/
	else
		cur_pos++;
	user_list_disp(cur_pos,RED);
}

void user_data_save()
{
	U8 i;
	for (i=0;i<4;i++)
	{
		I_write_block_eeprom(ADDPAS+ADDCOUNTPAS*i,sizeof(menupass[i]),&menupass[i].UserId);
	}
}

void menu_user_enter()
{
	U8 i;
	switch (cur_pos)
	{
		case 6:
			if (curusers>=3)
				curusers = 0;
			else
				curusers++;
			for (i=0;i<USERMENULISTNUM;i++)
			{
				user_list_disp(i,WHITE);
			}
			break;
		case 7:
			if (curusers<=0)
				curusers = 3;
			else
				curusers--;
			for (i=0;i<USERMENULISTNUM;i++)
			{
				user_list_disp(i,WHITE);
			}
			break;
		case 14:
			if (curusers)
			{
				if (menupass[curusers].Menucfig)
					menupass[curusers].Menucfig =0;
				else
					menupass[curusers].Menucfig =1;
			}
			break;
		case 15:
			if (menupass[curusers].Seqcfig)
				menupass[curusers].Seqcfig=0;
			else
				menupass[curusers].Seqcfig=1;
			break;
		case 16:
			if (menupass[curusers].Ptzcfig)
				menupass[curusers].Ptzcfig=0;
			else
				menupass[curusers].Ptzcfig=1;
			break;
		case 17:
			if (menupass[curusers].Spotcfig)
				menupass[curusers].Spotcfig=0;
			else
				menupass[curusers].Spotcfig=1;
			break;
		case 18:
			if (menupass[curusers].Reccfig)
				menupass[curusers].Reccfig=0;
			else
				menupass[curusers].Reccfig=1;
			break;
		case 19:
			if (menupass[curusers].Playcfig)
				menupass[curusers].Playcfig=0;
			else
				menupass[curusers].Playcfig=1;
			break;
		case 20:
			if (menupass[curusers].Stopcfig)
				menupass[curusers].Stopcfig=0;
			else
				menupass[curusers].Stopcfig=1;
			break;
/*			
		case 21:
			if (menupass[curusers].Bakcfig)
				menupass[curusers].Bakcfig=0;
			else
				menupass[curusers].Bakcfig=1;
			break;
*/
		case 21:
			if (menupass[curusers].Audiocfig)
				menupass[curusers].Audiocfig=0;
			else
				menupass[curusers].Audiocfig=1;
			break;
		default:
			break;
	}
	if (cur_pos<22)
		user_list_disp(cur_pos,RED);
	else if (cur_pos==22)
	{
		user_data_save();
		menu_user_off();
	}
	else if (cur_pos==23)
		menu_user_off();
}

void menu_user_add()
{
//	U8 i;
	if (cur_pos < 6)
	{
//		if (curusers)
		{
			if (menupass[curusers].UserName[cur_pos]>=122)
				menupass[curusers].UserName[cur_pos] = 48;
			else
				menupass[curusers].UserName[cur_pos]++;
		}
	}
	else if (cur_pos>7 && cur_pos<14)
	{
		if (menupass[curusers].Password[cur_pos-8]>=9)
			menupass[curusers].Password[cur_pos-8]=0;
		else
			menupass[curusers].Password[cur_pos-8]++;
	}
	user_list_disp(cur_pos,RED);
}

void menu_user_dec()
{
//	U8 i;
	if (cur_pos<6)
	{
//		if (curusers)
		{
			if (menupass[curusers].UserName[cur_pos]<=48)
				menupass[curusers].UserName[cur_pos] = 122;
			else
				menupass[curusers].UserName[cur_pos]--;
		}
	}
	else if (cur_pos>7 && cur_pos<14)
	{
		if (menupass[curusers].Password[cur_pos-8]<=0)
			menupass[curusers].Password[cur_pos-8]=9;
		else
			menupass[curusers].Password[cur_pos-8]--;
	}
	user_list_disp(cur_pos,RED);
}

void MouseUserRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_userpos[cur_pos*4]+offsetx_hd;
		miny = Pre_userpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_userpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_userpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_userpos[cur_pos*4]+offsetx;
		miny = Pre_userpos[cur_pos*4+1]+offsety;
		maxx = Pre_userpos[cur_pos*4+2]+offsetx;
		maxy = Pre_userpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
//		if ((cur_pos>7 && cur_pos<14) || (cur_pos<6 && curusers))
		if ((cur_pos>7 && cur_pos<14) || cur_pos<6)
			menu_user_add(); 
	}
}

void MouseUserLClick(U16 x,U16 y,U8 type)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_userpos[cur_pos*4]+offsetx_hd;
		miny = Pre_userpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_userpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_userpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_userpos[cur_pos*4]+offsetx;
		miny = Pre_userpos[cur_pos*4+1]+offsety;
		maxx = Pre_userpos[cur_pos*4+2]+offsetx;
		maxy = Pre_userpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
//		if (((cur_pos>7 && cur_pos<14) || (cur_pos<6 && curusers)))
		if (((cur_pos>7 && cur_pos<14) || cur_pos<6))
			menu_user_dec();
		else if (type == 26)
			menu_user_enter();
	}
}

void Mouse_User_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<USERMENULISTNUM;i++)
	{
//		if (curusers == 0 && i<6)
//			xpos = 406+offsety;
//		else
		if (MseType)
		{
			minx = Pre_userpos[i*4]+offsetx_hd;
			miny = Pre_userpos[i*4+1]+offsety_hd;
			maxx = Pre_userpos[i*4+2]+offsetx_hd;
			maxy = Pre_userpos[i*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_userpos[i*4]+offsetx;
			miny = Pre_userpos[i*4+1]+offsety;
			maxx = Pre_userpos[i*4+2]+offsetx;
			maxy = Pre_userpos[i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
			user_list_disp(cur_pos,WHITE);
			cur_pos=i;
			user_list_disp(cur_pos,RED);
			break;
		}
	}
}

void mUser_key_func()
{
	if(byKey_val==0) return;
	printf("mUser_key_func byKey_val = %x\n",byKey_val);

	switch(byKey_val)
	{
	case kESC:
		menu_user_off();
		break;
	case kUP:
		menu_user_up();
		break;
	case kDN:
		menu_user_down();
		break;
	case kET:
		menu_user_enter();
		break;
	case kRH:
	case kAD:
		menu_user_add();
		break;
	case kLT:
	case kDE:
		menu_user_dec();
		break;
	default:
		break;
    }
   byKey_val=0;
}	    


