// set<int> and unordered_set<int>
// some speed tests
// requires C++ 11
#include <set>
#include <unordered_set>
#include <iostream>
#include <sys/time.h>
using namespace std;
//===================================================================================================
int                randInt                                 ( int               lo,
                                                             int               hi )
{
  return (int)(lo + 1.0 * (hi - lo) * rand () / RAND_MAX);
}                                                             
//===================================================================================================
template<class T>
void               test                                    ( T               & aSet )
{
  for ( int j = 0; j < 10; j ++ )
  {
    for ( int i = 0; i < 1000000; i ++ )
      aSet . insert ( randInt ( 0, 10000000 ) );
    
    for ( int i = 0; i < 1000000; i ++ )
      aSet . find ( randInt ( 0, 10000000 ) );
      
    for ( int i = 0; i < 1000000; i ++ )
    {
      auto it = aSet . find ( randInt ( 0, 10000000 ) );
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
  set<int>  a;
  double st = timeStamp ();
  test ( a );
  cout << "set<int>:           " << (timeStamp() - st) << endl;

  unordered_set<int>  b;
  st = timeStamp ();
  test ( b );
  cout << "unordered_set<int>: " << (timeStamp() - st) << endl;
 
  return 0;
}
