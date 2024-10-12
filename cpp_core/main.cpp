#include <iostream>
#include "board.h"
#include "iterator.h"
#include "evidence.h"
#include <chrono>

using namespace std;

int main() {
    bool test = false;
    auto start = chrono::high_resolution_clock::now();
    Board const *board = Board::get_board("<(15,0),(3,3),(0,10)>", &test);
    Evidence e;
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
}