#include <iostream>
#include <vector>
#include "register.h"
#define MAX 3

struct Node
{
  bool IS_LEAF;
  int *key, size;
  Node **ptr;
  friend class BPTree;

  Node()
  {
    key = new int[MAX];
    ptr = new Node *[MAX + 1];
  }
};

class BPlusTree
{
private:
  Node *root;
  void insertInternal(int, Node *, Node *);
  Node *findParent(Node *, Node *);

public:
  //BPlusTree(){}
  BPlusTree()
  {
    root = NULL;
  }
  Register find(int id)
  {
    if (root == NULL)
    {
      cout << "Tree is empty\n";
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
          cout << "Found\n";
          return;
        }
      }
      cout << "Not found\n";
    }
  }
  void insert(int x)
  {
    if (root == NULL)
    {
      root = new Node;
      root->key[0] = x;
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
        }
        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
      }
      else
      {
        Node *newLeaf = new Node;
        int virtualNode[MAX + 1];
        for (int i = 0; i < MAX; i++)
        {
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
        cursor->ptr[cursor->size] = newLeaf;
        newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
        cursor->ptr[MAX] = NULL;
        for (i = 0; i < cursor->size; i++)
        {
          cursor->key[i] = virtualNode[i];
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
          newRoot->ptr[1] = newLeaf;
          newRoot->IS_LEAF = false;
          newRoot->size = 1;
          root = newRoot;
        }
        else
        {
          insertInternal(newLeaf->key[0], parent, newLeaf);
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
        cout << cursor->key[i] << " ";
      }
      cout << "\n";
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

  /**Falta implementar
    Register find(keyType key){

    }
*/
  /**Falta implementar busqueda por rangos*/
  std::vector<Register> find(keyType beginKey, keyType endKey)
  {
  }

  /**Falta implementar*/
  void insert()
  {
  }

  /**Falta implementar*/
  void erase()
  {
  }
};
