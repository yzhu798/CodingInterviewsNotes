#include "sort.h"
#include<time.h>
#include<cstdlib>
#include<cstring>
Sort::Sort()
{

}

Sort::Sort(int size)
{
    generateRandomArray(size,0,size);
}

Sort::Sort(int size, int minValue, int maxValue)
{
    generateRandomArray(size,minValue,maxValue);
}


void Sort::swap(vector<int> &arr, int i, int j)
{
    int temp =arr[i];
    arr[i] = arr[j] ;
    arr[j] = temp   ;
    //    arr[i] = arr[i] ^ arr[j];
    //    arr[j] = arr[i] ^ arr[j];
    //    arr[i] = arr[i] ^ arr[j];
}

void Sort::stdSort(const vector<int> &arr)
{
    arr2 =arr;
    std::sort(arr2.begin(),arr2.end());
}
void Sort::stdswap(vector<int> &arr, int i, int j)
{
    std::swap(arr[i],arr[j]);
}

void Sort::printf(const vector<int> &arr)
{
    cout<<__FUNCTION__<<endl;
    for(auto i:arr){
        cout<< i<<" ";
    }
    cout<<endl;
    cout<<endl;
}

void Sort::generateRandomArray(int size, int minValue, int maxValue)
{
    arr.resize(size);
    srand(time(0));
    for(int i=0;i<size ;i++){
        arr[i] =   (rand())% (maxValue -minValue +1) +minValue;
    }
}

bool Sort::check_result(vector<int> &arr, vector<int> &arr2)
{
    if(arr.size() !=arr2.size()){
        return false;
    }else{
        for(int i=0;i<arr.size();i++){
            if(arr[i] != arr2[i]){
                return false;
            }
        }
        return true;
    }
}

vector<int> Sort::getArr() const
{
    return arr;
}

void Sort::setArr(const vector<int> &value)
{
    arr = value;
}

// 冒泡排序

void Sort::BubbleSort(vector<int> &arr)
{
    cout<<__FUNCTION__<<endl;
    if(arr.size() <2){
        return ;
    }
    bool hasSorted = false; //有序
    //   1  ?比较相邻的元素。如果第一个比第二个大，就交换它们两个；
    //   2  ?对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对，
    //      这样在最后的元素应该会是最大的数；
    //   3  ?针对所有的元素重复以上的步骤，除了最后一个；
    //   4  ?重复步骤1~3，直到排序完成。
    //    原始
    //    38 50 53 83 36 30 21 97
    //    BubbleSort
    //                         97       1
    //                      83 97       2
    //                   53 83 97       3
    //                50 53 83 97       4
    //          36 38 50 53 83 97       5
    //    21 30 36 38 50 53 83 97       6 OK
    for (int end = arr.size() - 1; end > 0 && (!hasSorted); end--) {
        hasSorted =true;
        for (int i = 0; i < end; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr, i, i + 1);
                hasSorted = false;
            }
        }
        //        if(hasSorted)
        //            return;
    }
}
// 插入排序

void Sort::insertionSort(vector<int> &arr)
{
    //    cout<<__FUNCTION__<<endl;
    if(arr.size() <2){
        return ;
    }
    //    i是第几次对比的数据，i-1是之前有序的数据
    //    .....    5   3   4   8   9
    //    .....   i-1  i——————————-——》 i待插入的数据
    //    .....   i-1  i
    //    .....    j   j=[i-1]是
    //    26 3 79 99 86 77 0 10
    //
    //    3 26
    //    3 26 79
    //    3 26 79 99
    //    3 26 79 86 99
    //    3 26 77 79 86 99
    //    0 3 26 77 79 86 99
    //    0 3 10 26 77 79 86 99
    //     ?从第一个元素开始，该元素可以认为已经被排序；
    //     ?取出下一个元素，在已经排序的元素序列中从后向前扫描；
    //     ?如果该元素（已排序）大于新元素，将该元素移到下一位置；
    //     ?重复步骤3，直到找到已排序的元素小于或者等于新元素的位置；
    //     ?将新元素插入到该位置后；
    //     ?重复步骤2~5。


#if 1
    for (int i = 1; i < arr.size(); i++) {
#if 0
        int j = i - 1;
        for (; j >= 0 && arr[j] > arr[j + 1]; j--) {
            swap(arr, j, j + 1);
        }
#else
        //        优化版本 ERR
        int temp = arr[i];
        int newPos = i-1;
        /* i 之前的元素都是有序的，找到比temp小的插入到他后面，
         * 比temp大的，需要往后挪一个位置*/
        for (; newPos >= 0 && arr[newPos] > temp; newPos--) {
            arr[newPos+1] = arr[newPos];
        }
        arr[newPos+1] = temp;

        for(auto i:arr){
            cout<<i<<" ";
        }
        cout<<endl;
#endif
    }
#endif

}

