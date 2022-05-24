//
// Created by Dolev_Lenovo on 22/05/2022.
//

#include "OrgChart.hpp"
#include <stack>
#include <queue>

using namespace ariel;

void OrgChart::Iterator::generate_begin_reverse_order_iterator(OrgChart::Node *root)
{
    /*
    i had my own implementation for the function but it was not good for the tests
    i searched the web and used geeksforgeeks implemetation with stack and a queue
    */
    // https://www.geeksforgeeks.org/reverse-level-order-traversal/
    if (root == nullptr)
    {
        throw std::out_of_range("not good tree sended");
    }

    stack<Node *> Stack;
    queue<Node *> Queue;

    Queue.push(root);

    while (!Queue.empty())
    {
        /* Dequeue node and make it root */
        root = Queue.front();
        Queue.pop();
        Stack.push(root);
        // https://stackoverflow.com/questions/3610933/iterating-c-vector-from-the-end-to-the-beginning
        /* Enqueue from the right side first */
        for (auto i = root->children.rbegin(); i != root->children.rend(); ++i)
        {
            Queue.push(*i); //
        }
    }
    // Now pop all items from stack one by one and print them
    while (!Stack.empty())
    {
        inner_list.push_back(Stack.top());
        Stack.pop();
    }
}

void OrgChart::Iterator::generate_begin_level_order_iterator(OrgChart::Node *root)
{

    /*
  i had my own implementation for the function but it was not good for the tests
  i searched the web and used geeksforgeeks implemetation with stack and a queue
  */
    // https://www.geeksforgeeks.org/level-order-tree-traversal/

    if (root == nullptr)
    {
        throw std::out_of_range("not good tree sended");
    }

    // Create an empty queue for level order traversal
    queue<Node *> Queue;

    // Enqueue Root and initialize height
    Queue.push(root);

    while (!Queue.empty())
    {
        // Print front of queue and remove it from queue
        Node *temp = Queue.front();
        Queue.pop();
        inner_list.push_back(temp);

        // Enqueue all children of removed item
        for (auto i = temp->children.begin(); i != temp->children.end(); ++i)
        {
            Queue.push(*i);
        }
    }
}

void OrgChart::Iterator::generate_begin_preorder_iterator(OrgChart::Node *root)
{
    if (root == nullptr)
    {
        throw std::out_of_range("not good tree sended");
    }
    // https://www.tutorialspoint.com/data_structures_algorithms/tree_traversal_in_c.htm#

    inner_list.push_back(root);

    // loop over the children and call generate_begin_preorder_iterator on them also
    for (auto i = root->children.begin(); i != root->children.end(); ++i)
    {
        generate_begin_preorder_iterator(*i);
    }
}

OrgChart::Iterator::Iterator(OrgChart::Node *root, type_of_request type)
{
    if (root == nullptr)
    {
        throw std::out_of_range("not good tree sended");
    }
    switch (type)
    {
    case begin_reverse_order_enum:
        generate_begin_reverse_order_iterator(root);
        current = *inner_list.begin();
        break;
    case reverse_order_enum:
        current = end_helper_iterator;
        break;
    case begin_level_order_enum:
        generate_begin_level_order_iterator(root);
        current = *inner_list.begin();
        break;
    case end_level_order_enum:
        current = end_helper_iterator;
        break;
    case begin_preorder_enum:
        generate_begin_preorder_iterator(root);
        current = *inner_list.begin();
        break;
    case end_preorder_enum:
        current = end_helper_iterator;
        break;
    }
}

string &OrgChart::Iterator::operator*() const
{
    return current->value;
}

string *OrgChart::Iterator::operator->() const
{
    return &(current->value);
}

OrgChart::Iterator &ariel::OrgChart::Iterator::operator++()
{

    if (current == end_helper_iterator)
    {
        throw std::out_of_range("iterator is out of range");
    }
    if (inner_list.empty())
    {
        current = end_helper_iterator;
        return *this;
    }
    inner_list.erase(inner_list.begin());
    current = *inner_list.begin();
    return *this;
}

