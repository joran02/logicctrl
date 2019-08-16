#include "cmn.h"

U8 audio_cambuf[2][18];

U8 AUDIOMENULIST;

U8 audiocam;

U8* audiobmpback = NULL;
U8* audiobmpfore = NULL;

U8* Iconexitback = NULL;
U8* Iconexitfore = NULL;

 U16 tAudio_title_pos[]=
{
	58,86,	144,86,	230,86,	316,86,
	58,112,	144,112,	230,112,	316,112,
	58,138,	144,138,	230,138,	316,138,
	58,164,	144,164,	230,164,	316,164,
};

U16 Audio_list_pos[]=
{
	120,60,			292,60,
	120,86,	206,86,	292,86,	378,86,
	120,112,	206,112,	292,112,	378,112,
	120,138,	204,138,	292,138,	378,138,
	120,164,	204,164,	292,164,	378,164,
			152,230//,	240,230
};

U16 Pre_audiopos[]=
{
	120,60,		144,84,
	292,60,		316,84,

	120,86,		144,110,
	206,86,		230,110,
	292,86,		316,110,
	378,86,		402,110,

	120,112,		144,136,
	206,112,		230,136,
	292,112,		316,136,
	378,112,		402,136,
		
	120,138,		144,162,
	206,138,		230,162,
	292,138,		316,162,
	378,138,		402,162,

	120,164,		144,188,
	206,164,		230,188,
	292,164,		316,188,
	378,164,		402,188,

//	64,230,		224,254,
	152,230,		312,254
};


void audioMenu_Inital()
{
	memset(audio_cambuf[0],0x00,sizeof(audio_cambuf[0]));
	memset(audio_cambuf[1],0x00,sizeof(audio_cambuf[1]));
	audio_cambuf[0][0]=1;
	audio_cambuf[1][0]=1;
}

void audio_disp(U8 pos,U8 color)
{
}

void audio_ch_off()
{
	cur_pos = 0;
	menu_num = 0;
    	OSD_ALL_CLEAR();
	if (demo_num < PBFULL)
	{
		demo_osd_disp();
	}
	else
	{
		Playstatus = 0xff;
		if (Play_Mode/10 == 4)
			border_set(L4);
		else if (Play_Mode/10 == 8)
			border_set(L8);
		else if (Play_Mode/10 == 16)
			border_set(L16);
	}
}

void audio_ch_on()
{
	U8 i,tmp,buf[5];

	if (CheckUser(menupass[curuser].Audiocfig))
		return;
//	if (Cam_Num==CHANNEL16)
//		audiocam = CHANNEL8;
//	else
	audiocam = Cam_Num;
	menu_num = mAUDIO;
	cur_pos=0;
	memcpy(audio_cambuf[0],audio_cambuf[1],sizeof(audio_cambuf[1]));

	AUDIOMENULIST = audiocam+3;
	memset(buf,0x00,sizeof(buf));
	for(i=0;i<AUDIOMENULIST;i++)
	{
		if (i==AUDIOMENULIST-1)
			tmp = 18;
//		else if (i==AUDIOMENULIST-1)
//			tmp = 19;
		else 
			tmp = i;
		if(tmp!=cur_pos)audio_disp(tmp,WHITE);
		else audio_disp(cur_pos,RED);
	}
}

U16 getliveaudioch()
{
	U8 i;
	U16 tmp=0;
	for (i=0;i<audiocam;i++)
	{
		if (audio_cambuf[1][i+2])
			tmp |= WBIT(i);
	}
	return tmp;
}

