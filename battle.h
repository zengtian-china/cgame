#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "save.h"
#include "item.h"
#include "monster.h"
#include "player.h"

struct user; 
typedef struct user User;

struct monster; 
typedef struct monster monster;

int use_item(User *user,int id);

int battle(User *user,monster *monster);


