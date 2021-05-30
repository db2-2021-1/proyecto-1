#include "bptree.hpp"
Node *b_plus_tree::find_parent(Node *cursor, Node *child)
{
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF)
  {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++)
  {
    if (cursor->ptr[i] == child)
    {
      parent = cursor;
      return parent;
    }
    else
    {
      parent = find_parent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

void b_plus_tree::insertInternal(db2::literal &x, Node *cursor, Node *child, size_t pos)
{
  if (cursor->size < MAX)
  {
    int i = 0;
    while (x > cursor->key[i] && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--)
    {
      //cursor->key[j][0] = cursor->key[j - 1][0];
      //cursor->key[j][1] = cursor->key[j - 1][1];

      cursor->key[j] = cursor->key[j - 1];
      cursor->position.insert(std::make_pair(cursor->key[j], cursor->position.find(cursor->key[j - 1])->second));
    }
    for (int j = cursor->size + 1; j > i + 1; j--)
    {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    //cursor->key[i][0] = x;
    //cursor->key[i][1] = pos;

    cursor->key[i] = x;
    cursor->position.insert(std::make_pair(cursor->key[i], pos));

    cursor->size++;
    cursor->ptr[i + 1] = child;
  }
  else
  {
    Node *newInternal = new Node;
    //int virtualKey[MAX + 1][MAX + 1];

    db2::literal virtualKey[MAX + 1];
    std::map<db2::literal, long> aux_map;

    Node *virtualPtr[MAX + 2];
    for (int i = 0; i < MAX; i++)
    {
      //virtualKey[i][0] = cursor->key[i][0];
      //virtualKey[i][1] = cursor->key[i][1];

      virtualKey[i] = cursor->key[i];
      aux_map.insert(std::make_pair(cursor->key[i], cursor->position.find(cursor->key[i])->second));
    }
    for (int i = 0; i < MAX + 1; i++)
    {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < MAX)
      i++;
    for (int j = MAX + 1; j > i; j--)
    {
      //virtualKey[j][0] = virtualKey[j - 1][0];
      //virtualKey[j][1] = virtualKey[j - 1][1];

      virtualKey[j] = virtualKey[j - 1];
      aux_map.insert(std::make_pair(virtualKey[j], aux_map.find(virtualKey[j - 1])->second));
    }
    //virtualKey[i][0] = x;
    //virtualKey[i][1] = pos;

    virtualKey[i] = x;
    aux_map.insert(std::make_pair(virtualKey[i], pos));

    for (int j = MAX + 2; j > i + 1; j--)
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

      newInternal->key[i] = virtualKey[i];
      newInternal->position.insert(std::make_pair(virtualKey[i], aux_map.find(virtualKey[i])->second));
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
    {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root)
    {
      Node *newRoot = new Node;
      //newRoot->key[0][0] = cursor->key[cursor->size][0];
      //newRoot->key[0][1] = cursor->key[cursor->size][1];

      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->position.insert(std::make_pair(cursor->key[cursor->size], cursor->position.find(cursor->key[cursor->size])->second));

      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    }
    else
    {
      //insertInternal(cursor->key[cursor->size][0], findParent(root, cursor), newInternal,cursor->key[cursor->size][1]);

      insertInternal(cursor->key[cursor->size], find_parent(root, cursor), newInternal, cursor->position.find(cursor->key[cursor->size])->second);
    }
  }
}
Node *b_plus_tree::find_parent(Node *cursor, Node *child)
{
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF)
  {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++)
  {
    if (cursor->ptr[i] == child)
    {
      parent = cursor;
      return parent;
    }
    else
    {
      parent = find_parent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

auto b_plus_tree::findNode(const db2::literal &id)
{
  struct retorno
  {
    Node *cursor_;
    int i;
  };
  if (root == NULL)
  {
    std::cout << "Tree is empty\n";
  }
  else
  {
    Node *cursor = root;
    while (cursor->IS_LEAF == false)
    {
      for (int i = 0; i < cursor->size; i++)
      {
        if (id < cursor->key[i])
        {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++)
    {
      if (cursor->key[i] == id)
      {
        std::cout << "Found and in position: " << cursor->position.find(cursor->key[i])->second << "\n";
        return retorno{cursor, i};
      }
    }
    return retorno{nullptr, -1};
    std::cout << "Not found\n";
  }
}

std::vector<size_t> b_plus_tree::find(keyType low)
{
  std::vector<size_t> retorno;
  if (root == NULL)
  {
    return retorno;
  }
  auto nodo = findNode(low);
  if (nodo.cursor_ != nullptr)
  {
    retorno.push_back(nodo.cursor_->position.find(nodo.cursor_->key[nodo.i])->second);
  }
  return retorno; //el vector "retorno" deberia regresar vacio
}

void b_plus_tree::display(Node *cursor)
{
  if (cursor != NULL)
  {
    for (int i = 0; i < cursor->size; i++)
    {
      std::cout << cursor->key[i] << " ";
    }
    std::cout << "\n";
    if (cursor->IS_LEAF != true)
    {
      for (int i = 0; i < cursor->size + 1; i++)
      {
        display(cursor->ptr[i]);
      }
    }
  }
}

Node *b_plus_tree::get_root()
{
  return root;
}
void b_plus_tree::print_leaf(Node *cursor)
{
}

std::vector<size_t> b_plus_tree::find(keyType beginKey, keyType endKey)
{
}

b_plus_tree::b_plus_tree(std::string filename, db2::statement::type key_type) : key_type(key_type)
{
  std::fstream file;
  file.open(filename, std::ofstream::out | std::ofstream::trunc);
  root = NULL;
}

std::vector<size_t> b_plus_tree::get_positions(const db2::literal &key)
{
  // El tipo y tamaño del key están en key_type
  //TODO
  std::vector<size_t> retorno;
  if (root == NULL)
  {
    return retorno;
  }
  auto nodo = findNode(key);
  if (nodo.cursor_ != nullptr)
  {
    retorno.push_back(nodo.cursor_->position.find(nodo.cursor_->key[nodo.i])->second);
  }
  return retorno; //el vector "retorno" deberia regresar vacio
}

  size_t b_plus_tree::getPos(db2::literal id){
      if (root == NULL) {
    std::cout << "Tree is empty\n";
  } else {
    Node *cursor = root;
    while (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size; i++) {
        if (id < cursor->key[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++) {
      if (cursor->key[i] == id) {
        std::cout << "Found and in position: "<< cursor->position.find(cursor->key[i])->second <<"\n";
        return cursor->position.find(cursor->key[i])->second;
      }
    }
    std::cout << "Not found\n";
  }
  }

auto b_plus_tree::getMinNode(db2::literal keymin)
{
  struct retorno
  {
    Node *cursor_;
    int i;
  };
  if (root == NULL)
  {
    std::cout << "Tree is empty\n";
  }
  else
  {
    Node *cursor = root;
    while (cursor->IS_LEAF == false)
    {
      for (int i = 0; i < cursor->size; i++)
      {
        if (keymin < cursor->key[i])
        {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++)
    {
      if ((cursor->key[i] == keymin ) || (keymin < cursor->key[i+1] && cursor->key[i] < keymin))
      {
        std::cout << "Found and in position: " << cursor->position.find(cursor->key[i])->second << "\n";
        return retorno{cursor, i};
      }
    }
    return retorno{nullptr, -1};
    std::cout << "Not found\n";
  }
  //return retorno{cursor, i};
}

std::vector<size_t> b_plus_tree::get_positions(
    const db2::literal &key_ge,
    const db2::literal &key_le)
{
  std::vector<size_t> retorno;
  auto nodo_min = getMinNode(key_ge);
  auto cursor = nodo_min.cursor_;
  int i = nodo_min.i;
  /*if(cursor != nullptr){
    int ite = i;
    for(; i < cursor->size; ite++){
      if(cursor->key[ite] < key_le)
        retorno.push_back(cursor->position.find(cursor->key[ite])->second);
    }
    ite = 0;
    for (int i = 0; i < cursor->size; i++)
    {
      if (key_ge < cursor->key[i] && cursor->key[i] < key_le)
      {
        cursor = cursor->ptr[i];
        break;
      }
      if (i == cursor->size - 1)
      {
        cursor = cursor->ptr[i + 1];
        break;
      }
    }   
  }*/
  while (cursor->key[0] <= key_le)
    {
      int i = 0;
      for (i = 0; i < cursor->size; i++)
      {
        if (cursor->key[i] >= key_ge && cursor->key[i] <= key_le)
        {
          retorno.push_back(cursor->position.find(cursor->key[i])->second);
        }
      }
      cursor = cursor->ptr[cursor->size];
    }
  return retorno;
}

bool b_plus_tree::insert(const db2::literal &key, size_t position)
{
  auto x = key;
  size_t posicion_registro = position;
  if (root == NULL)
  {
    root = new Node;
    root->key[0] = x;
    root->position.insert(std::make_pair(x, posicion_registro));
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
      for (int i = 0; i < cursor->size; i++)
      {
        if (x < cursor->key[i])
        {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    if (cursor->size < MAX)
    {
      int i = 0;
      while (x > cursor->key[i] && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--)
      {
        cursor->key[j] = cursor->key[j - 1];
        cursor->position.insert(std::make_pair(cursor->key[j], cursor->position.find(cursor->key[j - 1])->second));
      }
      cursor->key[i] = x;
      cursor->position.insert(std::make_pair(x, posicion_registro));
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    }
    else
    {
      Node *newLeaf = new Node;
      db2::literal virtualNode[MAX + 1];
      std::map<db2::literal, long> aux_map;
      for (int i = 0; i < MAX; i++)
      {
        virtualNode[i] = cursor->key[i];
        aux_map.insert(std::make_pair(virtualNode[i], cursor->position.find(cursor->key[i])->second));
      }
      int i = 0, j;
      while (x > virtualNode[i] && i < MAX)
        i++;
      for (int j = MAX + 1; j > i; j--)
      {
        virtualNode[j] = virtualNode[j - 1];
        aux_map.insert(std::make_pair(virtualNode[j], aux_map.find(virtualNode[j - 1])->second));
      }
      virtualNode[i] = x;
      aux_map.insert(std::make_pair(virtualNode[i], posicion_registro));
      newLeaf->IS_LEAF = true;
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;
      for (i = 0; i < cursor->size; i++)
      {
        cursor->key[i] = virtualNode[i];
        cursor->position.insert(std::make_pair(virtualNode[i], aux_map.find(virtualNode[i])->second));
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
      {
        newLeaf->key[i] = virtualNode[j];
        newLeaf->position.insert(std::make_pair(virtualNode[j], aux_map.find(virtualNode[j])->second));
      }
      if (cursor == root)
      {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->position.insert(std::make_pair(newLeaf->key[0], aux_map.find(newLeaf->key[0])->second));

        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      }
      else
      {
        insertInternal(newLeaf->key[0], parent, newLeaf, newLeaf->position.find(newLeaf->key[0])->second);
      }
    }
  }
  return false;
}

bool b_plus_tree::delete_from(const db2::literal &key, size_t position)
{
  //TODO
  return false;
}
