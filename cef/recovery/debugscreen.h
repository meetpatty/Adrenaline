/*****************************************
 * Recovery - mydebug			 *
 * 			by harleyg :)	 *
 *****************************************/

#ifndef __DEBUGSCREEN_H__
#define __DEBUGSCREEN_H__

#include <psptypes.h>
#include <pspmoduleinfo.h>

#ifdef __cplusplus
extern "C" {
#endif

void rDebugScreenInit(void);

void rDebugScreenPrintf(const char *fmt, ...) __attribute__((format(printf,1,2)));

void rDebugScreenSetBackColor(u32 color);
void rDebugScreenSetTextColor(u32 color, u32 back);

void rDebugScreenPutChar(int x, int y, u32 color, u8 ch);

void rDebugScreenSetXY(int x, int y);
int rDebugScreenGetX(void);
int rDebugScreenGetY(void);

void rDebugScreenSetOffset(int offset);

void rDebugScreenClear(void);
void rDebugScreenClearLineWithColor(u32 color);

int rDebugScreenPrintData(const char *buff, int size);

#ifdef __cplusplus
}
#endif

#endif
