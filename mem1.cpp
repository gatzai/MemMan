#include <iostream>
#define MAXBUFFER 12
#define MINBUFFER 2

using namespace std;

struct mblnode{
    mblnode* left;
    mblnode* right;
    bool isused;
    size_t offset;
    size_t size;
};

mblnode* mbl = NULL;

int my_init();
int my_deinit();
char* my_new(size_t _n);

int main(){
    my_init();
    return 0;
}

//初始化
int my_init(){
    if(mbl!=NULL){
        my_deinit();
    }
    mblnode* p = new mblnode;
    p->left = p->right = p;
    p->isused = false;
    p->size = MAXBUFFER;
    mbl = p;
    return 0;
}

//检查初始化之前是否有分块
int my_deinit(){
    if(mbl == NULL){
        return 0;
    }

    mblnode* q = NULL;
    mblnode* p = mbl;
    //按照右指针环路检查
    do{
        q = p;
        p = p->right;
        delete q;
        q = NULL;
    }while(p == mbl);
    return 0;
}

//申请空间
char* my_init(size_t _n){

}