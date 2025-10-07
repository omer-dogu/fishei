#ifndef _OLED_H
#define _OLED_H

void OLED_Command(unsigned char cmd);
void OLED_Data(unsigned char val);
void OLED_SetPage(unsigned char page);
void OLED_SetSegment(unsigned char segment);
void OLED_FillPage(unsigned char page, unsigned char c);
void OLED_FillDisplay(unsigned char c);
void OLED_ClearDisplay(void);
void OLED_Rotate(int bRotate);
void OLED_Start(int bRotate);

void OLED_SetFont(int font);

void OLED_PutChar(char c);
void OLED_SetCursor(int row, int col);
void OLED_GetCursor(int *pRow, int *pCol);
void OLED_Scroll(int nLines);
void OLED_Return(void);
void OLED_NewLine(void);

void OLED_SetPixel(int x, int y, int c);
int OLED_GetPixel(int x, int y);
void OLED_Line(int x0, int y0, int x1, int y1, int c);

void OLED_putch(char c);

#define _STDIN  0
#define _STDOUT 1
#define _STDERR 2

#define FNT_SMALL       0       // 5x8
#define FNT_LARGE       1       // 7x16
#define FNT_BIG         2       // 15x32

#define OL_CLRPIXEL     0
#define OL_SETPIXEL     1
#define OL_INVPIXEL     2

#endif
