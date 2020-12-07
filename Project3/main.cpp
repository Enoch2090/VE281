#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include "kdtree.hpp"
using namespace std;
int main(){
    int k1[3] = {0,0,0};
    int k2[3] = {0,0,0};
    int k3[3] = {-1,2,0};
    int k4[3] = {1,-1,0};
    int k5[3] = {0,0,0};
    int k6[3] = {-1,2,-4};
    int k7[3] = {1,-1,-10};
    int k8[3] = {3,-2,3};
    int k9[3] = {4,-3,2};
    int k10[3] = {-2,4,9};
    int k11[3] = {-3,1,1};
    int k12[3] = {-6,-10,-7};
    int k13[3] = {1,-1,-10};
    int k14[3] = {16,23,-23};
    int k15[3] = {-23,32,34};
    int k16[3] = {-11,12,31};
    int k17[3] = {49,-11,-24};
    int k18[3] = {33,44,70};
    int k19[3] = {-100,-100,-100};
    std::vector<int> v1(k1, k1+3);
    std::vector<int> v2(k2, k2+3);
    std::vector<int> v3(k3, k3+3);
    std::vector<int> v4(k4, k4+3);
    std::vector<int> v5(k5, k5+3);
    std::vector<int> v6(k6, k6+3);
    std::vector<int> v7(k7, k7+3);
    std::vector<int> v8(k8, k8+3);
    std::vector<int> v9(k9, k9+3);
    std::vector<int> v10(k10, k10+3);
    std::vector<int> v11(k11, k11+3);
    std::vector<int> v12(k12, k12+3);
    std::vector<int> v13(k13, k13+3);
    std::vector<int> v14(k14, k14+3);
    std::vector<int> v15(k15, k15+3);
    std::vector<int> v16(k16, k16+3);
    std::vector<int> v17(k17, k17+3);
    std::vector<int> v18(k18, k18+3);
    std::vector<int> v19(k19, k19+3);
    auto t1 = vector3tuple<3>(v1);
    auto t2 = vector3tuple<3>(v2);
    auto t3 = vector3tuple<3>(v3);
    auto t4 = vector3tuple<3>(v4);
    auto t5 = vector3tuple<3>(v5);
    auto t6 = vector3tuple<3>(v6);
    auto t7 = vector3tuple<3>(v7);
    auto t8 = vector3tuple<3>(v8);
    auto t9 = vector3tuple<3>(v9);
    auto t10 = vector3tuple<3>(v10);
    auto t11 = vector3tuple<3>(v11);
    auto t12 = vector3tuple<3>(v12);
    auto t13 = vector3tuple<3>(v13);
    auto t14 = vector3tuple<3>(v14);
    auto t15 = vector3tuple<3>(v15);
    auto t16 = vector3tuple<3>(v16);
    auto t17 = vector3tuple<3>(v17);
    auto t18 = vector3tuple<3>(v18);
    auto t19 = vector3tuple<3>(v19);
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
    kdTree.insert(t5,1);
    kdTree.insert(t6,2);
    kdTree.insert(t7,3);
    kdTree.insert(t8,4);
    kdTree.insert(t9,5);
    kdTree.insert(t10,6);
    kdTree.insert(t11,7);
    kdTree.insert(t12,8);
    kdTree.insert(t13,100);
    kdTree.insert(t14,50);
    kdTree.insert(t15,70);
    kdTree.insert(t16,18);
    kdTree.insert(t17,20);
    kdTree.insert(t18,80);
    kdTree.insert(t19,-200);
    for (auto &item: kdTree){
        cout << item.second << endl;
    }
    cout << kdTree << endl;
    return 0;
}


//friend std::ostream& operator<<(std::ostream& os, const KDTree& threeDTree) {
//    threeDTree.printBT(os, "", threeDTree.root, false, true);
//    return os;
//}
//
//void printBT(std::ostream& os, const std::string& prefix, const Node* node, bool isLeft,
//             bool isRoot = false) const {
//    if (!isRoot) os << std::endl;
//    os << prefix;
//    os << (isLeft ? "├──" : "└──");
//    if (node != nullptr) {
//        auto& data = node->data;
//        os << "{{" << std::get<0>(data.first) << ", " << std::get<1>(data.first) << ", "
//           << std::get<2>(data.first) << "}, " << data.second << "}";
//        if (node->left || node->right) {
//            printBT(os, prefix + (isLeft ? "│  " : "   "), node->left, true);
//            printBT(os, prefix + (isLeft ? "│  " : "   "), node->right, false);
//        }
//    } else {
//        os << "NULL";
//    }
//}