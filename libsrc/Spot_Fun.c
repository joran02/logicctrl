#include "cmn.h"

U8 spot_flag;

void Spot_ch_set(U8 ch)
{
}

void Spot_ch_on()
{
	Spot_ch_set(0);
	spot_flag = ON;
}

void Spot_ch_off()
{
	spot_flag = OFF;
	Osd_char_clear(19, 8, WHITE, 15, 0xff,AD);
}

void Spot_key_fun()
{
	if(byKey_val==0) 
	{
		return;
	}
	switch(byKey_val)
	{
		case k01:
		case k02:
		case k03:
		case k04:
		case k05:
		case k06:
		case k07:
		case k08:
		case k09:
			Spot_ch_set((U8)(byKey_val-k01));
			break;
		case k10:
			Spot_ch_set(9);
			break;
		case k11:
		case k12:
		case k13:
		case k14:
		case k15:
		case k16:
			Spot_ch_set((U8)(byKey_val-k11));
			break;
		case kMRightdn:
		case kESC:
		case kSpot:
		case kFFAST:
			Spot_ch_off();
			break;
		default:
			break;
	}
	byKey_val = 0;
}

