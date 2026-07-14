#ifndef _MAP_H_
#define _MAP_H_

#include <stdio.h>
#include "monster.h"

#define MAP_HIGH 10
#define MAP_WIDTH 10

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
    int Min_lever;              //地图怪兽最低等级
    int Max_lever;              //地图怪兽最高等级
    int rates[5];               //地图遇敌不同怪兽出现概率
    monster *poll[5];            //地图对应怪兽池
    int monster_num;            //怪兽数量
}MapInfo;


#endif