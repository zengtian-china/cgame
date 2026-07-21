/*
 * @Author: zengtian
 * @Date: 2026-07-14 20:32:37
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-16 18:19:12
 * @Description: File Description
 * @FilePath: \cgame\item.h
 * @@file: 
 */

 #ifndef _ITEM_H_
 #define _ITEM_H_
#define MAX_ITEMS 20
#include "save.h"
//创建物品结构体
typedef struct item
{
    int id;
    char item[50];
    char effect[256];
    int price;
}items;

//创建物品详情列表
typedef struct _itemList{
    int size;
    items array[20];
} itemList;
//物品列表初始化
typedef struct _myitem
{
    int id;
    char item[50];
    char effect[256];
    int number;
}myitem;

//创建角色物品详情列表
typedef struct _myItemList{
    int size;
    myitem array[MAX_ITEMS];

}myItemList;

//获取角色物品详情列表
myItemList* getItemsList(User *user);

//打印背包信息
void printItemsList(myItemList *mylist);

//物品详情列表初始化
itemList * newItemsList();

// 获取物品详情列表
itemList* createItemsList();

// 显示物品详情列表
void show_itemList(itemList *list);

//根据物品id来查找对于的物品详情数据
items * find_items(int id,itemList *list);

//显示物品详情
void show_my(User *user);
//物品整理   物品排序算法
void sort_item(User * user);

#endif