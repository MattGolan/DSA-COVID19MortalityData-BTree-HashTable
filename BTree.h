#pragma once

#include<string>
#include<set>
#include<iostream>
using namespace std;


class BTree {

    /*====  Nested Node structure  ====*/

    struct Node {
        // Node variables
        int numberOfKeys;
        set<pair<int,int>> keyList;  //First value is the STAG(id) and second value is the statistic
        Node *childList[4];
        bool leaf;
        pair<int,int> split = {-99,-99};

        // Node constructors.
        Node(pair<int, int> data);
        Node(const set<pair<int,int>>& _keyList);
        Node(const set<pair<int,int>> &_keyList, Node *_childList[4]);
    };

    /*====  Private B Tree Variables  ====*/

    Node* root;
    int length = 99;
    const static int MAXCHILDREN = 3;

    Node *SplitNode(Node* _root);
    void DestroyTree(Node* _root);

public:

    // Tree Constructor.
    BTree();
    Node *InsertNode(Node* _root, pair<int,int> data);
    static int GetMaxLength();
    static int GetMaxChildren();
    int Search(Node* x, int k);
    Node* GetRoot();
    void ModifyNode(Node* _root);
    ~BTree();



};