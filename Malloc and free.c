#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#define MEMORY_POOL_SIZE 1024

typedef struct BlockHeader{
    size_t size;
    bool is_free;        
    struct BlockHeader *next;
}BlockHeader;
static unsigned char memory_pool[MEMORY_POOL_SIZE];
static BlockHeader *free_list=NULL;
void init_memory(){
    free_list=(BlockHeader*)memory_pool;
    free_list->size=MEMORY_POOL_SIZE -sizeof(BlockHeader);
    free_list->is_free=true;
    free_list->next=NULL;
}
void* my_malloc(size_t size){
    if(free_list==NULL){
        init_memory();
    }
    size=(size+7)& ~7;
    BlockHeader *current=free_list;
    while(current !=NULL){
        if(current->is_free && current->size>=size){
            if(current->size >= size +sizeof(BlockHeader)+8){
                BlockHeader *new_block=(BlockHeader*)((char*)current +sizeof(BlockHeader)+size);
                new_block->size=current->size -size-sizeof(BlockHeader);
                new_block->is_free=true;
                new_block->next=current->next;
                current->size=size;
                current->next=new_block;
            }
            current->is_free=false;
            return (void*)((char*)current +sizeof(BlockHeader));
        }
        current=current->next;
    }
    return NULL;
}
void my_free(void* ptr){
    if(ptr==NULL)return ;
    BlockHeader *block=(BlockHeader*)((char*)ptr -sizeof(BlockHeader));
    block->is_free=true;
    BlockHeader *current=free_list;
    while(current !=NULL){
        if(current->is_free && current->next !=NULL && current->next->is_free){
            current->size += sizeof(BlockHeader)+current->next->size;
            current->next=current->next->next;
        }else{
            current=current->next;
        }
    }
    printf("[Free]:Block at address %p is now FREE",ptr);
}
int main(void){
    
}
