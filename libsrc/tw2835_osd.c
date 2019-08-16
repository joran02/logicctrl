#include	"cmn.h"

#define CHARBITS_NUM   96

int fosd_fd[3] = { -1,-1 ,-1};

char version[30];
unsigned char camtitle_buf[24*32*50*2+3];

int g_sub_video_w=0;

U8	GetCodeLength(unsigned char *_str)
{
	 U8 _cnt_=0;

  while((*_str++)>=' ')++_cnt_;
  return (_cnt_);
}

U8 R1555( U16 clr )
{
       return  (clr  &   0x7C00 )  >>   7 ;
} 
 
U8 G1555( U16 clr )
{
       return  (clr  &   0x03E0 )  >>   2 ;
} 
 
U8 B1555( U16 clr )
{
       return  (clr  &   0x001F )  <<   3 ;
} 
 
U8 A1555( U16 clr )
{
       return  clr  &   0x8000   ?   255  :  0 ;
} 

U16 _24_to_1555( U8 r, U8 g, U8 b )
{
       return  (  0x8000   |  ((U16)(r >> 3 ) << 10 )  |  ((U16)(g >> 3 ) << 5 )  |  ((U16)b >> 3 ) ); 
} 


///////////////////////////////////////
void char_load()
{
	int ret;
  FILE *fp;
  char str[100];
  
  memset(str,0,sizeof(str));
  if(flag_1080p || flag_ov2710)
    sprintf(str,"/mnt/mtd/24x32/char.bits");
  else
    sprintf(str,"/mnt/mtd/char.bits");
//  fp = fopen("char.bits", "rb");
  fp = fopen(str, "rb");
  if(NULL == fp)
  {
    DBGPRINT("char.bits failed!\n");
    return ;
  }
  fread(&ret, 1,4, fp);
  fread(&s_CharEn.w, 1,4, fp);
  fread(&s_CharEn.h, 1,4, fp);
  fread(&ret, 1,4, fp);
  DBGPRINT("*************** s_CharEn.w:%d  s_CharEn.h:%d\n",s_CharEn.w,s_CharEn.h);
    if(s_CharEn.buf==NULL)
        s_CharEn.buf=malloc(s_CharEn.w*s_CharEn.h*2);
    else
    {
        free(s_CharEn.buf);
        s_CharEn.buf=malloc(s_CharEn.w*s_CharEn.h*2);
    }
    if(s_CharEn.buf!=NULL)
    fread(s_CharEn.buf, 1,s_CharEn.w*s_CharEn.h*2, fp);
    fclose(fp);


  memset(str,0,sizeof(str));
  if(flag_1080p || flag_ov2710)
    sprintf(str,"/mnt/mtd/24x32/wifi_signal.bits");
  else
    sprintf(str,"/mnt/mtd/wifi_signal.bits");
  fp = fopen(str, "rb");
  if(NULL == fp)
  {
    DBGPRINT("wifi_signal.bits failed!\n");
    return ;
  }
  fread(&ret, 1,4, fp);
  fread(&s_CharEn_1.w, 1,4, fp);
  fread(&s_CharEn_1.h, 1,4, fp);
  fread(&ret, 1,4, fp);
  DBGPRINT("*************** s_CharEn_1.w:%d  s_CharEn_1.h:%d\n",s_CharEn_1.w,s_CharEn_1.h);
    if(s_CharEn_1.buf==NULL)
        s_CharEn_1.buf=malloc(s_CharEn_1.w*s_CharEn_1.h*2);
    else
    {
        free(s_CharEn_1.buf);
        s_CharEn_1.buf=malloc(s_CharEn_1.w*s_CharEn_1.h*2);
    }
    if(s_CharEn_1.buf!=NULL)
    fread(s_CharEn_1.buf, 1,s_CharEn_1.w*s_CharEn_1.h*2, fp);
    fclose(fp);
}
int osd_image_display_ex(int x,int y,int pic_line_length,int image_width,int image_height,unsigned char *image_data,int len)
{
  int j;
	HI_CHAR *pDst = NULL;
	
	pDst = &iw_info_osd_buf[x*2+len];
	for(j=0; j<image_height; j++)
	{
		memcpy(pDst,&image_data[pic_line_length*j],image_width*2);
		pDst += len;
	}

	return 0;
}
int camtitle_osd_image_display_ex(int x,int y,int pic_line_length,int image_width,int image_height,unsigned char *image_data,int len)
{
  int j;
    HI_CHAR *pDst = NULL;

    pDst = &camtitle_buf[3+x*2+len];
    DBGPRINT("*********** image_width:%d, image_height:%d,pic_line_length:%d\n",image_width,image_height,pic_line_length);
    for(j=0; j<image_height; j++)
    {
        memcpy(pDst,&image_data[pic_line_length*j],image_width*2);
        pDst += len;
    }

	return 0;
}
void camtitle_char_line_display(char *str)
{   int i,len,j;
    int x,y;
    unsigned char *p=NULL;

    len=strlen(str);
    if(len>5)
        len=5;
    memset(camtitle_buf,0,sizeof(camtitle_buf));
    for(i=0,x=0;i<len;i++)
    {
        j=str[i]-0x20;
        if(s_CharEn.buf==NULL)
            return ;
        p=&s_CharEn.buf[j*(s_CharEn.w/CHARBITS_NUM)*2];
        camtitle_osd_image_display_ex(x,0,s_CharEn.w*2,s_CharEn.w/CHARBITS_NUM,s_CharEn.h,p,(s_CharEn.w/CHARBITS_NUM)*len*2);
        x+=(s_CharEn.w/CHARBITS_NUM);
    }
    camtitle_buf[0]=1;
    camtitle_buf[1]=(s_CharEn.w/CHARBITS_NUM)*len;
    camtitle_buf[2]=s_CharEn.h;

    switch(g_record_display_info)
    {
        case 'W':
            cam_name_rec_display(1);
            break;
        case 'M':
            cam_name_rec_display(2);
            break;
        case 'S':
            cam_name_rec_display(3);
            break;
        case 'T':
            cam_name_rec_display(4);
            break;
        case 'N':
            cam_name_rec_display(5);
            break;
    }
}
void char_line_display(char *str)
{
    int i,len,j;
    int x,y,z;
    unsigned char *p=NULL;

    len=strlen(str);
    if(len>30)
        len=30;

    memset(iw_info_osd_buf,0,sizeof(iw_info_osd_buf));
    for(i=0,x=0;i<len;i++)
    {
        j=str[i]-0x20;
        if(s_CharEn.buf==NULL)
            return ;
        p=&s_CharEn.buf[j*(s_CharEn.w/CHARBITS_NUM)*2];
        osd_image_display_ex(x,0,s_CharEn.w*2,s_CharEn.w/CHARBITS_NUM,s_CharEn.h,p,(s_CharEn.w/CHARBITS_NUM)*len*2);
        x+=(s_CharEn.w/CHARBITS_NUM);
    }
    iw_info_osd_buf_w=(s_CharEn.w/CHARBITS_NUM)*len;
    iw_info_osd_buf_h=s_CharEn.h;

    if(s_CharEn_1.buf==NULL)
        return ;
    p=&s_CharEn_1.buf[iw_signalval*(s_CharEn_1.w/5)*2];
    osd_image_display_ex(x-(s_CharEn_1.w/5)-2,0,s_CharEn_1.w*2,s_CharEn_1.w/5,s_CharEn_1.h,p,(s_CharEn.w/CHARBITS_NUM)*len*2);
}
int set_osd_image_display_info(int layer,int x,int y,int pic_line_length,int image_width,int image_height,unsigned char *image_data,int len)
{
  int j;
	HI_CHAR *pDst = NULL;
	
	pDst = &osd_info_buf[layer][x*2+len];
	for(j=0; j<image_height; j++)
	{
		memcpy(pDst,&image_data[pic_line_length*j],image_width*2);
		pDst += len;
	}

	return 0;
}
void set_osd_display(int layer,int mode,int x,int y,int x1,int y1,int flag,char *osd_prt)
{
  HI_S32 s32Ret = HI_FAILURE;
  RGN_ATTR_S stRgnAttr;
  RGN_CHN_ATTR_S stChnAttr;
    int i,j,z,w=0,h=0,len;
    unsigned char *p=NULL;
	#define Y_OFFSET 100

    DBGPRINT("layer:%d,x:%d,y:%d,flag:%d,str:%s \n",layer,x,y,flag,osd_prt);
    len=strlen(osd_prt);
    if(len>OSD_STR_LEN_MAX)
        len=OSD_STR_LEN_MAX;

    if(layer>RGN_NUM_MAX)
    {
			DBGPRINT("**** layer:%d ,RGN_NUM_MAX:%d *** \n",layer,RGN_NUM_MAX);
        return;
    }

    if(flag)
    {
        if(s_CharEn.buf==NULL)
            return ;
        w=(s_CharEn.w/CHARBITS_NUM)*len;
        h=s_CharEn.h;
        if(rgn_osd_info_w[layer]!=w || rgn_osd_info_h[layer]!=h)
        {
            rgn_osd_info_w[layer]=w;
            rgn_osd_info_h[layer]=h;
			
        DBGPRINT("set_osd_display HI_MPI_RGN_Create = %d \n",layer);

            HI_MPI_RGN_Destroy(RgnHandle[layer]);

            RgnHandle[layer]=layer;
            stRgnAttr.enType = OVERLAY_RGN;
          stRgnAttr.unAttr.stOverlay.enPixelFmt = PIXEL_FORMAT_RGB_1555;
          stRgnAttr.unAttr.stOverlay.stSize.u32Width  = w;
          stRgnAttr.unAttr.stOverlay.stSize.u32Height = h;
          stRgnAttr.unAttr.stOverlay.u32BgColor = 0;

          s32Ret = HI_MPI_RGN_Create(RgnHandle[layer], &stRgnAttr);
          if(HI_SUCCESS != s32Ret)
          {
            DBGPRINT("HI_MPI_RGN_Create (%d) failed with %#x!\n",RgnHandle[layer], s32Ret);
            return ;
          }
          if(mode>=2)
          {
              stChn[layer].s32DevId = 0;
              memset(&stChnAttr,0,sizeof(stChnAttr));
              stChnAttr.bShow = HI_TRUE;
              stChnAttr.enType = OVERLAY_RGN;
              stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = (x&(~0x03));
              stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = (y&(~0x03))+Y_OFFSET;
              stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
              stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
              stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;

              stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
              stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;

              s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[layer], &stChn[layer], &stChnAttr);
              if(HI_SUCCESS != s32Ret)
              {
                DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
                return ;
              }
////////////////////////////

            stChn[layer].s32DevId = 1;
              memset(&stChnAttr,0,sizeof(stChnAttr));
              stChnAttr.bShow = HI_TRUE;
              stChnAttr.enType = OVERLAY_RGN;
              stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = (x1&(~0x03));
              stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = (y1&(~0x03))+Y_OFFSET;
              stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
              stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
              stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;

              stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
              stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;

              s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[layer], &stChn[layer], &stChnAttr);
              if(HI_SUCCESS != s32Ret)
              {
                DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
                return ;
              }
////////////////////////////
            }
            else
            {
              stChn[layer].s32DevId = mode;
              memset(&stChnAttr,0,sizeof(stChnAttr));
              stChnAttr.bShow = HI_TRUE;
              stChnAttr.enType = OVERLAY_RGN;
              stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = (x&(~0x03));
              stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = (y&(~0x03))+Y_OFFSET;
              stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
              stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
              stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;

              stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
              stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;

              s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[layer], &stChn[layer], &stChnAttr);
              if(HI_SUCCESS != s32Ret)
              {
                DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
                return ;
              }
            }
        }
        else
        {
          memset(&stChnAttr,0,sizeof(stChnAttr));
          stChnAttr.bShow = HI_TRUE;
          stChnAttr.enType = OVERLAY_RGN;
          stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = (x&(~0x03));
          stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = (y&(~0x03))+Y_OFFSET;
          stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;//80;
          stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
          stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;

          stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
          stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;

          s32Ret = HI_MPI_RGN_SetDisplayAttr(RgnHandle[layer], &stChn[layer], &stChnAttr);
          if(HI_SUCCESS != s32Ret)
          {
              DBGPRINT("HI_MPI_RGN_SetDisplayAttr (%d) failed with %#x!\n",RgnHandle[layer], s32Ret);
              return ;
          }
        }
        memset(&osd_info_buf[layer],0,24*24*OSD_STR_LEN_MAX*2);
        for(i=0,z=0;i<len;i++)
        {
            j=osd_prt[i]-0x20;
            p=&s_CharEn.buf[j*(s_CharEn.w/CHARBITS_NUM)*2];
            set_osd_image_display_info(layer,z,0,s_CharEn.w*2,s_CharEn.w/CHARBITS_NUM,s_CharEn.h,p,(s_CharEn.w/CHARBITS_NUM)*len*2);
            z+=(s_CharEn.w/CHARBITS_NUM);
        }
        osd_info_buf_w[layer]=(s_CharEn.w/CHARBITS_NUM)*len;
        osd_info_buf_h[layer]=s_CharEn.h;

        info_osd_str.pData = &osd_info_buf[layer];
        info_osd_str.u32Width = osd_info_buf_w[layer];
        info_osd_str.u32Height = osd_info_buf_h[layer];
        info_osd_str.enPixelFormat = PIXEL_FORMAT_RGB_1555;


		
      s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle[layer],&info_osd_str);
      if(HI_SUCCESS != s32Ret)
      {
          DBGPRINT("HI_MPI_RGN_SetBitMap (%d) failed with %#x!\n",RgnHandle[layer], s32Ret);
          return ;
      }
    }
    else
    {
      memset(&stChnAttr,0,sizeof(stChnAttr));
      stChnAttr.bShow = HI_FALSE;
      stChnAttr.enType = OVERLAY_RGN;
      stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = (x&(~0x03));
      stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = (y&(~0x03))+Y_OFFSET;
      stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;//80;
      stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
      stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;

      stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
      stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;

	  DBGPRINT("HI_MPI_RGN_SetDisplayAttr %d\n", layer);
      s32Ret = HI_MPI_RGN_SetDisplayAttr(RgnHandle[layer], &stChn[layer], &stChnAttr);
      if(HI_SUCCESS != s32Ret)
      {
          DBGPRINT("HI_MPI_RGN_SetDisplayAttr (%d) failed with %#x!\n",RgnHandle[layer], s32Ret);
          return ;
      }
    }
}
////////////////////////////
void clear_osd(int x,int y,int w,int h,U8 layer)
{
# if 0
	int i,j;
	unsigned short* tmfrbuf=(unsigned short*)osdlayerinfo.osd_buf;
	if (layer == AD)
		tmfrbuf = (unsigned short*)osdlayerinfo.osd_buf;
	else if (layer == MD)
		tmfrbuf = (unsigned short*)mouselayerinfo.osd_buf;
	else if (layer == HD)
		tmfrbuf = (unsigned short*)infoshow.osd_buf;
	else if (layer == HMD)
		tmfrbuf = (unsigned short*)mouselayerhd.osd_buf;
	for (i=y;i<y+h;i++)
	{
		if (i>High_Resolution && layer!=HD)
		{
			break;
		}
		switch (layer)
		{
			case AD:
				tmfrbuf = (unsigned short*)(osdlayerinfo.osd_buf+i*osdlayerinfo.fixinfo.line_length);
				break;
			case MD:
				tmfrbuf = (unsigned short*)(mouselayerinfo.osd_buf+i*mouselayerinfo.fixinfo.line_length);
				break;
			case HD:
				tmfrbuf = (unsigned short*)(infoshow.osd_buf+i*infoshow.fixinfo.line_length);
				break;
			case HMD:
				tmfrbuf = (unsigned short*)(mouselayerhd.osd_buf+i*mouselayerhd.fixinfo.line_length);
				break;
			default:
				break;
		}
		for (j=x;j<x+w;j++)
		{
			if (j>WIDTH_RESOLUTION && layer!=HD)
			{
				break;
			}
			*(tmfrbuf+j) = 0x7c00;
		}
	}
#endif
}

