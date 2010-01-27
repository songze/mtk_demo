#include "mrc_base.h"
#include "mrc_win.h"


#include "res_str.h"
#include "i18n.h"

#include "icon.h"
#include "demo.h"


#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240

void DemoWinShow(int32 data);

int32 g_pos_y;
int32 g_timer, g_is_sound;
int32 x, y;
RECODE_S g_demoRec;

int32 DemoWriteRecode(PRECODE_S pRecode)
{
	int32 fd = 0;
	fd = mrc_open("rec.dat", MR_FILE_CREATE|MR_FILE_RDWR);
	if(!fd)
		return MR_FAILED;

	mrc_write(fd, pRecode, sizeof(RECODE_S));
	mrc_close(fd), fd = 0;

	return MR_SUCCESS;
}

int32 DemoReadRecode(PRECODE_S pRecode)
{
	int32 fd = 0;
	int32 len = 0;

	fd = mrc_open("rec.dat", MR_FILE_RDONLY);
	if(!fd)
		return MR_FAILED;

	len = mrc_read(fd, pRecode, sizeof(RECODE_S));
	mrc_close(fd);
	if(len != sizeof(RECODE_S))
		return MR_FAILED;
	else
		return MR_SUCCESS;
}

void DemoWinWinEvent(int32 data, int32 eventId)
{
	switch (eventId)
	{
	case WIN_EVENT_SHOW:
		break;
	case WIN_EVENT_REFRESH:
		break;
	case WIN_EVENT_PAUSE:
		break;
	case WIN_EVENT_EXIT:
		break;
	case WIN_EVENT_UPDATE:
		break;
	}
}

void DemoKeyWinEvent(int32 data, int32 type, int32 p1, int32 p2)
{
	if(MR_KEY_PRESS == type)
	{
		switch(p1)
		{
		case MR_KEY_LEFT:
		case MR_KEY_4:
			x -= 5;
			break;
		case MR_KEY_RIGHT:
		case MR_KEY_6:
			x += 5;
			break;						
		case MR_KEY_UP:
		case MR_KEY_2:
			y -= 5;
			break;
		case MR_KEY_DOWN:
		case MR_KEY_8:
			y += 5;
			break;
		case MR_KEY_1:
			g_is_sound = !g_is_sound;
			if(1 == g_is_sound)
				mrc_soundPlay(0, 1);
			else
				mrc_soundStop(0);

			DemoWriteRecode(&g_demoRec);
			break;
		case MR_KEY_SOFTRIGHT:
			if(1 == g_is_sound)
				mrc_soundStop(0);

			mrc_soundSet(0, "*",MR_SOUND_MIDI);

			if(g_timer)
				mrc_timerStop(g_timer), mrc_timerDelete(g_timer), g_timer = 0;

			IconDestroy();
			mrc_winClose();
			//mrc_exit();
			break;
		}
	}
	else if(MR_MOUSE_DOWN == type) /* 触摸屏释放 */
	{

	}
}

void DemoPaintBK(void)
{	
	int16 y=0;
	uint8 num = 0,i;
	P_ICON_MAP bmp = NULL;
	bmp = getIcon(IMAGE_BK);
	
	if(!bmp || !bmp->pdata)
		return;
	num = (SCREEN_HEIGHT - g_pos_y)/bmp->height;
	if((SCREEN_HEIGHT - g_pos_y)%bmp->height != 0)
		num++;

	if(g_pos_y>0)
		mrc_bitmapShowEx((uint16*)bmp->pdata,0,y,bmp->width,bmp->width,
			g_pos_y,BM_COPY,0,bmp->height - g_pos_y);

	for( i = 0; i < num; i ++ )
	{
		y = i*bmp->height + g_pos_y;
		mrc_bitmapShowEx((uint16*)bmp->pdata,0,y,bmp->width,
			bmp->width,bmp->height,BM_COPY,0,0);
	}
}

void DemoDrawPlane(int32 x, int32 y)
{
	P_ICON_MAP bmp = getIcon(IMAGE_PLANE);
	if(bmp && bmp->pdata)
		mrc_bitmapShowEx(bmp->pdata, x,y,bmp->width,bmp->width,
			bmp->height,BM_TRANSPARENT,0,0);
}


