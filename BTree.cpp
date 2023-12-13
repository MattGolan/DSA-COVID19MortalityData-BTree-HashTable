#include "BTree.h"

// Constructor 1 - creates node with passed in value and sets other parameters to defaults.
BTree::Node::Node(pair<int, int> data)
{
    numberOfKeys = 1;
    keyList.insert(data);
    for (unsigned int i = 0; i <= GetMaxChildren(); i++)
    {
        childList[i] = nullptr;
    }
    leaf = true;
}

// Constructor 2 - creates a new node with split of previous node (leaf).
BTree::Node::Node(const set<pair<int,int>> &_keyList)
{
    numberOfKeys = 49;
    keyList = _keyList;
    for (unsigned int i = 0; i <= GetMaxChildren(); i++)
    {
        childList[i] = nullptr;
    }
    leaf = true;
}

// Constructor 3 - creates a new node with split of previous node (non-leaf).
BTree::Node::Node(const set<pair<int,int>>& _keyList, Node* _childList[4]) {
    numberOfKeys = 1;
    keyList = _keyList;
    for (unsigned int i = 0; i <= GetMaxChildren(); i++)
    {
        childList[i] = _childList[i];
    }
    leaf = false;
}

/*====  B Tree Class  ====*/

// Private variables

// Performs split function for nodes.
BTree::Node* BTree::SplitNode(Node* _root)
{
    // Finds the middle value.
    set<pair<int,int>>::iterator itr_1;
    set<pair<int,int>>::iterator itr_2 = _root->keyList.begin();
    bool slow = true;
    if (_root->leaf)
    {
        // Fast and slow iterator approach.
        for (itr_1 = _root->keyList.begin(); itr_1 != _root->keyList.end(); ++itr_1)
        {
            if (slow)
            {
                ++itr_2;
            }
            slow = !slow;
        }
    }
    else if (!_root->leaf)
    {
        itr_2 = next(_root->keyList.begin());
    }

    // creates a set for the first half of keys.
    set<pair<int, int>> firstHalf;
    for (itr_1 = _root->keyList.begin(); itr_1 != itr_2; ++itr_1)
    {
        firstHalf.insert(*itr_1);
    }
    // creates a set for the second half of keys.
    set<pair<int, int>> secondHalf;
    if (_root->leaf)
    {
        for (itr_1 = itr_2++; itr_1 != _root->keyList.end(); ++itr_1)
        {
            secondHalf.insert(*itr_1);
        }
    }
    else
    {
        for (itr_1 = next(itr_2); itr_1 != _root->keyList.end(); ++itr_1)
        {
            secondHalf.insert(*itr_1);
        }
    }
    // Updates split integer with the values of the middle key.
    _root->split = {itr_2->first, itr_2->second};  //First value of itr_2 refers to the first value of the pair, i.e. STAG

    // Constructor call for leaf nodes.
    if (_root->leaf)
    {
        _root->childList[0] = new Node(firstHalf);
        _root->childList[1] = new Node(secondHalf);
    }

        // Constructor call for non-leaf nodes.
    else if (!_root->leaf)
    {
        // Splits pointers between two new nodes.
        Node* leftChildList[4];
        leftChildList[0] = _root->childList[0];
        leftChildList[1] = _root->childList[1];
        leftChildList[2] = nullptr;
        leftChildList[3] = nullptr;

        Node* rightChildList[4];
        rightChildList[0] = _root->childList[2];
        rightChildList[1] = _root->childList[3];
        rightChildList[2] = nullptr;
        rightChildList[3] = nullptr;

        // Calls constructor
        _root->childList[0] = new Node(firstHalf,leftChildList);
        _root->childList[1] = new Node(secondHalf,rightChildList);
        _root->childList[2] = nullptr;
        _root->childList[3] = nullptr;
    }

    //Updates value in node pushed upward.
    _root->leaf = false;
    _root->keyList.clear();
    _root->keyList.insert(_root->split);
    _root->numberOfKeys = 1;
    return _root;
}

void BTree::DestroyTree(Node* _root){
    if (_root != nullptr)
    {
        // Recursively delete child nodes
        for (int i = 0; i <= GetMaxChildren(); ++i)
        {
            DestroyTree(_root->childList[i]);
        }

        // Delete the current node
        delete _root;
    }
}

// Constructor
BTree::BTree()
{
    root = nullptr;
}

// Destructor
BTree::~BTree() {
    DestroyTree(root);
}

