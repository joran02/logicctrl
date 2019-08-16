#include "cmn.h"

#include <iconv.h> 
#include <string.h> 

#ifdef USE_TTF

#include <ft2build.h>
#include FT_FREETYPE_H

#endif

ValCammra CamMenu[CHANNELNUM];
CAMMENU cammenupic;
U8* CamName;

U8 MenuCamNum=0;

U16 camareawidth[CHANNELNUM],camareahigh[CHANNELNUM];

U16 recosdblock[CHANNELNUM]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

#define CAMERAMAXMENU 19


 U16 tCam_data_pos_n[]=
{
	534,56,
	534,82, 	550,82,	566,82,	582,82,	598,82,
	534,108,
	534,134,
	534,160,
	534,186,
	534,212,
	534,238,
	534,264,
	462,322,
	606,322,
	462,348,
	606,348,
	36,356,
	206,356
};

 U16 tCam_data_pos_p[]=
{
	534,88,
	534,114, 	550,114,	566,114,	582,114,	598,114,
	534,140,
	534,166,
	534,192,
	534,218,
	534,244,
	534,270,
	534,296,
	480,372,
	606,372,
	480,398,
	606,398,
	38,430,
	208,430
};

 U16* tCam_data_pos[]=
 {
	tCam_data_pos_n,
	tCam_data_pos_p
 };

U16 Pre_campos_n[] = 
{
	534,56,		642,76,		
		
	534,82,	546,102,		550,82,	562,102,		566,82,	578,102,		582,82,	594,102,		598,82,	610,102,

	534,108,		642,128,
	534,134,		642,154,
	534,160,		642,180,
	534,186,		642,206,
	534,212,		642,232,
	534,238,		642,258,
	534,264,		642,284,
	462,322,		498,342,
	606,322,		642,342,
	462,348,		498,368,
	606,348,		642,368,
	36,356,		196,380,
	206,356,		366,380
};

U16 Pre_campos_p[] = 
{
	534,88,		642,108,		
		
	534,114,	546,134,		550,114,	562,134,		566,114,	578,134,		582,114,	594,134,		598,114,	610,134,

	534,140,		642,160,
	534,166,		642,186,
	534,192,		642,212,
	534,218,		642,238,
	534,244,		642,264,
	534,270,		642,290,
	534,296,		642,316,
	480,372,		516,392,
	606,372,		642,392,
	480,398,		516,418,
	606,398,		642,418,
	38,430,		198,454,
	208,430,		368,454
};

U16* Pre_campos[] = 
{
	Pre_campos_n,
	Pre_campos_p
};

U8 wrflagcam = 0;




#define WIDTH   300
#define HEIGHT  50

unsigned char image[WIDTH][HEIGHT];




/* Replace this function with something useful. */
/*
void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;

printf("x =%d y = %d  width = %d rows = %d\n",x,y,bitmap->width,bitmap->rows );

    for ( j = y, q = 0; j < y_max; j++, q++ ){
     for ( i = x, p = 0; i < x_max; i++, p++ )
    {
      if ( i < 0      || j < 0       ||
           i >= WIDTH || j >= HEIGHT )
        continue;

      image[i][j] = bitmap->buffer[q * bitmap->width + p];
      //image[i][j] |= bitmap->buffer[q * bitmap->width + p];


      putchar( image[i][j] == 0 ? ' '
                                : image[i][j] < 128 ? '+'
                                                    : '*' );


    }
  printf("\n");
  }
  printf("\n");
}

*/
void
show_image( void )
{
  int  i, j;
 short *tmp = unParamname.pData;
 int tmpcount = 0;
    for ( j = 0; j < HEIGHT; j++ ){
  for ( i = 0; i < WIDTH; i++ )
  {

    {
        putchar( image[i][j] == 0 ? ' '
                                  : image[i][j] < 128 ? '+'
                                                      : '*' );
		if(image[i][j] == 0)
		tmp[tmpcount++] = 0;
		else
		tmp[tmpcount++] = 0xffff;

      
    }
  }putchar( '\n' );
}
}




#ifdef USE_TTF
int
main22232(  )
{
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;

  char*         filename;
  char*         filename2;
  char*         text;

  double        angle;
  int           target_height;
  int           n, num_chars;

  //从文件中读取数据

   char buffer[200];



  wchar_t *chinese_str = buffer;//L"东视  B猫B你们";
  wchar_t *chinese_str2 = L"中国制造";
  //unsigned int *chinese_ptr = (wchar_t *)chinese_str;
  /*

  {

    FILE *fp;
      printf("---filename2=---%s---\n", filename2);

     fp = fopen(filename2, "r");
   
    if(fp){
      fseek(fp, SEEK_SET, 0);
      int ret = fread(buffer, 1, 100, fp);
      printf("buffer=%s----%d\n", buffer,ret);
      fclose(fp);
      }


  }

  char *pin = buffer; 
     char *pout = outbuf; 
        
     int inlen = strlen(pin); 
     int outlen = BUFLEN; 
     int retsize; 
     iconv_t cd; 
     cd = iconv_open("WCHAR_T","UTF-8"); 
     if((iconv_t)-1 == cd) { 
             perror("iconv_open error"); 
             return -1; 
     } 
     retsize = iconv(cd, &pin, (size_t *)&inlen, &pout, (size_t *)&outlen); 
     if((size_t)-1 == retsize) { 
             perror("iconv error"); 
             return -2;    
     } 
     if(outlen > 0) { 
             printf("%s/n", outbuf); 
     } 
     chinese_str = outbuf;
     iconv_close(cd); 
*/




 // return 0;

  printf("wcslen = %d\n",wcslen(chinese_str2) );


  printf("strlen = %d\n",strlen(chinese_str2) );


  printf("sizeof = %d\n",sizeof(chinese_str) );
  printf("wchar_t = %d\n",sizeof(wchar_t) );

  filename      = "/nfs/JDJHTE.TTF";//argv[1];                           /* first argument     */
  printf("filename = %s\n",filename);
  text          = "qwe";//argv[2];                           /* second argument    */
  num_chars     = wcslen( chinese_str2 );
  angle         = ( 0.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  target_height = HEIGHT;

  printf("step 1 = %d\n",num_chars );

  error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */

  printf("step 2 = %d\n",num_chars );

  error = FT_New_Face( library, filename, 0, &face );/* create face object */
  /* error handling omitted */

  printf("step 3 = %d\n",num_chars );

#if 0
  /* use 50pt at 100dpi */
  error = FT_Set_Char_Size( face, 50 , 50,
                            72, 72 );                /* set character size */
#else
  error = FT_Set_Pixel_Sizes(face, 30, 30);
#endif

  printf("step 4 = %d\n",num_chars );

  /* error handling omitted */

  slot = face->glyph;

  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  pen.x = 0 * 64;
  pen.y = ( target_height - 25 ) * 64;

  printf("num_chars = %d\n",num_chars );

  for ( n = 0; n < num_chars; n++ )
  {
    /* set transformation */
    FT_Set_Transform( face, &matrix, &pen );

    printf("FT_Load_Char = %d\n",n);
    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, chinese_str2[n], FT_LOAD_RENDER );
    if ( error )
      continue;                 /* ignore errors */

    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 target_height - slot->bitmap_top );

    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }

  show_image();

  FT_Done_Face    ( face );
  FT_Done_FreeType( library );

  return 0;
}