void setliveaudio()
{
#if 0
	int ret,tmp;
	tmp = 0;

	if (demo_num >L16)
	{
		tmp = 0;
		ret=gstCommonParam->GST_DRA_enable_local_audio_play(0,tmp);
		if (audio_cambuf[1][0])
			ret=gstCommonParam->GST_FTC_AudioPlayCam(0);
		else
		{
			if (Play_Mode/100)
			{
				tmp = WBIT(Play_Mode%100);
				ret=gstCommonParam->GST_FTC_AudioPlayCam(tmp);
			}
			else
			{
				ret=gstCommonParam->GST_FTC_AudioPlayCam(0);
//				tmp = getliveaudioch();
//				ret=gstCommonParam->GST_FTC_AudioPlayCam(tmp);
			}
		}
	}
	else
	{
		ret=gstCommonParam->GST_FTC_AudioPlayCam(0);
		if (audio_cambuf[1][0] || audio_cambuf[1][1])
		{
			tmp = 0;
			ret=gstCommonParam->GST_DRA_enable_local_audio_play(0,tmp);
		}
		else
		{
			ret=gstCommonParam->GST_DRA_enable_local_audio_play(1,getliveaudioch());
		}
	}
	if (ret<0)
		printf("Record audio set error!\n");
#endif
}


void audio_menu_enter()
{
	U8 i;
	if (cur_pos==18)
	{
		audio_ch_off();
	}
//	else if (cur_pos==19)
//		audio_ch_off();
	else if (cur_pos<AUDIOMENULIST-1)
	{
		for (i=0;i<audiocam+2;i++)
		{
			if (i !=cur_pos && audio_cambuf[0][i])
			{
				audio_cambuf[0][i] = 0;
				audio_disp(i,WHITE);
				break;
			}
		}
		audio_cambuf[0][cur_pos] = 1;
		audio_disp(cur_pos,RED);
		memcpy(audio_cambuf[1],audio_cambuf[0],sizeof(audio_cambuf[0]));

		setliveaudio();
	}
}

void audio_menu_up()
{
	audio_disp(cur_pos,WHITE);
	if(cur_pos<=0) 
		cur_pos=18;
	else if (cur_pos==18)
		cur_pos=AUDIOMENULIST-2;
	else
		cur_pos--;
	audio_disp(cur_pos,RED);
}

void audio_menu_down()
{
	audio_disp(cur_pos,WHITE);
	if(cur_pos==18) 
		cur_pos=0;
	else
		cur_pos++;
	
	if ((audiocam < 16) && (cur_pos==AUDIOMENULIST-1))
		cur_pos = 18;
	audio_disp(cur_pos,RED);
}

void AudioMouseLClick(U16 x, U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_audiopos[cur_pos*4]+offsetx_hd;
		miny = Pre_audiopos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_audiopos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_audiopos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_audiopos[cur_pos*4]+offsetx;
		miny = Pre_audiopos[cur_pos*4+1]+offsety;
		maxx = Pre_audiopos[cur_pos*4+2]+offsetx;
		maxy = Pre_audiopos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		audio_menu_enter();
	}
}

void Mouse_audio_Move_Func(U16 x,U16 y)
{
	U8 i,tmp;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<AUDIOMENULIST;i++)
	{
		if (audiocam < 16)
		{
			if (i==AUDIOMENULIST-1)
				tmp = 18;
//			else if (i==AUDIOMENULIST-1)
//				tmp = 19;
			else
				tmp = i;
		}
		else
			tmp = i;
		if (MseType)
		{
			minx = Pre_audiopos[tmp*4]+offsetx_hd;
			miny = Pre_audiopos[tmp*4+1]+offsety_hd;
			maxx = Pre_audiopos[tmp*4+2]+offsetx_hd;
			maxy = Pre_audiopos[tmp*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_audiopos[tmp*4]+offsetx;
			miny = Pre_audiopos[tmp*4+1]+offsety;
			maxx = Pre_audiopos[tmp*4+2]+offsetx;
			maxy = Pre_audiopos[tmp*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != tmp))
		{
			audio_disp(cur_pos,WHITE);
			cur_pos=tmp;
			audio_disp(cur_pos,RED);
			break;
		}
	}
}

void audio_key_fun()
{
	if(byKey_val==0) 
	{
		return;
	}
	printf("audio_key_fun byKey_val = %x\n",byKey_val);
	switch(byKey_val)
	{
		case kLT:
		case kUP:
			audio_menu_up();
			break;
		case kRH:
		case kDN:
			audio_menu_down();
			break;
		case kET:
			audio_menu_enter();
			break;
		case kESC:
			audio_ch_off();
			break;
		default:
			break;
	}
	byKey_val = 0;
}