void draw_line(int x,int y,int x1,int y1,int minx,int miny,int maxx,int maxy,U8 layer,U16 color)
{
#if 0	

	int i,j;
	int tmp;
	unsigned short* tmfrbuf=(unsigned short*)osdlayerinfo.osd_buf;
	if (x>x1)
	{
		tmp = x;
		x = x1;
		x1 = tmp;
	}
	if (y>y1)
	{
		tmp = y;
		y = y1;
		y1 = tmp;
	}
	for (i=miny;i<maxy+1;i++)
	{
		if (layer == AD)
			tmfrbuf = (unsigned short*)(osdlayerinfo.osd_buf+i*osdlayerinfo.fixinfo.line_length);
		else if (layer == HD)
			tmfrbuf = (unsigned short*)(infoshow.osd_buf+i*infoshow.fixinfo.line_length);
		for(j=minx;j<maxx+1;j++)
		{
			if ((j>=x && j<x+3) || (j>=x1 && j<x1+3) || (i>=y && i<y+3) || (i>=y1 && i<y1+3))
			{
				if (j>=x && j<=x1+2 && i>=y && i<=y1+2)
				{
					*(tmfrbuf+j) = color;
//					*(tmfrbuf1+j) = 0x0000;
				}
				else
				{
					*(tmfrbuf+j) = 0x7c00;
//					*(tmfrbuf1+j) = 0x7c00;
				}
			}
			else
			{
				*(tmfrbuf+j) = 0x7c00;
//				*(tmfrbuf1+j) = 0x7c00;
			}
		}
	}
#endif
}
BXS_PIC1* getyuvfile(U8* buffile,char* strbuf)
{

	if (buffile==NULL)
	{
		printf(">>>>>>>>>>>>> function %s error!\n",strbuf);
		return NULL;
	}
	return (BXS_PIC1*)buffile;
}


