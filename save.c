/*
 * @Author: zengtian
 * @Date: 2026-07-09 20:15:42
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-16 13:59:59
 * @Description: File Description
 * @FilePath: \cgame\save.c
 * @@file: 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "save.h"
#include<dirent.h>
#define MAX_ITEMS 20

//创建一个动态列表
typedef struct _list{
    int size;
    int capacity;  //列表容量
    char **array;  //字符串数组
}List;
 // 初始化列表容量
List * newList(const int num){
    List * list = malloc(sizeof(List));
    if( list == NULL){
        return NULL; 
    }
    memset(list,0,sizeof(List));
    list->size = 0;
    list->capacity = num;
    list->array = malloc(sizeof(char *)* num);
    if( list->array == NULL){
        return NULL; 
    }
    return list;
}
// 列表扩容
void list_add(List *list) {
    if (list == NULL) return;
    
    int new_capacity = list->capacity * 2;
    char **tmp = list->array;
    char **new_array = malloc(sizeof(char *) * new_capacity);
    
    if (new_array == NULL) {
        // 扩容失败，保持原状态，不泄漏内存
        return;
    }
    
    for (int i = 0; i < list->size; i++) {
        new_array[i] = tmp[i];
    }
    for (int i = list->size; i < new_capacity; i++) {
        new_array[i] = NULL;
    }
    
    free(tmp);
    list->array = new_array;
    list->capacity = new_capacity;
}
//打印列表内容
int show_list(List *list){
    int index=0;
    int status = 1;
    for(int i=0;i<list->size;i++){
        printf("%d.%s \n",i+1,list->array[i]);
    }
    printf("请选择读取的存档():");

    if(scanf("%d",&index) != 1)
    {
        //清空缓存
        int c;
        while((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    return index;
    
}

// 列表添加数据
void push(List *list, const char *str) {
    if (list == NULL || str == NULL) return;
    if (list->size >= list->capacity) {
        list_add(list);
        // 检查扩容是否成功
        if (list->array == NULL) return;
    }
    list->array[list->size] = malloc(strlen(str) + 1);
    if (list->array[list->size] == NULL) return;
    strcpy(list->array[list->size], str);
    list->size++;
}

//内存销毁
void delList(List *list){
    for(int i =0;i<list->size;i++){
        free(list->array[i]);
    }
    free(list->array); 
    free(list);
}
//读取存档目录的的文件

void file_list(const char * path,List *list){
    DIR * dir;
    struct dirent *entry;
    dir = opendir(path);
    if(dir == NULL){
        perror("opendir failed");
        return ;
    }
    while ((entry = readdir(dir)) !=NULL)
    {
                // 过滤掉当前目录 "." 和父目录 ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // printf("%s\n",entry->d_name);
        //将文件存放到列表中
        strtok(entry->d_name,".");
        push(list,entry->d_name);
        
    }
    
}



User * newUser(){
    User *user = malloc(sizeof(User));
    if (user == NULL) return NULL;
    //将整块内存清零
    memset(user, 0, sizeof(User));
    return user;

}


// 读取角色信息
User * userData(const char *username){
    // 1. 预估一个足够大的空间（比如 256 字节）
    char *path = malloc(256); 
    //创建user
    User *user = newUser();
    
    if (path == NULL) return NULL;

    // %s 代表字符串，snprintf 会自动在最后加上 \0
    snprintf(path, 256, "./save/%s.txt", username);
    FILE *fp = fopen(path,"r");
    if (fp == NULL) return NULL;
    int count = fscanf(fp, "%49[^|]|%d|%ld|%d", user->uname, &user->level, &user->exp, &user->gold);
    if (count == 4) {
        printf("读取成功 -> 角色名: %s, 等级: %d, 经验:%ld,金币%d\n", user->uname, user->level, user->exp, user->gold);
    } else {
        printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
        }

    count = fscanf(fp,"%d|%d|%d",&user->strength,&user->physique,&user->agility);
    if (count!=3) printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
    else{
            printf("读取成功 -> 力量:%d,体质:%d, 敏捷:%d\n", user->strength, user->physique,user->agility);
    }
    count = fscanf(fp,"%d|%d|%d|%d",&user->max_hp,&user->hp,&user->max_mp,&user->mp);
    if (count !=4) printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
    else{
        printf("读取成功 -> 气血上限:%d, 当前气血:%d, 法力上限:%d, 当前法力:%d\n", user->max_hp, user->hp, user->max_mp, user->mp);
    }
    count = fscanf(fp,"%d|%d|%d",&user->attack,&user->defense,&user->speed);
    if (count !=3) printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
    else {
        printf("读取成功 -> 攻击:%d, 防御:%d,速度:%d\n", user->attack, user->defense,user->speed);
    }
    
    char tmp[255];
    // 读取背包信息
    fscanf(fp,"%s",tmp);
    int i =0 ;
    char *token1 = strtok(tmp, ",");

    while (token1 !=NULL && i<MAX_ITEMS )
    {
        sscanf(token1,"%d:%d",&user->invertory[i].item_id,&user->invertory[i].item_count);
        if(user->invertory[i].item_id == 0) break;
        printf("物品id->%d:物品数量->%d\n",user->invertory[i].item_id,user->invertory[i].item_count);
        i++;
        user->invertory_size +=1;  //记录背包物品数量
        token1 =strtok(NULL,",");
    }
    //释放文件内存
    fclose(fp);
    // 释放路径内存
    free(path);
    return user;
}
// 更新用户信息
void insertUser(User *user){
    char *path = malloc(256); 
    if (path == NULL) return;
    // %s 代表字符串，snprintf 会自动在最后加上 \0
    sprintf(path, "./save/%s.txt", user->uname);
    FILE *fp = fopen(path,"w+");
        if (fp == NULL) {
        perror("无法打开文件进行写入");
        return;
    }
    // 3. 写入基础信息 (对应读取的第一行)
    // 注意：这里必须加上 \n，因为 fscanf 读取时是按行或按空白符跳过的
    fprintf(fp, "%s|%d|%ld|%d\n", 
            user->uname, 
            user->level, 
            user->exp, 
            user->gold);

    // 4. 写入属性信息 (对应读取的第二行)
    fprintf(fp, "%d|%d|%d\n", 
            user->strength, 
            user->physique, 
            user->agility);

    // 5. 写入气血/法力信息 (对应读取的第三行)
    if( user->hp <=0 && user->mp <=0){
            fprintf(fp, "%d|%d|%d|%d\n", 
            user->max_hp, 
            user->max_hp, 
            user->max_mp, 
            user->max_mp);
            user->hp = user->max_hp;
            user->mp = user->max_mp;
    }else if(user->hp <=0 && user->mp>0)
    {
            fprintf(fp, "%d|%d|%d|%d\n", 
            user->max_hp, 
            user->max_hp, 
            user->max_mp, 
            user->mp);
            user->hp = user->max_hp;
    }else if(user->hp>0 && user->mp<=0){
            fprintf(fp, "%d|%d|%d|%d\n", 
            user->max_hp, 
            user->hp, 
            user->max_mp, 
            user->max_mp);
            user->mp = user->max_mp;
    }else if(user->hp>0 && user->mp >0){
            fprintf(fp, "%d|%d|%d|%d\n", 
            user->max_hp, 
            user->hp, 
            user->max_mp, 
            user->mp);
    }

    // 6. 写入战斗属性 (对应读取的第四行)
    fprintf(fp, "%d|%d|%d\n", 
            user->attack, 
            user->defense, 
            user->speed);
    printf("最终路径: %s\n", path); // 输出: ./save/tian.txt
    int first_item = 1;
    for(int i=0;i<user->invertory_size;i++){
        if(user->invertory[i].item_count !=0){
            if(!first_item){
                fprintf(fp,",");
            }
            fprintf(fp,"%d:%d",user->invertory[i].item_id,user->invertory[i].item_count);
            first_item = 0;
        }
    }
    fclose(fp);
    free(path);
}


void delUser(User *user){
    free(user);
}

void showUser(User *user){
    printf("读取成功 -> 角色名: %s, 等级: %d, 经验:%ld,金币%d\n", user->uname, user->level, user->exp, user->gold);
    printf("读取成功 -> 力量:%d,体质:%d, 敏捷:%d\n", user->strength, user->physique,user->agility);
    printf("读取成功 -> 气血上限:%d, 当前气血:%d, 法力上限:%d, 当前法力:%d\n", user->max_hp, user->hp, user->max_mp, user->mp);
    printf("读取成功 -> 攻击:%d, 防御:%d,速度:%d\n", user->attack, user->defense,user->speed);
    }

//创建角色信息
// 属性设定
User * UserInit(const char *name){
    //申请可以user对象
    User *user = newUser();
    //初始化
    
    strcpy(user->uname,name);
    user->level = 1;    //初始等级 1
    user->exp = 0;      //经验  0
    user->gold = 1000;  //
    user->strength = 10;
    user->physique = 2;   //血量系数 hp *(1.2)
    user->agility = 2;  //敏捷
    user->max_hp = (int)(100 *(1+(0.1)*user->physique));
    user->hp = user->max_hp;
    user->max_mp = 200;
    user->mp = user->max_mp;
    user->attack = (int)(user->strength * 0.8);
    user->defense = 3;
    user->speed = 2;
    //将创建好的角色信息存放到文件中
    insertUser(user);
    return user;

}

//将存档加载到列表中
List * loadList(){
    List *list = newList(5);
    file_list("./save",list);
    return list;
}

//判断角色是否存在
int isRoleExist(List *loadData,char *uname){
    for(int i=0;i<loadData->size;i++){
        if(0 == strcmp(loadData->array[i],uname)) 
        delList(loadData);
        return 1;
    }
    //销毁数据列表
    delList(loadData);
    return 0;
}

//读写文档，返回角色信息
User* read_save_main(){
    List *list = loadList();
    //增加修改
    int index = show_list(list);

    if(index <= 0 || index >list->size)
    {
        printf("无效\n");
        delList(list);
        return NULL;
    }

    User *user = userData(list->array[index-1]); 
    // user->max_hp = 100;
    // insertUser(user);
    // showUser(user);
    printf("正常\n");
    delList(list);
    printf("内存销毁失败\n");
    // delUser(user);
    return user;
}


User * create_save_main(char *uname){
    //判断角色是否已经存在
    
    List *loadData = loadList();
    if(isRoleExist(loadList(),uname)){
        printf("%s角色已存在\n",uname);

        return NULL;
    }
    else{
        printf("角色不存在\n");
        //创建角色
        User *user = UserInit(uname);
        return user;
    }


    // User* user = UserInit(uname);
}
