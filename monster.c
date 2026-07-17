#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "monster.h"

static monster g_monster[MONSTER_MAX];
static int g_monster_total = 0;

/**
 * @brief  从文件加载所有怪物数据
 * @param  filename  数据文件路径
 * @return 返回成功加载的怪物数量g_monster_total，失败返回-1
 */
int MonsterInit(const char *filename)
{
    printf("进入了怪兽初始化\n");
    char buf[BUF_LEN];
    
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        perror("fopen failed!");
        return -1;
    }
    g_monster_total = 0;
    while(fgets(buf, sizeof(buf), fp) != NULL)
    {
        //去掉行尾换行符
        buf[strcspn(buf, "\n")] = '\0';
        //跳过空行
        if(buf[0] == '\0')
        {
            continue;
        }
        //防止数组越界
        if(g_monster_total >= MONSTER_MAX)
        {
            break;
        }
        //解析怪物属性
        int ret = sscanf(
                        buf, "%[^,],%d,%d,%d,%d,%d,%d,%d", 
                        g_monster[g_monster_total].mon_name, &g_monster[g_monster_total].lever,
                        &g_monster[g_monster_total].HP, &g_monster[g_monster_total].Attack_power,
                        &g_monster[g_monster_total].Defense_power, &g_monster[g_monster_total].speed, 
                        &g_monster[g_monster_total].experience, &g_monster[g_monster_total].gold
                        );
        if(ret == 8)
        {
            //printf("解析成功！\n");
            g_monster_total++;
        }

    }
    fclose(fp);
    //返回g_monster_total只怪物
    return g_monster_total;
}
/**
 * @brief  随机抽取(地图中需要给怪物池，自己设置怪物出现概率)
 * @param  pool[]  怪物池
 * @param  rates[] 对应怪物出现概率，总和加起来100
 * @param  len 数组长度（怪物数量）
 * @return 抽中怪物的指针
 */
monster* RanSelect(monster *pool[], int rates[], int len)
{
    int ran_num = rand() % 100 + 1;
    int sum = 0;
    for(int i=0; i<len; i++)
    {
        sum +=rates[i];
        
        if(ran_num<=sum)
        {
            return pool[i];
        }
        

    }
    return pool[len-1];

}
//test专用

monster* GetMonsterIndex(int num)
{
    if(num>=g_monster_total || num <0)
    {
        return NULL;
    }

    return &g_monster[num];

} 





