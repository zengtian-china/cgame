#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json_storage.h"

static ItemConfig g_items[1000];
static int g_items_count;
//初始化 加载数组

void item_mgr_init(){
    memset(g_items,0,sizeof(g_items));
    g_items_count = 0;
}

void item_mgr_load_from_json(){
    const char *path = "../../data/items.json";
    //加载item.json数据
    cJSON *root = json_load_file(path);
    if (!root || cJSON_IsArray(root)) return ;
    //获取json列表长度
    int root_size = cJSON_GetArraySize(root);
    //循环遍历
    for(int i=0;i<root_size;i++){
        //获取单条item的json数据
        cJSON *json = cJSON_GetArrayItem(root,i);
        //判断单条数据是否存在
        if(!json) continue;
        //创建了ItemConfig结构体指针
        ItemConfig *item = json_parse_item(json);
        //判断是否有值
        if(!item){
            // 事情空间失败，将json全部销毁
            cJSON_Delete(root);
            continue;
        }
        g_items[i] = *item;
        g_items_count++;
        free(item);
    }
    cJSON_Delete(root);
}


ItemConfig* item_mgr_get_by_id(int id){
    for(int i=0; i<g_items_count;i++){
        if(g_items[i].id == id) return &g_items[i];
    }
    return NULL;
}
int item_mgr_get_count();