//
// Created by Яфаров Владимир on 27.10.2020.
//

#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>

class MaxHeap {
public:
    MaxHeap(int max_size);
    ~MaxHeap();
    MaxHeap operator() (const MaxHeap& h) = delete;
    MaxHeap& operator= (const MaxHeap& h) = delete;
    void decreaseKey(int i, long long delta);
    int getSize();
    long long getMax();
    void insert(long long x);
    void extractMax();
private:
    long long* data;
    int n;
    int max_size;
    void siftUp(int i);
    void siftDown(int i);
};

MaxHeap::MaxHeap(int _max_size) : data (new long long[_max_size]), n (0), max_size (_max_size) {}

MaxHeap::~MaxHeap() {
    delete[] data;
}

void MaxHeap::decreaseKey(int i, long long delta) {
    data[i] -= delta;
    siftUp(i);
}

int MaxHeap::getSize() {
    return n;
}


long long MaxHeap::getMax() {
    return data[0];
}

void MaxHeap::insert(long long x) {
    assert(n != max_size);
    data[n++] = x;
    siftUp(n - 1);
}

void MaxHeap::extractMax() {
    data[0] = data[n - 1];
    --n;
    siftDown(0);
}

void MaxHeap::siftUp(int i) {
    while (i > 0 && data[i] > data[(i - 1) / 2]) {
        std::swap(data[i], data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void MaxHeap::siftDown(int i) {
    while (2 * i + 1 < n) {
        int j = -1;
        if (data[2 * i + 1] > data[i]) {
            j = 2 * i + 1;
        }
        if (2 * i + 2 < n && data[2 * i + 2] > data[i] &&
            ((j == -1) || (data[2 * i + 1] < data[2 * i + 2]))) {
            j = 2 * i + 2;
        }
        if (j == -1) break;
        else {
            std::swap(data[i], data[j]);
            i = j;
        }
    }
}

int main() {
    //optimization cin and cout
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    //create some variables
    int num, k;
    long long temp;
    MaxHeap Heap_max = 100001;
    std::cin >> num >> k;
    long long a[k];
    //cycle of creating
    for (int i = 0; i < num; ++i) {
        std::cin >> temp;
        if (i < k) {
            Heap_max.insert(temp);
        } else if (temp < Heap_max.getMax()) {
            Heap_max.extractMax();
            Heap_max.insert(temp);
        }
    }
    for (int i = k-1; i >= 0; i--){
        a[i] = Heap_max.getMax();
        Heap_max.extractMax();
    }
    for (int i = 0; i < k; ++i) {
        std::cout << a[i] << '\n';
    }
}