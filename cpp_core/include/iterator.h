#ifndef ITERATOR_H
#define ITERATOR_H

#include <list>
#include "board.h"

class AbstractIterator {
    protected:
        const Board* board;
        std::pair<int, int> current, end;
        virtual AbstractIterator& advance() = 0;
    public: 
        AbstractIterator(const Board* board);
        bool hasNext() const;
        std::pair<int, int> getNext();
        virtual ~AbstractIterator() {}
};

class Iterator : public AbstractIterator {
    private:
        std::list<std::pair<int, int>>::const_iterator prev, it;
    protected:
        AbstractIterator& advance();
    public:
        Iterator(const Board* board);
        virtual void init();
};

#endif