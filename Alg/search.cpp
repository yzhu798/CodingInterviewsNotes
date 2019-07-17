#include "search.h"


Search::Search()
{

}

int Search::SequenceSearch(vector<int> &arr, int key)
{
    for(int i=0;i< arr.size();i++){
        if(arr[i] == key)
        {
            cout<<"find key at:"<<i<<endl;
            return i;
        }

    }
    cout<<"failed find key "<<key<<endl;
    return -1;
}

int Search::binarySearch(const vector<int> &arr, int key)
{
    int low =0;
    int high =arr.size()-1;
    int mid ;
    while(low <= high){
        mid = low + (high -low)/2;
        if(arr[mid] == key)
        {
            cout<<"find key at:"<<mid<<endl;
            return mid;
        }
        if(arr[mid] < key){
            low = mid+1;
        }
        if(arr[mid] > key){
            high = mid-1;
        }
    }
    cout<<"failed find key "<<key<<endl;
    return -1;
}

int Search::binarySearch(const vector<int> &arr, int low, int high,int key)
{
    if(low < 0 || high > arr.size()){
        cout<<"rang is err"<<endl;
        return -1;
    }
    if(low > high){
        cout<<"failed find key "<<key<<endl;
        return -1;
    }
    int mid = low+ (high -low) /2;
    if(arr[mid] == key)
    {
        cout<<"find key at:"<<mid<<endl;
        return mid;
    }
    if(arr[mid] < key){
        return binarySearch(arr, mid+1, high,key);
    }
    if(arr[mid] > key){
        return binarySearch(arr, low,  mid-1,key);
    }
}

//分布均匀下按比例查找
int Search::InsertionSearch(const vector<int> &arr, int low, int high, int key)
{
    if(low < 0 || high > arr.size()){
        cout<<"rang is err"<<endl;
        return -1;
    }
    if(low > high){
        cout<<"failed find key "<<key<<endl;
        return -1;
    }
    int mid = low+  (key - arr[low])/(arr[high] -arr[low]) * (high - low);
    if(arr[mid] == key)
    {
        cout<<"find key at:"<<mid<<endl;
        return mid;
    }
    if(arr[mid] < key){
        return binarySearch(arr, mid+1, high,key);
    }
    if(arr[mid] > key){
        return binarySearch(arr, low,  mid-1,key);
    }
}

void Search::GetAllNotIncluded(vector<int> &A,vector<int> &B)
{
    vector<int> res;
    //    遍历B中每一个元素，每次用元素与 A中元素进行二分比较，不存在加入到结果中
    for (int i = 0; i < B.size(); i++) {
        int l = 0;
        int r = A.size() - 1;
        bool contains = false;
        while (l <= r) {
            int mid = l + ((r - l) >> 1);
            if (A[mid] == B[i]) {
                contains = true;
                break;
            }
            if (A[mid] > B[i]) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        if (!contains) {
            res.push_back(B[i]);
        }
    }

    cout <<endl<<"GetAllNotIncluded B "<<endl;
    for(auto i:res){
        cout <<i<<" ";
    }
    cout <<endl;
}
