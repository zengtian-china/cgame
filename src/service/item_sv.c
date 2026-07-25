#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "item_sv.h"
#include "item_mgr.h"



int add(User *user, int item_id, int count){
    //检查输入数据
    if(!user || item_id<=0 || count <=0) return -1;
    //获取物品结构体信息
    ItemConfig * items = item_mgr_get_by_id(item_id);
    // 数量 每次添加
    if(!items || items->max_stack < count) return -1;
    
    //根据max_stack来区分 1 和 99的类型

    // 如果max_stack是1的情况
    if(items->max_stack == ONE){
        if(user->inventory_count+count > ITEM_MAX) return -1;
        for(int i=0;i<count;i++){
            user->invertory[user->inventory_count][0] = item_id;
            user->invertory[user->inventory_count][1] = ONE;
            user->inventory_count ++;
        }
    } else{
        // 99的情况
        int tmp_number = (count + 99 - 1) / 99;
        int num_count = count;
        for(int i =0;i<tmp_number && num_count >0;i++){
            // 循环次数 
            if(user->inventory_count+tmp_number > ITEM_MAX) return -1;
            user->invertory[user->inventory_count][0] = item_id;
            user->invertory[user->inventory_count][1] = num_count > 99 ?99:num_count;
            num_count-=99;
            user->inventory_count ++;
        }

    }
}


int sort(User *user){
    // 先根据 type类型来排序 
    for(int i=0;i<user->inventory_count;i++){
        // if(tmp->type)
        for(int j=i+1;j<user->inventory_count;j++){
            ItemConfig *tmp = item_mgr_get_by_id(user->invertory[i][0]);
            ItemConfig *tmp_1 = item_mgr_get_by_id(user->invertory[j][0]);
            // 找出类型最小的那个
            if(tmp->type < tmp_1->type){
                int tmp_num1 = user->invertory[i][0];
                int tmp_num2 = user->invertory[i][1];
                user->invertory[i][0] = user->invertory[j][0];
                user->invertory[i][1] = user->invertory[j][1];
                user->invertory[j][0] = tmp_num1;
                user->invertory[j][1] = tmp_num2;

            }
            // 类型相同 按照装备品质从高到低排
             else if (tmp->type == tmp_1->type &&(tmp->type == ITEM_TYPE_EQUIPMENT) && tmp->quality > tmp_1->quality){
                int tmp_num1 = user->invertory[i][0];
                int tmp_num2 = user->invertory[i][1];
                user->invertory[i][0] = user->invertory[j][0];
                user->invertory[i][1] = user->invertory[j][1];
                user->invertory[j][0] = tmp_num1;
                user->invertory[j][1] = tmp_num2;
            // 类型相同 按照药品效果类别从高到低排
            }else if(tmp->type == tmp_1->type &&(tmp->type == ITEM_TYPE_CONSUMABLE ) && tmp->effect_type > tmp_1->effect_type){
                int tmp_num1 = user->invertory[i][0];
                int tmp_num2 = user->invertory[i][1];
                user->invertory[i][0] = user->invertory[j][0];
                user->invertory[i][1] = user->invertory[j][1];
                user->invertory[j][0] = tmp_num1;
                user->invertory[j][1] = tmp_num2;
                //按照 id从低到高排
            }else if(tmp->type == tmp_1->type && tmp->id < tmp_1->id){
                int tmp_num1 = user->invertory[i][0];
                int tmp_num2 = user->invertory[i][1];
                user->invertory[i][0] = user->invertory[j][0];
                user->invertory[i][1] = user->invertory[j][1];
                user->invertory[j][0] = tmp_num1;
                user->invertory[j][1] = tmp_num2;
            }
        }
    }

}