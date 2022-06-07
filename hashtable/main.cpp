#include <iostream>
#include "CNode.h"
//#include "CLinkedList.h"
//#include "CHashSet.h"
#include <string>
#include <cassert>
#include <random>
#include <chrono>
#include <unordered_set>
#include <iomanip>

template<class T>
class CLinkedList{
private:
    size_t _currentSize = 0;

    CNode<T>* _root = nullptr;
    CNode<T>* _last = nullptr;
public:
    CLinkedList();
    ~CLinkedList();
    CNode<T> * addNode(const std::string &key, T value);
    bool addNextKey(const CNode<T> *prvek, const std::string &key);
    bool erase(const std::string & key);
    const CNode<T> * at(const std::string & key);
    const CNode<T> * at(int index);
    size_t size();
    CNode<T> * operator[](const std::string & key);
    CNode<T> * operator[](int index);
};


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
    auto* check = at(key);
    if (check != nullptr)
        throw std::range_error("CLinkedList::addNode key already in set");
    auto temp = _last;
    if(_currentSize == 0){
        temp = new CNode<T>(key, value);
        _root = temp;
        _last = temp;
        _currentSize++;
        return temp;
    }
    _last = _last->nextOrder = new CNode<T>(key, value);
    _last->prevOrder = temp;
    _currentSize++;
    return _last;
}

template<class T>
size_t CLinkedList<T>::size() {
    return _currentSize;
}

// returns nullptr if key not found
template<class T>
const CNode<T> * CLinkedList<T>::at(const std::string &key) {
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
    auto * found = operator[](key);
    if ( found == nullptr )
        return false;
    found->valid = false;
//    auto * prev = found->prevOrder;
//    auto * next = found->nextOrder;
//    if(prev != nullptr)
//        prev->nextOrder = next;
//    if(next != nullptr)
//        next->prevOrder = prev;
//    delete found;
    return true;
}

template<class T>
CLinkedList<T>::CLinkedList(): _root(nullptr), _last(nullptr), _currentSize(0) {}


template <class T>
class CHashSet {
private:
    CLinkedList<T>* _table = nullptr;
    size_t _size;
    CNode<T>* idLastAdded = nullptr;
    CNode<T>* firstAdded = nullptr;
public:
    explicit CHashSet(int size);
    ~CHashSet();
    int hash(const std::string &key, int modulo);
    bool insert(std::string key, T value);
    bool remove(std::string key);
    const CNode<T> & at(const std::string & key);
    CNode<T> & operator[](const std::string & key);
    bool isInSet(const std::string& key);
    bool print(std::string key);
    void printAll();

    struct Iterator {
        friend int CHashSet::hash(const std::string &key, int modulo);
//        friend CHashSet;
    private:
        using iterator_category =std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = CNode<T>;
        using pointer = CNode<T>*;
        using reference = CNode<T>&;

        pointer m_ptr;
    public:

        explicit Iterator(pointer node);
        reference operator *() const{return *m_ptr;}
        pointer operator->(){return m_ptr;}
        Iterator& operator++();
        Iterator operator++(int);
        friend bool operator ==(const Iterator& lhs,const Iterator& rhs){return rhs.m_ptr==lhs.m_ptr;}
        friend bool operator !=(const Iterator& lhs,const Iterator& rhs){return rhs.m_ptr!=lhs.m_ptr;}
        Iterator& operator =(const Iterator& rhs){ this->m_ptr = rhs.m_ptr; }
    };
    friend Iterator;
    Iterator begin() const;
    [[nodiscard]] Iterator end() const { return Iterator(nullptr);}

};

template<class T>
CHashSet<T>::CHashSet(int size):_size(size) {
    this->_table = new CLinkedList<T>[size]();
}



template<class T>
CHashSet<T>::~CHashSet() {
    delete [] _table;

}
// returns false if this key is already in the set
template<class T>
bool CHashSet<T>::insert(std::string key, T value) {
    size_t index = hash(key, _size);
    auto* temp = _table[index].addNode(key,value);
    if(temp == nullptr)
        throw std::out_of_range("CHashSet::insert");
    if(idLastAdded != nullptr){
        idLastAdded->nextKey=temp;
    }
    if(firstAdded == nullptr){
        firstAdded = temp;
    }
    idLastAdded = temp;
    return true;
}

template<class T>
bool CHashSet<T>::remove(std::string key) {
    size_t index = hash(key, _size);
    if(_table[index].erase(key))
        return true;
    return false;
}

template<class T>
const CNode<T> & CHashSet<T>::at(const std::string &key) {
    int index = hash(key, _size);
    auto* found = _table[index][key];
    if(found == nullptr)
        throw std::out_of_range("CHashSet::at()");
    return *found;
}

template<class T>
CNode<T> & CHashSet<T>::operator[](const std::string &key) {
    int index = hash(key, _size);
    auto* found = _table[index][key];
    if(found == nullptr)
        throw std::out_of_range("CHashSet::operator[]()");
    return *found;
}

