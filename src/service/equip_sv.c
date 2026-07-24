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
int init(void)
{
    const char *path = "../../data/items.json";
    cJSON *root = json_load_file(path);
    if (!root) return -1;
    int size = cJSON_GetArraySize(root);
    for (int i = 0; i < size; i++) {
        cJSON *tmp = cJSON_GetArrayItem(root, i);
        if (!tmp) {                       /* 修复1：取不到 = 失败，返回错误码 */
            cJSON_Delete(root);
            return -1;
        }
        Equips *equips = json_parse_equipment(tmp);
        if (!equips) {                    /* 修复3：判空，防 NULL 解引用 */
            cJSON_Delete(root);
            return -1;
        }
        if (g_equip_count >= MAX_EQUIPS) { /* 修复4：容量保护（换成你数组真实容量） */
            free(equips);
            cJSON_Delete(root);
            return -1;
        }
        memcpy(&g_equips[g_equip_count], equips, sizeof(Equips));  /* 修复6：用 count 做下标 */
        g_equip_count++;
        free(equips);                     /* 修复3：malloc 来的，用完必须 free */
    }
    cJSON_Delete(root);
    return g_equip_count;                 /* 修复5：成功路径返回加载数量 */
}
int main(){
    init();
    return 0;
}
//获取装备信息
Equips *get_by_id(int id){
    for(int i =0; i<g_equip_count;i++){
        if(g_equips[i].id == id){
            return &g_equips[i];
        }
    }
    return NULL;
}
//穿戴装备
int equip(User *user, int equip_id){

    int tmp =0;
    for(int i=0;i<user->inventory_count;i++){
        if(user->invertory[i][0] == equip_id){
            tmp = 1;
        }
    }
    if(tmp == 0) {
        printf("没有该装备\n");
        return 3;
    }
    Equips *equips = get_by_id(equip_id);
    if(!equips) return 3;
    //装备栏
    if(user->equipment[equips->slot] >0){
        //装备栏有东西了
        unequip(user,equips->slot);
    }
    user->equipment[equips->slot] = equip_id;
    //移除背包的的物品，添加移除之后的物品
}
//只有"卸下"（纯增加）才需要检查背包满。
int unequip(User *user, int slot){
    if(user == NULL || slot <0) return 2;
    if(user->equipment[slot] <0){
        printf("该槽位没有装备\n");
        return 3;
    }
    Equips * equips = get_by_id(user->equipment[slot]);
    if(user->inventory_count > 20) {
        printf("背包已经满了\n");
                 
    }

    user->equipment[slot] = -1;

}
void calc_bonus(User *user){

}