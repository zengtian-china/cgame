#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "item.h"


//物品详情初始化
items *newitems(){
    items *item = calloc(1,sizeof(items));
    if (item ==NULL) return NULL;
    return item;
}

//物品详情列表初始化
itemList * newItemsList(){
    itemList *list = calloc(1,sizeof(*list));
    return list;
}
//角色物品详情列表初始化
myItemList* newMyItemList(){
    myItemList *mylist = calloc(1,sizeof(myItemList));
    if (mylist == NULL) return NULL;
    return mylist;
}

//物品详情列表获取
itemList* createItemsList(){
    //创建物品详情列表
    itemList *list = newItemsList();
    FILE *fp = fopen("./data/item.txt","r");
    if(fp == NULL){
        perror("fopen item.txt failed\n");
        return list;
    }
    int i =0;
  while (i < MAX_ITEMS && fscanf(fp, "%d|%[^|]|%[^|]|%d", 
            &list->array[i].id,
            list->array[i].item,
            list->array[i].effect,
            &list->array[i].price) == 4) 
    {
        // 只有读取成功才增加计数
        list->size += 1;
        i++;
    }
    fclose(fp);
    // printf("list->size = %d\n",list->size);
    return list;

}
void show_itemList(itemList *list){
        if (list == NULL || list->size == 0) {
        printf("物品列表为空。\n");
        return;
    }
    printf("当前商城物品件数%d\n",list->size);
    // 打印表头
    printf("+------+----------------------+----------------------+----------+\n");

    printf("%*s%s\n", 25, "", "商城物品详情表");
    // 打印表格顶部边框
    printf("+------+----------------------+----------------------+----------+\n");
    // 打印表头
    printf("| %-4s | %-20s | %-20s | %-8s |\n", "ID", "物品", "效果", "价格");
    // 打印表头与数据之间的分隔线
    printf("+------+----------------------+----------------------+----------+\n");

    // 遍历列表并打印每一行
    for (int i = 0; i < list->size; i++) {
        // 使用固定的字段宽度来对齐各列
        // %-10d 表示左对齐，宽度为10的整数
        // %-20s 表示左对齐，宽度为20的字符串
        printf("| %-4d | %-21s | %-22s | %-6d |\n",
               list->array[i].id,
               list->array[i].item,
               list->array[i].effect,
               list->array[i].price);
    }

    // 打印表格底部边框
    printf("+------+----------------------+----------------------+----------+\n");
    printf("\n");

}
//根据物品id来查找对于的物品详情数据
items * find_items(int id,itemList *list){
    items *item = newitems();
    int found;
    for(int i=0;i<list->size;i++){
        if(list->array[i].id == id){
            item->id = list->array[i].id;
            item->price = list->array[i].price;
            strcpy(item->item,list->array[i].item);
            strcpy(item->effect,list->array[i].effect);
            found=1;
            break;
        }
    }
    if (!found) { free(item); return NULL; }
    return item;
}

//角色物品详情列表获取
myItemList* getItemsList(User *user){
    myItemList* mylist = newMyItemList();
    itemList *list = createItemsList();
    for(int i=0;i<user->invertory_size;i++){
        if (user->invertory[i].item_id== 0) break;
        mylist->array[i].id = user->invertory[i].item_id;
        mylist->array[i].number = user->invertory[i].item_count;
        items * it= find_items(user->invertory[i].item_id,list);
        if (it == NULL) continue;
        mylist->size +=1;
        strcpy(mylist->array[i].item,it->item);
        strcpy(mylist->array[i].effect,it->effect);
        free(it);
    }
    free(list);

    return mylist;
}

//背包展示
void printItemsList(myItemList *mylist) {
    if (mylist == NULL || mylist->size == 0) {
        printf("物品列表为空。\n");
        return;
    }

    // 打印表格顶部边框
    printf("+------+----------------------+----------------------+----------+\n");
    // 打印表头
    printf("%*s%s\n", 25, "", "用户物品详情表");
    printf("+------+----------------------+----------------------+----------+\n");
    // 打印表头
    printf("| %-4s | %-20s | %-20s | %-8s |\n", "ID", "物品", "效果", "数量");
    // 打印表头与数据之间的分隔线
    printf("+------+----------------------+----------------------+----------+\n");

    // 遍历列表并打印每一行
    for (int i = 0; i < mylist->size; i++) {
        // 使用固定的字段宽度来对齐各列
        // %-10d 表示左对齐，宽度为10的整数
        // %-20s 表示左对齐，宽度为20的字符串
        printf("| %-4d | %-21s | %-21s | %-8d |\n",
               mylist->array[i].id,
               mylist->array[i].item,
               mylist->array[i].effect,
               mylist->array[i].number);
    }

    // 打印表格底部边框
    printf("+------+----------------------+----------------------+----------+\n");
    printf("\n");

}


// 物品展示
void show_my(User *user){
    myItemList* mylist =  getItemsList(user);
    printItemsList(mylist);
    free(mylist);
}

//物品排序算法
void sort_item(User * user){
    int size = user->invertory_size;
    if (size <= 0) return;
    for(int i = 0; i< size;i++){
        // 把编号小的往前放 
        for(int j =0;j<size-i-1;j++){
            if(user->invertory[j].item_id >user->invertory[j+1].item_id){
                Item tmp = user->invertory[j]; 
                user->invertory[j] = user->invertory[j + 1];
                user->invertory[j + 1] = tmp;
            }
        }
    }

        for (int i = 0; i < size; i++) {
        // 如果当前物品数量已经是0，跳过
            if (user->invertory[i].item_count == 0) continue;

            // 向后寻找相同的 ID，把数量累加到 i 上
            for (int j = i + 1; j < size; j++) {
                if (user->invertory[i].item_id == user->invertory[j].item_id) {
                    // 累加数量
                    //如果 累计的数量超过了99
                    int tmp_x = user->invertory[i].item_count;
                    int tmp_y = user->invertory[j].item_count;
                    if(user->invertory[i].item_count + user->invertory[j].item_count >99){

                        user->invertory[i].item_count = 99;
                        // 将值赋给下一位
                        user->invertory[i+1].item_count = tmp_x+tmp_y-99;
                        break;
                    }else{
                        user->invertory[i].item_count = tmp_x + tmp_y;
                        user->invertory[j].item_count = 0;
                    }

                }
        }
        
    }
        int slow = 0;
        for (int fast = 0; fast < size; fast++) {
            if (user->invertory[fast].item_count > 0) {
                user->invertory[slow] = user->invertory[fast];
                slow++;
            }
        }
        for (int i = slow; i < size; i++) {
            user->invertory[i].item_id = 0;
            user->invertory[i].item_count = 0;
            user->invertory_size -=1;
        }

}