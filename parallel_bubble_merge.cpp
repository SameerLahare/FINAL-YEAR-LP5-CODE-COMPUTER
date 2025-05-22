#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Parallel Bubble Sort (Odd-Even Sort)
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool Swapped = true;

    while (Swapped) {
        Swapped = false;

        // Even phase
        #pragma omp parallel for shared(arr)
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                #pragma omp critical
                Swapped = true;
            }
        }

        // Odd phase
        #pragma omp parallel for shared(arr)
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                #pragma omp critical
                Swapped = true;
            }
        }
    }
}

// Merge function used in Merge Sort
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int left = l, right = m + 1;

    while (left <= m && right <= r) {
        if (arr[left] <= arr[right]) {
            temp.push_back(arr[left++]);
        } else {
            temp.push_back(arr[right++]);
        }
    }

    while (left <= m)
        temp.push_back(arr[left++]);

    while (right <= r)
        temp.push_back(arr[right++]);

    for (int i = l; i <= r; i++)
        arr[i] = temp[i - l];
}

// Parallel Merge Sort
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

    vector<int> arr(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    vector<int> original = arr;

    // Time Bubble Sort
    auto bubbleStart = high_resolution_clock::now();
    bubbleSort(arr);
    auto bubbleEnd = high_resolution_clock::now();

    cout << "Sorted array using Bubble Sort: ";
    for (int num : arr)
        cout << num << " ";
    cout << endl;

    arr = original;

    // Time Merge Sort
    auto mergeStart = high_resolution_clock::now();
    mergeSort(arr, 0, n - 1);
    auto mergeEnd = high_resolution_clock::now();

    cout << "Sorted array using Merge Sort: ";
    for (int num : arr)
        cout << num << " ";
    cout << endl;

    // Show durations in microseconds
    auto bubbleDuration = duration_cast<microseconds>(bubbleEnd - bubbleStart);
    auto mergeDuration = duration_cast<microseconds>(mergeEnd - mergeStart);

    cout << "Bubble Sort time: " << bubbleDuration.count() << " microseconds" << endl;
    cout << "Merge Sort time: " << mergeDuration.count() << " microseconds" << endl;

    return 0;
}
