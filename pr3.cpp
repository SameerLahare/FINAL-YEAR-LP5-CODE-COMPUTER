#include<iostream>
#include<vector>
#include<omp.h>

using namespace std;

int parallelMIN(vector<int> vec) {
    int min_val = vec[0];
    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] < min_val) {
            min_val = vec[i];
        }
    }
    return min_val;
}

int parallelMAX(vector<int> vec) {
    int max_val = vec[0];
    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > max_val) {
            max_val = vec[i];
        }
    }
    return max_val;
}

int parallelSum(vector<int> vec) {
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum;
}

float parallelAverage(vector<int> vec) {
    int sum = parallelSum(vec);
    return float(sum) / vec.size();
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> vec(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++) {
        cin >> vec[i];
    }

    int min = parallelMIN(vec);
    cout << "Minimum value: " << min << endl;

    int max = parallelMAX(vec);
    cout << "Maximum value: " << max << endl;

    int sum = parallelSum(vec);
    cout << "Sum of values: " << sum << endl;

    float avg = parallelAverage(vec);
    cout << "Average of values: " << avg << endl;

    return 0;
}