#endif
	  

#ifdef HISI_DEV
int CoverRegionControl(int x,int y,int w,int h,unsigned char flag,unsigned char ch)
{
	
	float t;
  int x_tmp,y_tmp,w_tmp,h_tmp;
  HI_S32 s32Ret = HI_FAILURE;
  RGN_CHN_ATTR_S stChnAttr;
  
	DBGPRINT(" CoverRegionControl %d %d %d %d %d %d \n",x,y,w,h,flag,ch);
	if (flag)//cam cover
	{
    s32Ret = HI_MPI_RGN_GetDisplayAttr(RgnHandle[2], &stChn[2], &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
    	DBGPRINT("HI_MPI_RGN_GetDisplayAttr failed with %#x!\n",s32Ret);
      return HI_FAILURE;
    }
    if(w<=0||h<=0)
    {
    	stChnAttr.bShow = HI_FALSE;
    }
    else
    {
			if(flag_1080p==1)
			{
			  t=(float)x*5.4;
			  x_tmp = (int)t;
			  t=y*4.4;
			  y_tmp = (int)t+10;
			  t=w*5.4;
			  w_tmp = (int)t;
			  t=h*4.4;
			  h_tmp = (int)t;
			}
			else if(flag_960p)
			{
			  t=(float)x*3.6;
			  x_tmp = (int)t;
			  t=y*3.4;
			  y_tmp = (int)t+27;
			  t=w*3.6;
			  w_tmp = (int)t;
			  t=h*3.4;
			  h_tmp = (int)t;
			}
			else//720p
			{
			  t=(float)x*3.6;
			  x_tmp = (int)t;
			  t=y*2.7;
			  y_tmp = (int)t+10;
			  t=w*3.6;
			  w_tmp = (int)t;
			  t=h*2.7;
			  h_tmp = (int)t;
			}
			x_tmp&=(~0x01);
			y_tmp&=(~0x03);
			w_tmp&=(~0x01);
			h_tmp&=(~0x03);
			DBGPRINT("%d,%d,%d,%d\n",x_tmp,y_tmp,w_tmp,h_tmp);
			DBGPRINT("%d,%d,%d,%d\n",x_tmp,y_tmp,w_tmp,h_tmp);
			DBGPRINT("%d,%d,%d,%d\n",x_tmp,y_tmp,w_tmp,h_tmp);
			DBGPRINT("%d,%d,%d,%d\n",x_tmp,y_tmp,w_tmp,h_tmp);
			DBGPRINT("%d,%d,%d,%d\n",x_tmp,y_tmp,w_tmp,h_tmp);
    	stChnAttr.bShow = HI_TRUE;//HI_FALSE;
    	stChnAttr.enType = COVER_RGN;
	    stChnAttr.unChnAttr.stCoverChn.stRect.s32X = x_tmp;
	    stChnAttr.unChnAttr.stCoverChn.stRect.s32Y = y_tmp;
	    stChnAttr.unChnAttr.stCoverChn.stRect.u32Width = w_tmp;
	    stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = h_tmp;
	    stChnAttr.unChnAttr.stCoverChn.u32Color = 0x0;
	    stChnAttr.unChnAttr.stCoverChn.u32Layer = 0;
    }
    s32Ret = HI_MPI_RGN_SetDisplayAttr(RgnHandle[2], &stChn[2], &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
    	DBGPRINT("HI_MPI_RGN_GetDisplayAttr failed with %#x!\n",s32Ret);
      return HI_FAILURE;
    }
	}
	return 0;
}
extern int cam_name_set_flag;
extern unsigned char camtitle_bitmap_buf[];
extern unsigned char camtitle_buf[];
void cam_name_rec_display(int ch)
{


	printf("cam_name_rec_display2 = %d\n",ch);

#if 1//#ifndef NETDVR
  HI_S32 s32Ret = HI_FAILURE;
  RGN_ATTR_S stRgnAttr;
  MPP_CHN_S stChn;
  RGN_CHN_ATTR_S stChnAttr;
    static int s_w=0,s_h=0;
    int i,k,a,b;
    unsigned char *pDst = NULL,*pDst0 = NULL,*pDst1 = NULL;

    DBGPRINT("*********** ch:%d g_record_display_info:%c,%d,Disk_Num:%d\n",ch,g_record_display_info,cam_name_set_flag,Disk_Num);
    DBGPRINT("*********** camtitle_buf[0]:%d, s_CamName[6].h:%d\n",camtitle_buf[0],s_CamName[6].h);
    if(cam_name_set_flag)
    {
        if(s_CamName[ch].w==1&&s_CamName[ch].h==1)
        {
            s_CamName[0].w=camtitle_bitmap_buf[1];
            s_CamName[0].h=camtitle_bitmap_buf[2];
            DBGPRINT("*********** %d  %d\n",s_CamName[0].w,s_CamName[0].h);
            memcpy(s_CamName[0].buf,&camtitle_bitmap_buf[3],s_CamName[0].w*s_CamName[0].h*2);
        }
        else
        {
//          s_CamName[0].w=camtitle_bitmap_buf[1];
//          s_CamName[0].h=camtitle_bitmap_buf[2];
//          memcpy(s_CamName[0].buf,&camtitle_bitmap_buf[3],s_CamName[0].w*s_CamName[0].h*2);
            if(s_CamName[ch].h>camtitle_bitmap_buf[2])
                k=camtitle_bitmap_buf[2];
            else
                k=s_CamName[ch].h;

            a=camtitle_bitmap_buf[1]*2;
            b=s_CamName[ch].w*2;

            pDst=s_CamName[0].buf;
            pDst0=&camtitle_bitmap_buf[3];
            pDst1=s_CamName[ch].buf;
            for(i=0;i<k;i++)
            {
                memcpy(pDst,pDst0,a);
                pDst+=a;
                pDst0+=a;
                memcpy(pDst,pDst1,b);
                pDst+=b;
                pDst1+=b;
            }
            s_CamName[0].w=camtitle_bitmap_buf[1]+s_CamName[ch].w;
            s_CamName[0].h=k;
            DBGPRINT("*********** %d  %d\n",s_CamName[0].w,s_CamName[0].h);
        }
    }
    else if(camtitle_buf[0])
    {
        if(s_CamName[ch].w==1&&s_CamName[ch].h==1)
        {
            s_CamName[0].w=camtitle_buf[1];
            s_CamName[0].h=camtitle_buf[2];
            DBGPRINT("*********** %d  %d\n",s_CamName[0].w,s_CamName[0].h);
            memcpy(s_CamName[0].buf,&camtitle_buf[3],s_CamName[0].w*s_CamName[0].h*2);
        }
        else
        {
//          s_CamName[0].w=camtitle_buf[1];
//          s_CamName[0].h=camtitle_buf[2];
//          memcpy(s_CamName[0].buf,&camtitle_buf[3],s_CamName[0].w*s_CamName[0].h*2);
            if(s_CamName[ch].h>camtitle_buf[2])
                k=camtitle_buf[2];
            else
                k=s_CamName[ch].h;

            a=camtitle_buf[1]*2;
            b=s_CamName[ch].w*2;

            pDst=s_CamName[0].buf;
            pDst0=&camtitle_buf[3];
            pDst1=s_CamName[ch].buf;
            for(i=0;i<k;i++)
            {
                memcpy(pDst,pDst0,a);
                pDst+=a;
                pDst0+=a;
                memcpy(pDst,pDst1,b);
                pDst+=b;
                pDst1+=b;
            }
            s_CamName[0].w=camtitle_buf[1]+s_CamName[ch].w;
            s_CamName[0].h=k;
            DBGPRINT("*********** %d  %d\n",s_CamName[0].w,s_CamName[0].h);
        }
    }
    else if(s_CamName[6].h)
    {
        if(s_CamName[ch].w==1&&s_CamName[ch].h==1)
        {
            memcpy(s_CamName[0].buf,s_CamName[6].buf,s_CamName[6].w*s_CamName[6].h*2);
            s_CamName[0].w=s_CamName[6].w;
            s_CamName[0].h=s_CamName[6].h;
        }
        else
        {
            if(s_CamName[ch].h>s_CamName[6].h)
                k=s_CamName[6].h;
            else
                k=s_CamName[ch].h;

            a=s_CamName[6].w*2;
            b=s_CamName[ch].w*2;

            pDst=s_CamName[0].buf;
            pDst0=s_CamName[6].buf;
            pDst1=s_CamName[ch].buf;
            for(i=0;i<k;i++)
            {
                memcpy(pDst,pDst0,a);
                pDst+=a;
                pDst0+=a;
                memcpy(pDst,pDst1,b);
                pDst+=b;
                pDst1+=b;
            }
            s_CamName[0].w=s_CamName[6].w+s_CamName[ch].w;
            s_CamName[0].h=k;
        }
    }
    else
    {
        memcpy(s_CamName[0].buf,s_CamName[ch].buf,s_CamName[ch].w*s_CamName[ch].h*2);
        s_CamName[0].w=s_CamName[ch].w;
        s_CamName[0].h=s_CamName[ch].h;

    DBGPRINT("*********** s_CamName[%d].w:%d, s_CamName[%d].h:%d\n",ch,s_CamName[0].w,ch,s_CamName[0].h);
    }
    //if(s_CamName[0].w==1||s_CamName[0].h==1)
        //return ;
    if(s_w!=s_CamName[0].w || s_h!=s_CamName[0].h)
    {
        s_w=s_CamName[0].w;
        s_h=s_CamName[0].h;

	DPRINTK("HI_MPI_RGN_Destroy [%d] \n",RgnHandle[0]);
        HI_MPI_RGN_Destroy(RgnHandle[0]);

        stRgnAttr.enType = OVERLAY_RGN;
      stRgnAttr.unAttr.stOverlay.enPixelFmt = PIXEL_FORMAT_RGB_1555;
      stRgnAttr.unAttr.stOverlay.stSize.u32Width  = s_CamName[0].w;
      stRgnAttr.unAttr.stOverlay.stSize.u32Height = s_CamName[0].h;
      stRgnAttr.unAttr.stOverlay.u32BgColor = 0;


	  
    //stRgnAttr.enType = OVERLAY_RGN;
    stRgnAttr.unAttr.stOverlay.enPixelFmt       = PIXEL_FORMAT_RGB_1555;
    stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 300;
    stRgnAttr.unAttr.stOverlay.stSize.u32Height = 50;
    stRgnAttr.unAttr.stOverlay.u32BgColor       = 0x00000000;
	stRgnAttr.unAttr.stOverlay.u32CanvasNum = 6;

	
     

      s32Ret = HI_MPI_RGN_Create(RgnHandle[0], &stRgnAttr);
      if(HI_SUCCESS != s32Ret)
      {
          DBGPRINT("HI_MPI_RGN_Create (%d) w:%d,h:%d failed with %#x!\n",RgnHandle[0],s_CamName[0].w,s_CamName[0].h, s32Ret);
          return ;
      }

      stChn.enModId = HI_ID_VENC;
      stChn.s32DevId = 0;
      stChn.s32ChnId = 0;

      memset(&stChnAttr,0,sizeof(stChnAttr));
      stChnAttr.bShow = HI_TRUE;
      stChnAttr.enType = OVERLAY_RGN;
      stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 20;
      stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 8;
      stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 0;
      stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
      stChnAttr.unChnAttr.stOverlayChn.u32Layer = 1;

      stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
      stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
	  stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;
        stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = HI_FALSE;

      s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[0], &stChn, &stChnAttr);
      if(HI_SUCCESS != s32Ret)
      {
          DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
          return ;
      }
//////////////////////////////////////////////////
//    stChn.enModId = HI_ID_GROUP;
//    stChn.s32DevId = 1;
//    stChn.s32ChnId = 0;
//
//    memset(&stChnAttr,0,sizeof(stChnAttr));
//    stChnAttr.bShow = HI_TRUE;
//    stChnAttr.enType = OVERLAY_RGN;
//    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 4;
//    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
//    stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 80;
//    stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
//    stChnAttr.unChnAttr.stOverlayChn.u32Layer = 1;
//
//    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
//    stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
//
//    s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle[0], &stChn, &stChnAttr);
//    if(HI_SUCCESS != s32Ret)
//    {
//        DBGPRINT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",RgnHandle[0], s32Ret);
//        return ;
//    }
//////////////////////////////////////////////////
    }

	unParamname.pData = s_CamName[0].buf;
	unParamname.pData = malloc(300*50*2);





	memset(unParamname.pData,0,300*50*2);


	//show_image();
	
	unParamname.u32Width = 300;//s_CamName[0].w;
	unParamname.u32Height = 50;//s_CamName[0].h;
	unParamname.enPixelFormat = PIXEL_FORMAT_RGB_1555;
	DBGPRINT("main2222***** %d  %d\n",s_CamName[0].w,s_CamName[0].h);
  s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle[0],&unParamname);
  if(s32Ret != HI_SUCCESS)
  {
  	DBGPRINT("HI_MPI_RGN_SetBitMap failed with %#x!\n", s32Ret);
    return ;
  }
