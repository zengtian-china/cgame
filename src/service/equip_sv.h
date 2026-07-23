
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
    float crit_bonus;
    float dodge_bonus;
    int set_id;
    int price;
    char description[256];

}Equips;

//装备配置文件路径
int init();
//获取装备信息
Equips *get_by_id(int id);
//穿戴装备
int equip(User *user, int equip_id);
//卸下装备
int unequip(User *user, int slot);
void calc_bonus(User *user);


typedef struct {
    int (*init)(const char *path);
    Equips *(*get_by_id)(int id);
    int (*equip)(User *user, int equip_id);
    int (*unequip)(User *user, int slot);
    void (*calc_bonus)(User *user);
    void (*release)(void);
} EquipmentService;

#endif