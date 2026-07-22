#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "player_sv.h"
#include "json_storage.h"
/*
玩家服务：创建、加载、保存、升级
─ json_storage
── memory_pool

*/

//创建角色
User *create(const char *name, int race, int class_id){

    // cJSON_AddRawToObject
}
//加载角色
User *load(const char *name){
    char *path = malloc(256);
    if (path == NULL) return NULL;
    snprintf(path, 256, "../../save/player_%s.json", name);
    cJSON *root = json_load_file(path);
    User *user = json_parse_user(root);
    cJSON_Delete(root);
    free(path);
    printf("this is player_sv\n");
    printf("username:%s\n",user->username);
    return user;
}
//保持数据
int save(User *user){

}
//删除用户内存
void release(User *user){
    free(user);
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



int main(){
    User *user = load("剑心无痕");
    user->gold += 300;
    cJSON *root = json_serialize_user(user);

    char *path = malloc(256);
    if (path ==NULL) {
        printf("创建路径内存空间失败\n");
        return 1;
    }
    snprintf(path,256,"../../save/player_%s.json",user->username);
    int status =json_save_file(path,root);
    free(path);
    if(status == 0){
        printf("保存成功\n");
        return 0;
    } else{
        printf("保存失败\n");
        return -1;
    }

}