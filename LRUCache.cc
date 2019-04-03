struct ListNodeLRU
{
   int key;
   int value;
   shared_ptr<ListNodeLRU> next;
   shared_ptr<ListNodeLRU> prev;

   ListNodeLRU( int kkey, int vvalue ) : key(kkey), value(vvalue)
   { }
   ListNodeLRU() : next( nullptr ), prev( nullptr )
   {
   }
};

class LRUCache
{
public:
   LRUCache( int capacity )
   {
      m_capacity = capacity;

      head = make_shared<ListNodeLRU>();
      tail = make_shared<ListNodeLRU>();
      head->next = tail;
      tail->prev = head;
   }

   int get( int key )
   {
      // Look if value exists in hash
      shared_ptr<ListNodeLRU>  pNode= GetFromMap( key );
      if( !pNode )
         return -1;

      // If value exists put it to the end of list
      MoveNodeToEnd( pNode );
      return pNode->value;
   }

   void put( int key, int value )
   {
      // If this key exists, update the node and move to the end
      shared_ptr<ListNodeLRU>  pNode = GetFromMap( key );
      if( pNode )
      {
         pNode->value = value;

         // If value exists put it to the end of list
         MoveNodeToEnd( pNode );

         return;
      }

      if( m_count == m_capacity )
         RemoveLeastRecentlyUsed();

      shared_ptr<ListNodeLRU> nodePtr = make_shared<ListNodeLRU>( key, value );
      InsertNodeToTheEnd( nodePtr );
   }

private:
   void InsertNodeToTheEnd( const shared_ptr<ListNodeLRU>& nodePtr )
   {
      nodes[ nodePtr->key ] = nodePtr;

      shared_ptr<ListNodeLRU> lastNode = tail->prev;
      lastNode->next = nodePtr;
      nodePtr->prev = lastNode;
      nodePtr->next = tail;
      tail->prev = nodePtr;

      m_count++;
   }

   shared_ptr<ListNodeLRU> GetFromMap( int key )
   {
      if( nodes.find( key ) != nodes.end() )
         return nodes[ key ];

      return nullptr;
   }

   void RemoveLeastRecentlyUsed()
   {
      shared_ptr<ListNodeLRU> firstNode = head->next;
      if( firstNode == tail )
         return;

      // remove from map
      nodes.erase( firstNode->key );

      auto next = firstNode->next;
      auto prev = firstNode->prev;
      prev->next = next;
      next->prev = prev;

      m_count--;
   }

   void MoveNodeToEnd( shared_ptr<ListNodeLRU> pNode )
   {
      if( pNode == nullptr )
         return;

      // Remove pNode from existing place
      auto next = pNode->next;
      auto prev = pNode->prev;
      prev->next = next;
      next->prev = prev;

      auto lastNode = tail->prev;
      lastNode->next = pNode;
      pNode->prev = lastNode;
      pNode->next = tail;
      tail->prev = pNode;
   }

   shared_ptr<ListNodeLRU> head = nullptr;
   shared_ptr<ListNodeLRU> tail = nullptr;
   unordered_map<int, shared_ptr<ListNodeLRU>> nodes;
   int m_count = 0;
   int m_capacity = 0;
};
