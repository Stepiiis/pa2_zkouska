/* hashMap.h */

// open addressing

#ifndef PA2_HASHMAP_H
#define PA2_HASHMAP_H

#include <iostream>
#include <iomanip>

#include "hash.h"

template <class Key, class Val> // ***************
class HashMap
{
    struct HTab 
    { 
      Key key; 
      Val val; 
    };
    HTab *hTab; // hash table, dyn. allocated
    int m; // hash table size
    int n; // usage (elements used)
    Key none; // empty element (special value)
    int search ( const Key & ) const;
  public:
    HashMap ( int );
    ~HashMap ( );
    void ins ( const Key &, const Val & );
    void del ( const Key & );
    bool incl ( const Key & ) const;
    bool read ( const Key &, Val & ) const;
    void print ( ) const;
};


template <class Key, class Val> // ***************
HashMap<Key, Val>::HashMap ( int size )
  : m ( size ), 
    n ( 0 ), 
    none ( Key ( ) )
{
  // none = Key ( ) - what is default for int / string?
  hTab = new HTab[m];
  for ( int i = 0; i < m; i++ )
    hTab[i].key = none;
}

template <class Key, class Val> // ***************
HashMap<Key, Val>::~HashMap ( ) 
{ 
  delete [] hTab; 
}

template <class Key, class Val> // ***************
int HashMap<Key, Val>::search ( const Key & k ) const
{
  int i = hash ( k, m );
  while ( hTab[i].key != none )
  {
    if ( hTab[i].key == k ) 
      return i;
    if ( ++i == m ) 
      i = 0; // i = (i+1) % m;
  }
  return i;
}

template <class Key, class Val> // ***************
bool HashMap<Key, Val>::incl ( const Key & k ) const
{
  int i = search ( k );
  if ( hTab[i].key == k ) 
    return true;
  return false;
}

template <class Key, class Val> // ***************
void HashMap<Key, Val>::ins( const Key & k, const Val & v )
{
  std::cout << "insert pair <" << k << ", " << v << '>';
  int i = search ( k );
  if ( hTab[i].key == none )
  {
    int j = hash ( k, m );
    std::cout << " hash code " << j;
    if ( j != i ) 
      std::cout << " - collision " << i;
    std::cout << std::endl;
    // at least one free element in hTab
    if ( ++n == m ) 
    { 
      n--; 
      throw "Ins: Hash table full!"; 
    }
    hTab[i].key = k; 
    hTab[i].val = v;
  }
  else
  {
    std::cout << " - key already exists" << std::endl;
    return;
  }
}

template <class Key, class Val> // ***************
void HashMap<Key, Val>::del ( const Key & k )
{
  int i = hash ( k, m );
  std::cout << "delete key " << k << " hash code " << i << std::endl;
  i = search ( k );
  if ( hTab[i].key == none ) 
    return;
  n--;
  hTab[i].key = none;
  if ( ++i == m ) 
    i = 0; // i = (i+1) % m;
  // erase & insert again!!!
  while ( hTab[i].key != none )
  {
    Key l = hTab[i].key;
    Val v = hTab[i].val;
    n--; 
    hTab[i].key = none; 
    ins ( l, v );
    if ( ++i == m ) 
      i = 0; // i = (i+1) % m;
  }
}

template <class Key, class Val> // ***************
bool HashMap<Key, Val>::read ( const Key & k, Val & v ) const
{
  int i = search ( k );
  if ( hTab[i].key != k ) 
    return false;
  v = hTab[i].val;
  return true;
}
template <class Key, class Val> // ***************
void HashMap<Key, Val>::print ( ) const
{
  std::cout << "n=" << n << " usage ratio " <<
  std::setprecision(2) << (float)n / (m - 1) << std::endl;
  for ( int i = 0; i < m; i++)
  {
    if ( hTab[i].key != none ) 
      std::cout << hTab[i].key;
    else 
      std::cout << "nn";
    std::cout << ' ';
  }
  std::cout << std::endl << std::endl;
}

#endif // PA2_HASHMAP_H
