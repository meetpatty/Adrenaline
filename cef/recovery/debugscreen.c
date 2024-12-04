/*****************************************
 * Recovery - mydebug                    *
 *                      by harleyg :)    *
 *****************************************/


#include <stdio.h>
#include <psptypes.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspge.h>
#include <stdarg.h>
//#include <pspdebug.h>

//#include "main.h"

#define PSP_SCREEN_WIDTH 480
#define PSP_SCREEN_HEIGHT 272
#define PSP_LINE_SIZE 512
#define PSP_PIXEL_FORMAT 3

static int X = 0, Y = 0;
static const int MX=68, MY=34;
static u32 bg_col = 0x00000000, fg_col = 0, fb_col = 0;
static u32* g_vram_base = (u32 *) 0x04000000;
static int g_vram_offset = 0;

static int init = 0;

static void clear_screen(u32 color)
{
	int x;
	u32 *vram = g_vram_base + (g_vram_offset>>2);

	for(x = 0; x < (PSP_LINE_SIZE * PSP_SCREEN_HEIGHT); x++)
	{
		*vram++ = color; 
	}

}

void rDebugScreenInit()
{
   X = Y = 0;
   /* Place vram in uncached memory */
   g_vram_base = (void *) (0x40000000 | (u32) sceGeEdramGetAddr());
   g_vram_offset = 0;
   sceDisplaySetMode(0, PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT);
   sceDisplaySetFrameBuf((void *) g_vram_base, PSP_LINE_SIZE, PSP_PIXEL_FORMAT, 1);
   clear_screen(bg_col);
   init = 1;
}

void rDebugScreenSetBackColor(u32 colour)
{
   bg_col = colour;
}

void rDebugScreenSetTextColor(u32 colour, u32 back)
{
	fg_col = colour;
	fb_col = back;
}

extern u8 msx[];

u8 *font_addr = msx;

void rDebugScreenPutChar( int x, int y, u32 color, u8 ch)
{
	int 	i,j, l;
	u8	*font;
	u32  pixel;
	u32 *vram_ptr;
	u32 *vram;

	if(!init) return;

	vram = g_vram_base + (g_vram_offset>>2) + x;
	vram += (y * PSP_LINE_SIZE);

	font = &font_addr[ (int)ch * 8];
	for (i=l=0; i < 8; i++, l+= 8, font++)
	{
		vram_ptr  = vram;
		for (j=0; j < 8; j++)
		{
			if ((*font & (128 >> j)))
				pixel = color;
			else
				pixel = fb_col;
//			pixel = bg_col;

			*vram_ptr++ = pixel; 
		}
		vram += PSP_LINE_SIZE;
	}
}

void  clear_line( int Y , u32 color)
{
	int x;
	u32 *vram = g_vram_base + (g_vram_offset>>2);
	vram += ( Y * PSP_LINE_SIZE) << 3;

	for(x = 0; x < (PSP_LINE_SIZE << 3); x++)
	{
		*vram++ = color;
	}
}

void rDebugScreenClearLineWithColor(u32 color)
{
	clear_line( Y, color);
}

/* Print non-nul terminated strings */
int rDebugScreenPrintData(const char *buff, int size)
{
	int i;
//	int j;
	char c;

	for (i = 0; i < size; i++)
	{
		c = buff[i];
		switch (c) 
		{
		case '\n':
			X = 0;
			Y ++;
			if (Y == MY)
				Y = 0;
			clear_line(Y , bg_col);
			break;
		case '\t':
//			for (j = (X & 31); X < 32; j++) {
			while(X < 32) {
				rDebugScreenPutChar( X*7 , Y * 8, fg_col, ' ');
				X++;
			}
			break;
		default:
			rDebugScreenPutChar( X*7 , Y * 8, fg_col, c);
			X++;
			if (X == MX)
			{
				X = 0;
				Y++;
				if (Y == MY)
					Y = 0;
				clear_line(Y , bg_col);
			}
		}
	}

	return i;
}

void rDebugScreenPrintf(const char *format, ...)
{
	va_list	opt;
	char buff[2048];
	int	bufsz;

	if(!init) {
		return;
	}

	va_start(opt, format);
	bufsz = vsnprintf( buff, (size_t) sizeof(buff), format, opt);
//	bufsz = my_vsprintf( buff, format, opt);
	va_end ( opt );

	(void) rDebugScreenPrintData(buff, bufsz);
}


void rDebugScreenSetXY(int x, int y)
{
	if( x<MX && x>=0 ) X=x;
	if( y<MY && y>=0 ) Y=y;
}

void rDebugScreenSetOffset(int offset)
{
        g_vram_offset = offset;
}

int rDebugScreenGetX()
{
	return X;
}

int rDebugScreenGetY()
{
	return Y;
}

void rDebugScreenClear()
{
	if(!init)
	{
		return;
	}

	rDebugScreenSetXY(0,0);
	clear_screen(bg_col);
}