// Inserts new node. Credit to Aman in Module 3 lecture videos for insertion strategy.
BTree::Node* BTree::InsertNode(Node* _root, pair<int,int> data)
{
    // Current pointer is a nullptr.
    if (_root == nullptr) {
        // Insert new node.
        _root = new Node(data);
        return _root;
    }
        // Root points at leaf node.
    else if (_root->leaf) {
        // Adds new key to leaf.
        _root->keyList.insert(data);
        _root->numberOfKeys += 1;
        // Checks whether leaf is overfilled with keys.
        if (_root->numberOfKeys == GetMaxLength())
        {
            _root = SplitNode(_root);
        }
        return _root;
    }
        // Current pointer points to a non-leaf node.
    else
    {
        // Compares each key in the node with the new state-time-age code and recursively calls a section.
        auto itr = _root->keyList.begin();

        if (itr->first > data.first)
        {
            _root->childList[0] = InsertNode(_root->childList[0], data);
        }
        else if ( _root->keyList.size()== 1)
        {
            _root->childList[1] = InsertNode(_root->childList[1], data);
        }
        else if (std::next(itr)->first > data.first)
        {
            _root->childList[1] = InsertNode(_root->childList[1], data);
        }
        else
        {
            _root->childList[2] = InsertNode(_root->childList[2], data);
        }
    }

    // Checks whether a split occurred (leaf or non-leaf)
    for (unsigned int i = 0; i < _root->numberOfKeys +1; i++)
    {
        // If yes, updates "root" node.
        if (_root->childList[i]->split.first != -99)
        {
            // Inserts split key into the node.
            _root->keyList.insert(_root->childList[i]->split);
            _root->numberOfKeys += 1;

            // Creates a new list of pointers and adds pointers to the correct location in pointer array.
            Node *newChildList[MAXCHILDREN+1];
            if (i == 0)
            {
                newChildList[0] = _root->childList[i]->childList[0];
                newChildList[1] = _root->childList[i]->childList[1];
                newChildList[2] = _root->childList[1];
                newChildList[3] = _root->childList[2];
            }
            else if (i == 1)
            {
                newChildList[0] = _root->childList[0];
                newChildList[1] = _root->childList[i]->childList[0];
                newChildList[2] = _root->childList[i]->childList[1];
                newChildList[3] = _root->childList[2];
            }
            else if (i == 2)
            {
                newChildList[0] = _root->childList[0];
                newChildList[1] = _root->childList[1];
                newChildList[2] = _root->childList[i]->childList[0];
                newChildList[3] = _root->childList[i]->childList[1];
            }
            else
            {
                cout << "Error: Number of keys not aligned with expectations" << endl;
            }

            Node* tempNode = _root->childList[i];
            for (unsigned int j = 0; j <= GetMaxChildren(); j++)
            {
                _root->childList[j] = newChildList[j];
            }
            // Deletes the old node value.
            delete tempNode;
        }
    }

    // Post splitting, checks whether the "root" node is overfilled
    if (_root->keyList.size() >= MAXCHILDREN)
    {
        _root = SplitNode(_root);
    }

    // Returns non-leaf node.
    return _root;
}

int BTree::GetMaxLength()
{
    return 99;
}

int BTree::GetMaxChildren()
{
    return MAXCHILDREN;
}

BTree::Node* BTree::GetRoot()
{
    return root;
}

// Traverses the B tree and returns mortality data.
int BTree::Search(BTree::Node* _root, int STAG)
{

    // Iterates through set and returns the value if in keyList;
    set<pair<int, int>>::iterator itr;
    for (itr =_root->keyList.begin(); itr != _root->keyList.end(); itr++) {
        if (itr->first == STAG) {
            return itr->second;
        }
    }
    // Branches based on leaf status.
    if (!_root->leaf)
    {
        // Compares each key in the node with the new state-time-age code and recursively calls a section.
        if (_root->keyList.begin()->first > STAG)
        {
            return Search(_root->childList[0], STAG);
        }
        else if (_root->keyList.begin()->first < STAG && (_root->keyList.size() == 1 || next((_root->keyList.begin()))->first > STAG))
        {
            return Search(_root->childList[1], STAG);
        } else {
            return Search(_root->childList[2], STAG);
        }
    }
    else
    {
        // Only reached if error occurs.
        return -87;
    }


}

// Modifies AVL Tree by updating root node.
void BTree::ModifyNode(Node* _root)
{
    root = _root;
}