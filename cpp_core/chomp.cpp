#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <unordered_map>
#include <set>
#include <stdexcept>

using namespace std;

void print_pair(const pair<int, int>& pair) {
    cout << pair.first << ";" << pair.second;
}

void print_list(const list<pair<int, int>>& list) {
    for (auto pair : list) {
        print_pair(pair);
    }
    cout << endl;
}

struct PairListHash {
    size_t operator()(const list<pair<int, int>>& vec) const {
        size_t hashVal = 0;
        for (const auto& p : vec) {
            hashVal ^= hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
        }
        return hashVal;
    }
};

struct ListPairEqual {
    bool operator()(const list<pair<int, int>>& lhs, const list<pair<int, int>>& rhs) const {
        return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
    }
};

ostream& operator<<(ostream& os, const pair<int, int>& pair) {
    os << "(" << pair.first << ";" << pair.second << ")";
    return os;
}

ostream& operator<<(ostream& os, const list<pair<int, int>>& list) {
    os << "<";
    for (const auto & it : list) {
        os << it << ",";
    }
    os << "\b>";
    return os;
}

int get_first(const pair<int, int>& pair) {
    return pair.first;
}

int get_second(const pair<int, int>& pair) {
    return pair.second;
}

class Board {
private:
    const list<pair<int, int>>* p_generators;
    static unordered_map<list<pair<int, int>>, Board*, PairListHash, ListPairEqual> boards;
    Board(const list<pair<int, int>> * p_generators) {
        this->p_generators = p_generators;
    }
public:
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    class Iterator {
        private:
            list<pair<int, int>>::const_iterator prev, it;
            const Board* board;
            pair<int, int> current;
        public:
            Iterator(const Board* board, int row=0, int col=0) : it(board->p_generators->begin()), board(board) {
                prev = it++;
                if (it == board->p_generators->end()) current = {0, 1};
                else  current = {row, col};
             }
            Iterator& operator++() {
                if (it != board->p_generators->end()) {
                    if (current.first < prev->first - 1) {
                        current.first += 1;
                    }
                    else {
                        current.first = 0;
                        current.second += 1;
                        if (current.second > it->second - 1) prev = it++;
                    }
                }
                return *this;
            }
            bool operator!=(const Iterator& other) const {
                return board != other.board || current != other.current;
            }
            pair<int, int> operator*() const {
                return current;
            }
    };
    Iterator begin() const {
        return Iterator(this, 1, 0);
    }
    Iterator end() const {
        return Iterator(this, 0, prev(p_generators->end())->second);
    }
    const list<pair<int, int>>& get_generators() const {
        return *p_generators;
    }
    friend ostream& operator<<(ostream& os, const Board& board) {
        os << *(board.p_generators);
        return os;
    }
    bool operator<(const Board& other) const {
        for (auto it = p_generators->begin(), end = prev(p_generators->end()), it2 = other.p_generators->begin(), end2 = other.p_generators->end(); it != end;) {
            const pair spike = {it -> first - 1, next(it)-> second - 1};
            while (it2 != end2 && it2->first > spike.first) it2 = next(it2);
            if (it2 == end2) return true;
            if (it2->second <= spike.second) return false;
            it = next(it);
        }
        return true;
    }
    template <typename GenIter1, typename GenIter2, typename GetFunc>
    static bool check(GenIter1 begin1, GenIter1 end1, GenIter2 begin2, GenIter2 end2, GetFunc& first, GetFunc& second) {
        auto match = [&first, &second](GenIter1& it1, GenIter2& it2) {
            return first(*it1) == it2->first && second(*it1) == it2->second;
        };
        end1 = prev(end1);
        end2 = prev(end2);
        GenIter1 it1 = begin1;
        GenIter2 it2 = begin2;
        while (it1 != end1 && it2 != end2 && match(it1, it2)) {
            it1 = next(it1);
            it2 = next(it2);
        }
        GenIter1 rit1 = end1;
        GenIter2 rit2 = end2;
        while (rit1 != begin1 && rit2 != begin2 && match(rit1, rit2)) {
            rit1 = prev(rit1);
            rit2 = prev(rit2);
        }
        if (it1 != rit1) {
            return false;
        }
        if (it2 == rit2) {
            if (it2 == begin2 && it1 == begin1) return first(*it1) < it2->first;
            if (it2 == end2 && it1 == end1) return second(*it1) < it2->second;
            return first(*it1) <= rit2->first && second(*it1) <= it2->second;
        }
        if (it2 != begin2 && prev(it2) == rit2) return true;
        if (it2 != end2 && next(it2) != rit2) return false;
        return first(*it1) <= rit2->first && second(*it1) <= it2->second;
    }
    bool can_reach(const Board* target) const {
        return
            this != target &&
            (
                check(p_generators->begin(), p_generators->end(), target->p_generators->begin(), target->p_generators->end(), get_first, get_second) ||
                check(p_generators->rbegin(), p_generators->rend(), target->p_generators->begin(), target->p_generators->end(), get_second, get_first)
            );
    }
    const Board* consume(const pair<int, int> &position) const {
        list generators(*p_generators);
        auto it = generators.begin();
        while (it != generators.end()) {
            if (position.first < it->first) ++it;
            else if (position.first == it->first) {
                if (position.second >= it->second) return this;
                generators.insert(next(it), position);
                it = next(it);
                break;
            }
            else {
                generators.insert(it, position);
                it = prev(it);
                break;
            }
        }
        while (it != generators.begin() && it->first <= prev(it)->first && it->second <= prev(it)->second) {
            generators.erase(prev(it));
        }
        return get_board(generators);
    }

    static const Board* get_board(int width, int height) {
        list<pair<int, int>> generators({{width, 0}, {0, height}});
        return get_board(generators);
    }

