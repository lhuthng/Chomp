#include <iostream>
#include "board.h"
#include "iterator.h"
#include "evidence.h"
#include <chrono>

using namespace std;

int main() {
    bool test = false;
    auto start = chrono::high_resolution_clock::now();
    Board const *board = Board::get_board("<(15,0),(0,12)>", &test);
    Evidence e(IteratorCode::ZIGZAG);
    Proof* proof = e.get(board);
    if (proof == nullptr) {
        cout << "Nope" << endl;   
    }
    else {
        cout << proof->move.first << " " << proof->move.second << endl;

    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;
    Board::clean_up();
}