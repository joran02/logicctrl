#include "cmn.h"

U8 LongKey;

void GetKey()
{
	if (net_info_ptr->key_code)
	{
		if (menu_num != mPTZControl)
		{
			byKey_val = net_info_ptr->key_code;
			if ((byKey_val /1000)==1)
				LongKey = 1;
			else if ((byKey_val /1000)==2)
				LongKey = 0;
			if ((byKey_val/1000) != 1)
				net_info_ptr->key_code = 0;
			DBGPRINT("byKey_val is %x,net_info_ptr->key_style is %x,menu_num = %d,camno:%d\n",byKey_val,net_info_ptr->key_style,menu_num,net_info_ptr->cam_no);
			byKey_val = byKey_val%1000;
			DBGPRINT("#### key:%x,longkey:%d\n",byKey_val,LongKey);
			if (byKey_val !=0xff)
				NETPTZ_Control_Fun(net_info_ptr->cam_no,net_info_ptr->key_style%1000,net_info_ptr->key_style/1000);
			else
				byKey_val  = 0;
		}
	}
}