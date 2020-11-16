#include "hashtable.hpp"
#include <iostream>
using namespace std;

int main()
{
    HashTable<int, int, std::hash<int>, std::equal_to<int>> ht;
    ht.insert(426, 851);
    ht.insert(426, 158);
    //ht.insert(682, 589);
    return 0;
}