#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "monster.h"
#include "map.h"




int main(void)
{
    srand((unsigned)time(NULL));
    


    int g_count = MonsterInit(FILE_NAME);
    printf("成功加载%d只怪物!\n", g_count);
    
    MapInfoInit();
    Map_ID choosed_map = ShowMap();
    EnterMap(choosed_map);
    Explore();
     
    
    


    

    
    return 0;
}