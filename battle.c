#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "battle.h"

int use_item(User *user,int id){

    for(int i= user->invertory_size-1;i>=0;i--){
        if(user->invertory[i].item_id == id && id !=0){
            user->invertory[i].item_count -= 1;
            //获取商品列表 
            itemList *list = createItemsList();
            items * it = find_items(id,list);
            if (it == NULL) {
                printf("系统错误：找不到该物品信息！\n");
                free(list);
                return -1;
            }
            int tmp_hp =0;
            int ret = sscanf(it->effect, "恢复%d", &tmp_hp);
            if (ret == 1 && tmp_hp > 0) {
                // 恢复气血逻辑
                if(user->hp + tmp_hp > user->max_hp) {
                    user->hp = user->max_hp;
                } else {
                    user->hp = user->hp + tmp_hp;
                }
                printf("成功使用 %s，恢复了 %d 点气血！\n", it->item, tmp_hp);
            } else {
                printf("无法解析物品效果。\n");
            }
            free(list);
            free(it);
            break;
        } 
    
    }
    // 排序
    sort_item(user);
    // 更新到文件中
    insertUser(user);
    return 1;
}

int battle(User *user,monster *monster){
    int tmp_battle = 0;
    int i =1;

    while (user->hp >0 && monster->HP >0)
    {
        printf("第%d回合\n",i);
        if(user->speed > monster->speed){
            //角色先手
            printf("1.攻击\n");
            printf("2.使用药品\n");
            printf("3.逃跑\n");
            printf("选择(1~3):");
            if (scanf("%d",&tmp_battle) != 1) {
                printf("输入的是无效数据,请重新输入\n");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            if(1 == tmp_battle) {
                //攻击
                if(user->attack >= monster->HP){
                    monster->HP = 0;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d,%s被ko了\n",
                        user->uname,
                        monster->mon_name,
                        user->attack,
                        monster->mon_name,
                        monster->HP,
                        monster->mon_name);

                    // 获取经验，获取金币
                        user->gold += monster->gold;
                        levelUpLogic(user,monster->experience);
                        return 1;
                } else{
                    monster->HP = monster->HP-user->attack;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d\n",
                        user->uname,
                        monster->mon_name,
                        user->attack,
                        monster->mon_name,
                        monster->HP);
                }
                //回血
            } else if (2==tmp_battle){
                int tmp_yao = 0;
                show_my(user);
                while (1)
                {
                    printf("选择使用的药品:");
                    if (scanf("%d",&tmp_yao) != 1) {
                        printf("输入的是无效数据,请重新输入\n");
                        int c; while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }
                    break;
                }
                use_item(user,tmp_yao);         

            } else if (3 == tmp_battle){
                return 0;
            }
        if(monster->HP > 0){
            if(monster->Attack_power> user->hp){
                    user->hp = 0;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d,%s被ko了\n",
                        monster->mon_name,
                        user->uname,
                        monster->Attack_power,
                        user->uname,
                        user->hp,
                        user->uname);
                    insertUser(user);
                    return 2;
                } else{
                    user->hp -= monster->Attack_power;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d\n",
                        monster->mon_name,
                        user->uname,
                        monster->Attack_power,
                        user->uname,
                        user->hp);
                }
            }

        }
        else{
            //怪物攻击
            if(monster->Attack_power> user->hp){
                    user->hp = 0;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d,%s被ko了\n",
                        monster->mon_name,
                        user->uname,
                        monster->Attack_power,
                        user->uname,
                        user->hp,
                        user->uname);
                    insertUser(user);
                    return 2;
                } else{
                    user->hp -= monster->Attack_power;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d\n",
                        monster->mon_name,
                        user->uname,
                        monster->Attack_power,
                        user->uname,
                        user->hp);
            }

            //角色后手
            printf("1.攻击\n");
            printf("2.使用药品\n");
            printf("3.逃跑\n");
            printf("选择(1~3):");
            while (1)
            {
                if (scanf("%d",&tmp_battle) != 1) {
                printf("输入的是无效数据,请重新输入\n");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                continue;
                }
                break;
            }
            if(1 == tmp_battle) {
                //攻击
                if(user->attack >= monster->HP){
                    monster->HP = 0;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d,%s被ko了\n",
                        user->uname,
                        monster->mon_name,
                        user->attack,
                        monster->mon_name,
                        monster->HP,
                        monster->mon_name);
                        // 获取经验，获取金币
                        user->gold += monster->gold;
                        levelUpLogic(user,monster->experience);
                        insertUser(user);
                        return 1;
                } else{
                    monster->HP = monster->HP-user->attack;
                    printf("%s使用了攻击，对%s造成了%d伤害,%s当前HP剩余%d\n",
                        user->uname,
                        monster->mon_name,
                        user->attack,
                        monster->mon_name,
                        monster->HP);
                }
                //回血
            } else if (2==tmp_battle){
                int tmp_yao = 0;
                show_my(user);
                while (1)
                {
                    printf("选择使用的药品:");
                    if (scanf("%d",&tmp_yao) != 1) {
                        printf("输入的是无效数据,请重新输入\n");
                        int c; while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }
                    break;
                }
                use_item(user,tmp_yao);         

            } else if (3 == tmp_battle){
                return 0;
            }
        }
        i++;
    
    }
    return 0;
}
