#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "monster.h"
#include "map.h"

// ===================== 方案1：外部战斗回调函数 =====================
void ExternalBattle(monster* mob)
{
    printf("\n【外部战斗模块触发】\n");
    printf("怪物：%s Lv.%d | HP:%d ATK:%d DEF:%d EXP:%d GOLD:%d\n",
           mob->mon_name, mob->lever, mob->HP, mob->Attack_power, mob->Defense_power, mob->experience, mob->gold);
    printf("回车返回地图\n");
    getchar();
    getchar();
}

int main(void)
{
    // 随机种子初始化
    srand((unsigned)time(NULL));

    // 1. 加载怪物文件，打通怪物模块与地图模块数据
    int mob_count = MonsterInit(FILE_NAME);
    if(mob_count < 0)
    {
        printf("怪物文件加载失败！程序退出\n");
        return -1;
    }
    printf("成功加载%d只怪物数据\n", mob_count);

    // 2. 初始化所有地图信息（绑定每个地图怪物池、概率）
    MapInfoInit();

    // ========== 切换使用方案1/方案2 ==========
    // 方案1：开启外部战斗回调（注释此行切换方案2内置战斗）
    //SetEncounterCallback(ExternalBattle);
    // 方案2：注释上面一行，直接使用map.c内置简易战斗，无需回调

    // 3. 选择地图
    Map_ID select_map = ShowMap();
    EnterMap(select_map);

    // 4. 进入地图WASD操控探索，移动概率遇敌
    printf("游戏启动：W上 S下 A左 D右 Q退出\n");
    getchar(); // 吸收回车
    Explore();

    return 0;
}
