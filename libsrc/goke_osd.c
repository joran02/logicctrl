#include "cmn.h"

int STREAMFORMAT_WIDTH=1920;
int STREAMFORMAT_HEIGHT=1080;

#ifdef GOKE_DEV //GOKE_DEV

#define STREAM_CH		0

#define GOKE_GPIO_LED					0
#define GOKE_GPIO_ADC					1
#define GOKE_GPIO_IRCU_A			2
#define GOKE_GPIO_IRCU_B			3
#define GOKE_GPIO_RESET				4
#define GOKE_GPIO_SENSOR_IN 	5
#define GOKE_GPIO_ALARM_OUT		6

typedef enum {

    GAPP_PM_COLOUR_BLACK            = 0x00000000,
    GAPP_PM_COLOUR_READ              = 0x00ff0000,
    GAPP_PM_COLOUR_BLUE              = 0x000000ff,
    GAPP_PM_COLOUR_GREEN            = 0x0000ff00,
    GAPP_PM_COLOUR_YELLOW          =0x00ffff00,
    GAPP_PM_COLOUR_MAGENTA       = 0xff800000,
    GAPP_PM_COLOUR_CYAN              = 0x0000ffff,
    GAPP_PM_COLOUR_WHITE            =0x00ffffff,

}GAPP_PM_ColorT ;

typedef struct {
	int				unit; //0- percent ,1-pixel
	int				x;
	int				y;
	int				width;
	int				height;
	u32				color;		// 0:Black, 1:Red, 2:Blue, 3:Green, 4:Yellow, 5:Magenta, 6:Cyan, 7:White
	u32				action;
} GAPP_WBS_PrivacyMaskT;

GADI_SYS_HandleT goke_ispHandle=NULL;
static GADI_SYS_HandleT osdHandle=NULL;
static GADI_SYS_HandleT pmHandle=NULL;
static GADI_SYS_HandleT gpioHandle[7];

//static int gpio_id[7]={33,38,35,36,27,30,32};
static int gpio_id[7]={12,15,20,23,27,11,10};




extern int flag_adc_cfg,flag_ircut_cfg,flag_light_cfg;

int goke_osd_init()
{
    int retVal = GADI_OK;
    
    gadi_osd_init();
    osdHandle = gadi_osd_open(&retVal);
    if(retVal != GADI_OK)
    {
      DBGPRINT("goke_osd_init error_1\n");
    }else
        osd_function_init(OSD_VECTOR_FONT_LIB);

    if(!osdHandle)
    {
        gadi_osd_init();
        osdHandle = gadi_osd_open(&retVal);
        if(retVal != GADI_OK)
        {
          DBGPRINT("goke_osd_init error\n");
          return (-1);
        }
        osd_function_init(OSD_VECTOR_FONT_LIB);
    }
    return retVal;
}

int goke_show_time(void)
{
    int width,height=0;
    int streamId=STREAM_CH,areaId=1,enable=1;
    static OSD_TextParamsT textPar =
    {
        .enable    = 1,
        .allfontsPath = "/usr/share/fonts/Vera.ttf",
//        .fontsPath = "/usr/share/fonts/Vera.ttf",
        .size      = 32,
        .color     = 7,//black
        .outline   = 5,
        .bold      = 0,
        .italic    = 0,
        .startX    = 70,
        .startY    = 0,
        .boxWidth  = 50,
        .boxHeight = 50,
    };
    int retVal = 0;
		
    if(flag_1080p)
    {
    	textPar.startX=71;
    	textPar.size = 28;
    }
    else if(flag_960p)
    {
    	textPar.startX=65;
    	textPar.size = 23;
    }
    else //720p
    {
    	textPar.startX=65;
    	textPar.size = 23;
    }
			
    textPar.enable = enable;

    gadi_sys_get_date(textPar.textStr);
    textPar.textLen = strlen(textPar.textStr);

//		{
//			int a;
//			
//			for(a=0;a<textPar.textLen;a++)
//				DBGPRINT("*********** val:0x%x,char:%c \n",textPar.textStr[a],textPar.textStr[a]);
//		}

    if(!osdHandle)
    {
        retVal = gadi_osd_init();
        osdHandle = gadi_osd_open(&retVal);
        if(retVal != GADI_OK)
        {
            DBGPRINT("gadi_osd_open error\n");
            return (-1);
        }
        osd_function_init(OSD_VECTOR_FONT_LIB);
    }

    width  = STREAMFORMAT_WIDTH;//stream_map[streamId].streamFormat.width;
    height = STREAMFORMAT_HEIGHT;//stream_map[streamId].streamFormat.height;

    retVal = osd_function_show_date(osdHandle, width, height, streamId, areaId, &textPar);
	if(retVal != 0)
        {
            printf("show date error.");
        }

    return retVal;

}

