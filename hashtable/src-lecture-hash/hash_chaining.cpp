// CHashSet - an implementation of hash table:
//   - fixed table size,
//   - collisions: chaining
//   - custom hash function
#include <unordered_set>
#include <iostream>
using namespace std;

template <class Key, class Hash>
class CHashSet
{
  public:        
    //---------------------------------------------------------------------------------------------
                   CHashSet                                ( int               hashSize,
                                                             const Hash      & hashFn )
      : m_HashTable ( new CLinkList [ hashSize ] ), 
        m_HashFunc ( hashFn ), 
        m_Size ( hashSize )
    {
    }
    //---------------------------------------------------------------------------------------------
                  ~CHashSet                                ( void )
    {
      delete [] m_HashTable;
    } 
    //---------------------------------------------------------------------------------------------
    void           insert                                  ( const Key       & k )
    {
      size_t idx = m_HashFunc ( k, m_Size );
      m_HashTable[idx] . add ( k );
    }
    //---------------------------------------------------------------------------------------------
    void           erase                                   ( const Key       & k )
    {
      size_t idx = m_HashFunc ( k, m_Size );
      m_HashTable[idx] . del ( k );
    }
    //---------------------------------------------------------------------------------------------
    bool           exists                                  ( const Key       & k )
    {
      size_t idx = m_HashFunc ( k, m_Size );
      return m_HashTable[idx] . exists ( k );
    }
    //---------------------------------------------------------------------------------------------
  private:
    //=============================================================================================
    struct TElem
    {
       
                   TElem                                   ( const Key       & k,
                                                             TElem           * n )
        : m_Key ( k ), 
          m_Next ( n ) 
      { 
      }
      Key            m_Key;
      TElem        * m_Next;
    };
    //=============================================================================================
    class CLinkList 
    {
      public:
        //-----------------------------------------------------------------------------------------
                   CLinkList                               ( void )
          : m_Head ( nullptr )
        {
        }
        //-----------------------------------------------------------------------------------------
                  ~CLinkList                               ( void )
        {
          while ( m_Head )
          {
            TElem * tmp = m_Head;
            m_Head = m_Head -> m_Next;
            delete tmp;
          }
        }
        //-----------------------------------------------------------------------------------------
        bool       isEmpty                                 ( void ) const
        {
          return m_Head == nullptr;
        }
        //-----------------------------------------------------------------------------------------
        void       add                                     ( const Key       & k )
        {
          if ( ! exists ( k ) ) 
            m_Head = new TElem ( k, m_Head );
        }
        //-----------------------------------------------------------------------------------------
        void       del                                     ( const Key       & k )
        {
          for ( TElem ** tmp = &m_Head; *tmp; tmp = &(*tmp )-> m_Next )
          {
            TElem * x = *tmp;
            if ( x -> m_Key == k )
            {
              *tmp = x -> m_Next;
              delete x;
              break;
            }
          }
        }
        //-----------------------------------------------------------------------------------------
        bool       exists                                  ( const Key       & k ) const
        {
          for ( TElem * tmp = m_Head; tmp; tmp = tmp -> m_Next )
            if ( tmp -> m_Key == k )
              return true;
          return false;
        }
        //-----------------------------------------------------------------------------------------
      private:               
        //-----------------------------------------------------------------------------------------
        TElem    * m_Head;
    };
    //============ =================================================================================
    CLinkList    * m_HashTable;
    Hash           m_HashFunc;
    size_t         m_Size;
};
//=================================================================================================
size_t             hashIntegers                            ( int               key,
                                                             size_t            m )
{
  return ((size_t) key ) % m;
}
//=================================================================================================
int                randInt                                 ( int               lo,
                                                             int               hi )
{
  return (int)(lo + 1.0 * (hi - lo) * rand () / RAND_MAX);
}                                                             
//=================================================================================================
int                main                                    ( void )
{
  unordered_set<int>  reference;
  CHashSet<int, size_t (*)(int,size_t)> aSet ( 100000, hashIntegers );
  int errors = 0;

  // a simple regression: compare our implementation and reference from STL

  for ( int j = 0; j < 10; j ++ )
  {
    for ( int i = 0; i < 1000000; i ++ )
    {
      int x = randInt ( 0, 10000000 );
      aSet . insert ( x );
      reference . insert ( x );
    }  
    
    for ( int i = 0; i < 10000000; i ++ )
      if ( aSet . exists ( i ) != (reference . count ( i ) == 1 ) ) 
        errors ++;
    for ( int i = 0; i < 1000000; i ++ )
    {
      int x = randInt ( 0, 10000000 );

      aSet . erase ( x );

      auto it = reference . find ( x );
      if ( it != reference . end () ) reference . erase ( it );
    }
    
    for ( int i = 0; i < 10000000; i ++ )
      if ( aSet . exists ( i ) != (reference . count ( i ) == 1) ) 
        errors ++;
  }  

  cout << "Errors: " << errors << endl;
  return 0;
}
