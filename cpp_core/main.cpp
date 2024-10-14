#include <iostream>
#include "board.h"
#include "iterator.h"
#include "evidence.h"
#include <chrono>
#include <sstream>

using namespace std;

int find(Evidence* e, int q, int r) {
    int p = q;
    do {
        stringstream ss;
        ss << "<(" << p << ",0),(" << q << ",1),(" << r << ",2),(0,3)>";
        Proof* proof = e->get(Board::get_board(ss.str()));
        if (proof == nullptr) break;
        else {
            if (proof->move.second == 0) return proof->move.first;
        }
        p++;
    } while (true);
    return p;
}

int main() {
    ExtendedEvidence e(IteratorCode::ZIGZAC);
    cout << find(&e, 22, 17) << endl;
    Board::clean_up();
}