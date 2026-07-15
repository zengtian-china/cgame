#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "item.h"
#include "save.h"

//购买商品
void shopping(User* user,int number,items * its){
    if(user->gold < number * its->price){
        printf("金币不够，购买失败，请通过打怪获取金币\n");
    }
    else{
        user->gold = user->gold - number*its->price;  //更新金币
        user->invertory[user->invertory_size].item_count = number;
        user->invertory[user->invertory_size].item_id = its->id;
        user->invertory_size +=1;
        //更新  写入文档 
        insertUser(user);
        //更新  重新获取用户物品详情列表
        myItemList *mylist =  getItemsList(user);
    }
}
//展示商品
void show_store(itemList *list,User *user){
    int number =0;
    int id = 0;
    printf("+------+----------------------+--------+\n");
    // 打印表头
    printf("| %-4s | %-20s | %-8s |\n", "ID", "物品","价格");
    for(int i=0;i<list->size;i++){
            printf("| %-4d | %-21s | %-6d |\n",
                list->array[i].id,
                list->array[i].item,
                list->array[i].price);
    }
    printf("+------+----------------------+--------+\n");
    printf("请选择购买的商品id(1~%d):\n",list->size);
    scanf("%d",&id);
    printf("请输入购买的数量:\n");
    scanf("%d",&number);
    shopping(user,number,find_items(id,list));

    //根据物品id来查找对于的物品详情数据
// items * find_items()
}


//显示商品信息
int main(void) {
    // 获取了商品信息
    itemList *list = createItemsList();
    User *user = read_save_main();
    int status;
    show_itemList(list);
    printf("\n");
    printf("┌───────────────────────────┐\n");
    printf("│        操作菜单          │\n");
    printf("├───────────────────────────┤\n");
    printf("│ 1. 购买商品 (%d金币)      │\n",user->gold);
    printf("│ 2. 退出游戏              │\n");
    printf("└───────────────────────────┘\n");
    printf("请输入选项编号: ");
    scanf("%d",&status);
    if( status == 1) {
        show_store(list, user);
    }
    else if(status == 2){
        printf("游戏结束");
    }
    free(list);
    free(user);
}



//购买逻辑
/*
    输入购买商品id
    输入购买商品数量
    判断金币 > 金额*数量
        获取 id 数量 
        userList ->  invertory_size
        
        角色物品列表 -> 更新数据(重新加载)

        // 物品列表是 20 
        

*/