int goke_osd_set_text(char *str)
{
    int retVal = 0;
    int width,height=0;
    int streamId=STREAM_CH,areaId=2;
    
    static OSD_TextParamsT textParam =
    {
        .enable    = 1,
        .allfontsPath = "/usr/share/fonts/Vera.ttf",
//        .fontsPath = "/usr/share/fonts/Vera.ttf",
        .size      = 32,
        .color     = 7,// 0:Black, 1:Red, 2:Blue, 3:Green, 4:Yellow, 5:Magenta, 6:Cyan, 7:White
        .outline   = 5,
        .bold      = 0,
        .italic    = 0,
        .startX    = 0,
        .startY    = 0,
        .boxWidth  = 50,
        .boxHeight = 50,
    };
    
    if(flag_1080p)
		textParam.size = 28;
	else if(flag_960p)
		textParam.size = 23;
	else //720p
		textParam.size = 23;
        
    width  = STREAMFORMAT_WIDTH;//stream_map[streamId].streamFormat.width;
    height = STREAMFORMAT_HEIGHT;//stream_map[streamId].streamFormat.height;

    textParam.textLen   = strlen(str);
    textParam.enable    = 1; // the web,not send text_enable,so if the text.length is 0,we clear the text in osd
    if(textParam.textLen == 0)
    {
      strncpy(textParam.textStr, "  ", 2);
      textParam.textLen=1;
      textParam.enable = 0;//clean text
    }
    strncpy(textParam.textStr, str, textParam.textLen+1);
    retVal = osd_function_show_text(osdHandle, width, height, streamId, areaId, &textParam);

    return retVal;

}

int goke_osd_show_bmp()
{
	DBGPRINT("\n***start show bmp***\n");
	int retVal = 0;
	int width,height=0;
	int streamId=STREAM_CH,areaId=0;

	static OSD_BmpParamsT bmpParam =
	{
	    .enable    = 1,
	    .startX    = 0,
	    .startY    = 0,
	    .bmpFile   = "/tmp/chan.bmp",//NULL,//"/root/gokelogo.bmp",
	};

	//strncpy(bmpParam.bmpFile, "/mnt/mtd/chan.bit", textParam.textLen+1);
	retVal = osd_function_show_bmp(osdHandle, streamId, areaId, &bmpParam);
	if(retVal != 0)
	{
	   	DBGPRINT("\n***show picture error.***ret=%d***\n", retVal);
	}
	gadi_sys_free(bmpParam.bmpFile);
	bmpParam.bmpFile = NULL;
	return retVal;
}


static int wbs_add_pm(const GAPP_WBS_PrivacyMaskT *pm_param)
{
    int retVal;
    unsigned char pmIndex=0;
    GADI_U32 enable = 1;//pm_param->action;//1;

    if(!pmHandle)
    {
        retVal = gadi_pm_init();
        pmHandle = gadi_pm_open(&retVal);
        if(retVal != GADI_OK)
        {
            DBGPRINT("gadi_pm_open error\n");
            goto err;
        }
    }
    retVal = gadi_pm_malloc(pmHandle, (void *)pm_param, &pmIndex);
    if(retVal != 0)
    {
        DBGPRINT("gadi_pm_malloc error:%d\n",retVal);
        goto err;
    }

    retVal =  gadi_pm_enable(pmHandle, pmIndex, enable);
    if(retVal != 0)
    {
        DBGPRINT("gadi_pm_enable error:%d\n",retVal);
        goto err;
    }

    return retVal;
err:
    DBGPRINT("wbs_add_pm failed!\n");
    return retVal;
}

