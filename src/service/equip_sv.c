#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "equip_sv.h"

/*
equip_sv

equip_sv.c

装备服务：装备管理、属性计算

*/


//装备配置文件路径
int init(){
    //初始化路径 
    char *path = "../../data/equips.json";
    cJSON *root= json_load_file(path);
    int size = cJSON_GetArraySize(root);
    for(int i=0;i<size;i++){

    
        Equips* equips = json_parse_equipment(root);
        char* str = cJSON_Print(root);
        printf("%s\n",str);
    }

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