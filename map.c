#include "map.h"       // 解决 Map_ID, MapInfo, MAP_TOWN 等报错
#include "player.h"    // 解决 User 未定义的报错
#include "monster.h"   // 解决 GetMonsterIndex 未声明的报错
#include "battle.h"
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>

OnEncounterMonster g_encounter_cb = NULL;


Map_ID g_current_map = MAP_TOWN;
int g_player_x;
int g_player_y;
char map[MAP_HIGH][MAP_WIDTH];

MapInfo map_list[5];

/**
 * @brief  初始化地图数据信息：每个地图名字，是否是安全区，等级分布
 *         怪物数量，怪物池对应怪物指针，每种怪物出现概率
 * @param  无
 * @return 无
 */
void MapInfoInit(void)
{
    //安全区
    map_list[MAP_TOWN ].ID = MAP_TOWN ;
    strcpy(map_list[0].name, "新手村");
    map_list[MAP_TOWN].IsSecure = 1;
    map_list[MAP_TOWN].Min_level = 1;
    map_list[MAP_TOWN].Max_level = 99;
    map_list[MAP_TOWN].monster_num = 0;
    memset(map_list[MAP_TOWN].poll, 0, sizeof(map_list[0].poll));
    memset(map_list[MAP_TOWN].rates, 0, sizeof(map_list[0].rates));

    //新手村外
    map_list[MAP_OUTNOV_VILLOGE].ID = MAP_OUTNOV_VILLOGE;
    strcpy(map_list[MAP_OUTNOV_VILLOGE].name, "新手村外");
    map_list[MAP_OUTNOV_VILLOGE].IsSecure = 0;
    map_list[MAP_OUTNOV_VILLOGE].Min_level = 1;
    map_list[MAP_OUTNOV_VILLOGE].Max_level = 5;
    map_list[MAP_OUTNOV_VILLOGE].monster_num = 3;
    map_list[MAP_OUTNOV_VILLOGE].poll[0] = GetMonsterIndex(0);
    map_list[MAP_OUTNOV_VILLOGE].poll[1] = GetMonsterIndex(1);
    map_list[MAP_OUTNOV_VILLOGE].poll[2] = GetMonsterIndex(2);
    map_list[MAP_OUTNOV_VILLOGE].rates[0] = 30;
    map_list[MAP_OUTNOV_VILLOGE].rates[1] = 30;
    map_list[MAP_OUTNOV_VILLOGE].rates[2] = 40;

    //东海湾
    map_list[MAP_ESAST_BAY].ID = MAP_ESAST_BAY;
    strcpy(map_list[MAP_ESAST_BAY].name, "东海湾");
    map_list[MAP_ESAST_BAY].IsSecure = 0;
    map_list[MAP_ESAST_BAY].Min_level = 5;
    map_list[MAP_ESAST_BAY].Max_level = 15;
    map_list[MAP_ESAST_BAY].monster_num = 3;
    map_list[MAP_ESAST_BAY].poll[0] = GetMonsterIndex(3);
    map_list[MAP_ESAST_BAY].poll[1] = GetMonsterIndex(4);
    map_list[MAP_ESAST_BAY].poll[2] = GetMonsterIndex(5);
    map_list[MAP_ESAST_BAY].rates[0] = 30;
    map_list[MAP_ESAST_BAY].rates[1] = 30;
    map_list[MAP_ESAST_BAY].rates[2] = 40;

    //大雁塔
    map_list[MAP_GOOSE_PAGODA].ID = MAP_GOOSE_PAGODA;
    strcpy(map_list[MAP_GOOSE_PAGODA].name, "大雁塔");
    map_list[MAP_GOOSE_PAGODA].IsSecure = 0;
    map_list[MAP_GOOSE_PAGODA].Min_level = 10;
    map_list[MAP_GOOSE_PAGODA].Max_level = 25;
    map_list[MAP_GOOSE_PAGODA].monster_num = 3;
    map_list[MAP_GOOSE_PAGODA].poll[0] = GetMonsterIndex(6);
    map_list[MAP_GOOSE_PAGODA].poll[1] = GetMonsterIndex(7);
    map_list[MAP_GOOSE_PAGODA].poll[2] = GetMonsterIndex(8);
    map_list[MAP_GOOSE_PAGODA].rates[0] = 30;
    map_list[MAP_GOOSE_PAGODA].rates[1] = 30;
    map_list[MAP_GOOSE_PAGODA].rates[2] = 40;

    //花果山
    map_list[MAP_HUAGUOSHAN].ID = MAP_HUAGUOSHAN;
    strcpy(map_list[MAP_HUAGUOSHAN].name, "花果山");
    map_list[MAP_HUAGUOSHAN].IsSecure = 0;
    map_list[MAP_HUAGUOSHAN].Min_level = 20;
    map_list[MAP_HUAGUOSHAN].Max_level = 35;
    map_list[MAP_HUAGUOSHAN].monster_num = 3;
    map_list[MAP_HUAGUOSHAN].poll[0] = GetMonsterIndex(9);
    map_list[MAP_HUAGUOSHAN].poll[1] = GetMonsterIndex(10);
    map_list[MAP_HUAGUOSHAN].poll[2] = GetMonsterIndex(11);
    map_list[MAP_HUAGUOSHAN].rates[0] = 30;
    map_list[MAP_HUAGUOSHAN].rates[1] = 30;
    map_list[MAP_HUAGUOSHAN].rates[2] = 40;



}

