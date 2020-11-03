#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
template <typename T, typename Compare = std::less<T>()>
void bubble_sort(std::vector<T> &vector, Compare comp)
{
    int len = (int)vector.size();
    for (int i = len - 2; i >= 0; i--)
    {
        for (int j = 0; j <= i; j++)
        {
            if (comp(vector[j + 1], vector[j]))
            {
                T temp = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = temp;
            }
        }
    }
}

template <typename T, typename Compare = std::less<T>()>
void insertion_sort(std::vector<T> &vector, Compare comp)
{
    int len = (int)vector.size();
    for (int i = 1; i <= len - 1; i++)
    {
        T temp = vector[i];
        int j = i - 1;
        while (j >= 0 && comp(temp, vector[j]))
        {
            vector[j + 1] = vector[j];
            --j;
        }
        vector[j + 1] = temp;
    }
}

template <typename T, typename Compare = std::less<T>()>
void selection_sort(std::vector<T> &vector, Compare comp)
{
    typename std::vector<T>::iterator it;
    int len = (int)vector.size();
    for (int i = 0; i <= len - 2; i++)
    {
        int temp_ind = i;
        for (int j = i + 1; j <= len - 1; j++)
        {
            if (comp(vector[j], vector[temp_ind]))
            {
                temp_ind = j;
            }
        }
        T temp = vector[temp_ind];
        vector[temp_ind] = vector[i];
        vector[i] = temp;
    }
}

template <typename T, typename Compare = std::less<T>()>
void merge_helper(std::vector<T> &vector, int left, int mid, int right, Compare comp)
{
    std::vector<T> temp(vector);
    int L_size = mid - left + 1;
    int R_size = right - mid;
    int temp_size = right - left + 1;
    int L_count = 0;
    int R_count = 0;
    int temp_count = 0;
    while (L_count < L_size && R_count < R_size)
    {
        if (!comp(vector[mid + 1 + R_count], vector[left + L_count]))
        {
            temp[temp_count++] = vector[left + L_count];
            L_count++;
        }
        else
        {
            temp[temp_count++] = vector[mid + 1 + R_count];
            R_count++;
        }
    }
    if (L_count == L_size)
    {
        for (int i = temp_count; i < temp_size; i++)
        {
            temp[i] = vector[mid + 1 + R_count];
            R_count++;
        }
    }
    else
    {
        for (int i = temp_count; i < temp_size; i++)
        {
            temp[i] = vector[left + L_count];
            L_count++;
        }
    }
    for (int i = 0; i < temp_size; i++)
    {
        vector[left + i] = temp[i];
    }
}

template <typename T, typename Compare = std::less<T>()>
void merge_sort_helper(std::vector<T> &vector, int left, int right, Compare comp)
{
    if (left >= right)
    {
        return;
    }
    int mid = (left + right) / 2;
    merge_sort_helper(vector, left, mid, comp);
    merge_sort_helper(vector, mid + 1, right, comp);
    merge_helper(vector, left, mid, right, comp);
}

template <typename T, typename Compare = std::less<T>()>
void merge_sort(std::vector<T> &vector, Compare comp)
{
    merge_sort_helper(vector, 0, (int)vector.size() - 1, comp);
}

template <typename T, typename Compare = std::less<T>()>
int partition_extra_helper(std::vector<T> &vector, int left, int right, Compare comp)
{
    int size = right - left + 1;
    int front = left;
    int end = right;
    std::srand((unsigned)std::time(nullptr));
    int p = rand() % (size);
    std::vector<T> temp;
    temp.resize(size);
    for (int i = 0; i < size; i++)
    {
        if (i!=p){
            if (comp(vector[left + i], vector[left + p]))
            { // smaller than the pivotat
                temp[-left + front] = vector[left + i];
                front++;
            }
            else
            {
                temp[-left + end] = vector[left + i];
                end--;
            }
        }
    }
    temp[-left + end] = vector[left + p];
    for (int i = 0; i < size; i++)
    {
        vector[left + i] = temp[i];
    }
    return end; // end is pivotat now
}

template <typename T, typename Compare = std::less<T>()>
void quick_sort_extra_helper(std::vector<T> &vector, int left, int right, Compare comp)
{
    int pivotat = 0;
    if (left >= right)
    {
        return;
    }
    pivotat = partition_extra_helper(vector, left, right, comp);
    quick_sort_extra_helper(vector, left, pivotat - 1, comp);
    quick_sort_extra_helper(vector, pivotat + 1, right, comp);
}

template <typename T, typename Compare = std::less<T>()>
void quick_sort_extra(std::vector<T> &vector, Compare comp)
{
    quick_sort_extra_helper(vector, 0, (int)vector.size() - 1, comp);
}

template <typename T, typename Compare = std::less<T>()>
int partition_inplace_helper(std::vector<T> &vector, int left, int right, Compare comp)
{
    int front = left + 1 ;
    int end = right;
    std::srand((unsigned)std::time(nullptr));
    int p = rand() % (right - left + 1); 
    T temp = vector[left];
    vector[left] = vector[left + p];
    vector[left + p] = temp;
    while (front <= end)
    {
        while (comp(vector[front], vector[left]) && (front < right))
        { // while vector[front]<pivot
            front++;
        }
        while (!(comp(vector[end], vector[left])) && (end > left))
        { // while not vector[end]<pivot
            end--;
        }
        if (front < end)
        {
            temp = vector[front];
            vector[front] = vector[end];
            vector[end] = temp;
        }
        else
        {
            temp = vector[left];
            vector[left] = vector[end];
            vector[end] = temp;
            return end; // end is pivotat now
        }
    }
    return end; // end is pivotat now
}

template <typename T, typename Compare = std::less<T>()>
void quick_sort_inplace_helper(std::vector<T> &vector, int left, int right, Compare comp)
{
    int pivotat = 0;
    if (left >= right)
    {
        return;
    }
    pivotat = partition_inplace_helper(vector, left, right, comp);
    quick_sort_inplace_helper(vector, left, pivotat - 1, comp);
    quick_sort_inplace_helper(vector, pivotat + 1, right, comp);
}

template <typename T, typename Compare = std::less<T>()>
void quick_sort_inplace(std::vector<T> &vector, Compare comp)
{
    quick_sort_inplace_helper(vector, 0, (int)vector.size() - 1, comp);
}

#endif //VE281P1_SORT_HPP
