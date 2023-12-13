#include "HashTable.h"

// Initializes state-age-time object.
HashTable::Data::Data()
{
    // Assigns all values with -1.
    numberOfDeathsMale = -1;
    numberOfDeathsFemale = -1;
    numberOfDeathsAll = -1;
}

// Constructor
HashTable::HashTable()
{
    stateMultiplier = 850;
    ageGroupMultiplier = 17;
    for (unsigned int i = 0; i < 44200; i++)
    {
        hashTable[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    for (unsigned int i = 0; i < 44200; i++)
    {
        if (hashTable[i] != nullptr)
        {
            delete hashTable[i];
        }
    }
}

// Hash function to determine the index for a given value.
int HashTable::Hash(const int& _state, const int& _ageGroup, const int& _time) const
{
    // Declares index and sets equal to zero.
    int index = 0;

    // Calculates the index based on data attributes;
    index += (_state * stateMultiplier);
    index += (_time* ageGroupMultiplier);
    index += _ageGroup;

    // Returns calculated index.
    return index;
}

// Creates node pointer and inserts into hash table based on Hash function.
void HashTable::InsertNode(const int& _index)
{
    hashTable[_index] = new Data;
}

// Updates object with data from the table.
void HashTable::CollisionResolution(const int& _gender, const int& _deaths, const int& _index)
{
    // Branches based on gender integer.
    if (_gender == 0)
    {
        hashTable[_index]->numberOfDeathsAll = _deaths;
    }
    else if (_gender == 1)
    {
        hashTable[_index]->numberOfDeathsMale = _deaths;
    }
    else
    {
        hashTable[_index]->numberOfDeathsFemale = _deaths;
    }
}

void HashTable::CheckForNode(const int& _state, const int& _ageGroup, const int& _gender, const int& _time, const int& _deaths)
{
    // Calls Hash function to determine index.
    int index = Hash(_state, _ageGroup, _time);
    // Inserts node if index location is empty
    if (hashTable[index] == nullptr)
    {

        InsertNode(index);

    }
    CollisionResolution(_gender, _deaths, index);
}


int HashTable::GetValue(const int& _state, const int& _ageGroup, const int& _gender, const int& _time) const
{
    // Calls hash function t o determine relevant index location.
    int index = Hash(_state, _ageGroup, _time);

    // Branches based on gender integer and returns death data.
    if (hashTable[index] == nullptr)
    {
        return -1;
    }
    if (_gender == 0)
    {
        return hashTable[index]->numberOfDeathsAll;
    }
    else if (_gender == 1)
    {
        return hashTable[index]->numberOfDeathsMale;
    }
    else
    {
        return hashTable[index]->numberOfDeathsFemale;
    }

}