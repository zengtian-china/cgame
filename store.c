#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "store.h"
//购买商品
int shopping(User* user,int number,items * its){
    if(user->gold < number * its->price){
        printf("金币不够，购买失败，请通过打怪获取金币\n");
        return -1;
    }
    else{
        // 判断购买的数量是否超过了99 单格最大数量为99
        int slots_needed = (number + 98) / 99;  // 例如：100个需要2格，250个需要3格
        if (user->invertory_size + slots_needed > MAX_ITEMS) {
            printf("背包空间不足，无法购买这么多物品。需要 %d 个空位，当前只剩 %d 个\n", 
                slots_needed, MAX_ITEMS - user->invertory_size);
            return -1;
        }
        user->gold -= number * its->price;
        int remaining = number;
        while (remaining > 0) {
                int count = (remaining > 99) ? 99 : remaining; 
                user->invertory[user->invertory_size].item_count = count;
                user->invertory[user->invertory_size].item_id = its->id;
                user->invertory_size += 1;
                remaining -= count;
        }
        insertUser(user);
        printf("购买成功！获得 %d 个物品\n", number);
        return 1;
    }
}
//展示商品

void show_store(itemList *list,User *user){
    int tmp_show_store_status = 1;
    while (tmp_show_store_status)
    {
        int number =0;
        int id = 0;
        int tmp_show_status = 0;
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
        printf("请选择购买的商品id(1~%d):",list->size);
        scanf("%d",&id);
        printf("请输入购买的数量:");
        scanf("%d",&number);
        if (1 == shopping(user,number,find_items(id,list))){
            printf("      购买成功\n");
            //更新用户物品详情列表
            // mylist =  getItemsList(user);
            printf("当前余额:%d金币\n",user->gold);
            printf("1.继续购买\n");
            printf("2.查看背包\n");
            printf("3.退出商店\n");
            printf("请输入你的选项(1~3):");
            scanf("%d",&tmp_show_status);
            if (1 == tmp_show_status) continue;
            if (2 == tmp_show_status) {
                show_my(user);
                break;
            }
            if (3 == tmp_show_status) break;

        } else{
            // printf("购买失败\n");
            printf("当前余额:%d金币\n",user->gold);
            printf("1.继续购买\n");
            printf("2.查看背包\n");
            printf("3.退出商店\n");
            printf("请输入你的选项(1~3):");
            scanf("%d",&tmp_show_status);
            if (1 == tmp_show_status) continue;
            if (2 == tmp_show_status) {
                show_my(user);
                break;
            }
            if (3 == tmp_show_status) break;
        }
    }
}


//显示商品信息
int store_main(User *user) {
    // 获取了商品信息
    itemList *list = createItemsList();

    sort_item(user);
    int status;
    while (1)
    {
        show_itemList(list);
        printf("store : 当前商城物品数量%d\n",list->size);
        printf("\n");
        printf("┌─────────────────────────────┐\n");
        printf("│        操作菜单             │\n");
        printf("├─────────────────────────────┤\n");
        printf("│ 1. 购买商品 (%d金币)       │\n",user->gold);
        printf("│ 2. 查看背包                 │\n");
        printf("│ 3. 退出商城                 │\n");
        printf("└─────────────────────────────┘\n");
        printf("请输入选项编号: ");
        scanf("%d",&status);
        if( status == 1) {
            show_store(list, user);
            sort_item(user);
            status  =3;
        }
        else if(status ==2){
            show_my(user);
            printf("store : 当前商城物品数量%d\n",list->size);
            continue;
        }
        else if(status == 3){
            printf("退出商城\n");
            break;
            //退回主菜单界面
        }
    }
    free(list);
}