// 选择排序

void Sort::selectionSort(vector<int> &arr)
{
    cout<<__FUNCTION__<<endl;
    if(arr.size() <2){
        return ;
    }


    //    ?初始状态：无序区为R[1..n]，有序区为空；
    //    ?第i趟排序(i=1,2,3…n-1)开始时，当前有序区和无序区分别为R[1..i-1]和R(i..n）。
    //     该趟排序从当前无序区中-选出关键字最小的记录 R[k]，将它与无序区的第1个记录R交换，
    //     使R[1..i]和R[i+1..n)分别变为记录个数增加1个的新有序区和记录个数减少1个的新无序区；
    //     ?n-1趟结束，数组有序化了。

    //    printf
    //    2 32 42 45 29 72 52 74
    //    selectionSort
    //    2   32 42 45 29 72 52 74
    //    2 29   42 45 32 72 52 74
    //    2 29 32   45 42 72 52 74
    //    2 29 32 42   45 72 52 74
    //    2 29 32 42 45   72 52 74
    //    2 29 32 42 45 52   72 74
    //    2 29 32 42 45 52 72   74
    //    result :1
    for (int i = 0; i < arr.size() - 1; i++) {
        int minIndex = i; //从第A[i=0]个数据开始，，并且排序后将值放在minIndex位置
        //        每次选择一个待排序 [i+1]...>的数据最小值
        for (int j = i + 1; j < arr.size(); j++) {
            minIndex = arr[j] < arr[minIndex] ? j : minIndex;
        }
        swap(arr, i, minIndex);
    }
}


// 堆排序

void Sort::heapSort(vector<int> &arr)
{
    //    ?将初始待排序关键字序列(R1,R2….Rn)构建成大顶堆，此堆为初始的无序区；
    //     ?将堆顶元素R[1]与最后一个元素R[n]交换，
    //      此时得到新的无序区(R1,R2,……Rn-1)和新的有序区(Rn),
    //      且满足R[1,2…n-1]<=R[n]；
    //     ?由于交换后新的堆顶R[1]可能违反堆的性质，
    //      因此需要对当前无序区(R1,R2,……Rn-1)调整为新堆，
    //      然后再次将R[1]与无序区最后一个元素交换，
    //      得到新的无序区(R1,R2….Rn-2)和新的有序区(Rn-1,Rn)。
    //     不断重复此过程直到有序区的元素个数为n-1，则整个排序过程完成。

    cout<<__FUNCTION__<<endl;
    if(arr.size() <2){
        return ;
    }
    for (int i = 0; i < arr.size() ; i++) {
        heapInsert(arr, i); //逐个插入数据 尾部 arr[0]是大顶堆。
    }
    int size = arr.size() ;
    swap(arr, 0, --size); //大顶堆，把上一次的大堆顶，放在arr[size-1];
    //对大顶堆，arr[0]  <---->  arr[size-1]进行堆调整;
    while (size > 0) {
        heapify(arr, 0, size);
        swap(arr, 0, --size);
    }
}

