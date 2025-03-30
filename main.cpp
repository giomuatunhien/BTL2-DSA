#include "main.h"
//#include "restaurant.cpp"


int main(int argc, char* argv[]) {
    auto start_time = std::chrono::high_resolution_clock::now();
    string fileName = "D:/test.txt";
    simulate(fileName);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "thoi gian chay " << elapsed_time.count() << " milliseconds" << std::endl;

    return 0;
}
