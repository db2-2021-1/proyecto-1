#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>

//#include "register.h"
#define MAX 3

#define blockFactor 2
#define hashSize 4
const std::string filename = "./sample.dat";
typedef int keyType;
struct Register
{
  keyType id;
  char firstName[16];
  char lastName[16];
  int age;

  Register() {}

  Register(keyType _id, std::string _firstName, std::string _lastName, int _age) : id(_id), age(_age)
  {
    for (int i = 0; i < 16; i++)
    {
      if (i < _firstName.size())
      {
        firstName[i] = _firstName[i];
      }
      else
      {
        firstName[i] = '\0';
      }
    }
    for (int i = 0; i < 16; i++)
    {
      if (i < _lastName.size())
      {
        lastName[i] = _lastName[i];
      }
      else
      {
        lastName[i] = '\0';
      }
    }
  }
};

struct Bucket
{
private:
  int cant_registros = 3;
  //std::vector<Register> registros;
  std::vector<long> registros;

public:
  Bucket() {}

  void insert(Register reg)
  {
    std::fstream file;
    file.open(filename);
    file.seekg(0, std::ios::end);
    long pos = file.tellg();
    file.write((char *)&reg, sizeof(Register));
    //indexVec.insert(indexVec.end(), std::pair<keyType, long>(reg.id, pos));
    file.close();
  }
};

struct Node
{
  bool IS_LEAF;
  int *key, size;
  Node **ptr;
  std::map<int, long> values; //int -> id, long -> posicion del registro
  friend class BPTree;
  Node()
  {
    key = new int[MAX];
    ptr = new Node *[MAX + 1];
  }
  int pos_registro = 0;
};

class BPlusTree
{
private:
  Node *root;
  void insertInternal(int x, Node *cursor, Node *child, long posicion)
  {
    std::cout << "fdsafdsafdsafds" << std::endl;
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
      cursor->values.insert(std::make_pair(x, posicion));
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
      }
      for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
      {
        newInternal->ptr[i] = virtualPtr[j];
      }
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
        insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal, posicion);
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
  //BPlusTree(){}
  BPlusTree()
  {
    std::fstream file;
    //file.open(filename);
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    root = NULL;
  }
  void find(int id)
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
      for (int i = 0; i < cursor->size; i++)
      {
        if (cursor->key[i] == id)
        {
          std::cout << "Found\n";
          return;
        }
      }
      std::cout << "Not found\n";
    }
  }

  void insert(Register reg)
  {
    int x = reg.id;
    std::cout<<"\n\n\n\n\n\n\n\n";
    std::cout << "x:" << x <<"\n"; 
    std::fstream file;
    file.open(filename);
    file.seekg(0, std::ios::end);
    long posicion_registro = file.tellg();
    long aux = posicion_registro;
    std::cout << "posicion_registro: " << posicion_registro << std::endl;
    file.write((char *)&reg, sizeof(Register));
    file.close();

    if (root == NULL) //Si el root no apunta nada, es decir arbol vacio
    {
      //std::cout << "if (root == NULL) " << std::endl;
      root = new Node;  //se crea el nodo al que root apunto
      root->key[0] = x; // este nodo tiene como su primer elemento de sus llaves = x
      (root->values).insert(std::make_pair(x, posicion_registro));
      root->IS_LEAF = true; // el root como es el unico, tambien es una hoja
      root->size = 1;       // el root tiene 1 elemento
    }
    else
    {
      //std::cout << "else sssss" << std::endl;

      Node *cursor = root; // como el root no esta vacio, minimamente el root tiene 1 elemento
      Node *parent;        // se crea un punto a node llamado parent
      while (cursor->IS_LEAF == false)
      {
        //std::cout << "while (cursor->IS_LEAF == false)" << std::endl;
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
        //std::cout << "(cursor->size < MAX)" << std::endl;
        int i = 0;
        while (x > cursor->key[i] && i < cursor->size)
          i++;
        for (int j = cursor->size; j > i; j--)
        {
          cursor->key[j] = cursor->key[j - 1];
        }
        cursor->key[i] = x;
        cursor->size++;
        //std::cout<<"posicion_registro: "<<posicion_registro<<"\n";
        //std::cout<<"x: "<<x<<"\n";
        cursor->values.insert(std::make_pair(x, posicion_registro));
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
      }
      else
      {
        //std::cout << "linea 301" << std::endl;

        Node *newLeaf = new Node;
        newLeaf->values.insert(std::make_pair(x, aux));
        int virtualNode[MAX + 1];
        for (int i = 0; i < MAX; i++)
        {
          //std::cout<<"cursor->key[i]: "<<cursor->key[i]<<"\n";
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
         // std::cout << "cursor->key[i]: " << cursor->key[i] << std::endl;
          cursor->key[i] = virtualNode[i];
          cursor->values.insert(std::make_pair(cursor->key[i], aux));
        }
        for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
        {
          newLeaf->key[i] = virtualNode[j];
        }
        if (cursor == root)
        {
          //std::cout << "LINEA 338" << std::endl;

          Node *newRoot = new Node;
          newRoot->key[0] = newLeaf->key[0];
          newRoot->ptr[0] = cursor;
          //std::cout<<"x: "<<x<<" posicion_registro: "<<posicion_registro<<std::endl;
          newRoot->values.insert(std::make_pair(x, posicion_registro));
          newRoot->ptr[1] = newLeaf;
          newRoot->IS_LEAF = false;
          newRoot->size = 1;
          root = newRoot;
        }
        else
        {
          //std::cout << "insertInternal" << std::endl;
          insertInternal(newLeaf->key[0], parent, newLeaf, posicion_registro);
        }
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
        std::cout << cursor->key[i] << " ";
        if (cursor->IS_LEAF)
        {
          std::cout << "posicion " << ((cursor->values).find(cursor->key[i]))->second << "  ";
        }
      }
      std::cout << "\n";
      if (cursor->IS_LEAF != true)
      {
        for (int i = 0; i < cursor->size + 1; i++)
        {
          print_leaf(cursor->ptr[i]);
        }
      }
    }
  }

  /*
    Register find(keyType key){

    }

    std::vector<Register> find(keyType beginKey, keyType endKey){

    }

    void insert(){

    }

    void remove(){

    }
  */
};
