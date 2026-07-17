/*
 * @Author: zengtian
 * @Date: 2026-07-09 20:15:42
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-17 20:09:20
 * @Description: File Description
 * @FilePath: \cgame\main.c
 * @@file: 
 */
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include <ncurses.h>
#include "map.h"
#include "save.h"
#include "item.h"
#include "store.h"
#include "player.h"
#include "battle.h"

#include "monster.h"

void enterGame(User *user){
    while (1)
    {
        int status;
        printf("\n欢迎来到梦幻文字版游戏\n");
        printf("1.菜单选项\n");
        printf("2.保存退出\n");
        printf("请输入你的选择(1~2)");
        if(scanf("%d",&status) !=1){
            printf("输入的是无效数据，请重新输入\n");
            int c;
            while ( (c=getchar()) !='\n' && c!=EOF);
            continue;
        }
        if( 1 ==status){
            while (1)
            {
                int tmp_1;
                printf("1.查看属性\n");
                printf("2.背包管理\n");
                printf("3.商店购买\n");
                printf("4.前往地图\n");
                printf("5.保存退出\n");
                printf("请选择(1~5):");
                if(scanf("%d",&tmp_1) !=1){
                    printf("输入的是无效数据，请重新输入\n");
                    int c;
                    while ( (c=getchar()) !='\n' && c!=EOF);
                    continue;
                }
                if(1 == tmp_1){
                    printf("属性函数\n");
                    showUser(user);
                } else if(2 == tmp_1){
                    printf("背包管理\n");
                    printItemsList(getItemsList(user));
                    //获取背包

                }else if(3 == tmp_1){
                    printf("商店购买\n");
                    store_main(user);
                }else if(4 == tmp_1){
                    printf("前往地图\n");
                    map_main(user);
                }else if(5 == tmp_1){
                    printf("保存退出\n");
                    insertUser(user);
                    delUser(user);
                    break;
                }else{
                    printf("输入的是无效数据,请重新输入\n");
                }
            }
            break;
        }
        else if(2 ==status){
            insertUser(user);
            delUser(user);
            break;
        }
        else{
            printf("输入的是无效数据,请重新输入\n");
        }
}
    

}

int main(void) {
    // printf("hello cgame!\n");
    int status = 1;
    printf("==== 梦幻文字版 - 打怪升级 ====\n");
    char username[255];
    User *user;
    while (status)
    {
        printf("\n1.新游戏\n");
        printf("2.继续游戏\n");
        printf("0.退出游戏\n");
        printf("请输入你的选择(0~2):");
        if(scanf("%d",&status) !=1){
            printf("输入的是无效数据，请重新输入\n");
            int c;
            while ( (c=getchar()) !='\n' && c!=EOF);
            continue;
        }
        switch(status){
        case 1:
            printf("请输入你要创建的角色名称:");
            
            scanf("%254s",username);
            user = create_save_main(username);
            if (user !=NULL){
                status = 0;
                enterGame(user);
            }else{
                printf("角色创建失败，请重试\n");
                    status = 1;
            }
            
            break;
        case 2:
            printf("读取存档\n");
            status = 0;
            user =read_save_main();
            if( user==NULL){
                printf("读取存档失败，请重试\n");
                status = 1;
                break;
            }
            enterGame(user);
            break;
        case 0:
            printf("退出游戏\n");
            break;
        default:
            printf("非法数据，请重新输入\n");
            break;
        }
    
    }

}