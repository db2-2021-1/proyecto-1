#include <utility>
#include <vector>
#include <algorithm>
#include "csv_lector.h"

Register notFound()
{
  return Register(-1, "NotFound", -1, -1, -1,
                  "NotFound", "NotFound", "NotFound",
                  "NotFound", "NotFound", "NotFound", -1);
}
int xddd = 0;

class BPlusTree
{
private:
  Node *root;
  keyType firstid;
  void insertInternal(int x, Node *cursor, Node *child, long posicion, bool flag)
  {
    //std::cout << "insertInternal" << std::endl;
    if (cursor->size < MAX)
    {
      int i = 0;
      while (x > cursor->key[i] && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--)
      {
        cursor->key[j] = cursor->key[j - 1];
      }
      for (int j = cursor->size + 1; j > i + 1; j--)
      {
        cursor->ptr[j] = cursor->ptr[j - 1];
      }
      cursor->key[i] = x;
      //cursor->values.insert(std::make_pair(x, posicion));
      cursor->size++;
      cursor->values.insert(std::make_pair(x, posicion));
      cursor->ptr[i + 1] = child;
    }
    else
    {
      Node *newInternal = new Node;
      int virtualKey[MAX + 1];
      Node *virtualPtr[MAX + 2];
      for (int i = 0; i < MAX; i++)
      {
        virtualKey[i] = cursor->key[i];
        //std::cout<<"LINEA 52: \n\n"<<"cursor->values.find(cursor->key[i])->second: "<<cursor->values.find(cursor->key[i])->second<<"\n\n";
        if (flag)
        {
          std::cout << "insert Internal : LINEA 54 \n";
        }
        newInternal->values.insert(std::make_pair(cursor->key[i], cursor->values.find(cursor->key[i])->second));
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
        virtualKey[j] = virtualKey[j - 1];
      }
      virtualKey[i] = x;
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
        newInternal->key[i] = virtualKey[j];
        //std::cout << "newInternal->key[i]: " << newInternal->key[i] << " newInternal->values.find(newInternal->key[i])->second: " << cursor->values.find(cursor->key[i])->second << std::endl;
        newInternal->values.insert(std::make_pair(newInternal->key[i], cursor->values.find(cursor->key[i])->second));
      }
      for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
      {
        newInternal->ptr[i] = virtualPtr[j];
      }
      //std::cout << " LINEA 81\n";
      //newInternal->values.insert()
      if (cursor == root)
      {
        Node *newRoot = new Node;
        newRoot->key[0] = cursor->key[cursor->size];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newInternal;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      }
      else
      {
        //std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        posicion = cursor->values.find(cursor->key[cursor->size])->second;
        if (flag)
        {
          std::cout << "insertInternal llamada 106\n";
        }
        insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal, posicion, flag);
      }
    }
  }
  Node *findParent(Node *cursor, Node *child)
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
        parent = findParent(cursor->ptr[i], child);
        if (parent != NULL)
          return parent;
      }
    }
    return parent;
  }

