
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

OrgChart &OrgChart::add_root(const string &vertex_name)
{
    /**
     * @brief Construct a new if object with the given vertex_name
     *
     */
    if (root_tree == nullptr)
    {
        root_tree = new Node(vertex_name);
    }
    else
    {

        root_tree->value = vertex_name;
    }
    return *this;
}

OrgChart &OrgChart::add_sub(const string &exsist, const string &insert_param)
{
    Node *found = find_n(exsist, root_tree);
    if (found == nullptr)
    {
        throw invalid_argument("not father exsist");
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

OrgChart::~OrgChart()
{

    /**
     * @brief destructor
     */

    /*
 i had my own implementation for the function but it was not good for the tests
 i searched the web and used geeksforgeeks implemetation with stack and a queue
 */
    // https://www.geeksforgeeks.org/reverse-level-order-traversal/
    if (root_tree == nullptr)
    {
        throw std::out_of_range("not good tree sended");
    }

    stack<Node *> Stack;
    queue<Node *> Queue;

    Queue.push(root_tree);

    while (!Queue.empty())
    {
        /* Dequeue node and make it root */
        auto root = Queue.front();
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
        delete Stack.top();
        Stack.pop();
    }
}

OrgChart::Node::Node(const string &value)
{
    this->value = string(value);
}

// clang++-9 -std=c++2a -Wall -Wextra -Werror -g -o main OrgChart.cpp

// int main()
// {

//     OrgChart chart2;
//     chart2.add_root("Owner");
//     chart2.add_root("New_Owner");
//     // CHECK_THROWS(chart2.add_sub("Owner", "General Manager"));        // Owner was replaced
//     // CHECK_THROWS(chart2.add_sub("Lebron James", "General_Manager")); // no such value
//     chart2.add_sub("New_Owner", "General_Manager");
//     chart2.add_sub("New_Owner", "Share_Holders");
//     chart2.add_sub("New_Owner", "Head_of_operations");
//     chart2.add_sub("General_Manager", "Head_Coach");
//     chart2.add_sub("General_Manager", "Assistant_Coach");
//     chart2.add_sub("Head_of_operations", "Team_Doctor");
//     chart2.add_sub("Head_of_operations", "Head_Scout");
//     chart2.add_sub("Share_Holders", "Sub_Share_Holder1");
//     chart2.add_sub("Share_Holders", "Sub_Share_Holder2");
//     // CHECK_THROWS(chart2.add_sub("head_of_operations", "Cheerleader")); // Head not head
//     chart2.add_sub("Head_Coach", "Captain");
//     chart2.add_sub("Team_Doctor", "Team_Physiotherapist");
//     cout << "begin level order " << endl;
//     cout << endl;
//     // test level order
//     vector<string> lvl_order;
//     for (auto it = chart2.begin_level_order(); it != chart2.end_level_order(); ++it)
//     {
//         lvl_order.push_back(*it);
//         cout << *it << " ";
//     }
//     cout << endl;
//     cout << endl;
//     cout << endl;

//     if (lvl_order.at(0).compare("New_Owner"))
//     {
//         cout << "level order test failed 11111 new Owner" << endl;
//     }
//     if (lvl_order.at(1).compare("General_Manager"))
//     {
//         cout << "level order test failed General_Manager" << endl;
//     };
//     if (lvl_order.at(2).compare("Share_Holders"))
//     {
//         cout << "level order test failed Share_Holders" << endl;
//     };
//     if (lvl_order.at(3).compare("Head_of_operations"))
//     {
//         cout << "level order test failed Head_of_operations" << endl;
//     };
//     if (lvl_order.at(4).compare("Head_Coach"))
//     {
//         cout << "level order test failed Head_Coach" << endl;
//     };
//     if (lvl_order.at(5).compare("Assistant_Coach"))
//     {
//         cout << "level order test failed Assistant_Coach" << endl;
//     };
//     if (lvl_order.at(6).compare("Sub_Share_Holder1"))
//     {
//         cout << "level order test failed Sub_Share_Holder1 " << endl;
//     };
//     if (lvl_order.at(7).compare("Sub_Share_Holder2"))
//     {
//         cout << "level order test failed Sub_Share_Holder2" << endl;
//     };
//     if (lvl_order.at(8).compare("Team_Doctor"))
//     {
//         cout << "level order test failed Team_Doctor" << endl;
//     };
//     if (lvl_order.at(9).compare("Head_Scout"))
//     {
//         cout << "level order test failed Head_Scout" << endl;
//     };
//     if (lvl_order.at(10).compare("Captain"))
//     {
//         cout << "level order test failed Captain" << endl;
//     };
//     if (lvl_order.at(11).compare("Team_Physiotherapist"))
//     {
//         cout << "level order test failed Team_Physiotherapist" << endl;
//     };

//     cout << endl;

//     lvl_order.clear();

//     // // add another field and check that it works
//     chart2.add_sub("Assistant_Coach", "Water_Boy");

//     cout << "check operator << " << endl;
//     cout << chart2 << endl;

//     cout << endl;
//     cout << endl;
//     cout << endl;

//     cout << "check default iterator " << endl;
//     // check default iterator

//     for (auto name : chart2)
//     {
//         lvl_order.push_back(name);
//         cout << name << " ";
//     }
//     cout << endl;
//     cout << endl;
//     cout << endl;

//     if (lvl_order.at(0).compare("New_Owner"))
//     {
//         cout << "level order test failed 11111 new Owner" << endl;
//     };
//     if (lvl_order.at(1).compare("General_Manager"))
//     {
//         cout << "level order test failed General_Manager" << endl;
//     };

//     if (lvl_order.at(2).compare("Share_Holders"))
//     {
//         cout << "level order test failed Share_Holders" << endl;
//     };
//     if (lvl_order.at(3).compare("Head_of_operations"))
//     {
//         cout << "level order test failed Head_of_operations" << endl;
//     };
//     if (lvl_order.at(4).compare("Head_Coach"))
//     {
//         cout << "level order test failed Head_Coach" << endl;
//     };
//     if (lvl_order.at(5).compare("Assistant_Coach"))
//     {
//         cout << "level order test failed Assistant_Coach" << endl;
//     };

//     if (lvl_order.at(6).compare("Sub_Share_Holder1"))
//     {
//         cout << "level order test failed Sub_Share_Holder1" << endl;
//     };

//     if (lvl_order.at(7).compare("Sub_Share_Holder2"))
//     {
//         cout << "level order test failed Sub_Share_Holder2" << endl;
//     };
//     if (lvl_order.at(8).compare("Team_Doctor"))
//     {
//         cout << "level order test failed Team_Doctor" << endl;
//     };

//     if (lvl_order.at(9).compare("Head_Scout"))
//     {
//         cout << "level order test failed Head_Scout" << endl;
//     };

//     if (lvl_order.at(10).compare("Captain"))
//     {
//         cout << "level order test failed Captain" << endl;
//     };

//     if (lvl_order.at(11).compare("Water_Boy"))
//     {
//         cout << " level order test faild Water_boy";
//     }

//     if (lvl_order.at(12).compare("Team_Physiotherapist"))
//     {
//         cout << " level order test faild Team_Pthaouraoist";
//     }
//     cout << endl;

//     cout << "test reverse levelr order " << endl;

//     // // test reverse level order
//     for (auto it = chart2.begin_reverse_order(); it != chart2.reverse_order(); ++it)
//     {
//         cout << *it << " ";
//     }
//     cout << endl;
//     cout << endl;
//     cout << endl;

//     cout << " begin preorder " << endl;

//     for (auto it = chart2.begin_preorder(); it != chart2.end_preorder(); ++it)
//     {
//         cout << *it << " ";
//     }

//     cout << endl;
//     cout << endl;
//     cout << endl;
// }