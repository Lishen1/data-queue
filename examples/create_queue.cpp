#include <data_queue/data_queue.h>

#include <vector>

int main() {
    std::vector<int> buffer ={1,2,3,4,5,6};
    daqu::storage_data_accesor<std::vector<int>> data{buffer};
}