#endif
}
#endif
#ifdef GM8126_DEV
int CoverRegionControl(int x,int y,int w,int h,unsigned char flag,unsigned char ch)
{	
	float t;
  fiosdmask_win_t win;
  fiosd_palette_t palette;
  fiosd_transparent_t tmp_transparent;
  int index = 5;
  int ret;
  int i_luminance, i_b_luminance;
  int i_cr, i_cb, i_b_cr, i_b_cb;
  
  if(w==0||h==0)
  {
  	x=719;
  	y=575;
  	w=1;
  	h=1;
  }
  //foreground
  i_luminance = 40;
  i_cb = 0x80;
  i_cr = 0x80;
  //background
  i_b_luminance = 40;
  i_b_cb = 0x80;
  i_b_cr = 0x80;

  ret = ioctl(fosd_fd[0], FIOSDMASKS_OFF, &index);
  
  //transparent 0-0%%, 1-50%, 2-75%, 3-100%
  tmp_transparent.windex = index;
  tmp_transparent.level = 0;
  ret = ioctl(fosd_fd[0], FIOSDMASKS_TRANSPARENT, &tmp_transparent);
  if (ret < 0) {
      printf("FIOSDMASKS_TRANSPARENT Fail!\n");
      goto end;
  }
  
  
	if(flag_1080p==1)
	{
	  t=(float)x*5.4;
	  win.windex = index;
	  win.x = (int)t;
	  t=y*4.4;
	  win.y = (int)t+27;
	  t=w*5.4;
	  win.width = (int)t;
	  t=h*4.4;
	  win.height = (int)t;
	}
	else
	{
		if(flag_480p)
		{
		  t=(float)x*1.8;
		  win.windex = index;
		  win.x = (int)t;
		  t=y*2;
		  win.y = (int)t+27;
		  t=w*1.8;
		  win.width = (int)t;
		  t=h*2;
		  win.height = (int)t;
		}
		else if(flag_576p)
		{
		  t=(float)x*1.7;
		  win.windex = index;
		  win.x = (int)t;
		  t=y*2.4;
		  win.y = (int)t+27;
		  t=w*1.7;
		  win.width = (int)t;
		  t=h*2.4;
		  win.height = (int)t;
		}
		else//720p
		{
		  t=(float)x*3.6;
		  win.windex = index;
		  win.x = (int)t;
		  t=y*2.7;
		  win.y = (int)t+27;
		  t=w*3.6;
		  win.width = (int)t;
		  t=h*2.7;
		  win.height = (int)t;
		}
	}	
//  t=(float)x*3.6;
//  win.windex = index;
//  win.x = (int)t;
//  t=y*2.7;
//  win.y = (int)t+27;
//  t=w*3.6;
//  win.width = (int)t;
//  t=h*2.7;
//  win.height = (int)t;
  win.color = 3;

  ret = ioctl(fosd_fd[0], FIOSDMASKS_WIN, &win);
  if (ret < 0) {
      printf("FIOSDMASKS_WIN Fail!");
      goto end;
  }
              
  palette.index = 3;
  palette.y = i_luminance;
  palette.cb = i_cb;
  palette.cr = i_cr;
  ret = ioctl(fosd_fd[0], FIOSDS_PALTCOLOR, &palette);
  if (ret < 0) {
      printf("FIOSDS_PALTCOLOR 0 Fail!");
      goto end;
  }
  palette.index = 4;
  palette.y = i_b_luminance;
  palette.cb = i_b_cb;
  palette.cr = i_b_cr;
  ret = ioctl(fosd_fd[0], FIOSDS_PALTCOLOR, &palette);
  if (ret < 0) {
      printf("FIOSDS_PALTCOLOR 1 Fail!");
      goto end;
  }
  
  ret = ioctl(fosd_fd[0], FIOSDMASKS_ON, &index);
  if (ret < 0) {
      printf("FIOSDMASKS_ON Fail!");
      goto end;
  }
  
//////////////////////////////////////////////////////  
  
  index = 6;
  //foreground
  i_luminance = 40;
  i_cb = 0x80;
  i_cr = 0x80;
  //background
  i_b_luminance = 40;
  i_b_cb = 0x80;
  i_b_cr = 0x80;

  ret = ioctl(fosd_fd[1], FIOSDMASKS_OFF, &index);
  
  //transparent 0-0%%, 1-50%, 2-75%, 3-100%
  tmp_transparent.windex = index;
  tmp_transparent.level = 0;
  ret = ioctl(fosd_fd[1], FIOSDMASKS_TRANSPARENT, &tmp_transparent);
  if (ret < 0) {
      printf("FIOSDMASKS_TRANSPARENT Fail!\n");
      goto end;
  }
  	
  win.windex = index;
  win.x = x;
  win.y = y+27;
  win.width = w;
  win.height = h;
  win.color = 3;

  ret = ioctl(fosd_fd[1], FIOSDMASKS_WIN, &win);
  if (ret < 0) {
      printf("FIOSDMASKS_WIN Fail!");
      goto end;
  }
              
  palette.index = 3;
  palette.y = i_luminance;
  palette.cb = i_cb;
  palette.cr = i_cr;
  ret = ioctl(fosd_fd[1], FIOSDS_PALTCOLOR, &palette);
  if (ret < 0) {
      printf("FIOSDS_PALTCOLOR 0 Fail!");
      goto end;
  }
  palette.index = 4;
  palette.y = i_b_luminance;
  palette.cb = i_b_cb;
  palette.cr = i_b_cr;
  ret = ioctl(fosd_fd[1], FIOSDS_PALTCOLOR, &palette);
  if (ret < 0) {
      printf("FIOSDS_PALTCOLOR 1 Fail!");
      goto end;
  }
  
  ret = ioctl(fosd_fd[1], FIOSDMASKS_ON, &index);
  if (ret < 0) {
      printf("FIOSDMASKS_ON Fail!");
      goto end;
  }
end:
    return 0;
}
void cam_name_rec_display(int ch)
{
	int ret;


  printf("cam_name_rec_display1 = %d\n",ch);
  fiosd_string_t string;
    
  string.windex   = 0;
  string.start    = 0;
  string.fg_color = 1;
  string.bg_color = 0;
  sprintf(&string.string[0],"CAM%02d",ch+1);
  ret = ioctl(fosd_fd[0], FIOSDS_STRING, &string);
  if(ret < 0) 
    printf("FIOSDS 0 STRING Fail!");

  string.windex   = 3;
  string.start    = 0;
  string.fg_color = 1;
  string.bg_color = 0;
  sprintf(&string.string[0],"CAM%02d",ch+1);
  ret = ioctl(fosd_fd[1], FIOSDS_STRING, &string);
  if(ret < 0) 
    printf("FIOSDS 1 STRING Fail!");
}
#endif

