#include <raylib.h>
#include <stdlib.h>
#include "map_parser.h"

#define XOR_KEY 0x5A

char *data;
char *data_copy_ptr;
int level = 0;

void LoadMapData(Map *map)
{
    level++;
    map->level = level;
	map->row= 1;
    map->column = 0;

    if(level==1)
    {
        data = LoadFileText("../resources/level-map/map.bin");
        int data_len = TextLength(data);
        for(int i=0; i<data_len; i++)
            data[i] ^= XOR_KEY;
        data_copy_ptr = data;
    }
    
    const char *sub_data = TextSubtext(data, 0, TextFindIndex(data, "E")-1);
    data += TextFindIndex(data, "E")+2;

    int sub_data_len = TextLength(sub_data);

    for(int i=0; i<sub_data_len; i++)
    {
		map->column++;
        if(sub_data[i]=='\n')
		{
			map->column = 0;
			map->row++;
		}
    }
    if(level==1)
        map->data = (char *)malloc(map->row*map->column*sizeof(char));
    else
        map->data = (char *)realloc(map->data, map->row*map->column*sizeof(char));

    int c = 0;
    for(int i=0; i<sub_data_len; i++)
        if(sub_data[i]!='\n')
            map->data[c++] = sub_data[i];
}

void UnloadMapData(Map *map)
{
    UnloadFileText(data_copy_ptr);
    if(map->data!=NULL)
        free(map->data);
    else
        fprintf(stderr, "ERROR: Could not load level due to insufficient storage. Terminating program.");
}
