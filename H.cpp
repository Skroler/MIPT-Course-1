//
// Created by Яфаров Владимир on 27.10.2020.
//

#include <iostream>
#include <vector>
#include <algorithm>

const long long One = 1ll;

void sorted(std::vector<long long>& A, long long start, long long end, long long byte);

void LSD_sort(std::vector<long long> & A, long long start, long long end, long long byte);

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    long long num;
    std::cin >> num;
    std::vector<long long> A(num);
    for (int i = 0; i < num; ++i) {
        std::cin >> A[i];
    }
    LSD_sort(A, 0, num, 63);
    for (int i = 0; i < num; ++i) {
        std::cout << A[i] << " ";
    }
    return 0;
}

void sorted(std::vector<long long>& A, long long start, long long end, long long byte)
{
    long long j = end - 1;
    for (int i = start; i < j; ++i) {
        if (((A[i] >> byte) & One) == 0) continue;
        while (j > i && ((A[j] >> byte) & One) == 1) --j;
        if (j > i) std::swap(A[i], A[j]);
    }
}

void LSD_sort(std::vector<long long> & A, long long start, long long end, long long byte)
{
    if (end - start <= 1) return;
    if (byte == -1) return;
    sorted(A, start, end, byte);
    long long j = start;
    while (j < end && ((A[j] >> byte) & One) == 0) ++j;
    LSD_sort(A, start, j, byte - 1);
    LSD_sort(A, j, end, byte - 1);
}