public:
  BPlusTree()
  {
    std::fstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    root = NULL;
  }
  Node *findNode(int id)
  {
    //std::cout<<"Node *findNode(int id): Node *findNode(int "<<id<<" )\n";
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
          return cursor;
        }
      }
      return nullptr;
    }
  }

  Register find(int id)
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
      if ((id == 17004 || id == 17013) && cursor->IS_LEAF)
      {
        //std::cout << "id: " << id << " IS_LEAF: \n"<<"(cursor->values.find(id))->second: "<<(cursor->values.find(id))->second<<"\n";
      }
      std::cout << "LINEA 206\n";
      for (int i = 0; i < cursor->size; i++)
      {
        std::cout<<cursor->key[i]<<"=="<<id<<"\n";
      }
      for (int i = 0; i < cursor->size; i++)
      {
        std::cout<<"cursor->key[i]: "<<cursor->key[i]<<" id: "<<id<<"\n";
        if (cursor->key[i] == id)
        {
          std::cout << "HOLHOLAHOOAH\n";
          auto posicion_registro = (cursor->values.find(id))->second;
          std::cout << "posicion_registro: " << posicion_registro << "\n";
          //std::cout << "id: " << id << "\n";
          //std::cout << "posicion_registro: " << posicion_registro << "\n";
          if (id != firstid && posicion_registro == 0)
          {
            std::cout << "return notFound();205\n";
            return notFound();
          }
          std::fstream file;
          std::string line;
          file.open(filename);
          file.seekg(posicion_registro);
          auto *ptrReg = new Register;
          file.read((char *)ptrReg, sizeof(Register));
          file.close();
          if ((id == 17004 || id == 17013) && cursor->IS_LEAF)
          {
            std::cout <<"QWERQWEQEWQEWS\n";
          }
          return *ptrReg;
        }
      }
      //std::cout << "posicion_registro: " << posicion_registro << "\n";
      //std::cout << "id: " << id << std::endl;
      std::cout << "id: " << id << "\n";
      std::cout << "return notFound();219\n\n";
      return notFound();
    }
  }

  std::vector<int> vector_ = {10434, 12095, 13861, 10710, 17013, 17004};
  void insert(Register reg)
  {
    bool flag_debug = false;
    int x = reg.id;
    if (std::find(vector_.begin(), vector_.end(), reg.id) != vector_.end() && (reg.id == 17004 || reg.id == 17013))
    {
      std::cout << "reg.id: " << reg.id << "\n";
      flag_debug = true;
    }
    std::fstream file;
    file.open(filename);
    file.seekg(0, std::ios::end);
    long posicion_registro = file.tellg();
    long aux = posicion_registro;

    file.write((char *)&reg, sizeof(Register));
    file.close();

    if (root == NULL)
    {
      if (flag_debug)
      {
        std::cout << "debug -> LINEA 245: \n";
      }
      root = new Node;
      root->key[0] = x;
      firstid = x;
      (root->values).insert(std::make_pair(x, posicion_registro));
      root->IS_LEAF = true;
      root->size = 1;
    }
    else
    {
      if (flag_debug)
      {
        std::cout << "debug -> LINEA 258: \n";
      }
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
        if (flag_debug)
        {
          std::cout << "debug -> LINEA 283: \n";
        }
        int i = 0;
        while (x > cursor->key[i] && i < cursor->size)
          i++;
        for (int j = cursor->size; j > i; j--)
        {
          cursor->key[j] = cursor->key[j - 1];
        }
        cursor->key[i] = x;
        cursor->size++;
        if (flag_debug)
        {
          std::cout << "x: " << x << " posicion_registro: " << posicion_registro << "\n";
        }
        cursor->values.insert(std::make_pair(x, posicion_registro));
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
      }
      else
      {
        if (flag_debug)
        {
          std::cout << "debug -> LINEA 302: \n";
        }
        Node *newLeaf = new Node;
        newLeaf->values.insert(std::make_pair(x, aux));
        int virtualNode[MAX + 1];
        for (int i = 0; i < MAX; i++)
        {
          newLeaf->values.insert(std::make_pair(cursor->key[i], cursor->values.find(cursor->key[i])->second));
          virtualNode[i] = cursor->key[i];
        }
        int i = 0, j;
        while (x > virtualNode[i] && i < MAX)
          i++;
        for (int j = MAX + 1; j > i; j--)
        {
          virtualNode[j] = virtualNode[j - 1];
        }
        virtualNode[i] = x;
        newLeaf->IS_LEAF = true;
        cursor->size = (MAX + 1) / 2;
        newLeaf->size = MAX + 1 - (MAX + 1) / 2;
        newLeaf->values.insert(std::make_pair(x, posicion_registro));
        cursor->ptr[cursor->size] = newLeaf;
        newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
        int aux = posicion_registro;
        cursor->ptr[MAX] = NULL;
        for (i = 0; i < cursor->size; i++)
        {
          cursor->key[i] = virtualNode[i];
          cursor->values.insert(std::make_pair(cursor->key[i], aux));
        }
        for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
        {
          newLeaf->key[i] = virtualNode[j];
        }
        if (cursor == root)
        {
          Node *newRoot = new Node;
          newRoot->key[0] = newLeaf->key[0];
          newRoot->ptr[0] = cursor;
          newRoot->values.insert(std::make_pair(x, posicion_registro));
          newRoot->ptr[1] = newLeaf;
          newRoot->IS_LEAF = false;
          newRoot->size = 1;
          root = newRoot;
        }
        else
        {
          if (flag_debug)
          {
            std::cout << "debug ->insertInternal: \n";
          }
          insertInternal(newLeaf->key[0], parent, newLeaf, posicion_registro, flag_debug);
        }
      }
      if (flag_debug)
      {
        std::cout << "LINEA 366\n\n";
      }
    }
  }
  void display(Node *cursor)
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

  Node *getRoot()
  {
    return root;
  }
  void print_leaf(Node *cursor)
  {
    if (cursor != NULL)
    {
      for (int i = 0; i < cursor->size; i++)
      {
        //&& std::find(vector_.begin(), vector_.end(), cursor->key[i]) != vector_.end()
        if (cursor->IS_LEAF && cursor->key[i] >= 10000 && 20000 >= cursor->key[i])
        {
          xddd++;
          std::cout << "cursor->key[i]: " << cursor->key[i] << " posicion: " << ((cursor->values).find(cursor->key[i]))->second << "\n";
          std::cout << "xddd: " << xddd << "\n";
        }
      }
      //std::cout << "\n";
      if (cursor->IS_LEAF != true)
      {
        for (int i = 0; i < cursor->size + 1; i++)
        {
          print_leaf(cursor->ptr[i]);
        }
      }
    }
  }
  void remove(keyType key)
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
        for (int i = 0; i < cursor->size; i++)
        {
          if (key < cursor->key[i])
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
        if (cursor->key[i] == key)
        {
          std::cout << "key: " << key;
          cursor->values.erase(key);
          return;
        }
      }
      return;
    }
  }
  std::vector<Register> find(keyType beginKey, keyType endKey)
  {
    std::vector<Register> registros;
    int ite = beginKey;
    Node *reg = findNode(ite);
    while (reg == nullptr)
    {
      ite++;
      if (ite > endKey)
      {
        return registros;
      }
      reg = findNode(ite);
    }
    std::cout << "Linea 472: (reg->key[0]): " << reg->key[0] << " beginKey: " << beginKey << "\n";
    while (reg->key[0] <= endKey)
    {
      int i = 0;
      for (i = 0; i < reg->size; i++)
      {
        if (reg->key[i] >= beginKey && reg->key[i] <= endKey)
        {
          std::cout << "find(keyType beginKey, keyType endKey): --> " << reg->key[i] << "\n";
          registros.push_back(find(reg->key[i]));
        }
      }
      reg = reg->ptr[reg->size];
    }
    return registros;
  }
};