    static const Board* get_board(string input) {
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
        if (input.length() < 2 || input.front() != '<' || input.back() != '>' ) {
            throw std::invalid_argument("Invalid format " + input);
        }
        input = input.substr(1, input.size() - 2);
        stringstream ss(input);
        string raw_pair;
        list<pair<int, int>> pairs;
        while (getline(ss, raw_pair, ',')) {
            if (raw_pair.length() < 2 || input.front() != '(' || input.back() != ')') {
                throw std::invalid_argument("Invalid format " + input);
            }
            const size_t comma = raw_pair.find(';');
            int x = stoi(raw_pair.substr(1, comma - 1));
            int y = stoi(raw_pair.substr(comma + 1, raw_pair.size() - comma - 2));
            pairs.emplace_back(x, y);
        }
        return get_board(pairs);
    }

    static const Board* get_board(list<pair<int, int>> generators) {
        simplify(generators);
        auto it = boards.find(generators);
        if (it != boards.end()) {
            return it->second;
        }
        boards.insert({generators, nullptr});
        it = boards.find(generators);
        it->second = new Board(&(it->first));
        return it->second;
    }
    static void clean_up() {
        for (auto&[fst, snd] : boards) delete snd;
        boards.clear();
    }
    static void flip(list<pair<int, int>>& list) {
        if (list.size() <= 1) return;
        for (auto left = list.begin(), right = prev(list.end()); left != next(right); ++left, --right) {
            swap(left->first, right->second);
            if (left != right) swap(left->second, right->first);
            if (left == right || left == prev(right)) break;
        }
    }

    static void simplify(list<pair<int, int>>& generators) {
        if (generators.empty()) generators.emplace_back(0, 0);
        generators.sort([](const pair<int, int>& lhs, const pair<int, int>& rhs) {
            return lhs.first > rhs.first || lhs.first == rhs.first && lhs.second < rhs.second;
        });
        for (auto it = generators.begin(); it != prev(generators.end());) {
            if (auto nit = next(it); it->first > nit->first && it->second < nit->second || it->first == nit->first && it->second < nit->second) it++;
            else if (it->first > nit->first && it->second >= nit->second) {
                generators.erase(it);
                if (nit != generators.begin()) it = prev(nit);
                else it = nit;
            }
            else generators.erase(nit);
        }
        for (auto left = generators.begin(), right = prev(generators.end()); left != generators.end(); ++left, --right) {
            if (left->first < right->second) {
                flip(generators);
                break;
            }
        }
        generators.begin()->second = 0;
        prev(generators.end())->first = 0;
    }
};
unordered_map<list<std::pair<int, int>>, Board*, PairListHash, ListPairEqual> Board::boards;

class Evidence {
    private:
        unordered_map<const Board*, const Board*> map;
    public:
        set<const Board*> losing_boards;
        Evidence() {
            const Board* poison = Board::get_board({{1, 0}, {0, 1}});
            map.insert({nullptr, nullptr});
            map.insert({poison, nullptr});
            losing_boards.insert(poison);
        }
        const Board* get(const Board* board) {
            if (!map.count(board)) {
                const Board* other = nullptr;
                if (check_from_losing_states(board, other)) {
                    map.insert({board, other});
                }
                if (check_pattern(board, other)) {
                    map.insert({board, other});
                }
                else {
                    bool found = false;
                    for (pair<int, int> move : *board) {
                        other = board->consume(move);
                        if (get(other) == nullptr) {
                            map.insert({board, other});
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        map.insert({board, nullptr});
                        losing_boards.insert(board);
                    }
                }
            }
            return map[board];
        }
        bool check_from_losing_states(const Board* board, const Board*& result) const {
            for (const Board* other : losing_boards) {
                if (other->can_reach(board)) {
                    result = other;
                    return true;
                }
            }
            result = nullptr;
            return false;
        }
        static bool check_pattern(const Board* board, const Board*& result) {
            switch(const list<pair<int, int>>& generators = board->get_generators(); generators.size()) {
                case 1:
                    return false;
                case 2: {
                    const vector pairs(generators.begin(), generators.end());
                    if (pairs[1].second == 2) {
                        result = board->consume({pairs[0].first - 1, 1});
                        return true;
                    }
                    if (pairs[0].first == pairs[1].second) {
                        result = board->consume({1, 1});
                        return true;
                    }
                    break;
                }
                case 3: {
                    vector pairs(generators.begin(), generators.end());
                    if (pairs[1] == make_pair(1, 1)) {
                        if (pairs[0].first == pairs[2].second) {
                            result = nullptr;
                            return true;
                        }
                        result = board->consume({pairs[2].second, 0});
                        return true;
                    }
                    if (pairs[2].second == 2) {
                        if (pairs[1].first == pairs[0].first - 1) {
                            result = nullptr;
                            return true;
                        }
                        result = board->consume({pairs[1].first + 1, 0});
                        return true;
                    }
                    break;
                }
                default: ;
            }
            return false;
        }
};

void show_evidence(Evidence& evidence, const Board* board) {
    const Board* other = evidence.get(board);
    cout << *board;
    if (other == nullptr) {
        cout << " is a losing state!";
    }
    else {
        cout << " is a winning state! an evidence is " << *other;
    }
    cout << endl;
}

int main() {
    Evidence evidence;
    cout << boolalpha;
    Board const *origin = Board::get_board("<(5;0), (2;2), (0;3)>"), *off = Board::get_board("<(6;0),(2;2),(0;3)>");
    for (pair move : *origin) {
        cout << move << " " << *origin->consume(move) << " " << origin->consume(move)->can_reach(off) << endl;
    }
    Board::get_board("<(5;0),(1;1),(0;3)>")->can_reach(off);
    Board::clean_up();
}