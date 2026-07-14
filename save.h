/*
 * @Author: zengtian
 * @Date: 2026-07-13 20:04:38
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-14 15:37:04
 * @Description: File Description
 * @FilePath: \cgame\save.h
 * @@file: 
 */
#define MAX_ITEMS 20
//创建
//创建
typedef struct _item{
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


User * read_save_main();

User * create_save_main(char *);

//销毁内存数据
void delUser(User *user);
//展示属性
void showUser(User *user);
// 更新用户数据
void insertUser(User *user);