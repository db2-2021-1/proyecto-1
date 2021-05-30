#include "bptree.hpp"

void b_plus_tree::insertInternal(db2::literal &x, Node *cursor, Node *child, size_t pos)
{
  if (cursor->size < MAX)
  {
    size_t i = 0;
    while (x > cursor->position_[i].first && i < cursor->size)
      i++;
    for (size_t j = cursor->size; j > i; j--)
    {
      //cursor->key[j][0] = cursor->key[j - 1][0];
      //cursor->key[j][1] = cursor->key[j - 1][1];
      cursor->position_[j] = cursor->position_[j-1];
      //cursor->position_[j].first = cursor->position_[j - 1].first;
      //cursor->position.insert(std::make_pair(cursor->key[j], cursor->position.find(cursor->key[j - 1])->second));
    }
    for (size_t j = cursor->size + 1; j > i + 1; j--)
    {
      cursor->ptr_[j] = cursor->ptr_[j - 1];
    }
    //cursor->key[i][0] = x;
    //cursor->key[i][1] = pos;
    cursor->position_.push_back(std::make_pair(x,pos));
    //cursor->key[i] = x;
    //cursor->position.insert(std::make_pair(cursor->key[i], pos));

    cursor->size++;
    cursor->ptr_[i + 1] = child;
  }
  else
  {
    Node *newInternal = new Node;
    //int virtualKey[MAX + 1][MAX + 1];

    //db2::literal virtualKey[MAX + 1];
    //std::map<db2::literal, long> aux_map;
    std::vector<std::pair<db2::literal, size_t>> position_ = cursor->position_;

    Node *virtualPtr[MAX + 2];
    
    for (size_t i = 0; i < MAX; i++)
    {
      //virtualKey[i][0] = cursor->key[i][0];
      //virtualKey[i][1] = cursor->key[i][1];
    
      //virtualKey[i] = cursor->key[i];
      //aux_map.insert(std::make_pair(cursor->key[i], cursor->position.find(cursor->key[i])->second));
    }
    for (size_t i = 0; i < MAX + 1; i++)
    {
      virtualPtr[i] = cursor->ptr_[i];
    }
    size_t i = 0, j;
    while (x > position_[i].first && i < MAX)
      i++;
    for (size_t j = MAX + 1; j > i; j--)
    {
      //virtualKey[j][0] = virtualKey[j - 1][0];
      //virtualKey[j][1] = virtualKey[j - 1][1];

      //virtualKey[j] = virtualKey[j - 1];
      //aux_map.insert(std::make_pair(virtualKey[j], aux_map.find(virtualKey[j - 1])->second));
      position_[j] = position_[j-1];
    }
    //virtualKey[i][0] = x;
    //virtualKey[i][1] = pos;

    //virtualKey[i] = x;
    //aux_map.insert(std::make_pair(virtualKey[i], pos));

    position_[i] = std::make_pair(x,pos);

    for (size_t j = MAX + 2; j > i + 1; j--)
    {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
    {
      //newInternal->key[i][0] = virtualKey[j][0];
      //newInternal->key[i][1] = virtualKey[j][1];

      //newInternal->position = aux_map.find(virtualKey[i])->second;

      //newInternal->key[i] = virtualKey[i];
      //newInternal->position.insert(std::make_pair(virtualKey[i], aux_map.find(virtualKey[i])->second));
    
      newInternal->position_[i] = position_[j]; 
    
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
    {
      newInternal->ptr_[i] = virtualPtr[j];
    }
    if (cursor == root)
    {
      Node *newRoot = new Node;
      //newRoot->key[0][0] = cursor->key[cursor->size][0];
      //newRoot->key[0][1] = cursor->key[cursor->size][1];

      //newRoot->key[0] = cursor->key[cursor->size];
      //newRoot->position.insert(std::make_pair(cursor->key[cursor->size], cursor->position.find(cursor->key[cursor->size])->second));
      
      newRoot->position_.push_back(cursor->position_[cursor->size]); 


      newRoot->ptr_[0] = cursor;
      newRoot->ptr_[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    }
    else
    {
      //insertInternal(cursor->key[cursor->size][0], findParent(root, cursor), newInternal,cursor->key[cursor->size][1]);

      insertInternal(cursor->position_[cursor->size].first, find_parent(root, cursor), newInternal, cursor->position_[cursor->size].second);
    }
  }
}
Node *b_plus_tree::find_parent(Node *cursor, Node *child)
{
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr_[0])->IS_LEAF)
  {
    return NULL;
  }
  for (size_t i = 0; i < cursor->size + 1; i++)
  {
    if (cursor->ptr_[i] == child)
    {
      parent = cursor;
      return parent;
    }
    else
    {
      parent = find_parent(cursor->ptr_[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

Node::iterator b_plus_tree::findNode(const db2::literal &id)
{
  if (root == NULL)
  {
    std::cout << "Tree is empty\n";
  }
  else
  {
    Node *cursor = root;
    while (cursor->IS_LEAF == false)
    {
      for (size_t i = 0; i < cursor->size; i++)
      {
        if (id < cursor->position_[i].first)
        {
          cursor = cursor->ptr_[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          cursor = cursor->ptr_[i + 1];
          break;
        }
      }
    }
    for (size_t i = 0; i < cursor->size; i++)
    {
      if (cursor->position_[i].first == id)
      {
        std::cout << "Found and in position: " << cursor->position_[i].second << "\n";
        return Node::iterator{cursor,i};
      }
    }
    return cursor->end();
    std::cout << "Not found\n";
  }
  return end();
}

Node *b_plus_tree::get_root()
{
  return root;
}


b_plus_tree::b_plus_tree(std::string filename, db2::statement::type key_type) : key_type(key_type)
{
  std::fstream file;
  file.open(filename, std::ofstream::out | std::ofstream::trunc);
  root = NULL;
}

std::vector<size_t> b_plus_tree::get_positions(const db2::literal &key)
{
  std::vector<size_t> positions;

  for(auto it = getMinNode(key); it != end(); it++)
  {
    if(it->first != key)
      break;
    positions.push_back(it->second);
  }

  return positions;
}



Node::iterator b_plus_tree::getMinNode(db2::literal keymin)
{
  struct retorno
  {
    Node *cursor_;
    int i;
  };
  Node *cursor = root;
  if (root == NULL)
  {
    return end();
  }
  else
  {
    size_t aux  = 0 ;
    while (cursor->IS_LEAF == false)
    {
      for (; aux < cursor->size; aux++)
      {
        if (keymin < cursor->position_[aux].first)
        {
          cursor = cursor->ptr_[aux];
          break;
        }
        if (aux == cursor->size - 1)
        {
          cursor = cursor->ptr_[aux + 1];
          break;
        }
      }
      aux = 0;
    }
    bool flag = false;
    size_t ite = 0;
    while(true){
      for(size_t i = 0 ; i < cursor->size; i ++ ){
        if(cursor->position_[i].first > keymin){
          flag = true;
          ite=i;
          break;
        }
      }
      if(flag){
        return Node::iterator{cursor,ite};
      }
      cursor = cursor->ptr_[cursor->size];
    }
    return cursor->end();
  }
}

std::vector<size_t> b_plus_tree::get_positions(
    const db2::literal &key_ge,
    const db2::literal &key_le)
{
  std::vector<size_t> positions;

  for(auto it = getMinNode(key_ge); it != end(); it++)
  {
    if(it->first > key_le)
      break;
    positions.push_back(it->second);
  }

  return positions;
}

bool b_plus_tree::insert(const db2::literal &key, size_t position)
{
  auto x = key;
  size_t posicion_registro = position;
  if (root == NULL)
  {
    root = new Node;
    //root->key[0] = x;
    //root->position.insert(std::make_pair(x, posicion_registro));
    root->position_.push_back(std::make_pair(x, posicion_registro));
    root->IS_LEAF = true;
    root->size = 1;
  }
  else
  {
    Node *cursor = root;
    Node *parent;
    while (cursor->IS_LEAF == false)
    {
      parent = cursor;
      for (size_t i = 0; i < cursor->size; i++)
      {
        //if (x < cursor->key[i])
        if(x < cursor->position_[i].first)
        {
          //cursor = cursor->ptr[i];
          cursor = cursor->ptr_[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          //cursor = cursor->ptr[i + 1];
          cursor = cursor->ptr_[i+1];
          break;
        }
      }
    }
    if (cursor->size < MAX)
    {
      size_t i = 0;
      while (x > cursor->position_[i].first && i < cursor->size)
        i++;
      for (size_t j = cursor->size; j > i; j--)
      {
        cursor->position_[j].first = cursor->position_[j - 1].first;
        cursor->position_.push_back(std::make_pair(cursor->position_[j].first, cursor->position_[j-1].second));
        //cursor->position.insert(std::make_pair(cursor->key[j], cursor->position.find(cursor->key[j - 1])->second));
      }
      //cursor->key[i] = x;
      //cursor->position.insert(std::make_pair(x, posicion_registro));
      
      //cursor->position_.push_back(std::make_pair());
      cursor->position_[i] = std::make_pair(x,posicion_registro);
      
      cursor->size++;
      cursor->ptr_[cursor->size] = cursor->ptr_[cursor->size -1];
      cursor->ptr_[cursor->size - 1] = NULL;


      //cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      //cursor->ptr[cursor->size - 1] = NULL;
    }
    else
    {
      Node *newLeaf = new Node;
      db2::literal virtualNode[MAX + 1];
      std::vector<size_t> posiciones;
      std::vector<std::pair<db2::literal, size_t>> posiciones_ = cursor->position_;
      //std::map<db2::literal, long> aux_map;
      for (size_t i = 0; i < MAX; i++)
      {
        //virtualNode[i] = cursor->position_[i].first;
        //aux_map.insert(std::make_pair(virtualNode[i], cursor->position.find(cursor->key[i])->second));
      }
      size_t i = 0, j;
      while (x > posiciones_[i].first && i < MAX)
      i++;
      for (size_t j = MAX + 1; j > i; j--)
      {
        //virtualNode[j] = virtualNode[j - 1];
        //aux_map.insert(std::make_pair(virtualNode[j], aux_map.find(virtualNode[j - 1])->second));
        posiciones_[j] = posiciones_[j - 1]; 
      }
      //virtualNode[i] = x;
      //aux_map.insert(std::make_pair(virtualNode[i], posicion_registro));

      posiciones_.push_back(std::make_pair(x,posicion_registro));
      newLeaf->IS_LEAF = true;
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      cursor->ptr_[cursor->size] = newLeaf;
      newLeaf->ptr_[newLeaf->size] = cursor->ptr_[MAX];
      cursor->ptr_[MAX] = NULL;
      for (i = 0; i < cursor->size; i++)
      {
        //cursor->key[i] = virtualNode[i];
        //cursor->position.insert(std::make_pair(virtualNode[i], aux_map.find(virtualNode[i])->second));
        cursor->position_[i] = posiciones_[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
      {
        //newLeaf->key[i] = virtualNode[j];
        //newLeaf->position.insert(std::make_pair(virtualNode[j], aux_map.find(virtualNode[j])->second));
        newLeaf->position_[i] = posiciones_[j];
      }
      if (cursor == root)
      {
        Node *newRoot = new Node;
        //newRoot->key[0] = newLeaf->key[0];
        //newRoot->position_[0].first = newLeaf->position_[0].first;
        newRoot->position_.push_back(posiciones_[0]);
        newRoot->ptr_[0] = cursor;
        newRoot->ptr_[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      }
      else
      {
        insertInternal(newLeaf->position_[0].first, parent, newLeaf,newLeaf->position_[0].second);
      }
    }
  }
  return false;
}

bool b_plus_tree::delete_from(const db2::literal &key, size_t position)
{
  auto cursor = findNode(key); 
  if(cursor.get_node() != nullptr){
    findNode(key).get_node()->position_[cursor.get_index()].second=-1;
    return true;
  }
  return false;
}

Node::iterator b_plus_tree::end()
{
  return Node::iterator(nullptr, 0);
}
