//
// Created by Яфаров Владимир on 27.10.2020.
//

#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>

struct Item {
    long long value;
    int* index;
    Item() : value (0), index (nullptr) {}
    Item(long long _value, int* _ind) : value (_value), index (_ind) {}
};

class Heap {
public:
    Heap(int max_size);
    ~Heap();
    Heap operator() (const Heap& h) = delete;
    Heap& operator= (const Heap& h) = delete;
    void decreaseKey(int i, long long delta);
    Item getMin();
    void insert(Item x);
    void extractMin();
private:
    Item* data;
    int n;
    int max_size;
    void siftUp(int i);
    void siftDown(int i);
};

Heap::Heap(int _max_size) : data (new Item[_max_size + 1]), n (0), max_size (_max_size) {}

Heap::~Heap() {
    delete[] data;
}

void Heap::decreaseKey(int i, long long delta) {
    data[i].value -= delta;
    siftUp(i);
}

Item Heap::getMin() {
    return data[1];
}

void Heap::insert(Item x) {
    assert(n != max_size);
    *(x.index) = n + 1;
    data[++n] = x;
    siftUp(n);
}

void Heap::extractMin() {
    data[1] = data[n];
    *(data[1].index) = 1;
    --n;
    siftDown(1);
}

void Heap::siftUp(int i) {
    while (i > 1 && data[i].value < data[i / 2].value) {
        *(data[i].index) = i / 2;
        *(data[i / 2].index) = i;
        std::swap(data[i], data[i / 2]);
        i /= 2;
    }
}

void Heap::siftDown(int i) {
    while (2 * i <= n) {
        int j = -1;
        if (data[2 * i].value < data[i].value) {
            j = 2 * i;
        }
        if (2 * i + 1 <= n && data[2 * i + 1].value < data[i].value &&
            ((j == -1) || (data[2 * i].value > data[2 * i + 1].value))) {
            j = 2 * i + 1;
        }
        if (j == -1) break;
        else {
            *(data[i].index) = j;
            *(data[j].index) = i;
            std::swap(data[i], data[j]);
            i = j;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    Heap h = Heap(1000001);
    int q;
    std::string cmd;
    std::cin >> q;
    int* commands = new int[1000001];
    for (int i = 1; i <= q; ++i) {
        std::cin >> cmd;
        if (cmd == "insert") {
            long long data;
            std::cin >> data;
            h.insert(Item(data, &commands[i]));
        } else if (cmd == "getMin") {
            std::cout << h.getMin().value << "\n";
        } else if (cmd == "extractMin") {
            h.extractMin();
        } else if (cmd == "decreaseKey") {
            int x;
            long long delta;
            std::cin >> x >> delta;
            h.decreaseKey(commands[x], delta);
        }
    }
}