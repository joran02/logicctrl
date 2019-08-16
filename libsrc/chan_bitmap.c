#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
//#include <linux/videodev.h>
#include <errno.h>
#include <linux/fb.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>
 #include <pthread.h>
#include	"cmn.h"

#define MAX_CHAN (32)
#define X_MAX 10//5
#define CHAR_H 18 //18
#define CHAR_W 16 //16

#define GetNvrBitmap  (3+200)

typedef struct _CHAN_BITMAP_ST_
{
	int use_chan_num;
	int total_chan;	
	int width[MAX_CHAN];
	int height[MAX_CHAN];
	int buf_size[MAX_CHAN];
	int chan_use_bitmap[MAX_CHAN];
	char ascii_buf[MAX_CHAN][20];
	char font_type[MAX_CHAN][50];
	char * pbuf;
}CHAN_BITMAP_ST;



char * GM_get_word_array(int id,char * bit_file);
static  int word_change(char * buf);


static int GetBufPos(int x,int y,int width)
{
	int pos;

	pos =  x/8 + y*(width/8);

	return pos;
}

static char Set_Bit(char iCam, int iBitSite)
{
	char value;

	value = 1;

	value = value << iBitSite;

	iCam = iCam |value;

	return iCam;
}

static char Get_Bit(char iCam, int iBitSite)
{
	iCam = iCam >> iBitSite;

	iCam = iCam & 0x01;

	return iCam;
}



static  int word_change(char * buf)
{
	int total_count = 2*CHAR_H;//18;
	char tmp;
	int i;
	char x;
	
	for(i = 0 ; i < total_count;i++)
	{
		x = buf[i];		
		
		x = ((x & 0x0F) << 4) + ((x & 0xF0) >> 4);		

    		x = ((x & 0x33) << 2) + ((x & 0xCC) >> 2);		

   		x = ((x & 0x55) << 1) + ((x & 0xAA) >> 1);

		//printf("%x->%x  3\n",buf[i],x);

		buf[i] = x;
	}
	
}


static  int Hisi_2bit_change_16bit(char * bit2_buf,char * bit16_buf)
{
	int total_count = 2*CHAR_H;//18;
	char tmp;
	int i;	
	short * write_16bit_buf ;
	int y,x;
	int pos;
	char * pSaveBitBuf;

	write_16bit_buf = (short*)bit16_buf;

	pSaveBitBuf = bit2_buf;

	for( y = 0; y< CHAR_H;y++)
	{
		for( x=0; x <CHAR_W*X_MAX; x++)
		{

			pos = GetBufPos(x,y,CHAR_W*X_MAX);				

			if ( flag_1080p || flag_ov2710)
			{
				if( Get_Bit(pSaveBitBuf[pos],x%8) == 1 )
				{
					//printf("ff");
					write_16bit_buf[2*y*CHAR_W*X_MAX*2 + 2*x] = 0xffff;
					write_16bit_buf[2*y*CHAR_W*X_MAX*2 + 2*x+1] = 0xffff;
					write_16bit_buf[(2*y+1)*CHAR_W*X_MAX*2 + 2*x] = 0xffff;
					write_16bit_buf[(2*y+1)*CHAR_W*X_MAX*2 + 2*x+1] = 0xffff;
				}
				else
				{
					//printf("00");
					write_16bit_buf[2*y*CHAR_W*X_MAX*2 + 2*x] = 0x0000;
					write_16bit_buf[2*y*CHAR_W*X_MAX*2 + 2*x+1] = 0x0000;
					write_16bit_buf[(2*y+1)*CHAR_W*X_MAX*2 + 2*x] = 0x0000;
					write_16bit_buf[(2*y+1)*CHAR_W*X_MAX*2 + 2*x+1] = 0x0000;
				}
			}else {
			if( Get_Bit(pSaveBitBuf[pos],x%8) == 1 )
			{
				//printf("ff");
				write_16bit_buf[y*CHAR_W*X_MAX + x] = 0x0000;
			}
			else
			{
				//printf("00");
				write_16bit_buf[y*CHAR_W*X_MAX + x] = 0x7fff;
			}
			}
		}
		printf("\n");
	}	
	
	
	return 1;
}



