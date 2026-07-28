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
        return 0;
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
        return 0;

    }
}

/*
    user 背包 ->是否要拿个该物品详情信息 是否有必要
     有 需要查看type
    如果是装备 count 2 id 找俩个
    消耗瓶， 计算数量 
    从后往前 根据格子容量来区别一下
    如果是 一个的情况下，得先去查找id数（定义一个变量统计id次数）count 要跟这个数小才能消除
    如果
*/
int remove(User *user,int item_id,int count){
    //检查输入数据
    if(!user || item_id<=0 || count <=0) return -1;
    //获取物品结构体信息
    ItemConfig * items = item_mgr_get_by_id(item_id);
    // 数量  是否要判断 max_stack 最大容量，不需要 如果是装备的话，最大就 1
    if(!items) return -1;
    //根据max_stack来区分 1 和 99的类型
        // 如果max_stack是1的情况
    if(items->max_stack == ONE){
        // max_stack的情况 判断是否count是否超过了真实容量
        if(user->inventory_count-count <0) return -1;
        // 先查找id 是否有那么多
        int find_count = 0;
        for(int i=0;i<user->inventory_count;i++){
            if(user->invertory[i][0] == item_id){
                find_count++;
            }
        }
        if(find_count < count){
            printf("没有那么多数量\n");
            return -1;
        }
        else{
            // 查找 item_id 移除
            for(int i=user->inventory_count-1;i>=0 && count>0;i--){
                if(user->invertory[i][0] == item_id && user->invertory[i][1] !=0){
                    user->invertory[i][1] = 0;
                    count--;
                }
            }
            
        }
        return 0;
    } else{
        // 99的情况 数量除以99 然后查找id
        int tmp_number = (count + 99 - 1) / 99;
        // 先查找id 是否有那么多
        int total_have = 0;
        for (int i = 0; i < user->inventory_count; i++) {
            if (user->invertory[i][0] == item_id) {
                total_have += user->invertory[i][1];
            }
        }
if (total_have < count) {
    printf("没有那么多数量\n");
    return -1;
    }   else{
            // 查找 item_id 移除
            for(int i=user->inventory_count-1;i>=0 && count>0;i--){
                if(user->invertory[i][0] == item_id && user->invertory[i][1] !=0){
                    int deduct = (user->invertory[i][1] <= count) 
                                ? user->invertory[i][1]   // 这格全扣
                                : count;                  // 只扣需要的
                    user->invertory[i][1] -= deduct;
                    count -= deduct;
                }
            }
            
        }
        return 0;

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



//使用物品
/*
- 0：成功
- 1：物品不存在
- 2：数量不足
- 3：不可使用
*/
int use(User *user,int item_id){
    if(!user || item_id<=0) return 3;
    
    // 获取物品信息
    ItemConfig *item = item_mgr_get_by_id(item_id);
    // 判断物品是否存在
    if (!item) return 1;

    // 查看类型，是否可用

}