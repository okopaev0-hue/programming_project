/**
 * @file tic_tac_toe.cpp
 * @brief Реализация функций игровой логики Tic Tac Toe.
 *
 * В этом файле реализованы функции, объявленные в `tic_tac_toe.hpp`.
 * Эти функции не зависят от консольного ввода-вывода и поэтому могут
 * использоваться как в основной игре, так и в автоматических тестах.
 */

#include "tic_tac_toe.hpp"

char getPlayerSymbol(int playerIndex) {
    if (playerIndex < 0) {
        return '?';
    }

    if (playerIndex == 0) {
        return 'X';
    }

    if (playerIndex == 1) {
        return 'O';
    }

    return static_cast<char>('A' + playerIndex - 2);
}

bool isValidGameMode(int gameMode) {
    return gameMode == 1 || gameMode == 2;
}

bool isValidPlayersCount(int playersCount) {
    return playersCount >= 2 && playersCount <= 9;
}

bool isValidRoundsCount(int roundsCount) {
    return roundsCount >= 1;
}

bool isValidBoardSize(int boardSize) {
    return boardSize == 3 || boardSize == 4 || boardSize == 5;
}

bool isCellInside(const Board& board, int row, int col) {
    int size = static_cast<int>(board.size());

    return row >= 0 && row < size && col >= 0 && col < size;
}

bool isCellFree(const Board& board, int row, int col) {
    if (!isCellInside(board, row, col)) {
        return false;
    }

    return board[row][col] == ' ';
}

bool makeMove(Board& board, int row, int col, char symbol) {
    if (!isCellFree(board, row, col)) {
        return false;
    }

    board[row][col] = symbol;
    return true;
}

bool checkWin(const Board& board, char symbol) {
    int size = static_cast<int>(board.size());

    for (int row = 0; row < size; row++) {
        bool rowWin = true;

        for (int col = 0; col < size; col++) {
            if (board[row][col] != symbol) {
                rowWin = false;
                break;
            }
        }

        if (rowWin) {
            return true;
        }
    }

    for (int col = 0; col < size; col++) {
        bool colWin = true;

        for (int row = 0; row < size; row++) {
            if (board[row][col] != symbol) {
                colWin = false;
                break;
            }
        }

        if (colWin) {
            return true;
        }
    }

    bool mainDiagonalWin = true;

    for (int i = 0; i < size; i++) {
        if (board[i][i] != symbol) {
            mainDiagonalWin = false;
            break;
        }
    }

    if (mainDiagonalWin) {
        return true;
    }

    bool secondDiagonalWin = true;

    for (int i = 0; i < size; i++) {
        if (board[i][size - 1 - i] != symbol) {
            secondDiagonalWin = false;
            break;
        }
    }

    if (secondDiagonalWin) {
        return true;
    }

    return false;
}

bool isDraw(const Board& board) {
    int size = static_cast<int>(board.size());

    if (size == 0) {
        return false;
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                return false;
            }
        }
    }

    return true;
}

std::vector<std::pair<int, int>> getFreeCells(const Board& board) {
    std::vector<std::pair<int, int>> freeCells;
    int size = static_cast<int>(board.size());

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                freeCells.push_back({row, col});
            }
        }
    }

    return freeCells;
}

std::pair<int, int> getRandomMove(const Board& board, std::mt19937& generator) {
    std::vector<std::pair<int, int>> freeCells = getFreeCells(board);

    if (freeCells.empty()) {
        return {-1, -1};
    }

    std::uniform_int_distribution<int> distribution(
        0,
        static_cast<int>(freeCells.size()) - 1
    );

    return freeCells[distribution(generator)];
}