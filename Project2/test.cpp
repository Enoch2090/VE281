#include "hashtable.hpp"
#include <iostream>
using namespace std;

bool eql(int a, int b)
{
    return a == b;
}

int main()
{
    HashTable<int, int, std::hash<int>, std::equal_to<int>> ht;
    return 0;
}