#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include "kdtree.hpp"
using namespace std;
int main(){
    int k1[5] = {1,2,3,4,5};
    int k2[5] = {2,7,8,9,0};
    int k3[5] = {3,4,3,2,1};
    int k4[5] = {4,9,8,7,6};
    std::vector<int> v1(k1, k1+5);
    std::vector<int> v2(k2, k2+5);
    std::vector<int> v3(k3, k3+5);
    std::vector<int> v4(k4, k4+5);
    auto t1 = vector2tuple<5>(v1);
    auto t2 = vector2tuple<5>(v2);
    auto t3 = vector2tuple<5>(v3);
    auto t4 = vector2tuple<5>(v4);
    auto p1 = make_pair(t1, 1);
    auto p2 = make_pair(t2, 2);
    auto p3 = make_pair(t3, 3);
    auto p4 = make_pair(t4, 4);
    vector<decltype(p1)> v;
    v.push_back(p1);
    v.push_back(p2);
    v.push_back(p3);
    v.push_back(p4);
    KDTree<decltype(t1), int> kdTree(v);
    return 0;
}