#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "battle.h"

int main(void)
{
    // 随机种子初始化
    srand((unsigned)time(NULL));

    // 1. 加载怪物文件
    int mob_count = MonsterInit(FILE_NAME);
    if(mob_count < 0)
    {
        printf("怪物文件加载失败！程序退出\n");
        return -1;
    }
    printf("成功加载%d只怪物数据\n", mob_count);
    
    // 2. 读取角色存档（如果不存在则程序退出）
    User *user = read_save_main();
    if(user == NULL) {
        printf("读取角色失败，程序退出\n");
        return -1;
    }

    // 3. 初始化地图信息和战斗回调（只做一次）
    MapInfoInit();
    SetEncounterCallback(battle);

    // 4. 建立地图系统的独立主菜单循环
    int menu_running = 1;
    while(menu_running)
    {
        printf("\n===== 地图探索 =====\n");
        printf("1. 前往地图探索\n");
        printf("2. 退出游戏\n");
        printf("请输入你的选择(1~2): ");
        
        int choice;
        scanf("%d", &choice);
        
        switch(choice)
        {
        case 1:
            int explore_alive = 1;
            
            while (explore_alive)
            {
                // 选择地图
                Map_ID select_map = ShowMap();
                EnterMap(select_map);
                
                
                getchar(); 
                
                
                int ret = Explore(user);
                
                if (ret == 1) 
                {
                   
                    printf("\n你死了！请重新选择地图 按回车键继续...\n");
                    getchar();
                    
                }
                else 
                {
                    
                    printf("\n按回车键继续...\n");
                    getchar();
                    explore_alive = 0; 
                }
            }
            break;
                
            case 2:
                printf("退出游戏。\n");
                menu_running = 0; 
                break;
                
            default:
                printf("输入无效，请重新输入。\n");
                break;
        }
    }

    return 0;
}