#ifndef _MAP_H_
#define _MAP_H_

#include <stdio.h>
#include "monster.h"

#include "save.h"
#include "map.h"       // 解决 Map_ID, MapInfo, MAP_TOWN, MAP_HIGH, OnEncounterMonster 未定义的问题
#include "player.h"    // 解决 User 未定义的问题
// #include "monster.h"   // 解决 GetMonsterIndex 未声明的问题



#define MAP_HIGH 20             //地图宽度
#define MAP_WIDTH 70            //地图长度
#define DANGER_RATE 15          //遇敌概率（百分比）

typedef enum{
    MAP_TOWN = 0,               //安全区，城镇
    MAP_OUTNOV_VILLOGE,         //新手村外
    MAP_ESAST_BAY,              //东海湾
    MAP_GOOSE_PAGODA,           //大雁塔
    MAP_HUAGUOSHAN              //花果山
}Map_ID;    

typedef struct MapInfo{
    Map_ID ID;
    char name[50];
    int IsSecure;               //是否在安全区1在，0野外；
    int Min_level;              //地图怪兽最低等级
    int Max_level;              //地图怪兽最高等级
    int rates[5];               //地图遇敌不同怪兽出现概率
    monster *poll[5];            //地图对应怪兽池
    int monster_num;            //怪兽数量
}MapInfo;

int Explore(User *user);

void draw(void);

void MapInit(void);

void EnterMap(Map_ID map_id);

Map_ID ShowMap(void);

void MapInfoInit(void);


typedef int (*OnEncounterMonster) (User *user, monster *target);

void SetEncounterCallback(OnEncounterMonster cb);

int InputCheck(int key);
void map_main(User *user);
#endif