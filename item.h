/*
 * @Author: zengtian
 * @Date: 2026-07-14 20:32:37
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-14 20:53:16
 * @Description: File Description
 * @FilePath: \cgame\item.h
 * @@file: 
 */
#define MAX_ITEMS_1 20
struct user; 
typedef struct user User; 

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
    myitem array[MAX_ITEMS_1];

}myItemList;

//获取角色物品详情列表
myItemList* getItemsList(User *user);

//打印背包信息
void printItemsList(myItemList *mylist);