void MenuCamInit()
{

	U8 i,tmp1[2];
	U16 chendy;
	
	
	{
		  unsigned char by_tmp;
			IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
			
			by_tmp=255-pIsp_st->u32cds[1];
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+DAYV,1,&by_tmp);
			by_tmp=255-pIsp_st->u32cds[0];
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*0+NIGHTV,1,&by_tmp);

	}
	
	chendy = 240+SysMenu.VideoFormat*48;	
	for (i=0;i<Cam_Num;i++)
	{
		memset(CamMenu[i].CamTitle,0x00,sizeof(CamMenu[i].CamTitle));
		I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*i+CAMTITLE,5,CamMenu[i].CamTitle);
//		printf("***************************** title:%s\n",CamMenu[i].CamTitle);
//		printf("title1:%d,title2:%d,title3:%d,title4:%d,title5:%d\n",CamMenu[i].CamTitle[0],CamMenu[i].CamTitle[1],CamMenu[i].CamTitle[2],CamMenu[i].CamTitle[3],CamMenu[i].CamTitle[4]);
		I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*i+BRIGHTNESS,8,&CamMenu[i].Brightness);
		I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*i+STARTX,2,tmp1);
		CamMenu[i].Starx=(tmp1[0]<<8)|tmp1[1];
		I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*i+STARTY,2,tmp1);
		CamMenu[i].Stary=(tmp1[0]<<8)|tmp1[1];
		I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*i+ENDX,2,tmp1);
		CamMenu[i].Endx=(tmp1[0]<<8)|tmp1[1];
		I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*i+ENDY,2,tmp1);
		CamMenu[i].Endy=(tmp1[0]<<8)|tmp1[1];
		I_read_byte_eeprom(ADDCAM+ADDCOUNTCAM*i+DAYV, &CamMenu[i].dayval);
		I_read_byte_eeprom(ADDCAM+ADDCOUNTCAM*i+NIGHTV, &CamMenu[i].nightval);

		if (CamMenu[i].Endy>chendy)
		{
			CamMenu[i].Endy=chendy;
			tmp1[0]=CamMenu[i].Endy>>8;
			tmp1[1]=CamMenu[i].Endy;
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+ENDY,2,tmp1);
		}

		if (!SysMenu.VideoFormat && CamMenu[i].PicVdel>0x10)
		{
			CamMenu[i].PicVdel=0x10;
			I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+PICVDEL,1,&CamMenu[i].PicVdel);
		}

