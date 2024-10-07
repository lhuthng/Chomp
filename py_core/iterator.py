def iterator(board):
    for row in range(len(board)):
        for col in range(board[row]):
            yield (col, row)

def reverse_iterator(board):
    for row in range(len(board)-1, -1, -1):
        for col in range(board[row]-1, -1, -1):
            yield (col, row)