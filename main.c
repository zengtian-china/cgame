#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "save.h"
#include "item.h"

void enterGame(User *user){
    while (1)
    {
        int status;
        printf("欢迎来到cgame游戏\n");
        printf("1.菜单选项\n");
        printf("2.退出\n");
        printf("请输入你的选择(1~2)");
        scanf("%d",&status);
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
                scanf("%d",tmp_1);
                if(1 == tmp_1){
                    printf("属性函数\n");
                    showUser(user);
                } else if(2 == tmp_1){
                    printf("背包管理\n");
                    //获取背包

                }else if(3 == tmp_1){
                    printf("商店购买\n");
                }else if(4 == tmp_1){
                    printf("前往地图\n");
                }else if(5 == tmp_1){
                    printf("保存退出\n");
                }else{
                    printf("输入的是无效数据,请重新输入\n");
                }
            }
            break;
        }
        else if(2 ==status){
            printf("game ov\n");
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
    printf("开始游戏\n");
    char username[255];
    while (status)
    {
        printf("1.新游戏\n");
        printf("2.继续游戏\n");
        printf("0.退出游戏\n");
        printf("请输入你的选择(1~3):");
        scanf("%d",&status);
        switch(status){
        case 1:
            printf("请输入你要创建的角色用户:");
            status = 0;
            scanf("%s",username);
            create_save_main(username);
            break;
        case 2:
            printf("读取存档\n");
            status = 0;
            read_save_main();
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