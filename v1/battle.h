/*
 * @Author: zengtian
 * @Date: 2026-07-16 13:45:26
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-16 15:32:00
 * @Description: File Description
 * @FilePath: \cgame\battle.h
 * @@file: 
 */
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

#endif


