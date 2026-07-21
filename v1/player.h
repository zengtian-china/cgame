/*
 * @Author: zengtian
 * @Date: 2026-07-15 20:22:26
 * @LastEditors: zengtian
 * @LastEditTime: 2026-07-15 20:54:31
 * @Description: File Description
 * @FilePath: \cgame\player.h
 * @@file: 
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "save.h"
void levelUp(User *user);
User * UserInit(const char *name);
void levelUpLogic(User *user, int exp);
void stat_growth(User *user);

#endif