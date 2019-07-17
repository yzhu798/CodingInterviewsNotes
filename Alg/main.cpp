#include <iostream>
#include <algorithm>
#include <string>
#include "sort.h"
#include "search.h"


using namespace std;

int main(int argc, char *argv[])
{
    int result =1;
    for(int i=0;i<1;i++){
        Sort sort(100,0,100);
        Search sh;

        sort.BubbleSort(sort.arr);
        sort.printf(sort.arr);
        //        cout<< sh.binarySearch(sort.arr,0,100,49)<<endl;
        cout<< sh.InsertionSearch(sort.arr,0,100,50)<<endl;
        cout<< sh.InsertionSearch(sort.arr,0,100,49)<<endl;

        //        sort.stdSort(sort.arr);
        //        sort.insertionSort(sort.arr);
        //        sort.printf(sort.arr);
        //        sort.BubbleSort(sort.arr);
        //        sort.countingSort(sort.arr);
        //        sort.selectionSort(sort.arr);
        //        sort.quickSort(sort.arr);
        //        sort.radixSort(sort.arr);
        //        sort.mergeSort(sort.arr);
        //        sort.printf(sort.arr);
        if(sort.check_result(sort.arr,sort.arr2) ==0){
            result =false;
            break;
        }
    }
    cout <<   "result :"<<result<<endl;
    return 0;
}
