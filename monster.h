#ifndef _MONSTER_H_
#define _MONSTER_H_

#define MONSTER_MAX 50          //最多怪兽数量
#define BUF_LEN 256                  //数组容量
#define FILE_NAME "./data/monster.txt"      //怪兽数据存放文件名

typedef struct monster{
    char mon_name[50];              //怪物名称
    int lever;                      //怪物等级
    int HP;                         //生命值
    int Attack_power;               //攻击力
    int Defense_power;              //防御力
    int speed;                       //速度
    int experience;                  //经验
    int gold;                        //金币
    

}monster;

//怪物数据初始化
int MonsterInit(const char *filename);

//随机抽取
monster* RanSelect(monster *pool[], int rates[], int len);

monster* GetMonsterIndex(int num);

#endif