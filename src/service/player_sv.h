#ifndef _PLAYER_SV_H_
#define _PLAYER_SV_H_
#define ITEM_MAX 20
#define ONE 1
#define ZERO 0
#define SLOT_MAX 6
#define MAX_PLAYER_TASKS 20   // 最多同时接取20个任务
#include "json_storage.h"

typedef struct _fileList{
    int size;
    int capacity;  //列表容量
    char **array;  //字符串数组
}fileList;

int is_name_exists(char *name);
fileList *getFileList();
void calc_battle_stats(User *user);
#endif