int goke_set_pm_param(int x,int y,int w,int h)
{
    GAPP_WBS_PrivacyMaskT pm;
        
    return 0;
        
    pm.unit = 1;
    if(x<STREAMFORMAT_WIDTH)
    	pm.x = x;//max width
    else
    	return 0;
    if(y<STREAMFORMAT_HEIGHT)
    	pm.y = y;//max height
    else
    	return 0;
    	
    if((x+w)<STREAMFORMAT_WIDTH)
    	pm.width = w;//max width
    else
    	pm.width = STREAMFORMAT_WIDTH;//max width
    if((y+h)<STREAMFORMAT_HEIGHT)
    	pm.height = h;//max height
    else
    	pm.height = STREAMFORMAT_HEIGHT;//max height
    	
    if(w==0&&h==0)
    	pm.action = 1;
    else
    	pm.action = 0;
    pm.color = 0;
    DBGPRINT("\n---------pm-----------\n"
            "witdh  :            %d\n"
            "height  :           %d\n"
            "x  :                %d\n"
            "y  :                %d\n"
            "action  :           %d\n"
            "color  :            %d\n"
            "----------------------\n",
            pm.width, pm.height, pm.x,
            pm.y, pm.action, pm.color
            );


    switch(pm.color) // convert to rgb888
    {
        case 0:
            pm.color = GAPP_PM_COLOUR_BLACK;
            break;
        case 1:
            pm.color = GAPP_PM_COLOUR_READ;
            break;
        case 2:
            pm.color = GAPP_PM_COLOUR_BLUE;
            break;
        case 3:
            pm.color = GAPP_PM_COLOUR_GREEN;
            break;
        case 4:
            pm.color = GAPP_PM_COLOUR_YELLOW;
            break;
        case 5:
            pm.color = GAPP_PM_COLOUR_MAGENTA;
            break;
        case 6:
            pm.color = GAPP_PM_COLOUR_CYAN;
            break;
        case 7:
            pm.color = GAPP_PM_COLOUR_WHITE;
            break;
        default:
            pm.color = GAPP_PM_COLOUR_WHITE;
            break;

    }
    wbs_add_pm(&pm);
    return 0;
}

int goke_gpio_get(int number,int *val)
{
	GADI_ERR errorCode = GADI_OK;
	GADI_GPIO_OpenParam config;
  GADI_S32 value;
	
  if(!gpioHandle[number])
  {
		config.num_gpio   = gpio_id[number];
		config.active_low = 0;
		config.direction  = 0;
		config.value      = 0;
		gpioHandle[number] = gadi_gpio_open(&errorCode, &config);
		if (errorCode != GADI_OK)
		{
		  DBGPRINT("gadi_gpio_open(number) failed.\n");
		  return errorCode;
		}
  }

	errorCode = gadi_gpio_read_value(gpioHandle[number], &value);
	if (errorCode != GADI_OK)
	{
		DBGPRINT("gadi_gpio_read_value(number) failed.\n");
		return errorCode;
	}
  
  *val=value;
  
  return 0;
}