void Sort::heapInsert(vector<int> &arr, int index)
{
    //    节点值>父节点，交换

    while (arr[index] > arr[(index - 1) / 2]) {
        swap(arr, index, (index - 1) / 2);
        index = (index - 1) / 2; //节点的父亲
    }
}
void Sort::heapify(vector<int> &arr, int index, int size)
{

    int left = index * 2 + 1;

    while (left < size) {
        int largest = left + 1 < size && arr[left + 1] > arr[left] ? left + 1 : left;
        largest = arr[largest] > arr[index] ? largest : index;
        if (largest == index) {
            break;
        }
        swap(arr, largest, index);
        index = largest;
        left = index * 2 + 1;
    }
}
// 快排序

void Sort::quickSort(vector<int> &arr)
{
    if (arr.size() < 2) {
        return;
    }
    quickSort(arr, 0, arr.size() - 1);
}

void Sort::quickSort(vector<int> &arr, int l, int r)
{

    //     ?从数列中挑出一个元素，称为 “基准”（pivot）；
    //     ?重新排序数列，所有元素比基准值小的摆放在基准前面，
    //     所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。
    //     在这个分区退出之后，该基准就处于数列的中间位置。
    //     这个称为分区（partition）操作；
    //     ?递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序。

    if (l < r) {
        int p = partition(arr, l, r);
        quickSort(arr, l, p - 1);
        quickSort(arr, p, r);
    }
}

int Sort::partition(vector<int> &arr, int l, int r)
{
    int less = l - 1;
    int more = r;
    while (l < more) {
        if (arr[l] < arr[r]) {
            swap(arr, ++less, l++);
        } else if (arr[l] > arr[r]) {
            swap(arr, --more, l);
        } else {
            l++;
        }
    }
    swap(arr, more, r);
    return l;
}
// 归并排序

void Sort::mergeSort(vector<int> &arr, int l, int r)
{
    if (l == r) {
        return;
    }
    int mid = l + ((r - l) >> 1);
    mergeSort(arr, l, mid);
    mergeSort(arr, mid + 1, r);
    merge(arr, l, mid, r);
}

void Sort::mergeSort(vector<int> &arr)
{
    if (arr.size() < 2) {
        return;
    }
    mergeSort(arr, 0, arr.size() - 1);
}

void Sort::merge(vector<int> &arr, int l, int m, int r)
{

    //    将已有序的子序列合并，得到完全有序的序列；
    //    即先使每个子序列有序，再使子序列段间有序。
    //    若将两个有序表合并成一个有序表，称为2-路归并。

    //     ?把长度为n的输入序列分成两个长度为n/2的子序列；
    //     ?对这两个子序列分别采用归并排序；
    //     ?将两个排序好的子序列合并成一个最终的排序序列。


    vector<int> help(r - l + 1 , 0);
    int i = 0;
    int p1 = l;
    int p2 = m + 1;
    while (p1 <= m && p2 <= r) {
        help[i++] = arr[p1] < arr[p2] ? arr[p1++] : arr[p2++];
    }
    while (p1 <= m) {
        help[i++] = arr[p1++];
    }
    while (p2 <= r) {
        help[i++] = arr[p2++];
    }
    for (i = 0; i < help.size(); i++) {
        arr[l + i] = help[i];
    }
}





// 计数排序

void Sort::countingSort(vector<int> &arr)
{
    cout<<__FUNCTION__<<endl;

    //    ?找出待排序的数组中最大和最小的元素；
    //     ?统计数组中每个值为i的元素出现的次数，存入数组C的第i项；
    //     ?对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加）；
    //     ?反向填充目标数组：将每个元素i放在新数组的第C(i)项，
    //    每放一个元素就将C(i)减去1。
    // only for 0~200 value
    if ( arr.size() < 2) {
        return;
    }
    int max = INT32_MIN;
    for (int i = 0; i < arr.size() ; i++) {
        max = std::max(max, arr[i]);
    }
    vector<int> bucket(max + 1 , 0);
    for (int i = 0; i < arr.size(); i++) {
        bucket[arr[i]]++;
    }
    int i = 0;
    for (int j = 0; j < bucket.size(); j++) {
        while (bucket[j]-- > 0) {
            arr[i++] = j;
        }
    }
}

// 桶排序

