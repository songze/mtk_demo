#include "mrc_base.h"

#include "demo.h"

#include "icon.h"

static T_ICON_MAP icon_map[MAX_IMAGE_COUNT];

void ADD_ICON(int16 id,const char* name,int16 w,int16 h)
{
	int32 ret = 0;
	icon_map[id].width =w;
	icon_map[id].height =h;
	ret = mrc_readFileFromMrpEx(NULL, name, (uint8**)&(icon_map[id].pdata), (int32 *)&(icon_map[id].len), 0);
	if( ret == MR_FAILED)
	{
		ret =mrc_ferrno();
		return;
	}
}

P_ICON_MAP getIcon(int32 index)
{
	if(index > MAX_IMAGE_COUNT)
		return NULL;
	else
		return &icon_map[index];
}

void IconDestroy(void)
{
	uint8  i;
	for( i=0; i<MAX_IMAGE_COUNT; i++)
	{
		if(icon_map[i].pdata != NULL && icon_map[i].len > 0)
		{
			free(icon_map[i].pdata), icon_map[i].pdata = NULL;
			icon_map[i].len = 0;
			icon_map[i].width = 0;
			icon_map[i].height = 0;
		}
	}
}

void freeIcon(uint8 index)
{
	if(icon_map[index].pdata != NULL && icon_map[index].len > 0)
	{
		free(icon_map[index].pdata), icon_map[index].pdata = NULL;
		icon_map[index].len = 0;
		icon_map[index].width = 0;
		icon_map[index].height = 0;
	}
}