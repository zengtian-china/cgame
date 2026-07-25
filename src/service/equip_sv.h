
#ifndef _EQUIP_H_
#define _EQUIP_H_
#include "player_sv.h"


int EquipFromInventory(User *user, int inventory_index);
int EquipDirectly(User *user, int equip_id);
int Unslot(User *user,int slot);

typedef struct {
    int (*init)(const char *path);

    int (*equip)(User *user, int equip_id);
    int (*unequip)(User *user, int slot);
    void (*calc_bonus)(User *user);
    void (*release)(void);
} EquipmentService;

#endif