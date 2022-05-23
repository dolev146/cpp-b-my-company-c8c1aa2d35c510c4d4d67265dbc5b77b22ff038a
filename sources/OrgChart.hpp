//
// Created by Dolev_Lenovo on 22/05/2022.
//

#ifndef ORGCHART_ORGCHART_HPP
#define ORGCHART_ORGCHART_HPP

/*
 * https://en.cppreference.com/w/cpp/language/noexcept noexcept explanation
 * The noexcept operator performs a compile-time check that returns true
 * if an expression is declared to not throw any exceptions.
 * It can be used within a function template's noexcept specifier to declare that the function will
 * throw exceptions for some types but not others.
 */

#include <iostream>
#include <unordered_map>
#include <utility>
#include <ostream>
#include <list>
#include <unordered_map>
#include <string>
#include <vector>
#include <exception>

// https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings

using namespace std;
namespace ariel
{

    enum type_of_request
    {
        reverse_order_enum,
        begin_reverse_order_enum,
        begin_level_order_enum,
        end_level_order_enum,
        begin_preorder_enum,
        end_preorder_enum

    };

    class OrgChart
    {

    private:
        class Node
        {
        public:
            explicit Node(const string &);

            vector<Node *> children;
            string value;
        };
        Node *find_n(const string &, Node *);

    public:
        Node *root_tree = nullptr;
        
        OrgChart() = default;
        // Shallow copy
        OrgChart(OrgChart &&) noexcept;
        // deep copy
        OrgChart(const OrgChart &);
        //------Operators-----
        OrgChart &operator=(OrgChart);
        ~OrgChart() = default;
        OrgChart &add_root(const string &);
        OrgChart &add_sub(const string &exsist, const string &insert_);

        class Iterator
        {
        public:
            Node *current;
            constexpr static Node* end_helper_iterator = nullptr;
            list<Node *> inner;
            Iterator() : current(nullptr) {}
            explicit Iterator(Node *temp) : current(temp) {}
            void generate_begin_reverse_order_iterator(Node *);
            void generate_begin_level_order_iterator(Node *);
            void generate_begin_preorder_iterator(Node *);

            Iterator(Node *, type_of_request);
            string &operator*() const;
            string *operator->() const;
            Iterator &operator++();
             Iterator operator++(int);
            bool operator==(const Iterator &) const;
            bool operator!=(const Iterator &) const;
        };

        Iterator begin() const;
        Iterator end() const;
        friend ostream &operator<<(ostream &, const OrgChart &);
        Iterator begin_reverse_order() const;
        Iterator reverse_order() const;
        Iterator begin_level_order() const;
        Iterator end_level_order() const;
        Iterator begin_preorder() const;
        Iterator end_preorder() const;
    };

}

#endif // ORGCHART_ORGCHART_HPP
