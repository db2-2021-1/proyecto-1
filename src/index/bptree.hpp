#include <utility>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

#include "statement.hpp"

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

  // Type and size of the key.
  db2::statement::type key_type;

  void insert_internal(int x, Node *cursor, Node *child, long posicion, bool flag);
  Node *find_parent(Node *cursor, Node *child);

public:
  b_plus_tree(std::string filename, db2::statement::type key_type);

  Node *find_node(int id);
  size_t find(int id);
  void insert(size_t reg);
  void display(Node *cursor);
  Node* get_root();
  void print_leaf(Node *cursor);
  void remove(keyType key);
  std::vector<size_t> find(keyType beginKey, keyType endKey);

  /// ... WHEN name IS key
  std::vector<size_t> get_positions(
    const db2::statement::literal& key
  );

  /// ... WHEN name IS BETWEEN ge AND le
  std::vector<size_t> get_positions(
    const db2::statement::literal& key_ge,
    const db2::statement::literal& key_le
  );

  /// INSERT
  bool insert(const db2::statement::literal& key, size_t position);

  /// DELETE FROM
  bool delete_from(const db2::statement::literal& key, size_t position);
};
