#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <omp.h>
#include <cstdlib>
using namespace std;

// Sequential implementations
template <typename T>
T sequentialMin(const vector<T> &arr) {
    return *min_element(arr.begin(), arr.end());
}

template <typename T>
T sequentialMax(const vector<T> &arr) {
    return *max_element(arr.begin(), arr.end());
}

template <typename T>
T sequentialSum(const vector<T> &arr) {
    return accumulate(arr.begin(), arr.end(), T(0));
}

template <typename T>
double sequentialAverage(const vector<T> &arr) {
    return static_cast<double>(sequentialSum(arr)) / arr.size();
}

// Parallel implementations
template <typename T>
T parallelMin(const vector<T> &arr) {
    T result = arr[0];
#pragma omp parallel for reduction(min : result)
    for (int i = 0; i < arr.size(); ++i) {
        result = min(result, arr[i]);
    }
    return result;
}

template <typename T>
T parallelMax(const vector<T> &arr) {
    T result = arr[0];
#pragma omp parallel for reduction(max : result)
    for (int i = 0; i < arr.size(); ++i) {
        result = max(result, arr[i]);
    }
    return result;
}

template <typename T>
T parallelSum(const vector<T> &arr) {
    T result = 0;
#pragma omp parallel for reduction(+ : result)
    for (int i = 0; i < arr.size(); ++i) {
        result += arr[i];
    }
    return result;
}

template <typename T>
double parallelAverage(const vector<T> &arr) {
    T sum = parallelSum(arr);
    return static_cast<double>(sum) / arr.size();
}

int main() {
    const int size = 100000;  // Use a large size to see performance difference
    vector<int> arr(size);

    // Initialize the array with random values
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000;
    }

    // Sequential calculations with timing
    double start, end;

    start = omp_get_wtime();
    int seqMin = sequentialMin(arr);
    end = omp_get_wtime();
    double timeSeqMin = end - start;

    start = omp_get_wtime();
    int seqMax = sequentialMax(arr);
    end = omp_get_wtime();
    double timeSeqMax = end - start;

    start = omp_get_wtime();
    int seqSum = sequentialSum(arr);
    end = omp_get_wtime();
    double timeSeqSum = end - start;

    start = omp_get_wtime();
    double seqAvg = sequentialAverage(arr);
    end = omp_get_wtime();
    double timeSeqAvg = end - start;

    // Parallel calculations with timing
    start = omp_get_wtime();
    int parMin = parallelMin(arr);
    end = omp_get_wtime();
    double timeParMin = end - start;

    start = omp_get_wtime();
    int parMax = parallelMax(arr);
    end = omp_get_wtime();
    double timeParMax = end - start;

    start = omp_get_wtime();
    int parSum = parallelSum(arr);
    end = omp_get_wtime();
    double timeParSum = end - start;

    start = omp_get_wtime();
    double parAvg = parallelAverage(arr);
    end = omp_get_wtime();
    double timeParAvg = end - start;

    // Output results and timing
    cout << "===== Results =====" << endl;
    cout << "Min: Seq = " << seqMin << ", Par = " << parMin << endl;
    cout << "Max: Seq = " << seqMax << ", Par = " << parMax << endl;
    cout << "Sum: Seq = " << seqSum << ", Par = " << parSum << endl;
    cout << "Avg: Seq = " << seqAvg << ", Par = " << parAvg << endl;

    cout << "\n===== Performance (in seconds) =====" << endl;
    cout << "Min: Seq = " << timeSeqMin << ", Par = " << timeParMin
         << ", Speedup = " << timeSeqMin / timeParMin << endl;
    cout << "Max: Seq = " << timeSeqMax << ", Par = " << timeParMax
         << ", Speedup = " << timeSeqMax / timeParMax << endl;
    cout << "Sum: Seq = " << timeSeqSum << ", Par = " << timeParSum
         << ", Speedup = " << timeSeqSum / timeParSum << endl;
    cout << "Avg: Seq = " << timeSeqAvg << ", Par = " << timeParAvg
         << ", Speedup = " << timeSeqAvg / timeParAvg << endl;

    return 0;
}
