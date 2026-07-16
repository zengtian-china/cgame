/*
 * @Author: zengtian
 * @Date: 2026-07-13 20:04:38
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-16 18:42:37
 * @Description: File Description
 * @FilePath: \cgame\save.h
 * @@file: 
 */
#ifndef _SAVE_H_
#define _SAVE_H_

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
    int invertory_size; //物品数量
    Item invertory[MAX_ITEMS];
}User;

//读取用户信息
User * read_save_main();
//创建用户信息
User * create_save_main(char *name);

//销毁内存数据
void delUser(User *user);
//展示属性
void showUser(User *user);
// 更新用户数据
void insertUser(User *user);

#endif