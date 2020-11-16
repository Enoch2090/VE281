#include "hashtable.hpp"
#include <iostream>
using namespace std;

int main()
{
    HashTable<int, int, std::hash<int>, std::equal_to<int>> ht;
    ht.insert(1, 2);
    ht.insert(3, 4);
    ht.insert(5, 6);
    ht.insert(7, 8);
    cout << ht[1] << ht[3] << ht[5] << ht[7] << ht[6] << endl;
    return 0;
}