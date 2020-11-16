#include "hashtable.hpp"
#include <iostream>
using namespace std;

int main()
{
    HashTable<int, int, std::hash<int>, std::equal_to<int>> ht;
    ht.insert(850, 694);
    ht[727] = 549;
    ht[175] = 636;
    ht[175] = 553;
    ht.insert(175,57);
    ht[746] = 847;
    ht[438] = 489;
    ht.printTable();
    return 0;
}