#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "player_sv.h"
/*
玩家服务：创建、加载、保存、升级
─ json_storage
── memory_pool

*/

//创建角色
User *create(const char *name, int race, int class_id){


}
//加载角色
User *load(const char *name){
    char *path = malloc(256);
    if (path == NULL) return NULL;
    snprintf(path, 256, "../../save/player%s.txt", name);
}
//保持数据
int save(User *user){

}
//删除用户内存
void release(User *user){

}
//升级
void levele_up(User *user){

}
//加经验
void add_exp(User *user, int exp){

}
//加金币
void add_gold(User *user,int gold){

}
//计算战斗属性
void calc_battle_stats(User *user){

}