void DemoWinShow(int32 data)
{
	int32 w, h;
	if(g_timer)
		mrc_timerStart(g_timer, 100, 0, DemoWinShow, 0);

	DemoPaintBK();
	DemoDrawPlane(x, y);

	mrc_textWidthHeight((char*)SGL_LoadString(STR_OK), 1, MR_FONT_MEDIUM, &w, &h);
	mrc_drawText((char*)SGL_LoadString(STR_OK), 0, 320-h, 0, 0, 0, 1, MR_FONT_MEDIUM);

	mrc_textWidthHeight((char*)SGL_LoadString(STR_EXIT), 1, MR_FONT_MEDIUM, &w, &h);
	mrc_drawText((char*)SGL_LoadString(STR_EXIT), 240-w, 320-h, 0, 0, 0, 1, MR_FONT_MEDIUM);

	g_pos_y += 3;
	if( g_pos_y > 44 )
		g_pos_y = 0;

	mrc_refreshScreen(0, 0, 240, 320);
}

void DemoResourceInit(void)
{
	mrc_soundSet(0, "background.mid", MR_SOUND_MIDI);
	ADD_ICON(IMAGE_PLANE, "plane.bmp", 41, 26);
	ADD_ICON(IMAGE_BK, "bk.bmp", 240, 44);
}
// static int32 pos;
// void timerCallBk(int32 data)
// {
// 	mrc_clearScreen(255,255, 255);
// 	if(g_timer)
// 		mrc_timerStart(g_timer, 500, data, timerCallBk, 0);
// 
// 	if(pos < mrc_wstrlen((char*)data))
// 		pos += 2;
// 	else
// 		pos = 0;
// 
// 	mrc_drawText(((char*)data)+pos,0,10,0,0,0,1,MR_FONT_MEDIUM);
// 	mrc_refreshScreen(0,0,240,320);
// }

int32 create_win();
static int32 g_fresh_timer;
static BOOL g_fresh_started;

void fresh_timer_cb(int32 data)
{
	mrc_winWinEvent(WIN_EVENT_SHOW);
}

int32 start_freshtimer()
{
    if(g_fresh_timer)
    {
		if(mrc_timerStart(g_fresh_timer, 100, 0, fresh_timer_cb, 1) == MR_SUCCESS)
        {
            g_fresh_started = TRUE;
            return MR_SUCCESS;
        }        
    }
    return MR_FAILED;
}

void stop_freshtimer()
{
    if(g_fresh_timer && g_fresh_started)
    {
        mrc_timerStop(g_fresh_timer);
        g_fresh_started = FALSE;
    }   
}


int32 mrc_init(void)
{
	g_timer = 0;
	x = y = g_pos_y = g_is_sound = 0;
	memset(&g_demoRec, 0, sizeof(g_demoRec));
	mrc_winInit();	
	DemoResourceInit();
	SGL_SetCurrentLanguage(1000);
	DemoReadRecode(&g_demoRec);

	g_is_sound = g_demoRec.isSound;

	if(1 == g_is_sound)
		mrc_soundPlay(0, 1);
	else
		mrc_soundStop(0);

	//g_timer = mrc_timerCreate();
    g_fresh_timer = mrc_timerCreate();
    start_freshtimer();
	//mrc_winNew(0, DemoWinWinEvent, DemoKeyWinEvent);

	create_win();

	return MR_SUCCESS;
}

// int32 mrc_init(void)
// {
// 	char* pstr = NULL;
// 	g_timer = 0;
// 	pos = 0;
// 	SGL_SetCurrentLanguage(1000);
// 	pstr = (char*)SGL_LoadString(STR_TEST);
// 
// 	g_timer = mrc_timerCreate();
// 	if(g_timer)
// 		mrc_timerStart(g_timer, 500, (int32)pstr, timerCallBk, 0);
// 
// 	return MR_SUCCESS;
// }

int32 mrc_event(int32 code, int32 param0, int32 param1)
{
	mrc_winEvent(code, param0, param1);
	//DemoKeyWinEvent(1, code, param0, param1);
	return MR_SUCCESS;
}

int32 mrc_pause(void)
{
	if(1 == g_is_sound)
		mrc_soundStop(0);
	return MR_SUCCESS;
}

int32 mrc_resume(void)
{
	if(1 == g_is_sound)
		mrc_soundPlay(0,1);
	return MR_SUCCESS;
}

int32 mrc_exitApp(void)
{
	return MR_SUCCESS;
}  
/********************************************************************/

void draw_control_grid();
void draw_tip();
void draw_next();
void draw_return();

#define COL 4
#define ROW 3

#define LINE_R 20
#define LINE_G 50
#define LINE_B 40
#define FOCUS_R 150
#define FOCUS_G 80
#define FOCUS_B 50

#define PADDING_X 5
#define PADDING_Y 5
#define TEXT_HEIGHT 100

static int32 g_dialog = MR_FAILED;
static int32 g_text_fd = MR_FAILED;
static int32 g_edit_fd = MR_FAILED;
static int32 g_menu_fd = MR_FAILED;

