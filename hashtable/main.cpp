#include <iostream>
#include "CNode.h"
#include <string>
#include <cassert>

template<class T>
class CLinkedList{
private:
    size_t _currentSize;
    size_t _size;
    CNode<T>* _root;
    CNode<T>* _last;
public:
    explicit CLinkedList(int initialSize = 0);
    ~CLinkedList();
    CNode<T> * addNode(const std::string &key, T value);
    bool addNextKey(const CNode<T> *prvek, const std::string &key);
    bool erase(const std::string & key);
    const CNode<T> * at(const std::string & key);
    const CNode<T> * at(int index);
    size_t size();
    CNode<T> * operator[](const std::string & key);
    CNode<T> * operator[](int index);
private:
};

template<class T>
CLinkedList<T>::CLinkedList(int initialSize): _size(initialSize), _currentSize(0) {
    this->_root = new CNode<T>();
    CNode<T>* temp = _root;
    for(int i = 0; i < (initialSize-1); ++i){
        temp->nextOrder = new CNode<T>();
        temp->prevOrder = temp;
        temp = temp->nextOrder;
    }
    temp->nextOrder = nullptr;
    _last = temp;
}

template<class T>
CLinkedList<T>::~CLinkedList() {
    CNode<T>* now = _root;
    CNode<T>* toBeDeleted;
    while(now != nullptr) {
        toBeDeleted = now;
        now = toBeDeleted->nextOrder;
        delete toBeDeleted;
    }
}

template<class T>
CNode<T> * CLinkedList<T>::addNode(const std::string & key,T value) {
   if(_currentSize >= _size){
       auto * temp =  _last;
       _last->nextOrder = new CNode<T>(key, value);
       _last = _last->nextOrder;
       _last->prevOrder = temp;
       _currentSize++;
       return _last;
   }
   else{
       auto temp = _root;
       while(!temp->key.empty()){
           temp = temp->nextOrder;
       }
       temp->key=key;
       _currentSize++;
       return temp;
   }

}

template<class T>
size_t CLinkedList<T>::size() {
    return _currentSize;
}

// returns nullptr if key not found
template<class T>
const CNode<T> *CLinkedList<T>::at(const std::string &key) {
    auto* temp = _root;

    while(temp != nullptr && temp->key != key)
        temp = temp->nextOrder;

    if (temp != nullptr && temp->key == key)
        return temp;
    return nullptr;
}
// returns nullptr if index not found
template<class T>
const CNode<T> *CLinkedList<T>::at(int index) {
    auto * temp = _root;
    if(temp == nullptr)
        return temp;
    for(int i = 0; i<index; i++)
    {
        if(temp->nextOrder != nullptr)
            temp = temp->nextOrder;
        else break;
    }
    return temp;
}

template<class T>
CNode<T> *CLinkedList<T>::operator[](const std::string &key) {
    auto* temp = _root;

    while(temp != nullptr && temp->key != key)
        temp = temp->nextOrder;

    if (temp != nullptr && temp->key == key)
        return temp;
    return nullptr;
}

template<class T>
CNode<T> *CLinkedList<T>::operator[](int index) {
    auto * temp = _root;
    if(temp == nullptr)
        return temp;
    for(int i = 0; i<index; i++)
    {
        if(temp->nextOrder != nullptr)
            temp = temp->nextOrder;
        else break;
    }
    return temp;
}

template<class T>
bool CLinkedList<T>::addNextKey(const CNode<T> *prvek, const std::string &key) {
    if (prvek == nullptr)
        return false;
    prvek->nextKey = key;
    return true;
}

template<class T>
bool CLinkedList<T>::erase(const std::string &key) {
    auto * found = this[key];
    if ( found == nullptr )
        return false;
    auto * prev = found->prevOrder;
    auto * next = found->nextOrder;
    if(prev != nullptr)
        prev->nextOrder = next;
    if(next != nullptr)
        next->prevOrder = prev;
    delete found;
    return true;
}


template <class T>
class CHashSet {
private:
    CLinkedList<T>* _table;
    size_t _size;
    CNode<T>* idLastAdded;
public:
    explicit CHashSet(int size);
    ~CHashSet();
    int hash(const std::string& key);
    bool insert(std::string key, T value);
    bool remove(std::string key);
    const CNode<T> & at(const std::string & key);
    CNode<T> & operator[](const std::string & key);
    bool isInSet(std::string key);
    bool print(std::string key);
    void printAll();
};


template<class T>
CHashSet<T>::CHashSet(int size):_size(size) {
    this->_table = new CLinkedList<T> [64];
}

template<class T>
int CHashSet<T>::hash(const std::string& key) {
    hashFunc hashFunction;
    return  hashFunction(key)% this->_size;
}

template<class T>
CHashSet<T>::~CHashSet() {
    delete [] _table;

}
// returns false if this key is already in the set
template<class T>
bool CHashSet<T>::insert(std::string key, T value) {
    size_t index = hash(key);
    auto* temp = _table[index].addNode(key,value);
    if(temp != nullptr)
        return false;
    if(idLastAdded != nullptr)
        idLastAdded->nextKey==key;
    idLastAdded = temp;
    return true;
}

template<class T>
bool CHashSet<T>::remove(std::string key) {
    size_t index = hash(key);
    if(_table[index]->erase(key))
        return true;
    return false;
}

template<class T>
const CNode<T> & CHashSet<T>::at(const std::string &key) {
    int index = hash(key);
    auto* found = _table[index][key];
    if(found == nullptr)
        throw std::out_of_range("CHashSet::at()");
    return *found;
}

template<class T>
CNode<T> & CHashSet<T>::operator[](const std::string &key) {
    int index = hash(key);
    auto* found = _table[index][key];
    if(found == nullptr)
        throw std::out_of_range("CHashSet::operator[]()");
    return *found;
}

template<class T>
bool CHashSet<T>::isInSet(std::string key) {
    int index = hash(key);
    auto* found = _table[index][key];
    if (found == nullptr)
        return false;
    return true;
}




int main(void){

    CLinkedList<int> temp (5);

    int i = 0;
    while(i<8) {
        std::cout << i << " " << temp.addNode(std::to_string(i), i) << " " << std::endl;
        i++;
    }
    auto print = temp.at(5);
    if(print != nullptr)
        std::cout << print->key << std::endl;
    print = temp.at("6");
    if(print != nullptr)
        std::cout << print->_value <<std::endl;
    print = temp.at("8");
    if(print != nullptr)
        std::cout << print->_value <<std::endl;

    CHashSet<int> setTest(64);

    assert(setTest.insert("ASDFD", 123));
    assert(!setTest.insert("ASDFD", 123));




    return EXIT_SUCCESS;
}