//		printf("startx:%d,stary:%d,endx:%d,eny:%d\n",CamMenu[i].Starx,CamMenu[i].Stary,CamMenu[i].Endx,CamMenu[i].Endy);
		camareawidth[i] = CamMenu[i].Endx-CamMenu[i].Starx;
		camareahigh[i] = CamMenu[i].Endy-CamMenu[i].Stary;

	}

	for(i=0;i<Cam_Num;i++)
	{
		/*Brightness_Ajust(i, CamMenu[i].Brightness);
		Hue_Ajust(i, CamMenu[i].Hue);
		Contrast_Ajust(i, CamMenu[i].Contrast);
		Stauration_Ajust(i, CamMenu[i].Satutrat);*/
		Hdelay_Ajust(i, CamMenu[i].PicHdel);
		Vdelay_Ajust(i, CamMenu[i].PicVdel);
		CoverRegionControl(CamMenu[i].Starx+8,CamMenu[i].Stary,camareawidth[i],camareahigh[i],1,i);
	}
}

void menu_camera_data_disp(U8 pos,U16 color)
{
}



void CamArea_Setup()
{

	camareawidth[0] = CamMenu[0].Endx-CamMenu[0].Starx;
	camareahigh[0] = CamMenu[0].Endy-CamMenu[0].Stary;

//	if (camareawidth[0] != 0 && camareahigh[0] != 0)
	{
		CoverRegionControl(CamMenu[0].Starx+8,CamMenu[0].Stary,camareawidth[0],camareahigh[0],1,0);
	}
}

