#ifndef _PLAYER_SV_H_
#define _PLAYER_SV_H_
typedef struct {

char username[20];      //用户名
int level;              //等级
long exp;               //经验
int gold;               //金币
int race;               //门派
// 基础属性
int strength;           //力量
int physique;           //体质
int endurance;          //耐力
int agility;            //敏捷         
int intelligence;       //魔法
// 战斗属性
int max_hp;             // 气血上限
int hp;                 // 当前气血
int max_mp;             // 法力上限
int mp;                 // 当前法力
int attack;             // 攻击
int defense;            // 防御
int speed;              // 速度
int magic_attack;       // 法伤
int magic_defense;      // 法防
float dodge_rate;       // 闪避率
float crit_rate;        // 暴击率
//装备
int equipment[6];       //装备ID数组:武器，头盔，铠甲，腰带，鞋子，项链
//背包
int invertory[20][2];   //背包：[物品ID，数量]
int inventory_count;    //背包物品数
int skills[10][2];      //技能：[技能ID，等级]
int skill_count;        //技能数量
//位置
int current_map;        //当前地图
int pos_x;              //x坐标
int pos_y;              //y坐标
//其他
int contribution;       //门派贡献
int reputation;         //声望
}User;

#endif