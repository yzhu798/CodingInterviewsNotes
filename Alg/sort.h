#ifndef SORT_H
#define SORT_H
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
class Sort
{
public:
    Sort();
    Sort(int size);
    Sort(int size,int minValue,int maxValue);

    void swap(vector<int> &arr ,int i,int j);
    void stdSort(const vector<int> &arr);
    void stdswap(vector<int> &arr, int i, int j);
    void printf(const vector<int> &arr);
    void generateRandomArray(int size,int minValue,int maxValue);
    bool check_result(vector<int> &arr,vector<int> &arr2);

public:
    vector<int> arr;
    vector<int> arr2;
    vector<int> getArr() const;
    void setArr(const vector<int> &value);


    void BubbleSort(vector<int> &arr);              // √∞≈›≈≈–Ú
    void insertionSort(vector<int> &arr);           // ≤Â»Î≈≈–Ú
    void selectionSort(vector<int> &arr);           // —°‘Ò≈≈–Ú
    void heapSort(vector<int> &arr);                // ∂—≈≈–Ú
    void quickSort(vector<int> &arr);               // øÏ≈≈–Ú
    void mergeSort(vector<int> &arr);               // πÈ≤¢≈≈–Ú
    void bucketSort(vector<int> &arr);              // Õ∞≈≈–Ú
    void radixSort(vector<int> &arr);               // ª˘ ˝≈≈–Ú
    void shellSort(vector<int> &arr);               // shell≈≈–Ú
    void countingSort(vector<int> &arr);            // º∆ ˝≈≈–Ú
private:
    void heapInsert(vector<int> &arr, int index);
    void heapify(vector<int> &arr, int index, int size);
    int partition(vector<int> &arr, int l, int r);
    void mergeSort(vector<int> &arr, int l, int r);
    void merge(vector<int> &arr, int l, int m, int r);
    void quickSort(vector<int> &arr, int l, int r);
    int maxbits(vector<int> &arr);
    void radixSort(vector<int> &arr, int begin, int end, int digit);
    int getDigit(int x, int d);
    void shellInsert(vector<int> &arr, int beg, int gap);
};

#endif // SORT_H
