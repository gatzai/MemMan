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

char BUFF[MAXBUFFER];

int my_init();
int my_deinit();
char* my_new(size_t _n);
int my_print();


int main(){
    my_init();
    int k = 1;
    my_print();
    while(true){
        if(k==0)break;
        cin >> k;
        my_new(k);
        my_print();
    }
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
    p->offset = 0;
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
char* my_new(size_t _n){
    char* ret = NULL;

    if(mbl == NULL)return NULL;
    if(_n > MAXBUFFER)return NULL;
    mblnode* p = mbl;

    //查看是否有其余可用剩余块
    for(p=mbl; p->right != mbl;p = p->right){
        if(p->isused || p->size<_n)continue;
        else break;
    }

    if(p == NULL || p->size < _n || p->isused)return NULL; //需求空间超出内存，或内存已满

    if(p->size > _n){
        //新块
        mblnode* newmbl = new mblnode;
        p->left->right = newmbl;        //更新前面的指针
        newmbl->size = _n;
        newmbl->isused = true;
        newmbl->left = p->left;
        newmbl->right = p;
        newmbl->offset = p->offset;

        //剩余块
        p->size-=_n;
        p->offset +=_n;
        p->left = newmbl;

        //指定分完块后的mbl
        mbl = p;
        ret = BUFF + newmbl->offset; //有何用？
    }
    else if(p->size == _n){
        p->isused = true;
        
        mbl = p;
        ret = BUFF + p->offset;
    }
    return ret;
}

int my_print(){
    if(NULL == mbl)return -1;
    mblnode* p = new mblnode;
    p = mbl;
    do{
        cout << "offset:"<< p->offset << endl;
        cout << "size:  "<< p->size << endl;
        cout << "isused:"<< p->isused<< endl;
        cout << "--------------------" << endl;
        p=p->right;
    }while(mbl != p);
    cout << endl;
    return 0;
}