int goke_gpio_set(int number,int flag)
{
	GADI_ERR errorCode = GADI_OK;
	GADI_GPIO_OpenParam config;
	
  if(!gpioHandle[number])
  {
		config.num_gpio   = gpio_id[number];
		config.active_low = 0;
		config.direction  = 1;
		if(flag)
			config.value      = 1;
		else
			config.value      = 0;
		gpioHandle[number] = gadi_gpio_open(&errorCode, &config);
		if (errorCode != GADI_OK)
		{
		  DBGPRINT("gadi_gpio_open(number) failed.\n");
		  return errorCode;
		}
  }
  else
  {
		if(flag)
		{
			errorCode = gadi_gpio_set(gpioHandle[number]);
			if (errorCode != GADI_OK)
			{
			    DBGPRINT("gadi_gpio_set(number) failed.\n");
			    return errorCode;
			}
		}
		else
		{
			errorCode = gadi_gpio_clear(gpioHandle[number]);
			if (errorCode != GADI_OK)
			{
			    DBGPRINT("gadi_gpio_clear(number) failed.\n");
			    return errorCode;
			}
		}
  }
}

int goke_isp_init(void)
{
  GADI_ERR ret=GADI_OK;
//	GADI_ISP_OpenParamsT    openParams;
//  ret=gadi_isp_init();
//  if(ret != GADI_OK)
//  	DBGPRINT("gadi_isp_init() failed! ret = %d\n", ret);
//	openParams.ispModuleIndex = 0;
//	goke_ispHandle = gadi_isp_open(&openParams, &ret);
//	if(goke_ispHandle == NULL || ret != GADI_OK)
//		DBGPRINT("gadi_isp_open() failed! ret = %d\n", ret);

    return ret;
}

int goke_dev_ctrl(int number,int *val)
{
	int value=*val;
  GADI_GPIO_AdcValue adcValue;
	GADI_ERR  ret = GADI_OK;
//	GADI_ISP_OpenParamsT  openParams;
	
	switch(number)
	{
		case 0x100://ir_ctrl(val);
			value=*val;
			DBGPRINT("************** flag_ircut_cfg:%d ************* \n",flag_ircut_cfg);
			if(flag_ircut_cfg)
			{
				if(value)
				{
					goke_gpio_set(GOKE_GPIO_IRCU_A,0);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_IRCU_B,1);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_LED,1);
				}
				else
				{
					goke_gpio_set(GOKE_GPIO_IRCU_A,1);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_IRCU_B,0);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_LED,0);
				}
			}
			else
			{
				if(value)
				{
					goke_gpio_set(GOKE_GPIO_IRCU_A,1);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_IRCU_B,0);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_LED,1);
				}
				else
				{
					goke_gpio_set(GOKE_GPIO_IRCU_A,0);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_IRCU_B,1);
					usleep(5000);
					goke_gpio_set(GOKE_GPIO_LED,0);
				}
			}
			usleep(5000);
			goke_gpio_set(GOKE_GPIO_IRCU_A,0);
			usleep(5000);
			goke_gpio_set(GOKE_GPIO_IRCU_B,0);
			DBGPRINT("************** flag_ircut_cfg:%d ************* \n",flag_ircut_cfg);
			break;
		case 0x200://gpio_reset();
			break;
		case 0x300://adc_val_get();
			if(flag_adc_cfg)
			{
				if(goke_gpio_get(GOKE_GPIO_ADC,&value))
					return 1;
				else
				{
					if(value)
					{
						if(flag_light_cfg)
						*val=0xc;
					else
						*val=0x3e8;
					}
					else{
						if(flag_light_cfg)
							*val=0x3e8;
						else
							*val=0xc;
						}
				}
			}
			else
			{
	      adcValue.channel = GADI_GPIO_ADC_CHANNEL_ONE;
	      adcValue.value = 0;
				if(gadi_gpio_read_adc(&adcValue) == GADI_OK)
					*val=adcValue.value;
				else
	      	return 1;
			}
			break;
		case 0x400://gpio_sensor();
			if(goke_gpio_get(GOKE_GPIO_SENSOR_IN,val))
				return 1;
			break;
		case 0x500://gpio_alarm(val);
			value=*val;
			goke_gpio_set(GOKE_GPIO_ALARM_OUT,value);
			break;
	}
	return 0;
}
#endif //GOKE_DEV
