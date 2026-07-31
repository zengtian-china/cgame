#ifndef _SERVICE_H_
#define _SERVICE_H_
#include "player_sv.h"
typedef struct {
    User *(*create)(const char *name, int race, int class_id);
    User *(*load)(const char *name);
    int (*save)(User *user);
    void (*release)(User *user);
    void (*level_up)(User *user);
    void (*add_exp)(User *user, int exp);
    void (*add_gold)(User *user, int gold);
    void (*calc_battle_stats)(User *user);
} PlayerService;

#endif