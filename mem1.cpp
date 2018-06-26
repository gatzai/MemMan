/*
*字符内存管理
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
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

int     my_init();
int     my_deinit();
char*   my_new(size_t _n);
int     my_del(char* _p);
void    my_merge_nil(mblnode* _p);      //左右都满
void    my_merge_right(mblnode* _p);
void    my_merge_left(mblnode* _p);
void    my_merge_both(mblnode* _p);
int     my_print();


int main(){
    my_init();
    int k = 1;
    my_print();
    while(true){
        if(k==0)break;

        //先申请，显示，再释放
        cin >> k;
        char* p1 = my_new(k);
        if(p1 != NULL){
            for(int i=0; i<k; ++i){
                p1[i]='h';
            }
        }
        my_print();
        my_del(p1);

        //第二次申请，显示，不释放
        cin >> k;
        char* p2 = my_new(k);
        if(p2 != NULL){
            for(int i=0; i<k; ++i){
                p2[i]='h';
            }
        }
        my_print();
    }
    return 0;
}

//---------------------初始化
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

//-------------------检查初始化之前是否有分块
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

//--------------------------申请空间
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
        //返回值设为offset在BUFF上的指针
        ret = BUFF + newmbl->offset;
    }
    else if(p->size == _n){
        p->isused = true;
        
        mbl = p;
        ret = BUFF + p->offset;
    }
    return ret;
}

//------------------------释放空间
int my_del(char* _p){
    if(_p == NULL)return -1;
    int ret = -1;
    mblnode* p = NULL;
    mblnode* pcur = NULL;

    p=mbl;
    do{
        if((p->offset + BUFF) == _p){
            pcur = p;
            break;
        }
        p=p->right;
    }while(p != mbl);

    if(NULL == pcur)return -1;

    //合并策略，还要看左右offset为0的情况
    if(pcur->left->isused && pcur->right->isused){
        //左右都满
        my_merge_nil(pcur);
        ret = 0;
    }else if(!pcur->left->isused && pcur->right->isused){
        //左空，右满
        if(pcur->offset != 0)
            my_merge_left(pcur);
        else 
            my_merge_nil(pcur);
        ret = 0; 
    }else if(pcur->left->isused && !pcur->right->isused){
        //左满，右空
        if(pcur->right->offset != 0)
            my_merge_right(pcur);
        else
            my_merge_nil(pcur);
        ret = 0;
    }else if(!pcur->left->isused && !pcur->right->isused){
        //左右都空 
        if(pcur->right->offset == 0)
            my_merge_left(pcur);
        else if(pcur->offset == 0)
            my_merge_right(pcur);
        else 
            my_merge_both(pcur);
        ret = 0;
    }

    return ret;
}

void my_merge_nil(mblnode* _p){
    _p->isused = false;
    if(_p->size > mbl->size)
        mbl = _p;
}

void my_merge_left(mblnode* _p){
    _p->left->right = _p->right;
    _p->right->left = _p->left;
    _p->left->size += _p->size;
    
    if(mbl->size < _p->left->size)      //这一步的意义？
        mbl = _p->left;

    delete _p;
    _p = NULL;
}

void my_merge_right(mblnode* _p){
    _p->left->right = _p->right;
    _p->right->left = _p->left;
    _p->right->offset = _p->offset;
    _p->right->size += _p->size;
    
    if(mbl->size < _p->right->size)
        mbl = _p->right;

    delete _p;
    _p = NULL;
}

void my_merge_both(mblnode* _p){
    _p->left->right = _p->right->right;
    _p->right->right->left = _p->left;
    _p->left->size += _p->size +_p->right->size;
    
    if(mbl->size < _p->left->size)
        mbl = _p->left;

    delete _p;
    delete _p->right;
    _p->right = NULL;
    _p = NULL;
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
    for(int i=0;i<strlen(BUFF); ++i){
        cout << BUFF[i];
    }
    cout << endl;
    return 0;
}