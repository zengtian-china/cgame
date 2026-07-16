#ifndef _STORE_H_
#define _STORE_H_
#include "item.h"
#include "save.h"

//购买商品
int shopping(User* user,int number,items * its);
//展示商品
void show_store(itemList *list,User *user);
//商品接口
int store_main(User *user);

#endif