#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "item_sv.h"
#include "item_mgr.h"



int add(User *user, int item_id, int* count){
    //检查输入数据
    if(!user || item_id<=0 || *count <=0) return -1;
    //获取物品结构体信息
    ItemConfig * items = item_mgr_get_by_id(item_id);
    // 数量 每次添加    是否要判断 items->max_stack  不需要
    if(!items) return -1;
    
    //根据max_stack来区分 1 和 99的类型

    // 如果max_stack是1的情况
    if(items->max_stack == ONE){
        if(user->inventory_count+*count > ITEM_MAX) return -1;
        for(int i=0;i<*count;i++){
            user->inventory[user->inventory_count][0] = item_id;
            user->inventory[user->inventory_count][1] = ONE;
            user->inventory_count ++;
        }
        return 0;
    } else{
        // 99的情况  如果背包满了， 格子还没满的情况
        int tmp_number = (*count + 99 - 1) / 99;
        int num_count = *count;
        //添加数据 先找到是否有相同的物品，并且相同的物品容量是否满了
        int index = find_inventory_id(user,item_id);

        //当前添加的物品存在，并且添加的数量可以和存在的物品数量合并
        if(index >=0 && user->inventory[index][1] !=99){
            int tmp_inventory_count = user->inventory[index][1];
            // 并且没有超出最大容量
            if(tmp_inventory_count + *count <=99){
                user->inventory[index][1] += num_count;
                *count =0;
                return 0;
            } else{
                // 满了 *count  - (99 -user->inventory[index][1])
                *count = *count  - (99 -user->inventory[index][1]);
                user->inventory[index][1] = 99;
                // 减去满的数量
                int tmp_number_1 = (*count + 99 - 1) / 99;
                int num_count1 = *count;

                for(int i =0;i<tmp_number_1 && num_count1 >0;i++){
                    //  背包要满的情况 应该让它先满
                    if(user->inventory_count >=ITEM_MAX) {
                        *count = num_count1;
                        return -1;
                    }
                    user->inventory[user->inventory_count][0] = item_id;
                    user->inventory[user->inventory_count][1] = num_count1 > 99 ?99:num_count1;
                    num_count1-=99;
                    user->inventory_count ++;
                }
            *count = num_count1;
            return 0;
            }

        }else{
            for(int i =0;i<tmp_number && num_count >0;i++){
            //  背包要满的情况
            if(user->inventory_count >= ITEM_MAX) {
                *count = num_count;
                return -1;
            }
            user->inventory[user->inventory_count][0] = item_id;
            user->inventory[user->inventory_count][1] = num_count > 99 ?99:num_count;
            num_count-=99;
            user->inventory_count ++;
        }
        *count = num_count;
        return 0;
        }
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
            if(user->inventory[i][0] == item_id){
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
                if(user->inventory[i][0] == item_id && user->inventory[i][1] !=0){
                    user->inventory[i][1] = 0;
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
            if (user->inventory[i][0] == item_id) {
                total_have += user->inventory[i][1];
            }
        }
if (total_have < count) {
    printf("没有那么多数量\n");
    return -1;
    }   else{
            // 查找 item_id 移除
            for(int i=user->inventory_count-1;i>=0 && count>0;i--){
                if(user->inventory[i][0] == item_id && user->inventory[i][1] !=0){
                    int deduct = (user->inventory[i][1] <= count) 
                                ? user->inventory[i][1]   // 这格全扣
                                : count;                  // 只扣需要的
                    user->inventory[i][1] -= deduct;
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
            ItemConfig *tmp = item_mgr_get_by_id(user->inventory[i][0]);
            ItemConfig *tmp_1 = item_mgr_get_by_id(user->inventory[j][0]);
            // 找出类型最小的那个
            if(tmp->type < tmp_1->type){
                int tmp_num1 = user->inventory[i][0];
                int tmp_num2 = user->inventory[i][1];
                user->inventory[i][0] = user->inventory[j][0];
                user->inventory[i][1] = user->inventory[j][1];
                user->inventory[j][0] = tmp_num1;
                user->inventory[j][1] = tmp_num2;

            }
            // 类型相同 按照装备品质从高到低排
             else if (tmp->type == tmp_1->type &&(tmp->type == ITEM_TYPE_EQUIPMENT) && tmp->quality > tmp_1->quality){
                int tmp_num1 = user->inventory[i][0];
                int tmp_num2 = user->inventory[i][1];
                user->inventory[i][0] = user->inventory[j][0];
                user->inventory[i][1] = user->inventory[j][1];
                user->inventory[j][0] = tmp_num1;
                user->inventory[j][1] = tmp_num2;
            // 类型相同 按照药品效果类别从高到低排
            }else if(tmp->type == tmp_1->type &&(tmp->type == ITEM_TYPE_CONSUMABLE ) && tmp->use_effect_type > tmp_1->use_effect_type){
                int tmp_num1 = user->inventory[i][0];
                int tmp_num2 = user->inventory[i][1];
                user->inventory[i][0] = user->inventory[j][0];
                user->inventory[i][1] = user->inventory[j][1];
                user->inventory[j][0] = tmp_num1;
                user->inventory[j][1] = tmp_num2;
                //按照 id从低到高排
            }else if(tmp->type == tmp_1->type && tmp->id < tmp_1->id){
                int tmp_num1 = user->inventory[i][0];
                int tmp_num2 = user->inventory[i][1];
                user->inventory[i][0] = user->inventory[j][0];
                user->inventory[i][1] = user->inventory[j][1];
                user->inventory[j][0] = tmp_num1;
                user->inventory[j][1] = tmp_num2;
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

    // 查看类型，是否可用 这里不能使用装备，装备逻辑在其他处
    if(item->type !=ITEM_TYPE_CONSUMABLE){
        return 3;
    }else{
        // 查看背包是否存在该物品已经数量
        int status_tmp = find_inventory_id(user,item_id);
        if(status_tmp <0) return 1;
        
        user->inventory[status_tmp][1] -= 1;
        if(user->inventory[status_tmp][1]<0)
        {
            sort(user);//排序一下
        }
    }
    

}

//从后往前
int find_inventory_id(User *user,int inventory_id){
    for(int i=user->inventory_count-1;i>=0;i--){
        if(user->inventory[i][0] == inventory_id && user->inventory[i][1]>0) return i;
    }
    return -1;
}


ItemConfig ** list_item(User *user){
    ItemConfig **item = calloc(user->inventory_count,sizeof(ItemConfig*));

    for(int i=0;i<user->inventory_count;i++){
        item[i] = item_mgr_get_by_id(user->inventory[i][0]);
        if(!item[i]) continue;
    }
    return item;
}


// 数据要传入到pyqt6中，不需要打印
void show(ItemConfig **item,int len){
    // for(int i=0)
    // for(int i=0;i<len;i++){
    //     printf("%s")
    // }
}