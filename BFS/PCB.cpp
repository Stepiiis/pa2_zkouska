#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <list>
#include <queue>
#include <cassert>
#include <cctype>
using namespace std;
/*
input    ::= { ws } '{' element { { ws } ',' element } { ws } '}' { ws }
element  ::= { ws } pad { ws } - { ws } pad
pad      ::= ( digit | letter ) { digit | letter }
digit    ::= '0' | '1' | ... | '9'
letter   ::= 'a' | 'b' | ... | 'z' | 'A' | 'B' | ... | 'Z'
ws       ::= ' ' | '\t' | '\r' | '\n'
*/
class CPCB {
public:
    CPCB(void) {_elements = new std::map<std::string,std::map<std::string,int>>;}

    ~CPCB(void) { delete _elements; }

    list<string> Short(const string &from, const string &to);

    friend istream &operator>>(istream &is, CPCB &pcb);

private:
    std::map<std::string,std::map<std::string,int>>* _elements;
};

istream &operator>>(istream &is, CPCB &pcb) {
    auto * tempelements = new std::map<std::string,std::map<std::string,int>>;
    char character;
    std::string pad1;
    std::string pad2;
    int braceCounter{};
    bool firstPad = true;
    bool secondPad = false;
    while(is.get(character)&&!is.eof()){
        if(std::isspace(character))
            continue;
        if(character==',' && secondPad)
        {
            if(pad1.empty()||pad2.empty()) {
                is.setstate(ios_base::failbit);
                break;
            }
            (*tempelements)[pad1][pad2]=0;
            (*tempelements)[pad2][pad1]=0;
            pad1.clear();
            pad2.clear();
            secondPad = false;
            firstPad = true;
            continue;
        }else if(!secondPad && character == ',')
            is.setstate(ios_base::failbit);
        if(character == '{') {
         if(braceCounter==0) {
             braceCounter++;
             continue;
         }
         else {
             is.setstate(ios_base::failbit);
             break;
         }
        }
        if(character == '}') {
            if(braceCounter == 1) {
                braceCounter++;
                (*tempelements)[pad1][pad2]=0;
                (*tempelements)[pad2][pad1]=0;
            }
            else
                is.setstate(ios_base::failbit);
            if(pad1.empty()||pad2.empty())
                is.setstate(ios_base::failbit);
            break;
        }
        if(character == '-') {
            firstPad = false;
            secondPad = true;
            continue;
        }
        if(!isalnum(character)) {
            is.setstate(ios_base::failbit);
            break;
        }
        if(firstPad) {
            pad1 += character;
            continue;
        }
        if(secondPad)
            pad2 += character;
    }
    if(is.failbit == 1){
        delete tempelements;
        return is;
    }
    delete pcb._elements;
    pcb._elements = tempelements;
    return is;
}

list<string> CPCB::Short(const string &from, const string &to) {
    if(_elements->count(from)==0)
        return list<string>{};
    queue<string> q;
    string target{};
    list<string> cesta;
    map<string,string> visited;
    q.push(from);
    while(!q.empty()){
        std::string current(q.front());
        q.pop();
        for(const auto & [soused, predchudce]: (*_elements)[current]){
            if(visited.count(soused)==1)
                continue;
            visited.emplace(soused,current);
            q.push(soused);

            if(soused == to )
            {
                target=to;
                break;
            }
        }
    }
    if(target.empty())
        return list<string>{};
    cesta.push_back(target);
    while(target!=from)
    {
        target = visited[target];
        cesta.push_front(target);
    }

    return cesta;
}

int main(){
    CPCB to;
    istringstream("  { 64U - 172 , 172 - ZZ , xx-ZZ,   X3 - ZZ, A24-ZZ, A73 - A24,B5-C7, A24 -B5, C7-f8 ,A73-B5} ")>>to;
    assert(to.Short("A73","f8") == (list<string>({"A73","B5","C7","f8"})));
    assert(to.Short("xx","xx") == (list<string>({"xx"})));
    assert(to.Short("X3","f8") == (list<string>({"X3","ZZ","A24","B5","C7","f8"})));
    assert(!(istringstream("  { A3-VW,R} ")>>to));
    assert(!(istringstream("  { } ")>>to));
    assert(istringstream("  { A3-VW } ")>>to);
    assert(istringstream("  { A3-VW , 172-ZZ, 23-A3} ")>>to);
    assert(to.Short("23","VW") == (list<string>({"23", "A3", "VW"})));
    assert(to.Short("xx","23") == (list<string>( {} )));
    assert(istringstream("{64U- 172\t,172 - ZZ,xx-ZZ,X3 - ZZ, A24-ZZ, A73 - A24\n,B5-C7, A24 -B5, C7-f8 ,A73-B5, A3-VW , 172-ZZ, 23-A3 } ")>>to);
    assert(to.Short("23","VW") == (list<string>({"23", "A3", "VW"})));
    assert(to.Short("X3","f8") == (list<string>({"X3","ZZ","A24","B5","C7","f8"})));
    assert(to.Short("xx","23") == (list<string>( {} )));
    return 0;
}