/**
 * @brief  选择要去往的地图
 * @param  无
 * @return 返回地图ID
 */
Map_ID ShowMap(void)
{
    int choose = 0;
    printf("====  选择地图  ====\n");
    printf("[0]新手村\n");
    printf("[1]新手村外\n");
    printf("[2]东海湾\n");
    printf("[3]大雁塔\n");
    printf("[4]花果山\n");
    printf("请选择要去往的地图：");

    if (scanf("%d", &choose) != 1) {
        printf("输入无效\n");
        int c; while ((c = getchar()) != '\n' && c != EOF);
        return g_current_map;
    }
    if(choose <0 || choose>4)
    {
        printf("输入无效\n");
        return g_current_map;
    }
    

    return (Map_ID)choose;
}

//地图入口
void EnterMap(Map_ID map_id)
{
    g_current_map = map_id;
    g_player_x = MAP_WIDTH/2;
    g_player_y = MAP_HIGH-1;
    printf("进入地图：%s\n", map_list[g_current_map].name);

}

//探索地图
int Explore(User *user)
{
    srand((unsigned)time(NULL));
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();

    MapInit();          //地图初始化

    while(1)
    {
        
        clear();
        draw();
        mvaddch(g_player_y, g_player_x, '@');
        mvprintw(20, 30,"%s",map_list[g_current_map].name);
        
        refresh();

        int key = getch();
        if(key == 'q')
        {
            endwin();
            return 0;
        }
        
        int move = InputCheck(key);

        
        if(move && rand()%100 <DANGER_RATE)
        {
            
           MapInfo *cur_map = &map_list[g_current_map];
           if(cur_map->IsSecure) continue;

            //随机抽取函数
            monster *target = RanSelect(cur_map->poll, cur_map->rates,
                                        cur_map->monster_num);
            if(target == NULL)
            {
                continue;
            }
           mvprintw((MAP_HIGH), (MAP_WIDTH+10)," \n"
                                                  "怪物名称:%s\n"
                                                  "等级：%d\n"
                                                  "生命值：%d\n"
                                                  "攻击力：%d\n"
                                                  "防御力:%d\n"
                                                  "速度:%d\n"
                                                  "经验:%d\n"
                                                  "金币:%d\n", 
                                        target->mon_name, target->lever, target->HP, target->Attack_power,
                                        target->Defense_power, target->speed, target->experience, target->gold);
            refresh();
            getch();

            if(g_encounter_cb != NULL && user !=NULL)
            {
                endwin();
                int battle_result = g_encounter_cb(user, target);
                insertUser(user);
                if(battle_result == 2)
                {
                    printf("\n你被怪物杀死了！...\n");
                    return 1;

                }
                initscr();
                cbreak();
                noecho();
                clear();
            }           
            
                       
        }
    }

}

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
        for(int x=0; x<MAP_WIDTH; x++)
        {
            mvaddch(y, x, map[y][x]);
        }
    }

}

int InputCheck(int key)
{
    int move = 0;
    switch(key)
    {
        
        case 'w':
        move = 1;
        if(g_player_y>1)
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
        return 0;
        break;

        default:
        break;

    } 

    return move;

}

void SetEncounterCallback(OnEncounterMonster cb)
{
    g_encounter_cb = cb;
}


void map_main(User *user){
        printf("前往地图\n");
        int mob_num= MonsterInit(FILE_NAME);
        printf("mod_num %d", mob_num);
        if (mob_num <= 0) {
            printf("怪物数据加载失败\n");
            return;
        }
        printf("monsterInit 调用了\n");
        MapInfoInit();
        printf("MapInfoInit 调用了\n");
        SetEncounterCallback(battle);
        printf("SetEncounterCallback 调用了\n");
        Map_ID map_id = ShowMap();
        EnterMap(map_id);
        printf("EnterMap 调用了\n");
        int explore_result = Explore(user);
        printf("============\n");
        if(explore_result == 1){
            printf("你被怪物击败了！损失10%%经验\n");
            user->exp = (long)(user->exp * 0.9);
            user->hp = user->max_hp;
            user->mp = user->max_mp;
            insertUser(user);
            printf("已返回城镇，气血法力已恢复\n");
        }
}