OrgChart::Iterator OrgChart::Iterator::operator++(int)
{
    OrgChart::Iterator temp = *this;
    ++(*this);
    return temp;
}

bool OrgChart::Iterator::operator==(const OrgChart::Iterator &other) const
{
    return this->current == other.current;
}

bool OrgChart::Iterator::operator!=(const OrgChart::Iterator &other) const
{
    return !(*this == other);
}

ostream &ariel::operator<<(ostream &os, const ariel::OrgChart &tree)
{
    /**
     * @brief print the tree
     *
     *
     */
    OrgChart::Iterator it = tree.begin();
    while (it != tree.end())
    {
        os << *it << " ";
        ++it;
    }
    return os;
}

/**
 * @brief makeing use of the switch case to make the iterator work
 *  with the enums
 * @return OrgChart::Iterator
 */

OrgChart::Iterator OrgChart::begin() const
{
    return Iterator(root_tree, begin_level_order_enum);
}

OrgChart::Iterator OrgChart::end() const
{
    return Iterator(root_tree, end_level_order_enum);
}

OrgChart::Iterator OrgChart::begin_reverse_order() const
{
    return Iterator(root_tree, begin_reverse_order_enum);
}

OrgChart::Iterator OrgChart::reverse_order() const
{
    return Iterator(root_tree, reverse_order_enum);
}

OrgChart::Iterator OrgChart::begin_level_order() const
{
    return Iterator(root_tree, begin_level_order_enum);
}

OrgChart::Iterator OrgChart::end_level_order() const
{
    return Iterator(root_tree, end_level_order_enum);
}

OrgChart::Iterator OrgChart::begin_preorder() const
{
    return Iterator(root_tree, begin_preorder_enum);
}

OrgChart::Iterator OrgChart::end_preorder() const
{
    return Iterator(root_tree, end_preorder_enum);
}

OrgChart &OrgChart::add_root(const string &vertex_param)
{
    /**
     * @brief Construct a new if object with the given vertex_param
     *
     */
    if (root_tree == nullptr)
    {
        root_tree = new Node(vertex_param);
    }
    else
    {
        // free the old root and make a new one
        delete root_tree;
        root_tree = new Node(vertex_param);
        // root_tree->value = vertex_param;
    }
    return *this;
}

OrgChart &OrgChart::add_sub(const string &exsist, const string &insert_param)
{
    Node *found = find_n(exsist, root_tree);
    if (found == nullptr)
    {
        throw invalid_argument("not exist");
    }
    if (found->children.empty())
    {
        Node *temp = new Node(insert_param);
        found->children.push_back(temp);
    }
    else
    {
        Node *temp = new Node(insert_param);
        found->children.push_back(temp);
    }
    return *this;
}

OrgChart::OrgChart(const OrgChart &other)
{
    /**
     * @brief copy constructor
     * @param other
     */

    root_tree = new Node(other.root_tree->value);
    for (auto i = other.root_tree->children.begin(); i != other.root_tree->children.end(); ++i)
    {
        add_sub(root_tree->value, (*i)->value);
    }
}

OrgChart::OrgChart(OrgChart &&other) noexcept
{
    /**
     * @brief move constructor
     * @param other
     */
    root_tree = other.root_tree;
    other.root_tree = nullptr;
}

OrgChart &OrgChart::operator=(OrgChart other)
{
    /**
     * @brief move assignment operator
     * @param other
     */
    
    swap(root_tree, other.root_tree);
    return *this;
}

OrgChart::Node *OrgChart::find_n(const string &find, OrgChart::Node *node)
{

    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->value == find)
    {
        return node;
    }

    for (auto &child : node->children)
    {
        Node *found = find_n(find, child);
        if (found != nullptr)
        {
            return found;
        }
    }
    return nullptr;
}



OrgChart::Node::Node(const string &value)
{

    this->value = string(value);
}


// clang++-9 -std=c++2a -Wall -Wextra -Werror -g -o OrgChart OrgChart.cpp