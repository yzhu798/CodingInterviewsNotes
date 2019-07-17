#ifndef SEARCH_H
#define SEARCH_H
#include<time.h>
#include<cstdlib>
#include<cstring>
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
class Search
{
public:
    Search();

    int SequenceSearch(vector<int> &arr,int key); //顺序查找
    int binarySearch(const vector<int> &arr,int key); //二分查找
    int binarySearch(const vector<int> &arr,int low,int high,int key); //二分查找
    int InsertionSearch(const vector<int> &arr,int low,int high,int key); //差值查找



    void GetAllNotIncluded(vector<int> &A, vector<int> &B);
};

#endif // SEARCH_H
