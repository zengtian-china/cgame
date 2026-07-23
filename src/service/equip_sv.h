
#ifndef _EQUIP_H_
#define _EQUIP_H_
#include "player_sv.h"
typedef struct _equips{
    int id;
    char name[50];
    int slot;
    int quality;
    int level_require;
    int attack_bonus;
    int defense_bonus;
    int speed_bonus;
    int magic_attack_bonus;
    int magic_defense_bonus;
    int max_hp_bonus;
    int max_mp_bonus;
    int crit_bonus;
    int dodge_bonus;
    int set_id;
    int price;
    int description;

}Equips;

//装备配置文件路径
int init(const char *path);
//获取装备信息
Equips *get_by_id(int id);
//穿戴装备
int equip(User *user, int equip_id);
//卸下装备
int unequip(User *user, int slot);
void calc_bonus(User *user);

#endif