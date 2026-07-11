#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "monster.h"




int main(void)
{
    srand((unsigned)time(NULL));

    int g_count = MonsterInit(FILE_NAME);
    printf("成功加载%d只怪物!\n", g_count);

    monster *pool[3] = {GetMonsterIndex(0), GetMonsterIndex(1), GetMonsterIndex(2)};
    int rates[3] = {1, 2, 97};

    monster *p = RanSelect(pool, rates, 3);

    printf("%s\n", p->mon_name);
    

    
    return 0;
}