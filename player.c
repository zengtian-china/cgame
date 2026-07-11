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

void showUser(User *user){
    printf("读取成功 -> 角色名: %s, 等级: %d, 经验:%ld,金币%d\n", user->uname, user->level, user->exp, user->gold);
    printf("读取成功 -> 力量:%d,体质:%d, 敏捷:%d\n", user->strength, user->physique,user->agility);
    printf("读取成功 -> 气血上限:%d, 当前气血:%d, 法力上限:%d, 当前法力:%d\n", user->max_hp, user->hp, user->max_mp, user->mp);
    printf("读取成功 -> 攻击:%d, 防御:%d,速度:%d\n", user->attack, user->defense,user->speed);
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
// 读取角色信息
User * userData(const char *username){
    // 1. 预估一个足够大的空间（比如 256 字节）
    char *path = malloc(256); 
    //创建user
    User *user = newUser();
    
    if (path == NULL) return NULL;

    // %s 代表字符串，snprintf 会自动在最后加上 \0
    snprintf(path, 256, "./save/%s.txt", username);
    printf("最终路径: %s\n", path); // 输出: ./save/tian.txt
    FILE *fp = fopen(path,"r");
    if (fp == NULL) return NULL;
    char name[100] ={0};
    int a;
    int b;
    int c;
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
    fprintf(fp, "%d|%d|%d|%d\n", 
            user->max_hp, 
            user->hp, 
            user->max_mp, 
            user->mp);

    // 6. 写入战斗属性 (对应读取的第四行)
    fprintf(fp, "%d|%d|%d\n", 
            user->attack, 
            user->defense, 
            user->speed);
    printf("最终路径: %s\n", path); // 输出: ./save/tian.txt
    int first_item = 1;
    for(int i=0;i<MAX_ITEMS;i++){
        if(user->invertory[i].item_count !=0){
            if(!file_list){
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
    return user;

}
//属性成长
void stat_growth(User *user){
    int strength = 0;
    int physique = 0;
    int agility = 0;
    int statuc = 1;
    printf("属性分配\n");
    while (statuc){
        printf("请自由分配5点属性点\n");
        printf("力量(0~5):");
        scanf("%d",&strength);
        printf("体质(0~5):");
        scanf("%d",&physique);
        printf("敏捷(0~5):");
        scanf("%d",&agility);
        if((strength >=0) &&(physique >=0) && (agility >=0) &&((strength+physique+agility) == 5)) {
            statuc = 0;
            break;
        } else{
            printf("分配属性点失败，请重新分配");
        }
    }
    user->strength = user->strength + strength;
    user->physique = user->physique + physique;
    user->agility = user->agility +agility;
    user->attack = (int)(user->strength * 0.8);
    user->max_hp = (int)(user->max_hp *(1+(0.1)*user->physique));
    user->speed = (int)(user->speed +(user->agility * 0.1));
}



// 升级
void levelUp(User *user){

    if (user->level < 10){
        user->level = user->level + 1;
        user->gold = user->gold + 100 * user->level; 
        user->strength = user->strength + 2;
        user->physique = user->physique + 2;   //血量系数 hp *(1.2)
        user->agility = user->agility + 2;  //敏捷
        user->max_hp = (int)(user->max_hp *(1+(0.1)*user->physique));
        user->hp = user->max_hp;
        user->max_mp = user->max_mp +150;
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.8);
        user->defense = user->defense +1 *user->level;
        user->speed = user->speed +1;

    } else if(user->level <20){
        user->level = user->level + 1;
        user->gold = user->gold + 200 * user->level; 
        user->strength = user->strength + 2*user->level;
        user->physique = user->physique + 2*user->level;;   //血量系数 hp *(1.2)
        user->agility = user->agility + 2*(user->level- 9);  //敏捷
        user->max_hp = (int)(user->max_hp *(1+(0.1)*user->physique));
        user->hp = user->max_hp;
        user->max_mp = user->max_mp +150*(user->level -9);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.8);
        user->defense = user->defense +(int)(1+0.1 * user->level/10)*(user->level);
        user->speed = user->speed +1*(user->level);

    } else if(user->level <30){
        user->level = user->level + 1;
        user->gold = user->gold + 300 * user->level; 
        user->strength = user->strength + 3*user->level;
        user->physique = user->physique + 3*user->level;;   //血量系数 hp *(1.2)
        user->agility = user->agility + 3*(user->level- 10);  //敏捷
        user->max_hp = (int)(user->max_hp *(1+(0.2)*user->physique));
        user->hp = user->max_hp;
        user->max_mp = user->max_mp +300*(user->level -5);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.9);
        user->defense = user->defense +(int)(1+0.2 * user->level/5)*(user->level);
        user->speed = user->speed +2*(user->level);
    } else if(user->level <40){
        user->level = user->level + 1;
        user->gold = user->gold + 500 * user->level; 
        user->strength = user->strength + 5*user->level;
        user->physique = user->physique + 5*user->level;;   //血量系数 hp *(1.2)
        user->agility = user->agility + 5*(user->level- 15);  //敏捷
        user->max_hp = (int)(user->max_hp *(1+(0.2)*user->physique));
        user->hp = user->max_hp;
        user->max_mp = user->max_mp +500*(user->level -15);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.9);
        user->defense = user->defense +(int)(1+0.2 * user->level/5)*(user->level);
        user->speed = user->speed +2*(user->level);
    } else if(user->level <50){
        user->level = user->level + 1;
        user->gold = user->gold + 1000 * user->level; 
        user->strength = user->strength + 10*user->level;
        user->physique = user->physique + 10*user->level;;   //血量系数 hp *(1.2)
        user->agility = user->agility + 10*(user->level- 20);  //敏捷
        user->max_hp = (int)(user->max_hp *(1+(0.5)*user->physique));
        user->hp = user->max_hp;
        user->max_mp = user->max_mp +1000*(user->level -20);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.9);
        user->defense = user->defense +(int)(1+0.5 * user->level/5)*(user->level);
        user->speed = user->speed +2*(user->level);
    } 
    //分配属性点
    stat_growth(user);
    //打印信息
    showUser(user);

}

