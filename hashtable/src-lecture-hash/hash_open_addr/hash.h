/* hash.h */

#include <cstring>
#include <string>

inline int hash(int k, int m)
{
  return k % m;
}

inline int hash(std::string s, int m)
{
  int h = 0, len = s.length();
  for ( int i = 0; i < len; i++ )
    h += s[i];
  return h % m;
}

inline int hash(const char *s, int m)
{
  unsigned int h = 5381;
  int c;
  while ((c = *s++))
    h += ( h << 5 ) + c;
  return h % m;  
}
