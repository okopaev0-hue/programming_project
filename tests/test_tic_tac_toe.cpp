/**
 * @file test_tic_tac_toe.cpp
 * @brief Набор модульных тестов для игровой логики Tic Tac Toe.
 *
 * Тесты написаны с использованием doctest и проверяют функции,
 * объявленные в `tic_tac_toe.hpp`.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <algorithm>
#include <random>
#include <utility>
#include <vector>

#include "tic_tac_toe.hpp"

TEST_CASE("getPlayerSymbol returns X for first player") {
    CHECK(getPlayerSymbol(0) == 'X');
}

TEST_CASE("getPlayerSymbol returns O for second player") {
    CHECK(getPlayerSymbol(1) == 'O');
}

TEST_CASE("getPlayerSymbol returns letters for additional players") {
    CHECK(getPlayerSymbol(2) == 'A');
    CHECK(getPlayerSymbol(3) == 'B');
    CHECK(getPlayerSymbol(4) == 'C');
    CHECK(getPlayerSymbol(8) == 'G');
}

TEST_CASE("getPlayerSymbol returns question mark for negative player index") {
    CHECK(getPlayerSymbol(-1) == '?');
    CHECK(getPlayerSymbol(-10) == '?');
}

TEST_CASE("isValidGameMode accepts valid modes") {
    CHECK(isValidGameMode(1));
    CHECK(isValidGameMode(2));
}

TEST_CASE("isValidGameMode rejects invalid modes") {
    CHECK_FALSE(isValidGameMode(0));
    CHECK_FALSE(isValidGameMode(3));
    CHECK_FALSE(isValidGameMode(-1));
    CHECK_FALSE(isValidGameMode(100));
}

TEST_CASE("isValidPlayersCount accepts valid player counts") {
    CHECK(isValidPlayersCount(2));
    CHECK(isValidPlayersCount(3));
    CHECK(isValidPlayersCount(5));
    CHECK(isValidPlayersCount(9));
}

TEST_CASE("isValidPlayersCount rejects invalid player counts") {
    CHECK_FALSE(isValidPlayersCount(0));
    CHECK_FALSE(isValidPlayersCount(1));
    CHECK_FALSE(isValidPlayersCount(10));
    CHECK_FALSE(isValidPlayersCount(-5));
}

TEST_CASE("isValidRoundsCount accepts positive round counts") {
    CHECK(isValidRoundsCount(1));
    CHECK(isValidRoundsCount(2));
    CHECK(isValidRoundsCount(10));
    CHECK(isValidRoundsCount(100));
}

TEST_CASE("isValidRoundsCount rejects zero and negative round counts") {
    CHECK_FALSE(isValidRoundsCount(0));
    CHECK_FALSE(isValidRoundsCount(-1));
    CHECK_FALSE(isValidRoundsCount(-10));
}

TEST_CASE("isValidBoardSize accepts 3, 4 and 5") {
    CHECK(isValidBoardSize(3));
    CHECK(isValidBoardSize(4));
    CHECK(isValidBoardSize(5));
}

TEST_CASE("isValidBoardSize rejects invalid sizes") {
    CHECK_FALSE(isValidBoardSize(0));
    CHECK_FALSE(isValidBoardSize(1));
    CHECK_FALSE(isValidBoardSize(2));
    CHECK_FALSE(isValidBoardSize(6));
    CHECK_FALSE(isValidBoardSize(-3));
}

TEST_CASE("isCellInside returns true for valid cells on 3x3 board") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK(isCellInside(board, 0, 0));
    CHECK(isCellInside(board, 1, 1));
    CHECK(isCellInside(board, 2, 2));
}

TEST_CASE("isCellInside returns false for invalid cells on 3x3 board") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK_FALSE(isCellInside(board, -1, 0));
    CHECK_FALSE(isCellInside(board, 0, -1));
    CHECK_FALSE(isCellInside(board, 3, 0));
    CHECK_FALSE(isCellInside(board, 0, 3));
    CHECK_FALSE(isCellInside(board, 100, 100));
}

TEST_CASE("isCellInside works on 5x5 board") {
    Board board(5, std::vector<char>(5, ' '));

    CHECK(isCellInside(board, 0, 0));
    CHECK(isCellInside(board, 4, 4));

    CHECK_FALSE(isCellInside(board, 5, 0));
    CHECK_FALSE(isCellInside(board, 0, 5));
}

TEST_CASE("isCellFree returns true for empty cells") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK(isCellFree(board, 0, 0));
    CHECK(isCellFree(board, 1, 1));
    CHECK(isCellFree(board, 2, 2));
}

TEST_CASE("isCellFree returns false for occupied cells") {
    Board board(3, std::vector<char>(3, ' '));

    board[0][0] = 'X';
    board[1][1] = 'O';

    CHECK_FALSE(isCellFree(board, 0, 0));
    CHECK_FALSE(isCellFree(board, 1, 1));
}

TEST_CASE("isCellFree returns false for cells outside board") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK_FALSE(isCellFree(board, -1, 0));
    CHECK_FALSE(isCellFree(board, 0, -1));
    CHECK_FALSE(isCellFree(board, 3, 0));
    CHECK_FALSE(isCellFree(board, 0, 3));
}

TEST_CASE("makeMove places symbol into empty cell") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK(makeMove(board, 0, 0, 'X'));
    CHECK(board[0][0] == 'X');

    CHECK(makeMove(board, 1, 1, 'O'));
    CHECK(board[1][1] == 'O');
}

TEST_CASE("makeMove does not overwrite occupied cell") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK(makeMove(board, 0, 0, 'X'));
    CHECK_FALSE(makeMove(board, 0, 0, 'O'));

    CHECK(board[0][0] == 'X');
}

TEST_CASE("makeMove returns false for invalid coordinates") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK_FALSE(makeMove(board, -1, 0, 'X'));
    CHECK_FALSE(makeMove(board, 0, -1, 'X'));
    CHECK_FALSE(makeMove(board, 3, 0, 'X'));
    CHECK_FALSE(makeMove(board, 0, 3, 'X'));
}

TEST_CASE("checkWin detects row win") {
    Board board = {
        {'X', 'X', 'X'},
        {' ', 'O', ' '},
        {'O', ' ', ' '}
    };

    CHECK(checkWin(board, 'X'));
}

TEST_CASE("checkWin returns false when row is incomplete") {
    Board board = {
        {'X', 'X', ' '},
        {' ', 'O', ' '},
        {'O', ' ', ' '}
    };

    CHECK_FALSE(checkWin(board, 'X'));
}

TEST_CASE("checkWin detects column win") {
    Board board = {
        {'O', 'X', ' '},
        {'O', 'X', ' '},
        {'O', ' ', 'X'}
    };

    CHECK(checkWin(board, 'O'));
}

TEST_CASE("checkWin returns false when column is incomplete") {
    Board board = {
        {'O', 'X', ' '},
        {' ', 'X', ' '},
        {'O', ' ', 'X'}
    };

    CHECK_FALSE(checkWin(board, 'O'));
}

TEST_CASE("checkWin detects main diagonal win") {
    Board board = {
        {'X', 'O', ' '},
        {' ', 'X', 'O'},
        {' ', ' ', 'X'}
    };

    CHECK(checkWin(board, 'X'));
}

TEST_CASE("checkWin returns false when main diagonal is incomplete") {
    Board board = {
        {'X', 'O', ' '},
        {' ', 'O', 'O'},
        {' ', ' ', 'X'}
    };

    CHECK_FALSE(checkWin(board, 'X'));
}

TEST_CASE("checkWin detects secondary diagonal win") {
    Board board = {
        {' ', 'O', 'X'},
        {' ', 'X', 'O'},
        {'X', ' ', ' '}
    };

    CHECK(checkWin(board, 'X'));
}

TEST_CASE("checkWin returns false when secondary diagonal is incomplete") {
    Board board = {
        {' ', 'O', 'X'},
        {' ', 'O', 'O'},
        {'X', ' ', ' '}
    };

    CHECK_FALSE(checkWin(board, 'X'));
}

TEST_CASE("checkWin returns false on empty board") {
    Board board(3, std::vector<char>(3, ' '));

    CHECK_FALSE(checkWin(board, 'X'));
    CHECK_FALSE(checkWin(board, 'O'));
}

TEST_CASE("checkWin works on 4x4 board") {
    Board winningBoard = {
        {'X', ' ', ' ', ' '},
        {' ', 'X', ' ', ' '},
        {' ', ' ', 'X', ' '},
        {' ', ' ', ' ', 'X'}
    };

    Board nonWinningBoard = {
        {'X', ' ', ' ', ' '},
        {' ', 'X', ' ', ' '},
        {' ', ' ', 'O', ' '},
        {' ', ' ', ' ', 'X'}
    };

    CHECK(checkWin(winningBoard, 'X'));
    CHECK_FALSE(checkWin(nonWinningBoard, 'X'));
}

TEST_CASE("checkWin works on 5x5 board") {
    Board winningBoard = {
        {'O', ' ', ' ', ' ', ' '},
        {'O', 'X', ' ', ' ', ' '},
        {'O', ' ', 'X', ' ', ' '},
        {'O', ' ', ' ', 'X', ' '},
        {'O', ' ', ' ', ' ', 'X'}
    };

    Board nonWinningBoard = {
        {'O', ' ', ' ', ' ', ' '},
        {'O', 'X', ' ', ' ', ' '},
        {' ', ' ', 'X', ' ', ' '},
        {'O', ' ', ' ', 'X', ' '},
        {'O', ' ', ' ', ' ', 'X'}
    };

    CHECK(checkWin(winningBoard, 'O'));
    CHECK_FALSE(checkWin(nonWinningBoard, 'O'));
}

TEST_CASE("isDraw returns true for full board") {
    Board board = {
        {'X', 'O', 'X'},
        {'X', 'O', 'O'},
        {'O', 'X', 'X'}
    };

    CHECK(isDraw(board));
}

TEST_CASE("isDraw returns false when board has empty cells") {
    Board board = {
        {'X', 'O', 'X'},
        {'X', ' ', 'O'},
        {'O', 'X', 'X'}
    };

    CHECK_FALSE(isDraw(board));
}

TEST_CASE("isDraw returns false for empty board") {
    Board board;

    CHECK_FALSE(isDraw(board));
}

TEST_CASE("getFreeCells returns all cells on empty 3x3 board") {
    Board board(3, std::vector<char>(3, ' '));

    std::vector<std::pair<int, int>> freeCells = getFreeCells(board);

    CHECK(freeCells.size() == 9);
}

TEST_CASE("getFreeCells returns only empty cells") {
    Board board = {
        {'X', 'O', 'X'},
        {' ', 'O', ' '},
        {'X', ' ', 'O'}
    };

    std::vector<std::pair<int, int>> freeCells = getFreeCells(board);

    CHECK(freeCells.size() == 3);

    CHECK(
        std::find(freeCells.begin(), freeCells.end(), std::make_pair(1, 0))
        != freeCells.end()
    );

    CHECK(
        std::find(freeCells.begin(), freeCells.end(), std::make_pair(1, 2))
        != freeCells.end()
    );

    CHECK(
        std::find(freeCells.begin(), freeCells.end(), std::make_pair(2, 1))
        != freeCells.end()
    );
}

TEST_CASE("getFreeCells returns empty vector when board is full") {
    Board board = {
        {'X', 'O', 'X'},
        {'X', 'O', 'O'},
        {'O', 'X', 'X'}
    };

    std::vector<std::pair<int, int>> freeCells = getFreeCells(board);

    CHECK(freeCells.empty());
}

TEST_CASE("getRandomMove returns only available cell when one cell is free") {
    Board board = {
        {'X', 'O', 'X'},
        {'X', ' ', 'O'},
        {'O', 'X', 'X'}
    };

    std::mt19937 generator(123);

    std::pair<int, int> move = getRandomMove(board, generator);

    CHECK(move.first == 1);
    CHECK(move.second == 1);
}

TEST_CASE("getRandomMove returns negative coordinates when no free cells exist") {
    Board board = {
        {'X', 'O', 'X'},
        {'X', 'O', 'O'},
        {'O', 'X', 'X'}
    };

    std::mt19937 generator(123);

    std::pair<int, int> move = getRandomMove(board, generator);

    CHECK(move.first == -1);
    CHECK(move.second == -1);
}

TEST_CASE("getRandomMove always returns a valid free cell") {
    Board board = {
        {'X', 'O', 'X'},
        {' ', 'O', ' '},
        {'X', ' ', 'O'}
    };

    std::mt19937 generator(123);

    for (int i = 0; i < 100; i++) {
        std::pair<int, int> move = getRandomMove(board, generator);

        REQUIRE(isCellInside(board, move.first, move.second));
        CHECK(isCellFree(board, move.first, move.second));
    }
}

TEST_CASE("getRandomMove works on empty 5x5 board") {
    Board board(5, std::vector<char>(5, ' '));

    std::mt19937 generator(123);

    std::pair<int, int> move = getRandomMove(board, generator);

    CHECK(isCellInside(board, move.first, move.second));
    CHECK(isCellFree(board, move.first, move.second));
}