#include <utility>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

#pragma once

const int MAX = 3;

using keyType = int;

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

int xddd = 0;

class b_plus_tree
{
private:
  Node *root;
  keyType firstid;

  void insert_internal(int x, Node *cursor, Node *child, long posicion, bool flag);
  Node *find_parent(Node *cursor, Node *child);

public:
  b_plus_tree(std::string filename);
  
  Node *find_node(int id);
  size_t find(int id);
  void insert(size_t reg);
  void display(Node *cursor);
  Node* get_root();
  void print_leaf(Node *cursor);
  void remove(keyType key);
  std::vector<size_t> find(keyType beginKey, keyType endKey);

};