void settransparence(unsigned short* buf,int length)
{
	#if 0
	int i;
//	U8 tmp[2] = {0x00,0xfc};
	if (buf == NULL)
	{
		printf("function settransparence buf is null!\n");
		return;
	}
	for (i=0;i<length;i++)
		*(buf+i)=0xfc00;
	#endif

}

void osd_bmp_photo(unsigned char * yuv_buf,int point_x,int point_y,int standard,int start_x,int start_y,
					int pic_width,int pic_height,int transparence_color,int transparence_degree)
{

#if 0
	int i, m,n;
	unsigned short* tmp;
	unsigned short* pvideo;
	if (yuv_buf == NULL)
	{
		printf("function osd_bmp_photo yuv_buf is null!\n");
		return;
	}
	tmp = (unsigned short*)yuv_buf;
	n = *(tmp+2);

	for (i=point_y,m = start_y; i<point_y+pic_height; i++,m++)
	{
		if (standard==HD)
		{
			if (i>HIGH_RESOLUTION_HD)
				break;
			pvideo = (unsigned short*)(infoshow.osd_buf + i*infoshow.fixinfo.line_length);
			if (pic_width>WIDTH_RESOLUTION_HD)
				pic_width=WIDTH_RESOLUTION_HD;
			if (pic_height>HIGH_RESOLUTION_HD)
				pic_height=HIGH_RESOLUTION_HD;
		}
		else
		{
			if (i>High_Resolution)
				break;
			if(standard==AD)
				pvideo = (unsigned short*)(osdlayerinfo.osd_buf + i*osdlayerinfo.fixinfo.line_length);
			else
				pvideo = (unsigned short*)(osdspot.osd_buf + i*osdspot.fixinfo.line_length);
		}
		if (transparence_color == MIXCOLOR && transparence_degree == 0)
		{
			settransparence(pvideo+point_x,pic_width);
		}
		else
		{
			tmp = (unsigned short*) (yuv_buf+8 + m*n*2);
			memcpy(pvideo+point_x,tmp+start_x,pic_width*2);
		}
	}
#endif
}

void color_full(U16 x,U16 y,U16 w,U16 h,U16 color,U8 layer)
{
#if 0
	U16 i,j;
	US* pvideo=(unsigned short*)osdlayerinfo.osd_buf;
	for (i=y;i<y+h;i++)
	{
		if (layer == AD)	
			pvideo = (US*)(osdlayerinfo.osd_buf + i*osdlayerinfo.fixinfo.line_length);
		else if (layer == HD)	
			pvideo = (US*)(infoshow.osd_buf + i*infoshow.fixinfo.line_length);
		else
			pvideo = (US*)(osdspot.osd_buf + i*osdspot.fixinfo.line_length);
		for (j=x;j<x+w;j++)
			*(pvideo+j) = color;
	}
#endif
	
}

