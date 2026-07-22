#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<dirent.h>
#include "player_sv.h"
#include "json_storage.h"
#define MAX_FILELIST 20
/*
// 登录，注册 生成一个密钥目录 该密钥下有角色信息(可以限制文件数量)
// 在线战斗  
玩家服务：创建、加载、保存、升级
─ json_storage
── memory_pool

*/

//创建角色
User *create(const char *name, int race, int class_id){
    User *user = calloc(1,sizeof(User));
    if (user == NULL){
        printf("角色用户内存空间申请失败\n");
        return NULL;
    }
    strcpy(user->username,name);
    user->exp = 0;
    user->gold = 1000;
    user->race = race;
    user->class = class_id;
    // 五维基础属性
    user->strength = 10;
    user->physique = 10;
    user->endurance = 10;
    user->agility = 10;
    user->intelligence = 10;
    // 战斗属性
    user->max_hp = 200;
    user->hp = 200;
    user->max_mp = 100;
    user->mp = 100;
    user->attack = 15;
    user->defense = 10;
    user->speed = 10;
    user->magic_attack = 10;
    user->magic_defense = 10;
    user->dodge_rate = 0.01f;
    user->crit_rate = 0.01f;
    user->inventory_count = 0;
    // 技能（初始无技能）
    user->skill_count = 0;
    // 地图与坐标（新手村出生点）
    user->current_map = 1001;
    user->pos_x = 128;
    user->pos_y = 128;
    // 声望
    user->contribution = 0;
    user->reputation = 0;
    return user;
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
    //先转成json数据
    cJSON *json = json_serialize_user(user);
    char *path = malloc(256);
    if (path ==NULL) {
        printf("创建路径内存空间失败\n");
        return 1;
    }
    snprintf(path,256,"../../save/player_%s.json",user->username);
    int status = json_save_file(path,json);
    free(path);
    if(status == 0){
        return 0;
    }else{
        printf("保存失败\n");
        return -1;
    }

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
/*
1. exp累加到user->exp
2. 检查是否满足升级条件
3. 如果满足，调用level_up
4. 递归检查是否连续升级
升级所需经验 = 基础经验 × (1.15 ^ (等级 - 1))
*/
    int max_exp = 100* pow(1.15, user->level - 1);
    user->exp += exp;
    if(user->level <10){
        while (user->exp >= max_exp)
        {
            user->exp = user->exp-max_exp;
            //升级
            levele_up(user);
            
        }
    }
}
//加金币
void add_gold(User *user,int gold){
    if(user ==NULL) return;
    
    user->gold += gold;
}

//计算战斗属性
void calc_battle_stats(User *user){

}



int main1(){
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

int main2(){
    //创建用户
    char *username = malloc(50);
    if (username == NULL) return -1;
    printf("请输入创建的用户名:");
    scanf("%s",username);
    User *user =create(username,2,421);
    save(user);

}

int main3(){
    DIR *dir = opendir("../../save/");
    if( dir == NULL) return -1;
    struct dirent * tmp;
    char **array = malloc(sizeof(char *) *20);
    char i=0;
    while ((tmp =readdir(dir)) != NULL)
    {
        if( strcmp(tmp->d_name,".") == 0 || strcmp(tmp->d_name,"..") == 0) continue;
        printf("%s\n",tmp->d_name);
        array[i] = malloc(256);
        sscanf(tmp->d_name,"player_%255[^.].json",array[i]);
        printf("%s\n",array[i]);
        i++;
    }
    
}


//初始化
fileList * newFileList(){
    fileList * list = malloc(sizeof(fileList));
    list->capacity = MAX_FILELIST;
    list->size =0;
    char ** array = malloc(sizeof(char *)*MAX_FILELIST);
    list->array = array;
    return list;
}
//销毁内存
void delFileList(fileList *list){
    for(int i=0;i<list->size;i++){
        free(list->array[i]);
    }
    free(list->array);
    free(list);
    
}
//列表扩容
void fileList_expand(fileList *list){
    list->capacity *=2;
    char ** new_array = malloc(sizeof(char *)* list->capacity);
    if (new_array == NULL) return;
    for(int i=0;i<list->size;i++){
        new_array[i] = list->array[i];
    }
    for(int i=list->size;i<list->capacity;i++){
        new_array[i] = NULL;
    }
    free(list->array);
    list->array = new_array;
}


//添加文件名
void add_file(fileList *list,char *p){
    //判断是否已满
    if(list ==NULL || p ==NULL) return;
    if (list->size == list->capacity){
        fileList_expand(list);
    }
    list->array[list->size] = malloc(strlen(p)+1);
    if(list->array[list->size] == NULL) return;
    strcpy(list->array[list->size], p); 
    list->size++;
}
// 获取角色列表
fileList *getFileList(){
    DIR *dir = opendir("../../save/");
    if( dir == NULL) 
    {
        return -1;
    }
    struct dirent * tmp;
    fileList *list = newFileList();
    if (list == NULL) 
    {
        closedir(dir);
        return -1;
    }
    while ((tmp =readdir(dir)) != NULL)
    {
        if( strcmp(tmp->d_name,".") == 0 || strcmp(tmp->d_name,"..") == 0) continue;
        printf("%s\n",tmp->d_name);
        char tmp_str[256] = {0};  // 初始化
        if (sscanf(tmp->d_name, "player_%255[^.].json", tmp_str) == 1) {
            add_file(list, tmp_str);  // 只有匹配成功才添加
        }
    }
    closedir(dir);
    return list;
}


//获取文件列表，判断创建的角色名是否已经存在
int is_name_exists(char *name){
    fileList *list = getFileList();
    for(int i=0;i<list->size;i++){
        if(strcmp(list->array[i],name) == 0) 
        {
            printf("list->array[i] :%s\n",list->array[i]);
            printf("name:%s\n",name);
            delFileList(list);
            return -1;
        }
    }
    delFileList(list);
    return 0;
}

int main5(){

    while (1)
    {
        char username[240];
        printf("请输入创建的角色名:");
        scanf("%s",username);
        int tmp_status = is_name_exists(username);
        if(tmp_status ==0){
        printf("可以创建,执行创建角色函数\n");
       User * user = create(username,1,231);
       int tmp_save_satus = save(user);
       if(tmp_save_satus == 0){
        printf("保存成功\n");
       }else{
        printf("失败\n");
       }
        
    }else{
        printf("新建角色失败，该角色已存在\n");
        break;
    }
    }

}