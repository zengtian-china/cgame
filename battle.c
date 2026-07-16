#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "battle.h"
int use_item(User *user,int id){
    printf("循环start\n");
    for(int i= user->invertory_size-1;i>=0;i--){
        printf("循环中\n");
        if(user->invertory[i].item_id == id){
            user->invertory[i].item_count -= 1;
            //获取商品列表 
            itemList *list = createItemsList();
            printf("商品列表\n");
            items * it = find_items(id,list);
            if (it == NULL) {
                printf("系统错误：找不到该物品信息！\n");
                free(list);
                return -1;
            }
            int tmp_hp =0;
            int ret = sscanf(it->effect, "恢复%d", &tmp_hp);
            printf("ref = %d tmp_hp=%d",ret,tmp_hp);
            free(list);
            free(it);
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
        } 
    }
    // 排序
    printf("循环end\n");
    sort_item(user);
    // 更新到文件中
    insertUser(user);
    return 1;
}



int battle(User *user,monster *monster){


    printf("进行回合制游戏\n");
    // 遇敌 
    int tmp_battle = 0;
    int i =1;
    // 判断先手

    while (user->hp >0 && monster->HP >0)
    {
        printf("第%d回合\n",i);
        if(user->speed > monster->speed){
            //角色先手
            printf("1.攻击\n");
            printf("2.使用药品\n");
            printf("3.逃跑\n");
            printf("选择(1~3):");
            scanf("%d",&tmp_battle);
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
                printf("选择使用的药品:");
                scanf("%d",&tmp_yao);
                use_item(user,tmp_yao);         

            } else if (3 == tmp_battle){
                return 0;
            }
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
            scanf("%d",&tmp_battle);
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
                        break;
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
                printf("选择使用的药品()");
                scanf("%d",&tmp_yao);
                use_item(user,tmp_yao);         

            } else if (3 == tmp_battle){
                return 0;
            }
        }
        i++;
    
    }

    


    // 出招  攻击 回复血量 逃跑

    //攻击 判断血量
    //  攻击值 > 当前hp
     // 血量 -> 0  
     //小于  血量-攻击值

    //怪物 出招  攻击
    // 攻击 判断血量
        //  攻击值 > 当前hp
        // 血量 -> 0  
        //小于  血量-攻击值
        // i++
        // 结束条件  血量为0
        // 标识符 胜利 失败
        // 胜利  经验值 + 金币

}


//使用药品  是否成功

// int main1(){
//     User *user= read_save_main();

//     show_my(user);

//     MonsterInit(FILE_NAME);
//     monster *monsters = GetMonsterIndex(2);

//     battle(user,monsters);
//     insertUser(user);
// }