void Sort::bucketSort(vector<int> &arr)
{
    cout<<__FUNCTION__<<endl;

    //    ?设置一个定量的数组当作空桶；
    //     ?遍历输入数据，并且把数据一个一个放到对应的桶里去；
    //     ?对每个不是空的桶进行排序；
    //     ?从不是空的桶里把排好序的数据拼接起来。

    // only for 0~200 value
    if ( arr.size() < 2) {
        return;
    }
    int max = INT32_MIN;
    for (int i = 0; i < arr.size() ; i++) {
        max = std::max(max, arr[i]);
    }

    vector<int> bucket(max + 1 , 0);
    for (int i = 0; i < arr.size(); i++) {
        bucket[arr[i]]++;
    }
    int i = 0;
    for (int j = 0; j < bucket.size(); j++) {
        while (bucket[j]-- > 0) {
            arr[i++] = j;
        }
    }
}
int Sort::maxbits(vector<int> &arr)
{
    int max = INT32_MIN;
    for (int i = 0; i < arr.size(); i++) {
        max = std::max(max, arr[i]);
    }
    int res = 0;
    while (max != 0) {
        res++;
        max /= 10;
    }
    return res;
}
// 基数排序

void Sort::radixSort(vector<int> &arr, int begin, int end, int digit)
{
    volatile int radix = 10;
    int i = 0, j = 0;
    //    int[] count = new int[radix];
    //    int[] bucket = new int[end - begin + 1];

    //    ?取得数组中的最大数，并取得位数；
    //     ?arr为原始数组，从最低位开始取每个位组成radix数组；
    //     ?对radix进行计数排序（利用计数排序适用于小范围数的特点）；


    vector<int> count(radix);
    vector<int> bucket(end - begin + 1);

    for (int d = 1; d <= digit; d++) {
        for (i = 0; i < radix; i++) {
            count[i] = 0;
        }
        for (i = begin; i <= end; i++) {
            j = getDigit(arr[i], d);
            count[j]++;
        }
        for (i = 1; i < radix; i++) {
            count[i] = count[i] + count[i - 1];
        }
        for (i = end; i >= begin; i--) {
            j = getDigit(arr[i], d);
            bucket[count[j] - 1] = arr[i];
            count[j]--;
        }
        for (i = begin, j = 0; i <= end; i++, j++) {
            arr[i] = bucket[j];
        }
    }
}

int Sort::getDigit(int x, int d)
{
    return ((x / ((int) std::pow(10, d - 1))) % 10);
}


// only for no-negative value
//基数排序
void Sort::radixSort(vector<int> &arr)
{
    if ( arr.size() < 2) {
        return;
    }
    radixSort(arr, 0, arr.size()  - 1, maxbits(arr));
}
// shell排序
void Sort::shellSort(vector<int> &arr)
{
    //    ?选择一个增量序列t1，t2，…，tk，其中ti>tj，tk=1；
    //    ?按增量序列个数k，对序列进行k 趟排序；
    //    ?每趟排序，根据对应的增量ti，将待排序列分割成若干长度为m 的子序列，
    //    分别对各子表进行直接插入排序。仅增量因子为1 时，
    //    整个序列作为一个表来处理，表长度即为整个序列的长度。

    int gap = arr.size()/2; //步长默认
    while(gap > 0){
        int beg = gap -1;   //开始比较的位置，一半的前一个数据开始往前
        while(beg >= 0 ){
            shellInsert(arr,beg,gap);
            beg--;
        }
        gap = gap / 2;
    }
}

void Sort::shellInsert(vector<int> &arr,int beg, int gap)
{
    for (int i = beg+gap; i < arr.size(); i+=gap) {
#if 0
        int j = i - gap;
        for (; j >= 0 && arr[j] > arr[j + gap]; j-=gap) {
            swap(arr, j, j + gap);
        }
#else
        //        优化版本 ERR
        int temp = arr[i];
        int newPos = i-gap;
        for (; newPos >= 0 && arr[newPos] > temp; newPos-=gap) {
            arr[newPos+gap] = arr[newPos];
        }
        arr[newPos+gap] = temp;
#endif
    }
}

