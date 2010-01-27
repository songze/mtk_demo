#ifndef _ICON_H_
#define _ICON_H_

typedef struct
{	
	int32 len;
	void* pdata;
	uint16 width;
	uint16 height;
}T_ICON_MAP, *P_ICON_MAP;

enum{
	IMAGE_BK,
	IMAGE_PLANE,

	MAX_IMAGE_COUNT
};

extern void ADD_ICON(int16 id,const char* name,int16 w,int16 h);
extern P_ICON_MAP getIcon(int32 index);
extern void freeIcon(uint8 index);
extern void IconDestroy(void);
#endif