
#include "cmn.h"

//==================================================================================

void LM_check(void)
{
	lossval = 0;
	lossval1 = 0;
	lossval2 = 0;
	lossval3 = 0;
#if 0
/*
	U8 tmp1,tmp2;
	U8 tmp22,tmp55;
	U16 alarm_info = 0;
*/
#if 1
	U8 tmp;
	if (menu_num && menu_num<mMouse)
	{
		return;
	}

	lossval = 0;
	RegisterReadWrite.ChipNo=0;
#ifdef PT2864
	write_nvp1114(0xff,0x01,0);
#endif
	read_nvp1114(oprateadd[dev_id][ADDLOSS],&tmp,0);	
//	printf("chip0:0x%x\n",tmp);
	if (!dev_id)
	{
		lossval  =tmp^0x0f;
		lossval  &= 0x0f;
	}
	else
		lossval = tmp >> 4;

	if (Cam_Num>CHANNEL4)
	{
		lossval1 = 0;
		RegisterReadWrite.ChipNo=1;
		tmp = 0;
#ifdef PT2864
	write_nvp1114(0xff,0x01,1);
#endif
		read_nvp1114(oprateadd[dev_id][ADDLOSS],&tmp,1);	
//		printf("chip1:0x%x\n",tmp);
		if (!dev_id)
		{
			lossval1  =tmp^0x0f;
			lossval1  &= 0x0f;
		}
		else
			lossval1 = tmp >> 4;

	}
	if (Cam_Num>CHANNEL8)
	{
		RegisterReadWrite.ChipNo=2;
		tmp = 0;
#ifdef PT2864
	write_nvp1114(0xff,0x01,2);
#endif
		read_nvp1114(oprateadd[dev_id][ADDLOSS],&tmp,2);	
		if (!dev_id)
		{
			lossval2  =tmp^0x0f;
			lossval2  &= 0x0f;
		}
		else
			lossval2 = tmp >> 4;

//		printf("chip2:0x%x\n",lossval2);
		RegisterReadWrite.ChipNo=3;
		tmp = 0;
#ifdef PT2864
	write_nvp1114(0xff,0x01,3);
#endif
		read_nvp1114(oprateadd[dev_id][ADDLOSS],&tmp,3);	
		if (!dev_id)
		{
			lossval3  =tmp^0x0f;
			lossval3  &= 0x0f;
		}
		else
			lossval3 = tmp >> 4;

//		printf("chip3:0x%x\n",lossval3);
	}
	if (Cam_Num>CHANNEL16 && ChipSelect == HI3520)
		GetSlaveLoss();
#endif
/*	
	lossval = lossval1 =lossval2=lossval3=0;
	tmp1=lossval & 0x0f;
	tmp2=lossval1 & 0x0f;
	tmp22=tmp1 | (tmp2<<4);
	tmp1=lossval2 & 0x0f;
	tmp2=lossval3 & 0x0f;
	tmp55=tmp1 | (tmp2<<4);

	alarm_info = (tmp55<<8) | tmp22;
	printf("loss1:%x,loss2:%x,loss3:%x,loss4:%x, lossread:%x\n",lossval,lossval1,lossval2,lossval3,alarm_info);
*/
//jlb090820	alarm_info = gstCommonParam->GST_DRA_get_alarm_cam_info();
#endif 
}  







