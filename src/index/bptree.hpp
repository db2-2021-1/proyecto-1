#include <utility>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include "statement.hpp"

#pragma once

const int MAX = 3;

using keyType = int;

struct Node
{
    bool IS_LEAF;
    //db2::literal *key;
    int size;
    //Node **ptr; // std::vector<>
    //std::map<db2::literal, size_t> position;

    std::vector<Node*>ptr_;
    std::vector<std::pair<db2::literal, size_t>> position_;
    //int -> id, long -> posicion del registro
    friend class BPTree;
    Node():ptr_(MAX+1){}
};

class b_plus_tree
{
private:
  Node *root;
  keyType firstid;
  std::map<keyType,size_t> position;

  // Type and size of the key.
  db2::statement::type key_type;

  //void insert_internal(db2::literal& x, Node *cursor, Node *child, long posicion, size_t flag);
  //Node *find_parent(Node *cursor, Node *child);

public:
  b_plus_tree(std::string filename, db2::statement::type key_type);

  void insertInternal(db2::literal& x, Node *cursor, Node *child, size_t pos);
  Node *find_node(int id);
  std::vector<size_t> find(keyType low);
  void insert(size_t reg);
  void display(Node *cursor);
  Node* get_root();
  void print_leaf(Node *cursor);
  void remove(keyType key);
  std::vector<size_t> find(keyType beginKey, keyType endKey);
  Node *find_parent(Node *cursor, Node *child);
  size_t getPos(db2::literal);


  auto findNode(const db2::literal& id);

  auto getMinNode(db2::literal);

  /// ... WHEN name IS key
  std::vector<size_t> get_positions(
    const db2::literal& key
  );

  /// ... WHEN name IS BETWEEN ge AND le
  std::vector<size_t> get_positions(
    const db2::literal& key_ge,
    const db2::literal& key_le
  );

  /// INSERT
  bool insert(const db2::literal& key, size_t position);

  /// DELETE FROM
  bool delete_from(const db2::literal& key, size_t position);
};