template<class T>
bool CHashSet<T>::isInSet(const std::string& key) {
    int index = hash(key, _size);
    auto* found = _table[index][key];
    if (found == nullptr)
        return false;
    return true;
}

template<class T>
CHashSet<T>::Iterator::Iterator(CHashSet::Iterator::pointer node) {
    m_ptr = node;
}

/// @returns if container is empty returns nullptr.
template<class T>
typename CHashSet<T>::Iterator CHashSet<T>::begin() const {
    return Iterator(firstAdded);
}

template<class T>
int CHashSet<T>::hash(const std::string& key, int modulo) {
    hashFunc hashFunction;
    return  hashFunction(key)% modulo;
}

template<class T>
typename CHashSet<T>::Iterator &CHashSet<T>::Iterator::operator++()
{
    auto temp = m_ptr->nextKey;
    while(temp != nullptr) {
        if(temp->valid==false)
            temp=temp->nextKey;
        else{
            break;
        }
    }
    m_ptr = temp;
    return *this;
}



template<class T>
typename CHashSet<T>::Iterator  CHashSet<T>::Iterator::operator++(int)
{
    auto temp = m_ptr->nextKey;
    while(temp != nullptr) {
        if(temp->valid==false)
            temp = temp->nextKey;
        else {
            temp = temp->nextKey;
            break;
        }
    }
    m_ptr = temp;
    return temp;
}



int main(void){

    CLinkedList<int> temp;
    CHashSet<int> setTest(256);
    std::unordered_set<std::string, hashFunc> setEtalon;
    std::basic_string<char> test;
    std::random_device rds;
    std::mt19937 rng(rds());
    char znak = abs((char)rng() % 127) + 1 ;
    auto startTime = std::chrono::system_clock::now();
    auto currentTime = startTime;
    int nrValues = 100;
    int nrLookup = 10;
    int i = 0;
    while(i<3) {
        std::cout << i << " " << (temp.addNode(std::to_string(i), i))->_value << " " << std::endl;
        i++;
    }
    auto print = temp[5];
    if(print != nullptr)
        std::cout << print->key << std::endl;
    print = temp["6"];
    if(print != nullptr)
        std::cout << print->_value <<std::endl;
    print = temp["8"];
    if(print != nullptr)
        std::cout << print->_value <<std::endl;

    assert(setTest.insert("AAAA", 123));
    try{setTest.insert("AAAA", 123);}
    catch(std::range_error&){ std::cout << "error caught correctly" << std::endl; }
    assert(setTest.insert("AAAAaaa",123));
    i=0;

    while( i<nrValues)
    {
        test += znak;
        if(i%45 == 0 && test.size()>45)
        {
            test = test.substr(40,5);
        }
        try{setEtalon.emplace(test);}
        catch(std::range_error &){ i--; std::cout << "already in set: " << test << std::endl;}

        znak = abs((char)rng()) % 127 + 1;
        i++;
    }
//    std::cout << setTest.at(test).key << " " << setTest.at(test)._value << std::endl;
    currentTime = std::chrono::system_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    double seconds = (deltaTime+0.0)/1000;
    std::cout << "std::unordered_set: adding " << i << " strings with a hashed key took " << seconds << " seconds" << std::endl;
    startTime =std::chrono::system_clock::now();
    i = 0;
    while(i<nrLookup){
        setEtalon.count(test);
        i++;
    }
    currentTime = std::chrono::system_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    std::cout << "std::unordered_set: looking up " << i << " values took " << deltaTime << " milliseconds" << std::endl;


    std::cout << std::endl;

    i=0;
    startTime = std::chrono::system_clock::now();
    currentTime = startTime;
    test.clear();
    while( i<nrValues)
    {
        test += znak;
        if(i%45 == 0 && test.size()>45)
        {
            test = test.substr(40,5);
        }
        try{setTest.insert(test,i);}
        catch(std::range_error &){ i--; std::cout << "already in set: " << test << std::endl;}

        znak = (abs((char)rng()) % 127) + 1;
        i++;
    }
//    std::cout << setTest.at(test).key << " " << setTest.at(test)._value << std::endl;
    currentTime = std::chrono::system_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    seconds = (deltaTime+0.0)/1000;
    std::cout << "CHashSet: adding " << i << " strings with a hashed key took " << seconds << " seconds" << std::endl;
    startTime =std::chrono::system_clock::now();
    i = 0;
    while(i<nrLookup){
        setTest.at(test).key;
        i++;
    }
    currentTime = std::chrono::system_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    std::cout << "CHashSet: looking up " << i <<  " values took " << deltaTime << " milliseconds" << std::endl;

    // test iteratoru
    for(const auto& it: setTest){
        std::cout << it._value << std::endl;
    }
    setTest.remove(test);
    for(const auto& it: setTest){
        std::cout << it._value << std::endl;
    }


    return EXIT_SUCCESS;
}


