/* main */

// open addressing

#include <cstdlib>
#include <string>

#include "hashmap.h"

using namespace std;

int main() 
{
  const int m = 17;
  int a[] = {17,13,26,34,1700,3400,10,17,60,27,56,77,14,28,1,99,72,18};
  int const na = sizeof(a)/sizeof(int);

  HashMap<int, int> hsa(m);

  cout << "Open addressing, hash table size: " << m << ", key is int type\n\n";

  try
  {
    for ( int i = 0; i < na; i++ )
    { 
      hsa.ins( a[i], i );
      hsa.print();
    }
  }
  catch (const char* txt) 
  { 
    cout << "Error: " << txt << endl << endl; 
  }
  cout << "\nincl + read test:\n";
  for ( int i = 1; i <= 10000; i++ )
    if ( hsa.incl ( i ) )
    {
      int val;
      if ( hsa.read ( i, val ) )
        cout << "key " << i << " exist in the map, val is " << val << endl;
    }
  cout << "\ndel test:\n" << endl;
  for ( int i = 0; i < na; i++ )
  {
    hsa.del(a[i]);
    hsa.print();
  }

  const char* b[] = {"karel", "josef", "petr", "pavel", "monika", "pavla", "karel", "petra"};
  const int nb = sizeof(b)/sizeof(char*);

  HashMap<string,int> hsb(m);

  cout << "Open addressing, hash table size: " << m << ", key is string type\n\n";

  for ( int i = 0; i < nb; i++ )
  {
    hsb.ins ( b[i], i );
    hsb.print ( );
  }
  cout << "\nincl + read test:\n";
  for ( int i = 0; i < nb; i++ )
    if ( hsb.incl ( b[i] ) )
    {
      int val;
      if ( hsb.read ( b[i], val ) ) 
        cout << "key " << b[i] << " exist in the map, val is " << val << endl;
    }
  cout << "\ndel test:\n" << endl;
  for ( int i = 0; i < nb; i++ )
  {
    hsb.del(b[i]);
    hsb.print();
  }

  HashMap<const char*, int> hsc(m);

  cout << "Open addressing, hash table size: " << m << ", key is const char * type\n\n";

  for (int i=0; i<nb; i++)
  {
    hsc.ins ( b[i], i );
    hsc.print ( );
  }
  cout << "\nincl + read test:\n";
  for ( int i = 0; i < nb; i++ )
    if ( hsc.incl ( b[i] ) )
    {
      int val;
      hsc.read ( b[i], val );  
      cout << "key " << i << " exist in the map, val is " << val << endl;
    }
  cout << "\ndel test:\n" << endl;
  for ( int i = 0; i < nb; i++ )
  {
    hsc.del(b[i]);
    hsc.print();
  }

  return 0;
}

