#include "sort.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

void print_v(const vector<int> &v)
{
    for (auto i : v)
    {
        cout << i << " ";
    }
    cout << "\n";
}

vector<int> gen_vec(int len)
{
    std::srand((unsigned)time(NULL));
    vector<int> vec;
    vec.resize(len);
    for (int i = 0; i < len; i++)
    {
        vec[i] = rand();
    }
    return vec;
}

void test_n(int n)
{
    vector<int> orig = gen_vec(n);
    vector<int> copy = orig;
    auto start = std::chrono::steady_clock::now();
    bubble_sort(copy, std::less<int>());
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    std::cout << "bubble sort elapsed: " << elapsed_time.count() << "\n";

    copy = orig;
    start = std::chrono::steady_clock::now();
    selection_sort(copy, std::less<int>());
    end = std::chrono::steady_clock::now();
    elapsed_time = end - start;
    std::cout << "selection sort elapsed: " << elapsed_time.count() << "\n";

    copy = orig;
    start = std::chrono::steady_clock::now();
    insertion_sort(copy, std::less<int>());
    end = std::chrono::steady_clock::now();
    elapsed_time = end - start;
    std::cout << "insertion sort elapsed: " << elapsed_time.count() << "\n";

    copy = orig;
    start = std::chrono::steady_clock::now();
    merge_sort(copy, std::less<int>());
    end = std::chrono::steady_clock::now();
    elapsed_time = end - start;
    std::cout << "merge sort elapsed: " << elapsed_time.count() << "\n";

    copy = orig;
    start = std::chrono::steady_clock::now();
    quick_sort_extra(copy, std::less<int>());
    end = std::chrono::steady_clock::now();
    elapsed_time = end - start;
    std::cout << "qsort_extra elapsed: " << elapsed_time.count() << "\n";

    copy = orig;
    start = std::chrono::steady_clock::now();
    quick_sort_inplace(copy, std::less<int>());
    end = std::chrono::steady_clock::now();
    elapsed_time = end - start;
    std::cout << "qsort_inplace elapsed: " << elapsed_time.count() << "\n";

    copy = orig;
    start = std::chrono::steady_clock::now();
    std::sort(copy.begin(), copy.end(), std::less<int>());
    end = std::chrono::steady_clock::now();
    elapsed_time = end - start;
    std::cout << "std::sort() elapsed: " << elapsed_time.count() << "\n";
}

int main()
{
    for (int i = 0; i < 1; i++)
    {
        test_n(10000);
    }
}