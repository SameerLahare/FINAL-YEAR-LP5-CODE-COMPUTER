#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

void bubbleSort(vector<int>& arr) {
    int n = arr.size(); 
    bool swapped = true;

    while (swapped) {
        swapped = false;
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int left = l, right = m + 1;

    while (left <= m && right <= r) {
        if (arr[left] <= arr[right])
            temp.push_back(arr[left++]);
        else
            temp.push_back(arr[right++]);
    }

    while (left <= m) temp.push_back(arr[left++]);
    while (right <= r) temp.push_back(arr[right++]);

    for (int i = l; i <= r; i++)
        arr[i] = temp[i - l];
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(arr, l, m);
            #pragma omp section
            mergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> original(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++)
        cin >> original[i];

    vector<int> bubbleArr = original;
    vector<int> mergeArr = original;

    clock_t bubbleStart = clock();
    bubbleSort(bubbleArr);
    clock_t bubbleEnd = clock();

    clock_t mergeStart = clock();
    mergeSort(mergeArr, 0, n - 1);
    clock_t mergeEnd = clock();

    cout << "\nSorted array using Bubble Sort: ";
    for (int num : bubbleArr)
        cout << num << " ";
    cout << endl;

    cout << "Sorted array using Merge Sort: ";
    for (int num : mergeArr)
        cout << num << " ";
    cout << endl;

    double bubbleDuration = double(bubbleEnd - bubbleStart) / CLOCKS_PER_SEC;
    double mergeDuration = double(mergeEnd - mergeStart) / CLOCKS_PER_SEC;

    cout << "\nBubble sort time in seconds: " << bubbleDuration << endl;
    cout << "Merge sort time in seconds: " << mergeDuration << endl;

    return 0;
}

