#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "equip_sv.h"
#include "json_storage.h"

/*
equip_sv

equip_sv.c

装备服务：装备管理、属性计算

*/
#define MAX_EQUIPS 100
static Equips g_equips[MAX_EQUIPS];
static int g_equip_count = 0;

//装备配置文件路径
int init(){
    //初始化路径 
    char *path = "../../data/equips.json";
    cJSON *root= json_load_file(path);
    if (!root) return -1;
    int size = cJSON_GetArraySize(root);
    for(int i=0;i<size;i++){
        cJSON *tmp = cJSON_GetArrayItem(root,i);
        if (tmp){
            cJSON_Delete(root);
            return 0;
        }
        Equips* equips = json_parse_equipment(tmp);
        printf("第%d回合 name:%s\n",i+1,equips->name);
        char* str = cJSON_Print(tmp);
        printf("%s\n",str);
        cJSON_Delete(str);
        memcpy(&g_equips[i],equips,sizeof(Equips));
        g_equip_count++;
    }

    cJSON_Delete(root);

}

int main(){
    init();
    return 0;
}
//获取装备信息
Equips *get_by_id(int id){

}
//穿戴装备
int equip(User *user, int equip_id){

}
//卸下装备
int unequip(User *user, int slot){

}
void calc_bonus(User *user){

}