void levelUpLogic(User *user, int exp){
    int max_exp = 0;
    printf("等级升级逻辑\n");
    // 更新最新经验值
    user->exp = user->exp +exp;
    if (user->level <10){
        max_exp = user->level *100;
        printf("当前等级小于10\n");
        printf("当前经验值%ld\n",user->exp);
        printf("升级所需的经验值%d\n",max_exp);
        while (max_exp <= user->exp)
        {
            printf("经验满足升级条件");
            user->exp = user->exp - max_exp;
            levelUp(user);
        }
        
    } else if(user->level <30){
        max_exp = user->level *200+(user->level * user->level *2);
        while (max_exp <= user->exp)
        {
            user->exp = user->exp - max_exp;
            levelUp(user);
        }
    } else if(user->level <40){
        max_exp = user->level *400+(user->level * user->level *5);
        while (max_exp <= user->exp)
        {
            user->exp = user->exp - max_exp;
            levelUp(user);
        }
    }   else if(user->level <50){
        max_exp = user->level *100+(user->level * user->level *10);
        while (max_exp <= user->exp)
        {
            user->exp = user->exp - max_exp;
            levelUp(user);
        }
    } else{

    }

}


int main(void){
    // List *list = newList(2);
    // file_list("./data",list);
    // show_list(list);
    // delList(list);
    User *user = userData("李四");
    // User *user = UserInit("李四");
    // insertUser(user);
    int i = 0;
    while(i<20){
        
        levelUpLogic(user,200*(i+1));
        //将数据更新到文件中
        insertUser(user);
        i++;

    }

    delUser(user);

    // user->max_hp = 10000;
    // insertUser(user);
    // delUser(user);
}
// 30
// 30 +30 *(1+0.1*(2-1))
// 63 + 30 *1.2
// 96 +30 *1.3