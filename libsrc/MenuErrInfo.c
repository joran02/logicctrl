#include "cmn.h"

U8 InfoId;
ERRMENU errmenupic;

U16 Pre_errpos[]=
{
	48,216,	208,250,
	224,216,	384,250,
	136,216,	296,250
};

void menu_err_data_disp(U8 pos,U8 color)
{
}
U8 infohigh[]=
{
	0,3,
	3,2,
	5,2,
	7,2,
	9,3,
	12,2,
	14,2,
	16,2,
	0,0,
	0,0,
	24,2,
	0,0,
	18,2,
	20,2,
	22,2,
	26,2,
	28,2,
	30,2,
	32,2,
	34,2,
	36,2,
	38,2,
	40,2
};


void MenuErrOn()
{
}

void Err_Menu_Enter()
{
}

void ErrMouseLClick(U16 x,U16 y)
{
}

void Err_Menu_Move()
{
	switch(InfoId&0x3f)
	{
		case FORMATING:
		case StopRecord:
		case Loaddefault:
		case ConRec:
		case SENDTESTORNOT:
			menu_err_data_disp(cur_pos,WHITE);
			if (cur_pos == 0)
			{
				cur_pos=1;
			}
			else if (cur_pos == 1)
			{
				cur_pos=0;
			}
			menu_err_data_disp(cur_pos,RED);
			break;
		case FORMATOK:
		case PASSERR:
		case TimeErr:
		case PermissonErr:
		case RecErr:
		case HddFullRec:
		case TimeSetErr:
		case PASSWORDERR:
		case SERCONNETCTERR:
		case USERNOTEXITERR:
		case SENDERR:
		case UNKNOWNERR:
		case INFONOTFULLERR:
		case TESTOK:
			cur_pos = 2;
			menu_err_data_disp(cur_pos,RED);
			break;
		default:
			break;
	}
}

void Mouse_Err_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	switch (InfoId&0x3f)
	{
		case FORMATING:
		case StopRecord:
		case Loaddefault:
		case ConRec:
		case SENDTESTORNOT:
			for (i=0;i<2;i++)
			{
				if (MseType)
				{
					minx = Pre_errpos[i*4]+offsetx_hd;
					miny = Pre_errpos[i*4+1]+offsety_hd;
					maxx = Pre_errpos[i*4+2]+offsetx_hd;
					maxy = Pre_errpos[i*4+3]+offsety_hd;
				}
				else
				{
					minx = Pre_errpos[i*4]+offsetx;
					miny = Pre_errpos[i*4+1]+offsety;
					maxx = Pre_errpos[i*4+2]+offsetx;
					maxy = Pre_errpos[i*4+3]+offsety;
				}
				if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
				{
					menu_err_data_disp(cur_pos,WHITE);
					cur_pos=i;
					menu_err_data_disp(cur_pos,RED);
					break;
				}
			}
			break;
		case FORMATOK:
		case PASSERR:
		case TimeErr:
		case PermissonErr:
		case RecErr:
		case HddFullRec:
		case TimeSetErr:
		case PASSWORDERR:
		case SERCONNETCTERR:
		case USERNOTEXITERR:
		case SENDERR:
		case UNKNOWNERR:
		case INFONOTFULLERR:
		case TESTOK:
			if (MseType)
			{
				minx = Pre_errpos[8]+offsetx_hd;
				miny = Pre_errpos[9]+offsety_hd;
				maxx = Pre_errpos[10]+offsetx_hd;
				maxy = Pre_errpos[11]+offsety_hd;
			}
			else
			{
				minx = Pre_errpos[8]+offsetx;
				miny = Pre_errpos[9]+offsety;
				maxx = Pre_errpos[10]+offsetx;
				maxy = Pre_errpos[11]+offsety;
			}
			if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
			{
				if (cur_pos != 2)
				{
					cur_pos = 2;
					menu_err_data_disp(cur_pos,RED);
				}
			}		
			else 
			{
				if (cur_pos != 3)
				{
					cur_pos = 3;
					menu_err_data_disp(cur_pos,WHITE);
				}
			}
			break;
		default:
			break;
	}
}

U8 Reccont=0;

void mErrInfo_key_func()
{
	if (!byKey_val)
	{
		if (InfoId == ConRec)
		{
			if (bsecond_change)
				Reccont++;

			if (Reccont>11)
			{
				cur_pos = 0;
				byKey_val = kET;
			}
			else
				return;
		}
		else
			return;
	}
	printf("mErrInfo_key_func byKey_val = %x\n",byKey_val);
	switch(byKey_val)
	{
		case kLT:
		case kRH:
		case kUP:
		case kDN:
			Err_Menu_Move();
			break;
		case kET:
			Err_Menu_Enter();
			break;
		case kESC:
			switch(InfoId&0x3f)
			{
				case FORMATING:
				case StopRecord:
				case Loaddefault:
				case ConRec:
				case SENDTESTORNOT:
					cur_pos = 1;
					break;
				case FORMATOK:
				case PASSERR:
				case TimeErr:
				case PermissonErr:
				case RecErr:
				case HddFullRec:
				case TimeSetErr:
				case PASSWORDERR:
				case SERCONNETCTERR:
				case USERNOTEXITERR:
				case SENDERR:
				case UNKNOWNERR:
				case INFONOTFULLERR:
				case TESTOK:
					cur_pos = 2;
					break;
				default:
					break;
			}
			Err_Menu_Enter();
			break;
		default:
			break;
	}
	byKey_val=0;
}

