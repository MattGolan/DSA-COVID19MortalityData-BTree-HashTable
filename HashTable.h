#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class HashTable
{
    // Define a structure to store COVID-19 deaths by gender.
    struct Data
    {
        // Variable declarations
        int numberOfDeathsMale;
        int numberOfDeathsFemale;
        int numberOfDeathsAll;

        // constructor
        Data();
    };

    // Variable declarations
    Data* hashTable[44200];
    int stateMultiplier;
    int ageGroupMultiplier;

    // Hash function to determine the index for a given value.
    int Hash(const int& _state, const int& _ageGroup, const int& _time) const;

public:

    // Constructor
    HashTable();

    // Destructor
    ~HashTable();

    // Insert a new node into the hash table.
    void InsertNode(const int& _index);

    // Updates gender-specific data of a node.
    void CollisionResolution(const int& _gender, const int& _deaths, const int& _index);

    // Calls combination of InsertNode() and ModifyNode() depending on whether the node exists.
    void CheckForNode(const int& _state, const int& _ageGroup, const int& _gender, const int& _time, const int& _deaths);

    // Retrieve the value associated with a given key
    int GetValue(const int& _state, const int& _ageGroup, const int& _gender, const int& _time) const;

};