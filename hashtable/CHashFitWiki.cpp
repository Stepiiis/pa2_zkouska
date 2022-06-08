//
// Created by stepis on 6/8/22.
//
#include <cstdlib>
#include <string>
#include <iostream>
#include <cassert>
#include <random>
using namespace std;

struct TItem {
    TItem();
    TItem(const string& key, const string& val, TItem* nextHash=nullptr,TItem* nextOrd=nullptr, TItem * prevOrd=nullptr);
    string m_Key;
    string m_Val;
    TItem * m_NextHash;
    TItem * m_NextOrder;
    TItem * m_PrevOrder;
};

TItem::TItem(){
    m_NextHash = NULL;
    m_NextOrder = NULL;
    m_PrevOrder = NULL;
}

TItem::TItem(const string& key,const string& val, TItem* nextHash,TItem* nextOrd, TItem * prevOrd)
: m_Key(key), m_Val(val), m_NextHash(nextHash), m_NextOrder(nextOrd), m_PrevOrder(prevOrd) 
{}

class CHash {
public:
    CHash(int m);

    ~CHash();

    CHash(const CHash &src) = delete;

    CHash &operator=(const CHash &src) = delete;

    bool Ins(const string& key,const string& value);

    bool IsSet(const string& key);

    bool Del(const string& key);

    template<typename func>
    void ForEach(func f);

private:
    unsigned int hashFn(const string & str);
    TItem **m_Table;
    int     m_Size;
    TItem * m_FirstOrder;
    TItem * m_LastOrder;
};
template<typename func>
void CHash::ForEach(func f){
    TItem* temp = m_FirstOrder;
    while(temp!=nullptr){
        f(temp);
        temp = temp->m_NextOrder;
    }
}

unsigned int CHash::hashFn(const string & str){
    unsigned int hash = 5381;
    int c;
    int i = 0;
    while((c=str[i])){
        hash += (hash << 5) + hash + c;
        ++i;
    }return hash % m_Size;;
}

CHash::CHash(int m)
: m_Table(nullptr), m_Size(m), m_FirstOrder(NULL), m_LastOrder(NULL)
{
     m_Table = new TItem* [m];
    for(int i=0; i<m;i++){
        m_Table[i] = NULL;
    }
 }


CHash::~CHash(){
    TItem* temp = m_FirstOrder;
    TItem* move;
    while(temp!=nullptr)
    {
        move = temp;
        temp = temp->m_NextOrder;
        delete move;
    }
    delete [] m_Table;
}
// first Call this and the remove from link
void removeFromOrderList(TItem* current){
    auto nextOrder = current->m_NextOrder;
    auto prevOrder = current->m_PrevOrder;
    if(nextOrder == nullptr || prevOrder == nullptr){
        if(nextOrder == nullptr)
            if(prevOrder!=nullptr)
                prevOrder->m_NextOrder = nullptr;
        if(prevOrder == nullptr)
            if(nextOrder!=nullptr)
                nextOrder->m_PrevOrder = nullptr;
        return;
    }
    nextOrder->m_PrevOrder = prevOrder;
    prevOrder->m_NextOrder = nextOrder;
}
 
// provide pointer to TItem just before the link
bool removeFromLink(TItem* justBefore){
    auto prev = justBefore;
    auto current = justBefore->m_NextHash;
    if(current != nullptr)
        prev->m_NextHash = current->m_NextHash;
    else 
        prev->m_NextHash = nullptr;
    delete current;
    return true;
}

bool CHash::Ins(const string& key, const string& value){
    unsigned int index = hashFn(key);
    TItem* root = m_Table[index];

    if(root == nullptr){
        root = new TItem(key, value,root,nullptr,m_LastOrder);
        if(m_FirstOrder==nullptr)
            m_FirstOrder = root;
        if(m_LastOrder!=nullptr){
            m_LastOrder->m_NextOrder=root;
        }
        m_LastOrder = root;
        m_Table[index] = root;
        return true;
    }

    while(root->m_NextHash != nullptr){
        if(root->m_Key == key){ return false; }
        root = root->m_NextHash;
    }
    
    root->m_NextHash = new TItem(key, value,nullptr,nullptr,m_LastOrder);
 
    if(m_LastOrder!=nullptr){
        m_LastOrder->m_NextOrder=root->m_NextHash;;
    }
    m_LastOrder = root->m_NextHash;
    return true;
}

bool CHash::IsSet(const string& key){
    unsigned int index = hashFn(key);
    auto root = m_Table[index];
    if(root == nullptr)
        return false;
    while(root->m_NextHash!= nullptr){
        if(root->m_Key == key){ return true; }
        root = root->m_NextHash;
    }
    if(root->m_Key == key)
        return true;
    return false;
}


bool CHash::Del(const string& key){
    unsigned int index = hashFn(key);
    auto root = m_Table[index];
    while(root->m_NextHash!= nullptr){
        if(root->m_Key == key){ break; }
        root = root->m_NextHash;
    }
    if(root->m_NextHash == nullptr)
        return false;
    removeFromOrderList(root->m_NextHash);
    removeFromLink(root->m_NextHash);
    return true;
}

void print(TItem* it){std::cout << it->m_Val << std::endl;}

int main(){
    CHash testHash(5);

    testHash.Ins("1","1");
    testHash.Ins("2","2");
    testHash.Ins("3","3");
    testHash.Ins("4","4");
    testHash.Ins("5","5");
    assert(!testHash.IsSet("ASDF"));
    
    testHash.ForEach(print);



    CHash setTest(1024);
    std::basic_string<char> test;
    std::random_device rds;
    std::mt19937 rng(rds());
    char znak = abs((char)rng() % 127) + 1 ;
    int nrValues = 1000000;
    int nrLookup = 100;
    int i = 0;

    test.clear();
    while( i<nrValues)
    {
        test += znak;
        if(i%45 == 0 && test.size()>45)
        {
            test = test.substr(40,5);
        }
        setTest.Ins(test,std::to_string(i));

        znak = (abs((char)rng()) % 127) + 1;
        i++;
    }

    i = 0;
    while(i<nrLookup){
        if(test.size()>3)
            test.substr(0,test.size()-1);
        setTest.IsSet(test);
        i++;
    }



    return EXIT_SUCCESS;
}