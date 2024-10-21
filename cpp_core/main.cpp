#include <iostream>
#include "board.h"
#include "iterator.h"
#include "evidence.h"
#include <chrono>
#include <sstream>

using namespace std;


int main() {
    ExtendedEvidence e(IteratorCode::ZIGZAC);
    auto start = std::chrono::high_resolution_clock::now();
    Proof* proof = e.get(Board::get_board("<(50,0),(0,3)>"));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;
    Board::clean_up();
}