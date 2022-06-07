#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
using namespace std;

class CTrain{
public:
    //default constructor

    //default destructor

    //Add
    void Add(istringstream & is);
    //Dest
    set<string> Dest (const string & from, int maxCost);



private:

    std::map<std::string, set<string>> mutualJunctions;

};

void CTrain::Add(istringstream &is) {
    std::string nazev1;
    std::string nazev2;
    getline(is,nazev1);
    while(getline(is,nazev2)) {
        mutualJunctions[nazev2].insert(nazev1);
        mutualJunctions[nazev1].insert(nazev2);
        nazev1 = nazev2;
    }
}



set<string> CTrain::Dest(const string &from, int maxCost) {
    std::set<string> nalezene;
    nalezene.insert(from);
    std::deque<string> toExplore;
    toExplore.push_back(from);
    if (mutualJunctions.count(from) == 0) {
        return nalezene;
    }
    while(!toExplore.empty() && maxCost>0) {
        std::string current = toExplore.front();
        toExplore.pop_front();
        for (auto &it: mutualJunctions[current]) {
            toExplore.push_back(it);
            nalezene.emplace(it);
        }
        maxCost--;
    }

    return nalezene;
}



int main(){
    CTrain t0;
    istringstream iss;
    iss . clear ();
    iss . str ("Forest Hill\nNational Museum\nWhite City\nChina Town\nWembley\nNew Cross\n");
    t0 . Add ( iss );
    iss . clear ();
    iss . str ("Lady Palace\nKings Cross\nOldTown\nNew Cross\nNew Aley\n");
    t0 . Add ( iss );
    iss . clear ();
    iss . str ("ZOO\nChina Town\nKings Cross\nFantom Bridge\n");
    t0 . Add ( iss );


    set<string> s = (t0.Dest("National Museum", 1));
/*Forest Hill
  National Museum
  White City*/

    set<string> t = (t0.Dest("China Town", 0));
/*China Town*/

    set<string> u = (t0.Dest("China Town", 2));
/*China Town
  Fantom Bridge
  Kings Cross
  Lady Palace
  National Museum
  New Cross
  Old Town
  Wembley
  White City
  ZOO*/

    set<string> v = (t0.Dest("ZOO", 2));
/*China Town
  Kings Cross
  Wembley
  White City
  ZOO*/

return EXIT_SUCCESS;
}