void CamArea_Clear()
{
	wrflagcam = 1;
	CoverRegionControl(0,0,0,0,0,MenuCamNum);
}

void menu_camera_on()
{
}

void menu_camera_down()
{
	menu_camera_data_disp(cur_pos,WHITE);
	if(cur_pos<CAMERAMAXMENU-1)	cur_pos++; 
	else if(cur_pos==CAMERAMAXMENU-1)cur_pos = 0;    
	if (CamMenu[MenuCamNum].InputMatch && cur_pos>1 && cur_pos<6)
		cur_pos=6;
	menu_camera_data_disp(cur_pos,RED);
}
void menu_camera_up()
{
	menu_camera_data_disp(cur_pos,WHITE);
	if(cur_pos==0)
		cur_pos = CAMERAMAXMENU-1;         
	else 
		cur_pos--;
	if (CamMenu[MenuCamNum].InputMatch && cur_pos>1 && cur_pos<6)
		cur_pos=1;
	menu_camera_data_disp(cur_pos,RED);
}

void menu_camera_do()
{
	U8 i,tmp[2];

	
	for (i=0;i<Cam_Num;i++)
	{
//		memset(CamMenu[i].CamTitle,0x00,sizeof(CamMenu[i].CamTitle));
//		I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*i+CAMTITLE,5,CamMenu[i].CamTitle);
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+CAMTITLE,5,CamMenu[i].CamTitle);
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+BRIGHTNESS,12,&CamMenu[i].Brightness);
//		printf("startx:%d,stary:%d,endx:%d,eny:%d\n",CamMenu[i].Starx,CamMenu[i].Stary,CamMenu[i].Endx,CamMenu[i].Endy);
		
		tmp[0]=CamMenu[i].Starx>>8;
		tmp[1]=CamMenu[i].Starx;
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+STARTX,2,tmp);
		tmp[0]=CamMenu[i].Stary>>8;
		tmp[1]=CamMenu[i].Stary;
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+STARTY,2,tmp);
		tmp[0]=CamMenu[i].Endx>>8;
		tmp[1]=CamMenu[i].Endx;
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+ENDX,2,tmp);
		tmp[0]=CamMenu[i].Endy>>8;
		tmp[1]=CamMenu[i].Endy;
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+ENDY,2,tmp);
		
		tmp[0]=CamMenu[i].dayval;
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+DAYV,1,tmp);
		tmp[0]=CamMenu[i].nightval;
		I_write_block_eeprom(ADDCAM+ADDCOUNTCAM*i+NIGHTV,1,tmp);
		
	}
	for (i=0;i<Cam_Num;i++)
		OsdY_line_display(i,0,cammenupic.recname,0);
}

void Camget_area()
{
	camareawidth[MenuCamNum] = CamMenu[MenuCamNum].Endx- CamMenu[MenuCamNum].Starx;
	camareahigh[MenuCamNum] = CamMenu[MenuCamNum].Endy- CamMenu[MenuCamNum].Stary;

	CamArea_Setup();
}

void menu_camera_add()
{
}
void menu_camera_dec()
{
}

void menu_camera_off()
{
	if (wrflagcam)
		MenuCamInit();
	OSD_ALL_CLEAR();
	demo_set();
	menu_num=0;
    	menu_main_on();
}

void menu_camera_enter()
{
}

void MouseCamRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_campos[SysMenu.VideoFormat][cur_pos*4]+offsetx_hd; 
		miny = Pre_campos[SysMenu.VideoFormat][cur_pos*4+1]+offsety_hd;
		maxx = Pre_campos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx_hd;
		maxy = Pre_campos[SysMenu.VideoFormat][cur_pos*4+3] +offsety_hd;
	}
	else
	{
		minx = Pre_campos[SysMenu.VideoFormat][cur_pos*4]+offsetx;
		miny = Pre_campos[SysMenu.VideoFormat][cur_pos*4+1]+offsety;
		maxx = Pre_campos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx;
		maxy = Pre_campos[SysMenu.VideoFormat][cur_pos*4+3] +offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		menu_camera_add(); 
}

void MouseCamLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_campos[SysMenu.VideoFormat][cur_pos*4]+offsetx_hd; 
		miny = Pre_campos[SysMenu.VideoFormat][cur_pos*4+1]+offsety_hd;
		maxx = Pre_campos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx_hd;
		maxy = Pre_campos[SysMenu.VideoFormat][cur_pos*4+3] +offsety_hd;
	}
	else
	{
		minx = Pre_campos[SysMenu.VideoFormat][cur_pos*4]+offsetx;
		miny = Pre_campos[SysMenu.VideoFormat][cur_pos*4+1]+offsety;
		maxx = Pre_campos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx;
		maxy = Pre_campos[SysMenu.VideoFormat][cur_pos*4+3] +offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos>16)
			menu_camera_enter();
		else
			menu_camera_dec(); 
	}
}

void Camgetstar(U16 x,U16 y)
{
	U16 chstarx,chstary;
	if (MseType == 0)
	{
		chstarx=24+offsetx;
		chstary = 52+offsety+SysMenu.VideoFormat*6;
	}
	else
	{
		chstarx=24+offsetx_hd;
		chstary = 52+offsety_hd+SysMenu.VideoFormat*6;
	}
	CamMenu[MenuCamNum].Starx= x-chstarx;
	CamMenu[MenuCamNum].Stary= y-chstary;

}

void Camgetend(U16 x,U16 y)
{
	U16 chstarx,chstary,chendx,chendy;
	if (MseType == 0)
	{
		chstarx=24+offsetx;
		chstary = 52+offsety+SysMenu.VideoFormat*6;
		chendx=374+offsetx;
		chendy = 292+SysMenu.VideoFormat*54+offsety;
	}
	else
	{
		chstarx=24+offsetx_hd;
		chstary = 52+offsety_hd+SysMenu.VideoFormat*6;
		chendx=374+offsetx_hd;
		chendy = 292+offsety_hd+SysMenu.VideoFormat*54;
	}
//	printf("x:%d,y:%d,sx:%d,sy:%d,ex:%d,ey:%d\n",x,y,chstarx,chstary,chendx,chendy);
	if (x>chstarx && x<chendx)
	{
		if ((x-chstarx)>CamMenu[MenuCamNum].Starx)
			CamMenu[MenuCamNum].Endx= x-chstarx;
		else
		{
			CamMenu[MenuCamNum].Endx= CamMenu[MenuCamNum].Starx;
			CamMenu[MenuCamNum].Starx = x-chstarx;
		}
	}
	else if (x>chstarx && x>chendx)
	{
		if ((chendx-chstarx)>CamMenu[MenuCamNum].Starx)
			CamMenu[MenuCamNum].Endx= chendx-chstarx;
		else
		{
			CamMenu[MenuCamNum].Endx= CamMenu[MenuCamNum].Starx;
			CamMenu[MenuCamNum].Starx = chendx-chstarx;
		}
	}
	else if (x<chstarx)
	{
		CamMenu[MenuCamNum].Endx= CamMenu[MenuCamNum].Starx;
		CamMenu[MenuCamNum].Starx = 0;
	}
	
	camareawidth[MenuCamNum] = CamMenu[MenuCamNum].Endx - CamMenu[MenuCamNum].Starx;

	if (y>chstary && y<chendy)
	{
		if ((y-chstary)>CamMenu[MenuCamNum].Stary)
			CamMenu[MenuCamNum].Endy= y-chstary;
		else
		{
			CamMenu[MenuCamNum].Endy= CamMenu[MenuCamNum].Stary;
			CamMenu[MenuCamNum].Stary = y-chstary;
		}
	}
	else if (y>chstary && y>chendy)
	{
		if ((chendy-chstary)>CamMenu[MenuCamNum].Stary)
			CamMenu[MenuCamNum].Endy= chendy-chstary;
		else
		{
			CamMenu[MenuCamNum].Endy= CamMenu[MenuCamNum].Stary;
			CamMenu[MenuCamNum].Stary = chendy-chstary;
		}
	}
//	else if (y>chstary && y<CamMenu[MenuCamNum].Stary)
//	{
//		CamMenu[MenuCamNum].Endy= CamMenu[MenuCamNum].Stary;
//		CamMenu[MenuCamNum].Stary = y-chstary;
//	}
	else if (y<chstary)
	{
		CamMenu[MenuCamNum].Endy= CamMenu[MenuCamNum].Stary;
		CamMenu[MenuCamNum].Stary = 0;
	}

	camareahigh[MenuCamNum] = CamMenu[MenuCamNum].Endy-CamMenu[MenuCamNum].Stary;
	
//	printf("sx:%d,sy:%d,ex:%d,ey:%d,w:%d,h:%d\n",CamMenu[MenuCamNum].Starx,CamMenu[MenuCamNum].Stary,
//		CamMenu[MenuCamNum].Endx,CamMenu[MenuCamNum].Endy,camareawidth[MenuCamNum],camareahigh[MenuCamNum]);
/*
	U16 tmpx,tmpy,tmpt,tmpb;
	tmpt = 62+offsety;
	tmpb = tmpt + 210+SysMenu.VideoFormat*48;
	if (x<(24+offsetx))
		tmpx = 0;
	else if (x>(310+offsetx))
		tmpx = 310;
	else
		tmpx = x-(24+offsetx);
	if (tmpx<30)
		tmpx = 30;
	if (y<tmpt)
		tmpy = 0;
	else if (y>tmpb)
		tmpy=tmpb-tmpt;
	else
		tmpy = y-tmpt;
	if (tmpy<50)
		tmpy = 50;
	
	if (CamMenu[MenuCamNum].Starx>tmpx)
	{
		CamMenu[MenuCamNum].Endx= CamMenu[MenuCamNum].Starx;
		CamMenu[MenuCamNum].Starx = tmpx;
	}
	else
		CamMenu[MenuCamNum].Endx = tmpx;
	
	if (CamMenu[MenuCamNum].Endx == CamMenu[MenuCamNum].Starx)
		CamMenu[MenuCamNum].Endx++;
	
	camareawidth[MenuCamNum] = CamMenu[MenuCamNum].Endx - CamMenu[MenuCamNum].Starx;

	if (CamMenu[MenuCamNum].Stary>tmpy)
	{
		CamMenu[MenuCamNum].Endy= CamMenu[MenuCamNum].Stary;
		CamMenu[MenuCamNum].Stary = tmpy;
	}
	else
		CamMenu[MenuCamNum].Endy = tmpy;

	if (CamMenu[MenuCamNum].Endy == CamMenu[MenuCamNum].Stary)
		CamMenu[MenuCamNum].Endy++;
//	printf("################## endx:%d,endy:%d\n",CamMenu[MenuCamNum].Endx,CamMenu[MenuCamNum].Endy);
	camareahigh[MenuCamNum] = CamMenu[MenuCamNum].Endy-CamMenu[MenuCamNum].Stary;
*/
}

