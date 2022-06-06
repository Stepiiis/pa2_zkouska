// comparison of various hash functions
// some speed tests
// requires C++ 11
#include <set>
#include <unordered_set>
#include <iostream>
#include <sys/time.h>
using namespace std;

//===================================================================================================
class hashInferior
{
  public:
    size_t         operator ()                             ( const string    & str ) const
    {
      unsigned int h = 0, len = str . length ();
      for ( unsigned int i = 0; i < len; i ++ ) 
        h += str[i];
      return h; 
    }
};
//===================================================================================================
class hashSimple
{
  public:
    size_t         operator ()                             ( const string    & str ) const
    {
      size_t h = 5381, len = str . length ();
      for ( size_t i = 0; i < len; i ++ ) 
        h += (h << 5 ) + str[i];
      return h; 
    }
};
//===================================================================================================
class hashLongStr
{
  public:
    size_t         operator ()                             ( const string    & str ) const
    {
      size_t h = 5381, len = str . length ();
      if ( len > 32 ) 
        len = 32;
      for ( size_t i = 0; i < len; i ++ ) 
        h += (h << 5 ) + str[i];
      return h; 
    }
};
//===================================================================================================
int                randInt                                 ( int               lo,
                                                             int               hi )
{
  return (int)(lo + 1.0 * (hi - lo) * rand () / RAND_MAX);
}                                                             
//===================================================================================================
string             randString                              ( int               lo,
                                                             int               hi )
{
  int l = randInt ( lo, hi );
  string s ( l, ' ' );
  
  for ( int i = 0; i < l; i ++ )
    s[i] = randInt ( 32, 127 ); // printable characters
  return s;  
}                                                             
//===================================================================================================
template<class T>
void               test                                    ( T              && aSet,
                                                             int               strLenMin,
                                                             int               strLenMax )
{
  for ( int j = 0; j < 5; j ++ )
  {
    for ( int i = 0; i < 300000; i ++ )
      aSet . insert ( randString ( strLenMin, strLenMax ) );
    
    for ( int i = 0; i < 1000000; i ++ )
      aSet . find ( randString ( strLenMin, strLenMax ) );
      
    for ( int i = 0; i < 100000; i ++ )
    {
      auto it = aSet . find ( randString ( strLenMin, strLenMax ) );
      if ( it != aSet . end () ) aSet . erase ( it );
    }
  }  
}
//===================================================================================================
double             timeStamp                               ( void )
{
  struct timeval tv;
  gettimeofday ( &tv, nullptr );
  return tv . tv_sec + 0.000001 * tv . tv_usec;
}
//===================================================================================================
int main ( void )
{
  cout << "Short strings:" << endl;
  
  double st = timeStamp ();
  test ( unordered_set<string,hashInferior> (), 10, 100 );
  cout << "unordered_set<hashInferior>: " << (timeStamp() - st) << endl;

  st = timeStamp ();
  test ( unordered_set<string,hashSimple> (), 10, 100 );
  cout << "unordered_set<hashSimple>: " << (timeStamp() - st) << endl;

  st = timeStamp ();
  test ( unordered_set<string, hashLongStr>(), 10, 100 );
  cout << "unordered_set<hashLongStr>: " << (timeStamp() - st) << endl;

  st = timeStamp ();
  test ( set<string>(), 10, 1000 );
  cout << "set: " << (timeStamp() - st) << endl;


  cout << "Long strings (be patient):" << endl;
  st = timeStamp ();
  test ( unordered_set<string, hashInferior>(), 10, 1000 );
  cout << "unordered_set<hashInferior>: " << (timeStamp() - st) << endl;

  st = timeStamp ();
  test ( unordered_set<string, hashSimple> (), 100, 1000 );
  cout << "unordered_set<hashSimple>: " << (timeStamp() - st) << endl;

  st = timeStamp ();
  test( unordered_set<string,hashLongStr> (), 10, 1000 );
  cout << "unordered_set<hashLongStr>: " << (timeStamp() - st) << endl;

  st = timeStamp ();
  test ( set<string>(), 10, 1000 );
  cout << "set: " << (timeStamp() - st) << endl;

  return 0;
}
