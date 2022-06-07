#include <iostream>
#include "CNode.h"
#include "CLinkedList.h"
#include "CHashSet.h"
#include <string>
#include <cassert>
#include <random>
#include <chrono>
#include <unordered_set>


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
    int nrValues = 100'000;
    int nrLookup = 1000;
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





    return EXIT_SUCCESS;
}