U8 credpos=0;

U16 linex,liney;

U8 getmousestatus()
{
	return credpos;
}

void Mouse_Cam_Move_Func(U16 x, U16 y, U8 type)
{
	U16 i=0;
	U16 chstarx,chstary,chendx,chendy;
	U16 minx,miny,maxx,maxy;
	if (MseType == 0)
	{
		chstarx=24+offsetx;
		chendx=374+offsetx;
		chstary = 52+offsety+SysMenu.VideoFormat*6;
		chendy = 292+SysMenu.VideoFormat*54+offsety;
	}
	else
	{
		chstarx=24+offsetx_hd;
		chendx=374+offsetx_hd;
		chstary = 52+offsety_hd+SysMenu.VideoFormat*6;
		chendy = 292+offsety_hd+SysMenu.VideoFormat*54;
	}
	switch (type)
	{
		case 0:
			for (i=0;i<CAMERAMAXMENU;i++)
			{
				if (MseType == 0)
				{
					minx = Pre_campos[SysMenu.VideoFormat][i*4]+offsetx;
					miny = Pre_campos[SysMenu.VideoFormat][i*4+1]+offsety;
					maxx = Pre_campos[SysMenu.VideoFormat][i*4+2]+offsetx;
					maxy = Pre_campos[SysMenu.VideoFormat][i*4+3]+offsety;
				}
				else
				{
					minx = Pre_campos[SysMenu.VideoFormat][i*4]+offsetx_hd;
					miny = Pre_campos[SysMenu.VideoFormat][i*4+1]+offsety_hd;
					maxx = Pre_campos[SysMenu.VideoFormat][i*4+2]+offsetx_hd;
					maxy = Pre_campos[SysMenu.VideoFormat][i*4+3]+offsety_hd;
				}
				if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
				{
					menu_camera_data_disp(cur_pos,WHITE);
					cur_pos=i;
					menu_camera_data_disp(cur_pos,RED);
					break;
				}
			}
			if (credpos)
			{
				if (MseType == 0)
					draw_line(linex, liney, x, y,chstarx,chstary,chendx,chendy,AD,0x0000);
				else
					draw_line(linex, liney, x, y,chstarx,chstary,chendx,chendy,HD,0x0000);
			}
			break;
		case 24:
			if (x>chstarx && x<chendx && y>chstary && y<chendy)
			{
				credpos = 1;
				wrflagcam = 1;
				linex = x;
				liney = y;
				CamArea_Clear();
				Camgetstar(x,y);
			}
			break;
		case 26:
			if (credpos)
			{
				credpos = 0;
				wrflagcam = 1;
				Camgetend(x,y);
				if (MseType == 0)
					clear_osd(chstarx, chstary, chendx-chstarx, chendy-chstary,AD);
				else
					clear_osd(chstarx, chstary, chendx-chstarx, chendy-chstary,HD);
				CamArea_Setup();
				for (i=13;i<CAMERAMAXMENU-2;i++)
				{
					menu_camera_data_disp(i,WHITE);
				}
			}
			else
				MouseCamLClick(x,y);
			break;
		case 25:
		case 30:
			MouseCamRClick(x,y);
			break;
		case 29:
			if (cur_pos>0 && cur_pos<6)
				menu_camera_enter();
			break;
		case 31:
			if (!credpos && cur_pos<17)
				MouseCamLClick(x,y);
			break;
		default:
			break;
	}
	
}


void mCAMERA_key_func()
{
	
	if(byKey_val==0) return;

	switch(byKey_val)
	{
		case kESC:
			menu_camera_off();
			break;
		case kUP:
			menu_camera_up();
	   		break;
		case kDN:
			menu_camera_down();
			break;
		case kRH:
		case kAD:
	 		menu_camera_add(); 
			break;
	   	case kLT:
		case kDE:
			menu_camera_dec();
			break;
		case kET:
			menu_camera_enter();
			break;
		default:       //CALL  SWAP   ENTER
			break;
    }
	byKey_val=0;
}