/* create dialog */
int32 create_dialog()
{
    const char * title = "\x68\x07\x98\x98\x00\x00";
    const char * text = "\x8f\xd9\x66\x2f\x5b\xf9\x8b\xdd\x68\x46\x51\x85\x5b\xb9\xff\x0c\x66\x2f\x54\x26\x77\x0b\x52\x30\xff\x1f\x00\x00";

    return (g_dialog = mrc_dialogCreate(title, text, MR_DIALOG_OK_CANCEL));
}
void release_dialog()
{
     if(g_dialog != MR_FAILED)
     {
        if(mrc_dialogRelease(g_dialog) == MR_SUCCESS)
        {
            g_dialog = MR_FAILED;
        }
     }
}

int32 create_text()
{
    const char* text = "\x8f\xd9\x66\x2f\x52\x1a\x52\x1b\x5e\xfa\x76\x84\x65\x87\x67\x2c\x68\x46\x51\x85\x5b\xb9\xff\x0c\x66\x2f\x54\x26\x77\x0b\x89\xc1\xff\x1f\x00\x00";
   
    return (g_text_fd = mrc_textCreate("\x68\x07\x98\x98\x00\x00", text, MR_DIALOG_CANCEL));
}
void release_text()
{
    if(g_text_fd != MR_FAILED)
    {
        if(mrc_textRelease(g_text_fd) == MR_SUCCESS)
        {
            g_text_fd = -1;
        }
    }
}

int32 create_edit()
{
    const char* title = "\x8b\xf7\x8f\x93\x51\x65\x59\xd3\x54\x0d\x00\x00";
    const char * text = "\x68\x07\x98\x98\x00\x00";

    return (g_edit_fd = mrc_editCreate(title, text, MR_EDIT_ANY, 10));
}
void release_edit()
{
    if(g_edit_fd != MR_FAILED)
    {
        if(mrc_editRelease(g_edit_fd) == MR_SUCCESS)
        {
            g_edit_fd = -1;
        }
    }
}

int32 create_menu()
{
    const char* title = "\x83\xdc\x53\x55\x68\x07\x98\x98\x00\x00";
    
    g_menu_fd = mrc_menuCreate(title, 3);
    if(g_menu_fd != MR_FAILED)
    {
        int i;
        char buf[32];
        for(i=0; i < 3; ++i)
        {
            mrc_sprintf(buf, "menu %d", i+1); 
            mrc_menuSetItem(g_menu_fd, buf, i);
        }
        mrc_menuShow(g_menu_fd);
    }
    return g_menu_fd;
}
void release_menu()
{
    if(g_menu_fd != MR_FAILED)
    {
        if(mrc_menuRelease(g_menu_fd) == MR_SUCCESS)
        {
            g_menu_fd = -1;
        }
    }
}

       
typedef int32 (*create_fun)();
typedef void (*release_fun)();

typedef struct grid_info
{
	const char * text;
	create_fun create;
    release_fun    release;
}grid_info;


static int g_current_pos;
static grid_info actions[] = {
	{"select create dialog", create_dialog, release_dialog},
	{"press enter create text", create_text, release_text},
	{"press select create edit", create_edit, release_edit},
	{"to create menu", create_menu, release_menu},
	{NULL, NULL, NULL}
};
#define ACTION_LEN (sizeof(actions)/sizeof(grid_info))

void refresh_screen()
{
	mrc_clearScreen(230, 200, 200);
	draw_control_grid();
    draw_tip();
	draw_next();
	draw_return();
	mrc_refreshScreen(0, 0, 240, 320);
}

void TestWinWinEvent(int32 data, int32 eventId)
{
	switch (eventId)
	{
	case WIN_EVENT_SHOW:
		{
			refresh_screen();			
		}
		break;
	case WIN_EVENT_REFRESH:
		break;
	case WIN_EVENT_PAUSE:
		break;
	case WIN_EVENT_EXIT:
		break;
	case WIN_EVENT_UPDATE:
		break;
	}
}

