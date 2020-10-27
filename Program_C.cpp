//
// Created by Яфаров Владимир on 27.10.2020.
// Attempt #96793470
//

#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>
#include <vector>

long long Partition (std::vector <long long> &a, long long start, long long end){
    //int k = rand()%(end-start);
    int k = (start + end) / 2;
    long long pivot = a[k];
    long long j = end;
    long long i = start;
    while (i <= j) {
        while (a[i] < pivot) {
            ++i;
        }
        while (a[j] > pivot) {
            j--;
        }
        if (i >= j) break;
        std::swap(a[i++],a[j--]);
    }
    return (j);
}

long long find_statistic (std::vector <long long> &a, long long n, long long k){
    long long start = 0;
    long long end = n-1;
    long long mid = 0;
    long long max = 0;
    while (1){
        long long mid = Partition(a, start, end);
        if (k == mid){
            for (int i = start; i <= k; i++){
                if (max < a[i]) max = a[i];
            }
            return (max);
        }
        else {
            if (k > mid) start = mid+1;
            else if (k < mid) end = mid;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    srand(time(0));
    int n, k;

    std::cin >> n >> k;
    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    std::cout << find_statistic(a, n, k);

    return 0;
}