void GM_16width_change_to_12width(char * buf)
{
	short word[X_MAX];	
	int i;
	short tmp_word;
	int old_x,old_y;
	int pos;

	for( old_y = 0 ; old_y < CHAR_H; old_y++)
	{
		memcpy(&tmp_word,&buf[old_y*2*X_MAX + 0],2);
		word[0] = (tmp_word&0x0fff);


		memcpy(&tmp_word,&buf[old_y*2*X_MAX + 1],2);
		word[1] = (tmp_word>>4)&0x0fff;

		memcpy(&tmp_word,&buf[old_y*2*X_MAX + 3],2);
		word[2] = (tmp_word&0x0fff);

		memcpy(&tmp_word,&buf[old_y*2*X_MAX + 4],2);
		word[3] = (tmp_word>>4)&0x0fff;

		memcpy(&tmp_word,&buf[old_y*2*X_MAX + 6],2);
		word[4] = (tmp_word&0x0fff);

		memcpy(&buf[old_y * 2 * X_MAX],&word[0],X_MAX*2);
		
	}
	
}


char * GM_get_word_array(int id,char * bit_file)
{
	FILE * fp = NULL;
	int rel;
	int fileOffset;
	char * tmp_buf =NULL;
	char * word_buf =NULL;
	char * pic_buf = NULL;
	CHAN_BITMAP_ST * p_st=NULL;

	fp = fopen(bit_file,"rb");

	if( fp )
	{
		rel = fseek(fp,0L,SEEK_END);
		if( rel != 0 )
		{
			printf("fseek error!!\n");
			goto GET_ERROR;
		}

		fileOffset = ftell(fp);
		if( fileOffset == -1 )
		{
			printf(" ftell error!\n");
			goto GET_ERROR;
		}

		//printf(" fileOffset = %ld\n",fileOffset);

		rewind(fp);	

		tmp_buf = (char*)malloc(fileOffset);
		if( tmp_buf == NULL)
		{
			printf(" malloc Error!=n");
			goto GET_ERROR;
		}

		rel = fread(tmp_buf,1,fileOffset,fp);
		if( rel != fileOffset )
		{
			printf(" fread Error!=n");
			goto GET_ERROR;
		}
		
		p_st = (CHAN_BITMAP_ST *)tmp_buf;		

		word_buf = (char*)malloc(CHAR_W/8*CHAR_H);
		if( word_buf == NULL)
		{
			printf(" malloc Error!=n");
			goto GET_ERROR;
		}

		

		{
			int x,y;
			int ptr_pos =0;

			pic_buf = tmp_buf + sizeof(CHAN_BITMAP_ST);

			GM_16width_change_to_12width(pic_buf);
			
			for( y = 0; y < p_st->height[0];y++)
			{
				memcpy(&word_buf[ptr_pos],&pic_buf[y*p_st->width[0]/8 + id*2],2);
				ptr_pos+= 2;
			}
		}
		

		if( fp )
			fclose(fp);

		if( tmp_buf )
			free(tmp_buf);

		word_change(word_buf);

		return word_buf;	
		
	}else
	{
		printf("open file error!\n");
	}

GET_ERROR:

	if( fp )
		fclose(fp);

	if( tmp_buf )
		free(tmp_buf);

	if( word_buf )
		free(word_buf);

	return NULL;
}



char * Hisi_get_chan_bitmap_array(int chan,char * bit_file,int * pic_width,int * pic_height)
{
	FILE * fp = NULL;
	int rel,flag;
	int fileOffset;
	char * tmp_buf =NULL;
	char * word_buf =NULL;
	char * pic_buf = NULL;
	CHAN_BITMAP_ST * p_st=NULL;

	fp = fopen(bit_file,"rb");

	if( fp )
	{
		rel = fseek(fp,0L,SEEK_END);
		if( rel != 0 )
		{
			printf("fseek error!!\n");
			goto GET_ERROR;
		}

		fileOffset = ftell(fp);
		if( fileOffset == -1 )
		{
			printf(" ftell error!\n");
			goto GET_ERROR;
		}

		//printf(" fileOffset = %ld\n",fileOffset);

		rewind(fp);	

		tmp_buf = (char*)malloc(fileOffset);
		if( tmp_buf == NULL)
		{
			printf(" malloc Error!=n");
			goto GET_ERROR;
		}

		rel = fread(tmp_buf,1,fileOffset,fp);
		if( rel != fileOffset )
		{
			printf(" fread Error!=n");
			goto GET_ERROR;
		}
		
		p_st = (CHAN_BITMAP_ST *)tmp_buf;	
		
		if(p_st->chan_use_bitmap[0]==0)
			goto GET_ERROR;		

		// ת16ɫͼ£¬80x18x2 
		word_buf = (char*)malloc(CHAR_W*X_MAX*CHAR_H*2*4);
		if( word_buf == NULL)
		{
			printf(" malloc Error!=n");
			goto GET_ERROR;
		}		
		

		if( fp )
			fclose(fp);

		if( tmp_buf )
			free(tmp_buf);

		pic_buf = tmp_buf + sizeof(CHAN_BITMAP_ST);

		Hisi_2bit_change_16bit(pic_buf,word_buf);

		*pic_width = CHAR_W*X_MAX;//80;
		*pic_height = CHAR_H;//18;
		if (flag_1080p || flag_ov2710)
		{
			*pic_width = CHAR_W*X_MAX*2;
			*pic_height = CHAR_H*2;
		}

		return word_buf;	
		
	}else
	{
		printf("open file error!\n");
	}

GET_ERROR:

	if( fp )
		fclose(fp);

	if( tmp_buf )
		free(tmp_buf);

	if( word_buf )
		free(word_buf);

	return NULL;
}