void Bmp_All_Show(int x,int y,U8* buf,U8 layer)
{
#if 0
	BXS_PIC1 * bxs_pic = NULL;

	if (buf == NULL)
	{
		printf("function Bmp_All_Show buf is null!\n");
		return;
	}
	bxs_pic = ( BXS_PIC1 * )buf;
	if (layer == AD)
	{
		if (x<0 && y<0)
			osd_bmp_photo(buf,(WIDTH_RESOLUTION-bxs_pic->pic_width)/2,(High_Resolution-bxs_pic->pic_height)/2,AD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else if (x<0)
			osd_bmp_photo(buf,(WIDTH_RESOLUTION-bxs_pic->pic_width)/2,y,AD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else if (y<0)
			osd_bmp_photo(buf,x,(High_Resolution-bxs_pic->pic_height)/2,AD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else
			osd_bmp_photo(buf,x,y,AD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
	}
	else if (layer == SD)
	{
		if (x<0 && y<0)
			osd_bmp_photo(buf,(WIDTH_RESOLUTION-bxs_pic->pic_width)/2,(High_Resolution-bxs_pic->pic_height)/2,SD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else if (x<0)
			osd_bmp_photo(buf,(WIDTH_RESOLUTION-bxs_pic->pic_width)/2,y,SD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else if (y<0)
			osd_bmp_photo(buf,x,(High_Resolution-bxs_pic->pic_height)/2,SD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else
			osd_bmp_photo(buf,x,y,SD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
	}
	else if (layer == HD)
	{
		if (x<0 && y<0)
			osd_bmp_photo(buf,(WIDTH_RESOLUTION_HD-bxs_pic->pic_width)/2,(HIGH_RESOLUTION_HD-bxs_pic->pic_height)/2,HD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else if (x<0)
			osd_bmp_photo(buf,(WIDTH_RESOLUTION_HD-bxs_pic->pic_width)/2,y,HD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else if (y<0)
			osd_bmp_photo(buf,x,(HIGH_RESOLUTION_HD-bxs_pic->pic_height)/2,HD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
		else
			osd_bmp_photo(buf,x,y,HD,0,0,bxs_pic->pic_width,bxs_pic->pic_height,MIXCOLOR,15);
	}
#endif
	
}

void Bmp_Part_Show(int x,int y,int starx, int stary, int width, int high, U8* buf,U8 layer)
{
#if 0
	if (buf == NULL)
	{
		printf("function Bmp_Part_Show buf is null!\n");
		return;
	}
	osd_bmp_photo(buf,x,y,layer,starx,stary,width,high,MIXCOLOR,15);
#endif

}
U8 infoshowflag = 0;

void OSD_ALL_CLEAR()
{
#if 0
	clear_osd(0,0,WIDTH_RESOLUTION,High_Resolution,AD);
	clear_osd(0,0,WIDTH_RESOLUTION_HD,HIGH_RESOLUTION_HD,HD);
	infoshowhide(0);
//	gstCommonParm->GST_OSD_clear_osd(0xff);
#endif

}

void Osd_line_display(U16 posX,U16 posY,U8 color,U8* pchar, U8 fond_index,U8 layer)
{
#if 0
	U8 lngth_ = GetCodeLength(pchar);
	U8 indx;
	U8 i;
	U16 wordwidth,wordhigh;
	U8 tmp[2];
	BXS_PIC1 * bxs_pic = NULL;

	if (pchar == NULL)
	{
		printf("function Osd_line_display pchar is null!\n");
		return;
	}
	if (color == RED)
	{
		if (systemmenupic.numcharfore == NULL)
		{
			printf("function Osd_line_display systemmenupic.numcharfore is null!\n");
			return;
		}
		bxs_pic = getyuvfile(systemmenupic.numcharfore,"systemmenupic.numcharfore");
		wordwidth = bxs_pic->pic_width/8;
		wordhigh = bxs_pic->pic_height/12;
		for(i=0;i<lngth_;i++)
		{
			indx = pchar[i]-0x20;
			tmp[0] = indx/8;
			tmp[1] = indx%8;
			osd_bmp_photo(systemmenupic.numcharfore,posX+i*wordwidth,posY,layer,
						tmp[1]*wordwidth,tmp[0]*wordhigh,wordwidth,wordhigh,RGB(190, 190, 190),fond_index);
		}
	}
	else
	{
		if (systemmenupic.numcharlocal == NULL || systemmenupic.numcharback==NULL)
		{
			printf("function Osd_line_display systemmenupic.numcharlocal or systemmenupic.numcharback is null!\n");
			return;
		}
		if (fond_index == 0xff)
			bxs_pic = getyuvfile(systemmenupic.numcharlocal,"systemmenupic.numcharlocal");
		else
			bxs_pic = getyuvfile(systemmenupic.numcharback,"systemmenupic.numcharlocal");
		wordwidth = bxs_pic->pic_width/8;
		wordhigh = bxs_pic->pic_height/12;
		for(i=0;i<lngth_;i++)
		{
			indx = pchar[i]-0x20;
			tmp[0] = indx/8;
			tmp[1] = indx%8;
			if (fond_index == 0xff)
				osd_bmp_photo(systemmenupic.numcharlocal,posX+i*wordwidth,posY,layer,
							tmp[1]*wordwidth,tmp[0]*wordhigh,wordwidth,wordhigh,RGB(190, 190, 190),fond_index);
			else
				osd_bmp_photo(systemmenupic.numcharback,posX+i*wordwidth,posY,layer,
							tmp[1]*wordwidth,tmp[0]*wordhigh,wordwidth,wordhigh,RGB(190, 190, 190),fond_index);
		}
	}
	#endif

}

void Osd_char_clear(U16 posX,U16 posY,U8 color,U8 length,U8 index,U8 layer)
{
	 U8 temp[] = "                                        ";
	temp[length] = '\0';
	Osd_line_display(posX,posY,color,temp,0,layer);
}

void Osd_One_display(U16 posX,U16 posY,U8 color,U8 value, U8 langue,U8 layer)
{
	 U8 temp[2];//char
	temp[0]=value;
	temp[1]='\0';
	if (temp[0] <10)
		temp[0] += 0x30;
	Osd_line_display(posX,posY,color,temp,langue,layer);
}

void Osd_Dec2_display(U16 posX,U16 posY,U8 color,U8 value,U8 index,U8 layer)
{
	 U8 temp[3];
	temp[0]=(value/10)+0x30;
	temp[1]=(value%10)+0x30;
	temp[2]='\0';
	Osd_line_display(posX,posY,color,temp,index,layer);
}

void Osd_Dec3_display(U16 posX,U16 posY,U8 color,U16 value,U8 index,U8 layer)
{
	 U8 temp[4],temp8;//,i;
	temp8=value/10;
	temp[0]=(temp8/10)+0x30;
	temp[1]=(temp8%10)+0x30;
	temp[2]=(value%10)+0x30;
	temp[3]='\0';
	Osd_line_display(posX,posY,color,temp,index,layer);
}

void Osd_Hex2_display(U16 posX,U16 posY,U8 color,U8 value,U8 index,U8 layer)
{
	 U8 temp[3];
	temp[0]=(value>>4)+0x30;
	temp[1]=(value&0x0f)+0x30;
	if (temp[0]>0x39)
		temp[0] += 0x07;
	if (temp[1]>0x39)
		temp[1] += 0x07;
	temp[2]='\0';
	Osd_line_display(posX,posY,color,temp,index,layer);
}

void OsdY_line_display(U8 ch,U8 timeposx,U8* pchar, U8 fond_index)
{
}

void set_osdAlpha(U8 alpha,U8 layer)
{
}

int OsdLayerInit()
{	
	return 1;
}

void clear_info()
{
#if 0
	int i,j;
	unsigned short* tmfrbuf;
	BXS_PIC1 * bxs_pic = NULL;
	if (systemmenupic.promptchar == NULL)
	{
		printf("function clear_info systemmenupic.promptchar is null!\n");
		return;
	}
	bxs_pic = getyuvfile(systemmenupic.promptchar,"systemmenupic.promptchar");

	for (i=0;i<bxs_pic->pic_height/28;i++)
	{
		if (MseType == 0)
			tmfrbuf = (unsigned short*)(mouselayerinfo.osd_buf+(i+32)*mouselayerinfo.fixinfo.line_length);
		else
			tmfrbuf = (unsigned short*)(mouselayerhd.osd_buf+(i+32)*mouselayerhd.fixinfo.line_length);
		for (j=0;j<bxs_pic->pic_width;j++)
		{
			*(tmfrbuf+j) = 0x7c00;
		}
	}
	infoshowflag = 0;
#endif
	
}

void infoshowhide(U8 flag)
{
#if 0
	if (!flag)
		clear_info();
//#if 0
	HI_BOOL bShow= flag;
	if (!infoshowflag)
		return;
	if (ioctl(infoshow.deviceid, FBIOPUT_SHOW_HIFB, &bShow) < 0)
	{
		printf("Put devicepath: %s variable screen info failed!\n",infoshow.devicepath);
	}
#endif

}

void infoup(U16 x,U16 y,U8 index)
{
#if 0
//	HIFB_POINT_S stPoint = {x, y};
	BXS_PIC1 * bxs_pic = NULL;
	unsigned short* tmp = NULL;
	unsigned short* pvideo=NULL;
	clear_info();
	bxs_pic = getyuvfile(systemmenupic.promptchar,"systemmenupic.promptchar");
	tmp = (unsigned short*)systemmenupic.promptchar;
	infoshowflag = 1;
	tmp = (unsigned short*) (systemmenupic.promptchar+8 + index*bxs_pic->pic_height*bxs_pic->pic_width/14);
	if (MseType==0)
	{
		pvideo = (unsigned short*)mouselayerinfo.osd_buf + 32*bxs_pic->pic_width;
		memcpy(pvideo,tmp,bxs_pic->pic_height*bxs_pic->pic_width/14);
	}
	else
	{
		pvideo = (unsigned short*)mouselayerhd.osd_buf + 32*bxs_pic->pic_width;
		memcpy(pvideo,tmp,bxs_pic->pic_height*bxs_pic->pic_width/14);
	}
/*
	if (ioctl(infoshow.deviceid, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0)
	{
		printf("info flesh failur !\n");
	}
*/
#endif
	
}

int Initialinfolayer()
{
	return 1;
}
/*
int FindSTR1( char  * byhtml, char *cfind, int nStartPos)
{
	int i;
	long length;
	char * tmphtml;
	char * temfind ;

	
	if( byhtml == NULL || cfind == NULL)
		return -1;

	tmphtml =(char *) byhtml + nStartPos;

	temfind = cfind;

	if(nStartPos < 0)
		return -1;

	length = strlen(tmphtml); 
	
	for( i = 0 ; i < length; i++)
	{		

		if( (*tmphtml) != (*temfind) )
		{
			tmphtml++;

			continue;
		}

		while( (*tmphtml) == (*temfind) )
		{
			//printf(" %c.%c|",*tmphtml,*temfind);
			tmphtml++;
			temfind++;

			if( (*temfind) == NULL ) // 找到了。
			{			
				return nStartPos + i;
			}
		}

		//printf("\n");	
		
		if( (*temfind) == NULL ) // 找到了。
		{			
			return nStartPos + i;
		}
		else
		{	// 从与temfind首字节相同的那一位的后一位重新开始找，
			temfind = cfind;
			tmphtml = (char *)byhtml + nStartPos + i + 1;
		}
	}

	return -1;
}
*/
void getmousefile(unsigned char * pRGBBuf)
{
}
/*
int numcharsize;
int recnamesize;
void Load_Menu_Pic(U8** pic_buf,char* filename)
{
	FILE * fp = NULL;
	int ret ;
//	BXS_PIC1 * bxs_pic = NULL;
	int offset;
	
	fp = fopen(filename,"rb");

	if( fp  == NULL )
	{
		printf("open file %s error!\n",filename);
		goto open_faild;
	}

	if( fseek(fp ,0L,SEEK_END) != 0 )
	{
		printf("fseek file is wrong!\n");
		goto open_faild;
	}

	offset = ftell(fp);
	if( offset == -1 )
	{
		printf(" ftell error!\n");
		goto open_faild;
	}
	
	rewind(fp);	
	if( *pic_buf == NULL )
	{
//		printf("**malloc!\n");
		*pic_buf = (unsigned char *)malloc(offset);
	}
//	printf("**malloc ok!\n");

	if( *pic_buf == NULL )
	{
		printf("pic_buf malloc error!\n");
		goto open_faild;
	}

	ret = fread(*pic_buf,1,offset,fp);
	if( ret != offset )
	{
		printf("read bmp data error!\n");
		goto open_faild;
	}

	
	fclose(fp);
	fp = NULL;	
//	bxs_pic = *pic_buf;
//	printf("h:%d,w:%d\n",bxs_pic->pic_height,bxs_pic->pic_width);
	ret = FindSTR1(filename,"numcharback.rgb",0);
	if (ret>=0)
		numcharsize = offset;
	ret = FindSTR1(filename,"recname.rgb",0);
	if (ret>=0)
		recnamesize= offset;
	
	ret = FindSTR1(filename,"mouse.rgb",0);

	if( ret >= 0 )
	{
		printf("load mouse icon file %s\n",filename);
		getmousefile(*pic_buf);
	}
//	printf("******************* return\n");
	return;
open_faild:

	if( fp )
	{
		fclose(fp);
		printf("openfile!\n");
		fp = NULL;
	}

	if( *pic_buf )
	{
		free(*pic_buf);
		printf("openfile22222222!\n");
	}
	*pic_buf = NULL;
	
}
*/

#ifdef GM8126_DEV
void set_osd_win_color(int osd_fd, int idx, int x, int y, int w, int h, char *str, int start,
                       uint8_t pal_fg, uint8_t pal_bg, uint8_t PY1, uint8_t PCb1, uint8_t PCr1, uint8_t PY2, uint8_t PCb2, uint8_t PCr2)
{
    fiosd_win_t win;
    fiosd_palette_t palette;
    fiosd_string_t string;
    fiosd_transparent_t transparent;
    int index;
    int ret;    

    /* disable OSD windows */
    index = idx;            
    ret   = ioctl(osd_fd, FIOSDS_OFF, &index);    
    if(ret < 0) {
        printf("FIOSDS_OFF Fail!");
        return;
    }
    
    /* set OSD window x,y postion and dimension */
    win.x      = x;
    win.y      = y;
    win.hdim   = w;
    win.vdim   = h;
    win.windex = idx;
    ret = ioctl(osd_fd, FIOSDS_WIN, &win);
    if(ret < 0) {
        printf("FIOSDS_WIN Fail!");
        return;
    }

    /* set OSD foreground color */
    palette.index = pal_fg;
    palette.y     = PY1;
    palette.cb    = PCb1;
    palette.cr    = PCr1;
    ret = ioctl(osd_fd, FIOSDS_PALTCOLOR, &palette);
    if(ret < 0) {
        printf("FIOSDS_PALTCOLOR 0 Fail!");
        return;
    }
    
    /* set OSD backfrpund color */           
    palette.index = pal_bg;
    palette.y     = PY2;
    palette.cb    = PCb2;
    palette.cr    = PCr2;
    ret = ioctl(osd_fd, FIOSDS_PALTCOLOR, &palette);
    if(ret < 0) {
        printf("FIOSDS_PALTCOLOR 1 Fail!");
        return;
    }
        
    /* set OSD Transparent */    
    transparent.level  = FOSD_TRANSPARENT_50PERCENT;//FOSD_TRANSPARENT_100PERCENT;
    transparent.windex = index;
    ret = ioctl(osd_fd, FIOSDS_TRANSPARENT, &transparent);
    if(ret < 0) {
        printf("FIOSDMASKS_TRANSPARENT Fail!");
        return;
    }
    
    /* set OSD window color and display string */    
    string.windex   = index;
    string.start    = start;
    string.fg_color = pal_fg;
    string.bg_color = pal_bg;
    if(str)
      strcpy(&string.string[0], str);            
    ret = ioctl(osd_fd, FIOSDS_STRING, &string);
    if(ret < 0) {
        printf("FIOSDS_STRING Fail!");
        return;
    }
    
    /* enable OSD display */        
    ret = ioctl(osd_fd, FIOSDS_ON, &index);
    if(ret < 0) {
        printf("FIOSDS_ON Fail!");
        return;
    }
}

#endif
int net_info_display(int flag)
{
#ifdef GM8126_DEV
	char buf[2][100];
	int i,j,ret,index,len;
	unsigned int tmp;
  fiosd_string_t string;
  
  if(flag)
  {
	  string.windex   = 0;
	  string.start    = 0;
	  string.fg_color = 1;
	  string.bg_color = 0;
		sprintf(&string.string[0],"%03d %03d %03d %03d",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3]);
	  ret = ioctl(fosd_fd[2], FIOSDS_STRING, &string);
	  if(ret < 0) 
	  {
	    DBGPRINT("FIOSDS 0 STRING Fail!");
	    return -1;
	  }
	  string.windex   = 1;
	  string.start    = 32;
	  string.fg_color = 1;
	  string.bg_color = 0;
		sprintf(&string.string[0],"%03d %03d %03d %03d",NetStaticMenu.SubNet[0],NetStaticMenu.SubNet[1],NetStaticMenu.SubNet[2],NetStaticMenu.SubNet[3]);
	  ret = ioctl(fosd_fd[2], FIOSDS_STRING, &string);
	  if(ret < 0) 
	  {
	    DBGPRINT("FIOSDS 0 STRING Fail!");
	    return -1;
	  }
	  string.windex   = 2;
	  string.start    = 64;
	  string.fg_color = 1;
	  string.bg_color = 0;
		sprintf(&string.string[0],"%03d %03d %03d %03d",NetStaticMenu.GateWay[0],NetStaticMenu.GateWay[1],NetStaticMenu.GateWay[2],NetStaticMenu.GateWay[3]);	
	  ret = ioctl(fosd_fd[2], FIOSDS_STRING, &string);
	  if(ret < 0) 
	  {
	    DBGPRINT("FIOSDS 0 STRING Fail!");
	    return -1;
	  }
	  string.windex   = 3;
	  string.start    = 96;
	  string.fg_color = 1;
	  string.bg_color = 0;
	  
//	  memset(buf,0,sizeof(buf));
//	  gstCommonParam->GST_NET_get_svr_mac_id(buf[0],buf[1]);
//	  
//	  memset(buf[0],0,100);
//	  len=strlen(buf[1]);
//	  for(i=0,j=0;i<len;i++)
//	  {
//	  	if(buf[1][i]>='0'&&buf[1][i]<='9')
//	  	{
//	  		buf[0][j]=buf[1][i];
//	  		j++;
//	  	}
//	  }
//	  tmp=atoi(buf[0]);
//	  
//	  memset(buf[0],0,100);
//	  sprintf(buf[0],"%08X",tmp);
//	  DBGPRINT("buf[0]:%X \n",tmp);
	  
	  sprintf(&string.string[0],"                ");
//	  memcpy(&string.string[0],buf[0],strlen(buf[0]));
	  memcpy(&string.string[0],version,6);
		sprintf(buf[0],"%d",NetStaticMenu.Port);
		memcpy(&string.string[7],buf[0],strlen(buf[0]));
//	  sprintf(&string.string[0],"%d",NetStaticMenu.Port);
//		sprintf(&string.string[0],"%03d %03d %03d %03d",NetStaticMenu.DDNS1[0],NetStaticMenu.DDNS1[1],NetStaticMenu.DDNS1[2],NetStaticMenu.DDNS1[3]);
	  ret = ioctl(fosd_fd[2], FIOSDS_STRING, &string);
	  if(ret < 0) 
	  {
	    DBGPRINT("FIOSDS 0 STRING Fail!");
	    return -1;
	  }
    index = 0;            
    ret=ioctl(fosd_fd[2], FIOSDS_ON, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_ON Fail!");
      return -1;
    }
    index = 1;            
    ret=ioctl(fosd_fd[2], FIOSDS_ON, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_ON Fail!");
      return -1;
    }
    index = 2;            
    ret=ioctl(fosd_fd[2], FIOSDS_ON, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_ON Fail!");
      return -1;
    }
    index = 3;            
    ret=ioctl(fosd_fd[2], FIOSDS_ON, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_ON Fail!");
      return -1;
    }
  }
  else
  {
    index = 0;            
    ret=ioctl(fosd_fd[2], FIOSDS_OFF, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_OFF Fail!");
      return -1;
    }
    index = 1;            
    ret=ioctl(fosd_fd[2], FIOSDS_OFF, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_OFF Fail!");
      return -1;
    }
    index = 2;            
    ret=ioctl(fosd_fd[2], FIOSDS_OFF, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_OFF Fail!");
      return -1;
    }
    index = 3;            
    ret=ioctl(fosd_fd[2], FIOSDS_OFF, &index);    
    if(ret<0)
    {
      DBGPRINT("FIOSDS_OFF Fail!");
      return -1;
    }
  }
#endif
  return 0;
}
#ifdef HISI_DEV
void cam_name_load(void)
{
	char file_name[100];
  FILE *fp;
  int j,i;

    s_CamName[0].w=0;
    s_CamName[0].h=0;
    s_CamName[0].buf=malloc(CAM_NAME_MAX_W*CAM_NAME_MAX_H*2);


    for(i=1,j=0;j<5;j++,i++)
    {
        memset(file_name,0,sizeof(file_name));
        switch(j)
        {
            case 0:
                if(flag_1080p || flag_ov2710)
                    sprintf(file_name,"/mnt/mtd/24x32/w.bits");
                else
                    sprintf(file_name,"/mnt/mtd/w.bits");
                break;
            case 1:
                if(flag_1080p || flag_ov2710)
                    sprintf(file_name,"/mnt/mtd/24x32/m.bits");
                else
                    sprintf(file_name,"/mnt/mtd/m.bits");
                break;
            case 2:
                if(flag_1080p || flag_ov2710)
                    sprintf(file_name,"/mnt/mtd/24x32/s.bits");
                else
                    sprintf(file_name,"/mnt/mtd/s.bits");
                break;
            case 3:
                if(flag_1080p || flag_ov2710)
                    sprintf(file_name,"/mnt/mtd/24x32/t.bits");
                else
                    sprintf(file_name,"/mnt/mtd/t.bits");
                break;
            case 4:
                if(flag_1080p || flag_ov2710)
                    sprintf(file_name,"/mnt/mtd/24x32/n.bits");
                else
                    sprintf(file_name,"/mnt/mtd/n.bits");
                break;
        }
      fp = fopen(file_name, "rb");
      if(NULL == fp)
      {
        DBGPRINT("Load %s failed!\n", file_name);
        continue;
      }
      fseek(fp,4,SEEK_CUR);
      fread(&s_CamName[i].w, 1,4, fp);
      fread(&s_CamName[i].h, 1,4, fp);
      fseek(fp,4,SEEK_CUR);
      DBGPRINT("%s ,s_CamName[%d].w:%d  ,s_CamName[%d].h:%d\n",file_name,i,s_CamName[i].w,i,s_CamName[i].h);
        if(s_CamName[i].buf==NULL)
            s_CamName[i].buf=malloc(s_CamName[i].w*s_CamName[i].h*2);
        else
        {
            free(s_CamName[i].buf);
            s_CamName[i].buf=malloc(s_CamName[i].w*s_CamName[i].h*2);
        }
        if(s_CamName[i].buf!=NULL)
        fread(s_CamName[i].buf, 1,s_CamName[i].w*s_CamName[i].h*2, fp);
        fclose(fp);

        if(Disk_Num==0) s_CamName[5].w=s_CamName[5].h=1;
    }
    s_CamName[6].w=0;
    s_CamName[6].h=0;
    s_CamName[6].buf=Hisi_get_chan_bitmap_array(0,"/mnt/mtd/chan.bit",&s_CamName[6].w,&s_CamName[6].h);
      DBGPRINT("*********** %d  %d\n",s_CamName[6].w,s_CamName[6].h);
}

void time_num_load()
{
	int i;
	char file_name[100];
  FILE *fp;
	
  for(i=0;i<12;i++)
  {
        memset(file_name,0,sizeof(file_name));
        if(flag_1080p || flag_ov2710)
            sprintf(file_name,"/mnt/mtd/24x32/%d.bits",i);
        else
            sprintf(file_name,"/mnt/mtd/%d.bits",i);
      fp = fopen(file_name, "rb");
      if(NULL == fp)
      {
      DBGPRINT("Load %s failed!\n", file_name);
      continue;
	  }
	  fread(&s_TimeNum[i].w, 1,4, fp);
	  fread(&s_TimeNum[i].h, 1,4, fp);
	  
	  DBGPRINT("%s %d  %d\n",file_name,s_TimeNum[i].w,s_TimeNum[i].h);
	  
  	if(s_TimeNum[i].buf==NULL)
  		s_TimeNum[i].buf=malloc(s_TimeNum[i].w*s_TimeNum[i].h*2);
  	else
  	{
  		free(s_TimeNum[i].buf);
  		s_TimeNum[i].buf=malloc(s_TimeNum[i].w*s_TimeNum[i].h*2);
  	}
  	if(s_TimeNum[i].buf!=NULL)
  	{
	  	fread(s_TimeNum[i].buf, 1,s_TimeNum[i].w*s_TimeNum[i].h*2, fp);

		int myi = 0;
		int myj= 0;

		short *myp = s_TimeNum[i].buf;
		for(myi = 0;myi <  s_TimeNum[i].h;myi ++ ){
		for(myj = 0;myj < s_TimeNum[i].w ;myj ++ )
			{
			
				if(	myp[myj+s_TimeNum[i].w*myi])
				{
					myp[myj+s_TimeNum[i].w*myi] = 0xffff;
					//printf("*");
				}else{
					myp[myj+s_TimeNum[i].w*myi] = 0x00;
					//printf(" ");
				}
			//mycount++;
			}
		//printf("\n");
			}
  		}
  	fclose(fp);
  }  
}



static unsigned short * facebtBuffer = NULL;



int showOneFaceRectClear(rect_t *pRect)

{
	int rectCounty = 0;
			unsigned short * facebtBuffer2 = facebtBuffer + pRect->top*MAX_FACE_VIDEO_WIDTH+pRect->left;
			
	//printf("showOneFaceRect \n");
	for(rectCounty = 0;rectCounty < pRect->bottom - pRect->top ;rectCounty ++)
		{
		
			
	int rectCountx = 0;
	facebtBuffer2 = facebtBuffer + (pRect->top+rectCounty)*MAX_FACE_VIDEO_WIDTH+pRect->left;
			for(rectCountx = 0;rectCountx < pRect->right - pRect->left ;rectCountx ++)
				{
					if(rectCounty < MAX_FACE_VIDEO_EAGE_W||rectCountx < MAX_FACE_VIDEO_EAGE_W)
					facebtBuffer2[rectCountx] = 0xffff;

					
					if(pRect->bottom - pRect->top - rectCounty < MAX_FACE_VIDEO_EAGE_W||pRect->right - pRect->left - rectCountx < MAX_FACE_VIDEO_EAGE_W)
					facebtBuffer2[rectCountx] = 0xffff;
				}


		}
	//printf("showOneFaceRect ... \n");
	return 0;
}

int showOneFaceRect(rect_t *pRect)

{
	int rectCounty = 0;
			unsigned short * facebtBuffer2 = facebtBuffer + pRect->top*MAX_FACE_VIDEO_WIDTH+pRect->left;
			
	//printf("showOneFaceRect \n");
	for(rectCounty = 0;rectCounty < pRect->bottom - pRect->top ;rectCounty ++)
		{
		
			
	int rectCountx = 0;
	facebtBuffer2 = facebtBuffer + (pRect->top+rectCounty)*MAX_FACE_VIDEO_WIDTH+pRect->left;
			for(rectCountx = 0;rectCountx < pRect->right - pRect->left ;rectCountx ++)
				{
					if(rectCounty < MAX_FACE_VIDEO_EAGE_W||rectCountx < MAX_FACE_VIDEO_EAGE_W)
					facebtBuffer2[rectCountx] = 0x03e0;//0x7c00;

					
					if(pRect->bottom - pRect->top - rectCounty < MAX_FACE_VIDEO_EAGE_W||pRect->right - pRect->left - rectCountx < MAX_FACE_VIDEO_EAGE_W)
					facebtBuffer2[rectCountx] = 0x03e0;//0x7c00;
				}


		}
	//printf("showOneFaceRect ... \n");
	return 0;
}
int showRectOnVenc(rect_t *pRect,int count)
{
	printf("enter ...... \n");
	if(!facebtBuffer)
		return -1;
	int rectCount = 0;
	memset(facebtBuffer, 0xff, MAX_FACE_VIDEO_WIDTH*MAX_FACE_VIDEO_HEIGHT*2);
	for(rectCount = 0;rectCount < count ;rectCount ++)
	{
		if((pRect[rectCount].right <=MAX_FACE_VIDEO_WIDTH )&&(pRect[rectCount].bottom <=MAX_FACE_VIDEO_HEIGHT )&&(pRect[rectCount].top >= 0 )&&(pRect[rectCount].left  >= 0 ))
			showOneFaceRect(&pRect[rectCount]);
		else
			printf("error rect\n");
		
	}
	//printf("showRectOnVenc \n");
	int  s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle[4],&unParamFace);
	//printf("showRectOnVenc %d\n",s32Ret);
  if(s32Ret != HI_SUCCESS)
  {
  	DBGPRINT("showRectOnVenc failed with %#x!\n", s32Ret);
    return HI_FAILURE;
  }
  /*
  for(rectCount = 0;rectCount < count ;rectCount ++)
  {
	  if((pRect[rectCount].right <=MAX_FACE_VIDEO_WIDTH )&&(pRect[rectCount].bottom <=MAX_FACE_VIDEO_HEIGHT )&&(pRect[rectCount].top >= 0 )&&(pRect[rectCount].left  >= 0 ))
		  showOneFaceRectClear(&pRect[rectCount]);
	  else
		  printf("error rect\n");
	  
  }

	*/


  
	return 0;
}

int CoverRegionCreat()
{
  HI_S32 i,j;
  HI_S32 s32Ret = HI_FAILURE;
  RGN_ATTR_S stRgnAttr;
  RGN_CHN_ATTR_S stChnAttr;

	memset(rgn_osd_info_w,0,sizeof(rgn_osd_info_w));
	memset(rgn_osd_info_h,0,sizeof(rgn_osd_info_h));
  /****************************************
   step 1: create overlay regions
  ****************************************/
  for (i = 0; i < RGN_NUM_MAX; i++)
  {


    stRgnAttr.enType = OVERLAY_RGN;
    stRgnAttr.unAttr.stOverlay.enPixelFmt       = PIXEL_FORMAT_RGB_1555;
    stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 300;
    stRgnAttr.unAttr.stOverlay.stSize.u32Height = 300;
    stRgnAttr.unAttr.stOverlay.u32BgColor       = 0x0000000;
	stRgnAttr.unAttr.stOverlay.u32CanvasNum = 6;
  	if(i==2)
	    stRgnAttr.enType = COVER_RGN;
  	else
	    stRgnAttr.enType = OVERLAY_RGN;

    stRgnAttr.unAttr.stOverlay.enPixelFmt = PIXEL_FORMAT_RGB_1555;
    if(i==3)
    {
	stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 30*14;
  	stRgnAttr.unAttr.stOverlay.stSize.u32Height = 20;
    }
    else
    {
    stRgnAttr.unAttr.stOverlay.stSize.u32Width  = s_TimeNum[0].w*19;
    stRgnAttr.unAttr.stOverlay.stSize.u32Height = s_TimeNum[0].h;
    }

	 if(i==4)
    {
	stRgnAttr.unAttr.stOverlay.stSize.u32Width  = MAX_FACE_VIDEO_WIDTH;
  	stRgnAttr.unAttr.stOverlay.stSize.u32Height = MAX_FACE_VIDEO_HEIGHT;
    }

	 
    if(i>=5)
    {
	stRgnAttr.unAttr.stOverlay.stSize.u32Width  = OSD_STR_LEN_MAX*14;
  	stRgnAttr.unAttr.stOverlay.stSize.u32Height = 20;
  	}
  	
  	rgn_osd_info_w[i]=stRgnAttr.unAttr.stOverlay.stSize.u32Width;
  	rgn_osd_info_h[i]=stRgnAttr.unAttr.stOverlay.stSize.u32Height;
  	
    stRgnAttr.unAttr.stOverlay.u32BgColor = 0x7c00*(i%2) + ((i+1)%2)*0x1f;
	if(i>=5)
		{
    		stRgnAttr.unAttr.stOverlay.u32BgColor = 0x00000000;
		}

    RgnHandle[i] = i;

	DPRINTK("HI_MPI_RGN_Create [%d] %d-%d\n",i,rgn_osd_info_w[i],rgn_osd_info_h[i]);

    s32Ret = HI_MPI_RGN_Create(RgnHandle[i], &stRgnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        DBGPRINT("HI_MPI_RGN_Create (%d) failed with %#x!\n", \
               RgnHandle[i], s32Ret);
        return HI_FAILURE;
    }
  }


  /*********************************************
   step 2: display overlay regions to venc groups
  *********************************************/
  for (i = 0; i < RGN_NUM_MAX; i++)
  {
  	if(i==2)
    	stChn[i].enModId = HI_ID_VPSS;
  	else
    	stChn[i].enModId = HI_ID_VENC;
	stChn[i].s32DevId = 0;//1
	stChn[i].s32ChnId = 0;
	if(i == 4)
		{
		
	stChn[i].s32ChnId = 0;//1
		}

    memset(&stChnAttr,0,sizeof(stChnAttr));

    if(i==2)
    {
    	stChnAttr.bShow = HI_FALSE;
    	stChnAttr.enType = COVER_RGN;
	   stChnAttr.unChnAttr.stCoverChn.enCoverType = AREA_RECT;
	    stChnAttr.unChnAttr.stCoverChn.stRect.s32X = 40;
	    stChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 40;
	    stChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 40;
	    stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 40;
	    stChnAttr.unChnAttr.stCoverChn.u32Color = 0x0;
	    stChnAttr.unChnAttr.stCoverChn.u32Layer = 0;
    }
    else if(i==3)
    {
    	stChnAttr.bShow = HI_TRUE;
    	stChnAttr.enType = OVERLAY_RGN;
	   	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 180;
	    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 8;
	    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
	    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 3;
	
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
	   
    }
    else
    {
        stChnAttr.bShow = HI_TRUE;
        stChnAttr.enType = OVERLAY_RGN;
        if(i==0)
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 20;
        else
        {
            if(flag_1080p || flag_ov2710)
            	{
            		if (flag_ov2710&&flag_1080p==0)
				stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 1920-23*s_TimeNum[0].w*2;
			else
	                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 1920-20*s_TimeNum[0].w;
            	}
            else if(flag_960p)
                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 1120;
            else
                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 1120;
        }
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 8+60;
        stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 0;
        stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
        stChnAttr.unChnAttr.stOverlayChn.u32Layer = 1;//i+1;

        stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
        stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
		
		stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
		stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
		stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;
		stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = HI_FALSE;

    }
    /*if( i == 4)
    {
    	stChnAttr.bShow = HI_TRUE;
    	stChnAttr.enType = OVERLAY_RGN;
	   	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 4;
	    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 200;
	    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
	    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;
	
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
	
  	}*/
    if( i == 4)
    {
    	stChnAttr.bShow = HI_TRUE;
    	stChnAttr.enType = OVERLAY_RGN;
	   	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
	    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
	    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
	    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 0;
	    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 1;
	
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
	
  	}
    if(i>=5)
    {
    	stChnAttr.bShow = HI_TRUE;
    	stChnAttr.enType = OVERLAY_RGN;
	   	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 4;
	    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 20*(i-4)+32;
	    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 5;
	    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;
	
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
	
  	}

	stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;
        stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = HI_FALSE;

    s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[i], &stChn[i], &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
    	DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[i], s32Ret);
      return HI_FAILURE;
    }
  }
  

////////////	
	{
	  unsigned char by_tmp;
		IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
		
		g_sub_video_w=pAllInfo->onvif_st.vcode[1].w;
	}
//  for (i = 4; i <9; i++)
//  {
//  	if(i==2)
//  		continue;
//  	
//  	if(i==2)
//    	stChn[i].enModId = HI_ID_VIU;
//  	else
//    	stChn[i].enModId = HI_ID_GROUP;
//    stChn[i].s32DevId = 1;
//    stChn[i].s32ChnId = 0;
//
//    memset(&stChnAttr,0,sizeof(stChnAttr));
//
//    if(i==2)
//    {
//    	stChnAttr.bShow = HI_FALSE;
//    	stChnAttr.enType = COVER_RGN;
//	    stChnAttr.unChnAttr.stCoverChn.stRect.s32X = 40;
//	    stChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 40;
//	    stChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 40;
//	    stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 40;
//	    stChnAttr.unChnAttr.stCoverChn.u32Color = 0x0;
//	    stChnAttr.unChnAttr.stCoverChn.u32Layer = 0;
//    }
//    else if(i==3)
//    {
//    	stChnAttr.bShow = HI_TRUE;
//    	stChnAttr.enType = OVERLAY_RGN;
//	   	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = g_sub_video_w-19*8;
//	    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 16;
//	    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
//	    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
//	    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 3;
//	
//	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
//	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
//    }
//    else
//    {
//    	stChnAttr.bShow = HI_TRUE;
//    	stChnAttr.enType = OVERLAY_RGN;
//	    if(i==0)
//	    	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 4;
//	    else
//	    {
//	    	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = g_sub_video_w-19*8;
//	    }
//	    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;//8;
//	    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
//	    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
//	    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 1;//i+1; 
//	
//	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
//	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
//    }
//    if(i>=4)
//    {
//    	stChnAttr.bShow = HI_TRUE;
//    	stChnAttr.enType = OVERLAY_RGN;
//	   	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 4;
//	    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 20*(i-4)+32;
//	    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
//	    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
//	    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 4;
//	
//	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
//	    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
//  	}
//
//    s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[i], &stChn[i], &stChnAttr);
//    if(HI_SUCCESS != s32Ret)
//    {
//    	DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[i], s32Ret);
//      return HI_FAILURE;
//    }
//  }
////////////

	
#if  (defined (AI_TYPE_ALI_DMY))
    for(i=5;i<RGN_NUM_MAX;i++)
#else
	for(i=5;i<RGN_NUM_MAX;i++)
#endif
	
    {
    	DPRINTK("HI_MPI_RGN_Destroy [%d]\n",i);
        HI_MPI_RGN_Destroy(RgnHandle[i]);
    }
 
    //清理掉ssid osd显示部分
    HI_MPI_RGN_Destroy(RgnHandle[3]);

    memset(&unParamtime,0,sizeof(unParamtime));
	
    unParamtime.pData = malloc(s_TimeNum[0].w*19*s_TimeNum[0].h*2*2);

	
    memset(unParamtime.pData,s_TimeNum[0].buf[16],s_TimeNum[0].w*19*s_TimeNum[0].h*2*2);

	
    unParamtime.u32Width = s_TimeNum[0].w*19;
    unParamtime.u32Height = s_TimeNum[0].h;
    unParamtime.enPixelFormat = PIXEL_FORMAT_RGB_1555;


	facebtBuffer = malloc(1920*1080*2);


	//memset(facebtBuffer, 0xff, 1920*1080*2);

    unParamFace.u32Width = MAX_FACE_VIDEO_WIDTH;
    unParamFace.u32Height = MAX_FACE_VIDEO_HEIGHT;
    unParamFace.enPixelFormat = PIXEL_FORMAT_RGB_1555;
    unParamFace.pData = (HI_VOID*)facebtBuffer;


	memset(facebtBuffer, 0xff, MAX_FACE_VIDEO_WIDTH*MAX_FACE_VIDEO_HEIGHT*2);


	

}
#endif
int osd_init(void)
{
    time_num_load();
    cam_name_load();    
     CoverRegionCreat();
    char_load();
    cam_name_rec_display(5);
	return 0;
}

