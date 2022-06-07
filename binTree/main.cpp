#include <iostream>
#include <vector>
#include <cassert>

template<typename A,typename T>
class CBinTree{
    struct CNode{
        CNode* _right = nullptr;
        CNode* _left = nullptr;
        A _key;
        std::vector<std::pair<A,T>> _value;
    };
public:
    CBinTree() = default;
    ~CBinTree();
    bool add(A key,T value);
    bool isInTree(A key);
    T& find(A key)const;
    bool remove(A key);
    A getSmallest() const;
    A getBiggest() const;
private:
    CNode* _root = nullptr;
    void removeNode(CNode* ptr);
};



template<typename A, typename T>
bool CBinTree<A, T>::add(A key, T value) {
    auto temp = _root;
    auto parent = _root;
    while(temp!=nullptr)
    {
        parent = temp;
        if(!(key<temp->_key)&&!(temp->_key<key))
        {
            temp->_value.emplace_back(key, value);
            return false;
        }
        if(key<temp->_key)
            temp=temp->_left;
        else if(temp->_key<key)
            temp=temp->_right;
    }

    temp = new CBinTree::CNode;
    if(parent != nullptr) {
        if (parent->_key < key)
            parent->_right = temp;
        else {
            parent->_left = temp;
        }
    }
    if(_root == nullptr) _root = temp;
    temp->_key = key;
    temp->_value.emplace_back(key, value);

    return true;
}

template<typename A, typename T>
T &CBinTree<A, T>::find(A key) const {
    auto temp = _root;
    while(temp!=nullptr)
    {
        if(!(key<temp->_key) && !(temp->_key<key)){
            for(auto & it: temp->_value)
            {
                if(it.first == key)
                    return it.second;
            }
        }
        else if(key<temp->_key)
            temp=temp->_left;
        else if(temp->_key<key)
            temp=temp->_right;
    }
    throw std::range_error("Key was not in tree");
}

template<typename A, typename T>
bool CBinTree<A, T>::isInTree(A key) {
    auto temp = _root;
    while(temp!=nullptr)
    {
        if(!(key<temp->_key) && !(temp->_key<key))
            return true;
        if(key<temp->_key)
            temp=temp->_left;
        else if(temp->_key<key)
            temp=temp->_right;
    }
    return false;
}

template<typename A, typename T>
bool CBinTree<A, T>::remove(A key) {
    CNode * temp = _root;
    CNode* parent = _root;
    while((temp)!=nullptr)
    {
        if(!(key<temp->_key) && !(temp->_key<key))
            break;
        else if(key<(temp)->_key) {
            parent = temp;
            (temp) = (temp)->_left;
        }
        else if((temp)->_key<key) {
            parent = temp;
            (temp) = (temp)->_right;
        }
    }

    if( temp != nullptr)  {
        auto toRemove = temp;
        auto move = temp;
        if (move->_left != nullptr && move->_right != nullptr) {
            move = move->_left;
            while (move->_right != nullptr) {
                parent = move;
                move = move->_right;
            }
            toRemove->_value = move->_value;
            toRemove->_key = move->_key;
            CNode *toDelete = move;
            if(parent!=toRemove) parent->_right = toDelete->_left;
            else parent->_left = nullptr;
            delete toDelete;
            return true;
        }else if(move->_left == nullptr && move->_right == nullptr){
            if(move->_key<parent->_key)
                parent->_left = nullptr;
            else
                parent->_right = nullptr;
            delete move;
        }
        else if( move->_left == nullptr) {
            if(move->_key<parent->_key)
                parent->_left = move->_right;
            else
                parent->_right = move->_right;
            toRemove->_key = move->_key;
            toRemove->_value = move->_value;
            delete move;
            return true;
        }else if( move->_right == nullptr){
            if(move->_key<parent->_key)
                parent->_left = move->_left;
            else
                parent->_right = move->_left;
            toRemove->_key = move->_key;
            toRemove->_value = move->_value;
            delete move;
            return true;
        }
    }
    return false; // key was not in tree, could not remove
}

template<typename A, typename T>
void CBinTree<A, T>::removeNode(CBinTree::CNode *ptr) {
if(ptr->_left != nullptr)
    removeNode(ptr->_left);
if(ptr->_right != nullptr)
    removeNode(ptr->_right);
delete ptr;
}

template<typename A, typename T>
CBinTree<A, T>::~CBinTree() {
    removeNode(_root);
}

template<typename A, typename T>
A CBinTree<A, T>::getSmallest() const {
    auto temp = _root;
    while(temp->_left!= nullptr)
        temp = temp->_left;
    if(temp!= nullptr){
        return temp->_key;
    }
    return A{};
}
// returns empty initialized value if empty
template<typename A, typename T>
A CBinTree<A, T>::getBiggest() const {
    auto temp = _root;
    while(temp->_right!= nullptr)
        temp = temp->_right;
    if(temp != nullptr){
        return temp->_key;
    }
    return A{};

}


int main(){
    CBinTree<int, int> tree;
    tree.add(8,1);
    tree.add(4,1);
    tree.add(12,1);
    tree.add(18,1);
    tree.add(3,1);
    tree.add(5,1);
    tree.add(6,1);
    tree.add(10,1);
    tree.add(11,1234);
    assert(tree.isInTree(11));
    assert(!tree.isInTree(16));
    std:: cout << tree.find(11)<<std::endl;
    std:: cout << tree.remove(11) << std::endl;
    std:: cout << tree.getSmallest() << std::endl;
    std:: cout << tree.getBiggest() << std::endl;
    return EXIT_SUCCESS;
}