int GM_save_chan_bitmap_file(char * tmp_buf ,int fileOffset,char * bit_file)
{
	FILE * fp = NULL;
	int rel;	

	fp = fopen(bit_file,"wb");

	if( fp )
	{
		rel = fwrite(tmp_buf,1,fileOffset,fp);
		if( rel != fileOffset )
		{
			printf(" fwrite  Error!=%d \n",rel);			
		}	

		fclose(fp);

		return 1;
	}
	

GET_ERROR:
	
	if( fp )
		fclose(fp);
	
	return -1;
}


char * GM_get_chan_bitmap_file(int * file_length,char * bit_file)
{
	FILE * fp = NULL;
	int rel;
	int fileOffset;
	char * tmp_buf =NULL;
	char * word_buf =NULL;
	char * pic_buf = NULL;	

	fp = fopen(bit_file,"rb");

	if( fp )
	{
		rel = fseek(fp,0L,SEEK_END);
		if( rel != 0 )
		{
			printf("fseek error!!\n");
			goto GET_ERROR;
		}

		fileOffset = ftell(fp);
		if( fileOffset == -1 )
		{
			printf(" ftell error!\n");
			goto GET_ERROR;
		}

		//printf(" fileOffset = %ld\n",fileOffset);

		rewind(fp);	

		tmp_buf = (char*)malloc(fileOffset);
		if( tmp_buf == NULL)
		{
			printf(" malloc Error!=n");
			goto GET_ERROR;
		}

		rel = fread(tmp_buf,1,fileOffset,fp);
		if( rel != fileOffset )
		{
			printf(" fread Error!=n");
			goto GET_ERROR;
		}		
		
		

		if( fp )
			fclose(fp);		

		*file_length = fileOffset;	

		return tmp_buf;	
		
	}else
	{
		printf("open file error!\n");
	}

GET_ERROR:

	if( fp )
		fclose(fp);

	if( tmp_buf )
		free(tmp_buf);

	if( word_buf )
		free(word_buf);

	return NULL;
}


int  GST_MENU_gui_msg_set_chan_bitmap(char * msg)
{	
	int length = 0;
	int ret;
	
	if(msg[0] == GetNvrBitmap )
	{		
		memcpy(&length,&msg[1],4);

		printf("length = %d\n",length);

		if( length > 10 )
		{
			ret = GM_save_chan_bitmap_file(&msg[5],length - 5,"/mnt/mtd/chan.bit");
			if( ret < 0 )
			{
				printf("GM_save_chan_bitmap_file error\n ");
				return -1;
			}else
			{
				bReset = 1;
				printf("GM_save_chan_bitmap_file success\n ");
			}
		}	
		
		return 1;
	}else
		return 0;
}


int  GST_MENU_gui_msg_get_chan_bitmap(char * msg,int * length)
{
	char * file_buf = NULL;
	int file_length = 0;
	
	if(msg[0] == GetNvrBitmap )
	{
		file_buf = GM_get_chan_bitmap_file(&file_length,"/mnt/mtd/chan.bit");
		if( file_buf == NULL )
			return -1;

		printf("send bitmap file len =%d\n",file_length);

		memcpy(msg,file_buf,file_length);

		* length = file_length;

		free(file_buf);
		
		return 1;
	}else
		return -1;
}



