#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include "save.h"

void stat_growth(User *user){
    int strength = 0;
    int physique = 0;
    int agility = 0;
    int statuc = 1;
    printf("属性分配\n");
    while (statuc){
        printf("请自由分配5点属性点\n");
        printf("力量(0~5):");
        if (scanf("%d",&strength) != 1) {
            printf("输入无效，请重新输入\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        printf("体质(0~5):");
        if (scanf("%d",&physique) != 1) {
            printf("输入无效，请重新输入\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        printf("敏捷(0~5):");
        if (scanf("%d",&agility) != 1) {
            printf("输入无效，请重新输入\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        if (strength>=0 && strength<=5 &&
        physique>=0 && physique<=5 &&
        agility>=0 && agility<=5 &&
        strength+physique+agility == 5){
            statuc = 0;
            break;
        } else{
            printf("分配属性点失败，请重新分配\n");
        }
    }
    user->strength = user->strength + strength;
    user->physique = user->physique + physique;
    user->agility = user->agility +agility;
    user->attack = (int)(user->strength * 0.8);
    user->max_hp = user->max_hp + physique * 5;
    user->speed = (int)(user->speed + agility * 0.7);
}



void levelUp(User *user){

    if (user->level >= 50) {
        printf("已达到最高等级！\n");
        return;
    }

    if (user->level < 10){
        user->level = user->level + 1;
        user->gold = user->gold + 100 * user->level; 
        user->strength = user->strength + 2;
        user->physique = user->physique + 2;
        user->agility = user->agility + 2;
        user->max_hp = user->max_hp + user->physique * 5;
        user->hp = user->max_hp;
        user->max_mp = user->max_mp + 150;
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.8);
        user->defense = user->defense + user->level;
        user->speed = user->speed + 1;

    } else if(user->level <20){
        user->level = user->level + 1;
        user->gold = user->gold + 200 * user->level; 
        user->strength = user->strength + 2*user->level;
        user->physique = user->physique + 2*user->level;
        user->agility = user->agility + 2*(user->level- 9);
        user->max_hp = user->max_hp + user->physique * 5;
        user->hp = user->max_hp;
        user->max_mp = user->max_mp + 150*(user->level -9);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.8);
        user->defense = user->defense + (int)(1 + 0.1 * user->level / 10.0) * user->level;
        user->speed = user->speed + user->level;

    } else if(user->level <30){
        user->level = user->level + 1;
        user->gold = user->gold + 300 * user->level; 
        user->strength = user->strength + 3*user->level;
        user->physique = user->physique + 3*user->level;
        user->agility = user->agility + 3*(user->level- 10);
        user->max_hp = user->max_hp + user->physique * 5;
        user->hp = user->max_hp;
        user->max_mp = user->max_mp + 300*(user->level -5);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.9);
        user->defense = user->defense + (int)(1 + 0.2 * user->level / 5.0) * user->level;
        user->speed = user->speed + 2*user->level;
    } else if(user->level <40){
        user->level = user->level + 1;
        user->gold = user->gold + 500 * user->level; 
        user->strength = user->strength + 5*user->level;
        user->physique = user->physique + 5*user->level;
        user->agility = user->agility + 5*(user->level- 15);
        user->max_hp = user->max_hp + user->physique * 5;
        user->hp = user->max_hp;
        user->max_mp = user->max_mp + 500*(user->level -15);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.9);
        user->defense = user->defense + (int)(1 + 0.2 * user->level / 5.0) * user->level;
        user->speed = user->speed + 2*user->level;
    } else if(user->level <50){
        user->level = user->level + 1;
        user->gold = user->gold + 1000 * user->level; 
        user->strength = user->strength + 10*user->level;
        user->physique = user->physique + 10*user->level;
        user->agility = user->agility + 10*(user->level- 20);
        user->max_hp = user->max_hp + user->physique * 5;
        user->hp = user->max_hp;
        user->max_mp = user->max_mp + 1000*(user->level -20);
        user->mp = user->max_mp;
        user->attack = (int)(user->strength * 0.9);
        user->defense = user->defense + (int)(1 + 0.5 * user->level / 5.0) * user->level;
        user->speed = user->speed + 2*user->level;
    } 
    stat_growth(user);
    showUser(user);

}

void levelUpLogic(User *user, int exp){
while (1) {
    int max_exp;
    if (user->level < 10) max_exp = user->level * 100;
    else if (user->level < 30) max_exp = user->level*200 + user->level*user->level*2;
    else if (user->level < 40) max_exp = user->level*400 + user->level*user->level*5;
    else if (user->level < 50) max_exp = user->level*100 + user->level*user->level*10;
    else break;
    if (max_exp > user->exp) break;
    user->exp -= max_exp;
    levelUp(user);
}

}