void TestKeyWinEvent(int32 data, int32 type, int32 p1, int32 p2)
{
	if(MR_KEY_PRESS == type)
	{
		int pos = g_current_pos % COL;

		mrc_printf("**************g_current_pos %d\n", g_current_pos);
		switch(p1)
		{
		case MR_KEY_LEFT:
			if(pos != 0)
			{
				g_current_pos -= 1;
			}
			break;
		case MR_KEY_RIGHT:
			if(pos != COL-1)
			{
				g_current_pos += 1;
			}
			break;
		case MR_KEY_UP:
			g_current_pos -= COL;
			if(g_current_pos < 0)
			{
				g_current_pos += COL;
			}
			break;
		case MR_KEY_DOWN:
			g_current_pos += COL;
			if(g_current_pos >= ROW * COL)
			{
				g_current_pos -= COL;
			}
			break;
		case MR_KEY_SELECT: /* select */
			if(g_current_pos < ACTION_LEN)
			{
               if(actions[g_current_pos].create() != MR_FAILED)
               {
                    stop_freshtimer();
               }
			}
			break;
		}
	}
    else if(MR_DIALOG_EVENT == type)
    {
        if(g_current_pos < ACTION_LEN)
        {
            start_freshtimer();
            actions[g_current_pos].release();
        }
       
        if(p1 == MR_DIALOG_KEY_OK)
        {
            
        }
        else if(p1 == MR_DIALOG_KEY_CANCEL)
        {
        }
    }
    else if(MR_MENU_SELECT == type)
    {
    }
 }

/* 创建常规窗口 */
int32 create_win()
{
	int ret;
	g_current_pos = 0;
	if(!(ret = mrc_winNew(0, TestWinWinEvent, TestKeyWinEvent)))
	{
		return ret;
	}
}

void calc_rect(mr_screenRectSt* rect, int pos, int w, int h)
{
	rect->x = (pos % COL)*(w+1) + PADDING_X;
	rect->y = (pos / COL)*(h+1) + PADDING_Y;
	rect->w = w-4;
	rect->h = h-4;	
}

void draw_tip()
{
    char buf[128];
    mr_screenRectSt rect;
    mr_colourSt colour;
    if(g_current_pos < ACTION_LEN && g_current_pos >= 0 
        && actions[g_current_pos].text != NULL)
    {
         mrc_strncpy(buf, actions[g_current_pos].text, 127);                
    }
    else
    {
         mrc_strncpy(buf, "use 'left', 'right', 'top', 'down' to change your select", 127);
    }
    
    rect.x = 20;
    rect.y = 320 -80;
    rect.w = 200;
    rect.h = 80;
    colour.r = colour.g = colour.b = 120;
    mrc_drawTextEx(buf, 20, 320 - 80, rect, colour, DRAW_TEXT_EX_IS_AUTO_NEWLINE, MR_FONT_BIG);
   
}

void draw_control_grid()
{
	mr_screeninfo info;
	mr_screenRectSt rect;
	int w, h;
	int i, j;
	int n;
	int x1, x2, y1, y2;
	uint8 buf[8];

	mrc_getScreenInfo(&info);
	
	w = (info.width - PADDING_X*2) / COL;
	h = (info.height - PADDING_Y*2 - TEXT_HEIGHT) / ROW;


	/* draw coloumn */
	y1 = PADDING_Y;
	y2 = y1 + ROW * h;
	for(i=0; i<=COL; ++i)
	{
		x1 = PADDING_X+i*w;
		mrc_drawLine(x1, y1, x1, y2, LINE_R, LINE_G, LINE_B);
	}
	/* draw rows */
	x1 = PADDING_X;
	x2 = x1 + COL * w;
	for(i=0; i<=ROW; ++i)
	{
		y1 = PADDING_Y+i*h;
		mrc_drawLine(x1, y1, x2, y1, LINE_R, LINE_G, LINE_B);
	}

	/* draw focus */
	if(g_current_pos != -1)
	{
		calc_rect(&rect, g_current_pos, w, h);
		mrc_drawRect(rect.x, rect.y, rect.w, rect.h, FOCUS_R, FOCUS_G, FOCUS_B);
	}

	/* draw effect */
	//mrc_EffSetCon(0, 0, 240, 320, 250, 128, 128);

	/* draw items */

	n = COL * ROW;
	for(i = 0; i < n; ++i)
	{		
		mrc_sprintf(buf, "%d", i+1); 
		calc_rect(&rect, i, w, h);
		mrc_drawText(buf, rect.x+2, rect.y, 20, 30, 20, 0, MR_FONT_BIG); 
	}
}



void draw_next()
{
	int w,h;
	const char * str = "next";
	mrc_textWidthHeight(str, 0, MR_FONT_MEDIUM, &w, &h);
	mrc_drawText(str, 0, 320-h, 0, 0, 0,0, MR_FONT_MEDIUM);
}
		
void draw_return()
{
	int w,h;
	const char * str = "return";
	mrc_textWidthHeight(str, 0, MR_FONT_MEDIUM, &w, &h);
	mrc_drawText(str, 240-w, 320-h, 0, 0, 0,0, MR_FONT_MEDIUM);

}