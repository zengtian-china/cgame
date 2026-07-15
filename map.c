#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"

static Map_ID g_current_map = MAP_TOWN;
static int g_player_x;
static int g_player_y;
static char map[MAP_HIGH][MAP_WIDTH];

MapInfo map_list[5];

void MapInfoInit(void)
{
    //安全区
    map_list[MAP_TOWN ].ID = MAP_TOWN ;
    strcpy(map_list[0].name, "新手村");
    map_list[MAP_TOWN].IsSecure = 1;
    map_list[MAP_TOWN].Min_lever = 1;
    map_list[MAP_TOWN].Max_lever = 99;
    map_list[MAP_TOWN].monster_num = 0;
    memset(map_list[MAP_TOWN].poll, 0, sizeof(map_list[0].poll));
    memset(map_list[MAP_TOWN].rates, 0, sizeof(map_list[0].rates));

    //新手村外
    map_list[MAP_OUTNOV_VILLOGE].ID = MAP_TOWN ;
    strcpy(map_list[MAP_OUTNOV_VILLOGE].name, "新手村");
    map_list[MAP_OUTNOV_VILLOGE].IsSecure = 1;
    map_list[MAP_OUTNOV_VILLOGE].Min_lever = 1;
    map_list[MAP_OUTNOV_VILLOGE].Max_lever = 99;
    map_list[MAP_OUTNOV_VILLOGE].monster_num = 0;
    map_list[MAP_OUTNOV_VILLOGE].poll[0] = GetMonsterIndex(0);
    map_list[MAP_OUTNOV_VILLOGE].poll[1] = GetMonsterIndex(1);
    map_list[MAP_OUTNOV_VILLOGE].poll[2] = GetMonsterIndex(2);
    map_list[MAP_OUTNOV_VILLOGE].rates[0] = 30;
    map_list[MAP_OUTNOV_VILLOGE].rates[1] = 30;
    map_list[MAP_OUTNOV_VILLOGE].rates[2] = 40;

    //东海湾
    map_list[MAP_ESAST_BAY].ID = MAP_TOWN ;
    strcpy(map_list[MAP_ESAST_BAY].name, "新手村");
    map_list[MAP_ESAST_BAY].IsSecure = 1;
    map_list[MAP_ESAST_BAY].Min_lever = 1;
    map_list[MAP_ESAST_BAY].Max_lever = 99;
    map_list[MAP_ESAST_BAY].monster_num = 0;
    map_list[MAP_ESAST_BAY].poll[0] = GetMonsterIndex(3);
    map_list[MAP_ESAST_BAY].poll[1] = GetMonsterIndex(4);
    map_list[MAP_ESAST_BAY].poll[2] = GetMonsterIndex(5);
    map_list[MAP_ESAST_BAY].rates[0] = 30;
    map_list[MAP_ESAST_BAY].rates[1] = 30;
    map_list[MAP_ESAST_BAY].rates[2] = 40;

    //大雁塔
    map_list[3].ID = MAP_TOWN ;
    strcpy(map_list[MAP_GOOSE_PAGODA].name, "新手村");
    map_list[MAP_GOOSE_PAGODA].IsSecure = 1;
    map_list[MAP_GOOSE_PAGODA].Min_lever = 1;
    map_list[MAP_GOOSE_PAGODA].Max_lever = 99;
    map_list[MAP_GOOSE_PAGODA].monster_num = 0;
    map_list[MAP_GOOSE_PAGODA].poll[0] = GetMonsterIndex(6);
    map_list[MAP_GOOSE_PAGODA].poll[1] = GetMonsterIndex(7);
    map_list[MAP_GOOSE_PAGODA].poll[2] = GetMonsterIndex(8);
    map_list[MAP_GOOSE_PAGODA].rates[0] = 30;
    map_list[MAP_GOOSE_PAGODA].rates[1] = 30;
    map_list[MAP_GOOSE_PAGODA].rates[2] = 40;

    //花果山
     map_list[MAP_HUAGUOSHAN].ID = MAP_TOWN ;
    strcpy(map_list[MAP_HUAGUOSHAN].name, "新手村");
    map_list[MAP_HUAGUOSHAN].IsSecure = 1;
    map_list[MAP_HUAGUOSHAN].Min_lever = 1;
    map_list[MAP_HUAGUOSHAN].Max_lever = 99;
    map_list[MAP_HUAGUOSHAN].monster_num = 0;
    map_list[MAP_HUAGUOSHAN].poll[0] = GetMonsterIndex(9);
    map_list[MAP_HUAGUOSHAN].poll[1] = GetMonsterIndex(10);
    map_list[MAP_HUAGUOSHAN].poll[2] = GetMonsterIndex(11);
    map_list[MAP_HUAGUOSHAN].rates[0] = 30;
    map_list[MAP_HUAGUOSHAN].rates[1] = 30;
    map_list[MAP_HUAGUOSHAN].rates[2] = 40;



}

//选择地图
Map_ID ShowMap(void)
{
    int choose;
    printf("====  选择地图  ====\n");
    printf("[0]新手村外\n");
    printf("[1]东海湾\n");
    printf("[2]大雁塔\n");
    printf("[4]花果山\n");
    printf("请选择要去往的地图：");


    if(choose <0 || choose>4)
    {
        printf("输入无效\n");
        return g_current_map;
    }
    scanf("%d", &choose);

    return (Map_ID)choose;
}

//地图入口
void EnterMap(Map_ID map_id)
{
    g_current_map = map_id;
    g_player_x = MAP_WIDTH/2;
    g_player_y = MAP_HIGH-1;

}

//地图初始化
void MapInit(void)
{
    //双循环赋值，#为地图边界，.为行走空格
   for(int y = 0; y<MAP_HIGH; y++)
   {
    for(int x =0; x<MAP_WIDTH; x++)
    {
        if(y == 0 || y == MAP_HIGH-1 || x == 0 || x == MAP_WIDTH-1)
        {
            map[y][x] = '#';
        }
        else{
            map[y][x] = '.';
        }
    }
   }

}
//地图画布
void draw(void)
{
    for(int y=0; y<MAP_HIGH; y++)
    {
        for(int x=0; x<<MAP_WIDTH; x++)
        {
            mvaddch(x, y, map[y][x]);
        }
    }

}

//探索地图
void explore(void)
{
    srand(time(0));
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();

    MapInit();

    while(1)
    {
        int move = 0;
        clear();
        draw();
        mvaddch(g_player_y, g_player_x, '@');
        refresh();

        int key = getch();

        switch(key)
        {
            case 'w':
            if(g_player_y>1)
            move = 1;
            g_player_y--; break;

            case 's':
            move = 1;
            if(g_player_y<MAP_HIGH-2)
            g_player_y++; break;

            case 'a':
            move = 1;
            if(g_player_x>1)
            g_player_x--; break;

            case 'd':
            move = 1;
            if(g_player_x<MAP_WIDTH-2)
            g_player_x++; break;

            case 'q':
            endwin();
            return;

        }

        if(move && rand()%100 <DANGER_RATE)
        {
            int choose_num;
            prinrf("遇到一只野生怪兽！\n");
            printf("1.把它击败\n");
            printf("2.逃走\n");
            scanf("%d", &choose_num);

            switch(choose_num)
            {
                case 1:
                printf("补充战斗函数\n"); break;

                case 2:
                printf("回到新手村\n");
            }
        }
    }

}









