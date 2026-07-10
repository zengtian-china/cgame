#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#define MAX_ITEMS 20
//创建
typedef struct item{
    int item_id;
    int item_count;
}Item;


typedef struct user
{
    char uname[50]; //角色名
    int level;      //等级
    long exp;       //经验
    int gold;       //金币
    int strength;   //力量
    int physique;   //体质
    int agility;    //敏捷
    int max_hp;     //气血上限
    int hp;         //当前气血
    int max_mp;     //法力上限
    int mp;         //当前法力
    int attack;     //攻击
    int defense;    //防御
    int speed;      //速度
    Item invertory[MAX_ITEMS];
}User;

User * newUser(){
    User *user = malloc(sizeof(User));
    if (user == NULL) return NULL;
    //将整块内存清零
    memset(user, 0, sizeof(User));
    return user;

}

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
void show_list(List *list){
    //判断是否有数据
    // if((list->size) == 0){
    //     return ;
    // }
    for(int i=0;i<list->size;i++){
        printf("%s ",list->array[i]);
    }
    printf("\n");
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

User * userData(const char *username){
    // 1. 预估一个足够大的空间（比如 256 字节）
    char *path = malloc(256); 
    //创建user
    User *user = newUser();
    
    if (path == NULL) return;

    // %s 代表字符串，snprintf 会自动在最后加上 \0
    snprintf(path, 256, "./save/%s.txt", username);
    printf("最终路径: %s\n", path); // 输出: ./save/tian.txt
    FILE *fp = fopen(path,"r");
    if (fp == NULL) return ;
    char name[100] ={0};
    int a;
    int b;
    int c;
    int count = fscanf(fp, "%49[^|]|%d|%ld|%d", user->uname, &user->level, &user->exp, &user->gold);

    if (count == 4) {
        printf("读取成功 -> 名字: %s, 数值: %d, %ld, %d\n", user->uname, user->level, user->exp, user->gold);
    } else {
        printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
        }

    count = fscanf(fp,"%d|%d|%d",&user->strength,&user->physique,&user->agility);
    if (count!=3) printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
    else{
            printf("读取成功 -> 数值: %d, %d, %d\n", user->strength, user->physique,user->agility);
    }

    count = fscanf(fp,"%d|%d|%d|%d",&user->max_hp,&user->hp,&user->max_mp,&user->mp);
    if (count !=4) printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
    else{
        printf("读取成功 -> 数值: %d, 数值: %d, %d, %d\n", user->max_hp, user->hp, user->max_mp, user->mp);
    }
    count = fscanf(fp,"%d|%d|%d",&user->attack,&user->defense,&user->speed);
    if (count !=3) printf("读取失败！文件格式可能不匹配。只读到了 %d 项。\n", count);
    else {
        printf("读取成功 -> 数值: %d, %d, %d\n", user->attack, user->defense,user->speed);
    }
    
    char tmp[255];
    // 读取背包信息
    fscanf(fp,"%s",tmp);
    printf("%s\n",tmp);
    int i =0 ;
    char *token1 = strtok(tmp, ",");

    while (token1 !=NULL && i<MAX_ITEMS)
    {
        sscanf(token1,"%d:%d",&user->invertory[i].item_id,&user->invertory[i].item_count);
        printf("物品id%d:物品数量%d\n",user->invertory[i].item_id,user->invertory[i].item_count);
        i++;
        token1 =strtok(NULL,",");

        
    }
    
    //释放文件内存
    fclose(fp);
    // 释放内存
    free(path);
    return user;
    
}

int main(void){
    List *list = newList(2);
    file_list("./data",list);
    show_list(list);
    delList(list);
    userData("tian");
}