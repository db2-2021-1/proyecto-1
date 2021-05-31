#include <utility>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <iterator>

#include "statement.hpp"

#pragma once

const int MAX = 3;

struct Node
{
    bool IS_LEAF;
    size_t size;
    std::vector<Node*>ptr_;
    std::vector<std::pair<db2::literal, size_t>> position_;
    friend class BPTree;

    class iterator
    {
    private:
        Node* node;
        size_t index;

        void next_value()
        {
            size_t node_size = node->position_.size();

            if(node == nullptr || !node->IS_LEAF) // end
            {
                node = nullptr;
                index = 0;
            }
            else if(index >= node_size-1) // Last element
            {
                node = node->ptr_.back();
                index = 0;
            }
            else
            {
                index++;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<db2::literal, size_t>;
        using pointer = value_type*;
        using reference = value_type&;

        iterator(Node* node, size_t index):
            node(node),
            index(index)
        {};

        iterator(){};

        reference operator*() const
        {
            return node->position_[index];
        }

        pointer operator->()
        {
            return &node->position_[index];
        }

        iterator& operator++()
        {
            next_value();
            return *this;
        }

        iterator operator++(int)
        {
            iterator temp = *this;
            next_value();
            return temp;
        }

        friend bool operator==(const iterator& l, const iterator& r)
        {
            return (l.node == r.node) && (l.index == r.index);
        };

        friend bool operator!=(const iterator& l, const iterator& r)
        {
            return !(l == r);
        }

        Node* get_node()
        {
            return node;
        }

        size_t get_index()
        {
            return index;
        }
    };

    iterator begin()
    {
        return iterator(position_.empty()? nullptr : this, 0);
    }

    iterator end()
    {
        return iterator(nullptr, 0);
    }

    Node():
        ptr_(MAX+1),
        position_(MAX)
    {}
};

class b_plus_tree
{
private:
  Node *root;
  db2::statement::type key_type;
public:
  b_plus_tree(std::string filename, db2::statement::type key_type);
  void insertInternal(db2::literal& x, Node *cursor, Node *child, size_t pos);
  void insert(size_t reg);
  Node* get_root();
  Node *find_parent(Node *cursor, Node *child);


  Node::iterator findNode(const db2::literal& id);

  Node::iterator getMinNode(db2::literal);

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

  Node::iterator end();
};
