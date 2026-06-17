#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <random>
#include <utility>

#include "tic_tac_toe.hpp"

/**
 * @brief Очищает экран консоли.
 *
 * В Windows используется команда `cls`, в других системах — `clear`.
 * Функция применяется перед перерисовкой игрового поля, чтобы создать
 * эффект обновления одного и того же экрана.
 */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief Выводит ASCII-логотип игры.
 *
 * Логотип отображается в начале программы, перед каждым ходом
 * и перед выводом итоговых результатов.
 */
void printLogo() {
    std::cout << R"(
  _______ _        _______             _______
 |__   __(_)      |__   __|           |__   __|
    | |   _  ___     | | __ _  ___       | | ___   ___
    | |  | |/ __|    | |/ _` |/ __|      | |/ _ \ / _ \
    | |  | | (__     | | (_| | (__       | | (_) |  __/
    |_|  |_|\___|    |_|\__,_|\___|      |_|\___/ \___|

)";
}
/**
 * @brief Считывает целое число из стандартного ввода.
 *
 * Если пользователь вводит некорректное значение, функция очищает поток
 * ввода и повторяет запрос до тех пор, пока не будет введено целое число.
 *
 * @return Введённое пользователем целое число.
 */
int getNumberInput() {
    int value;

    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return value;
}
/**
 * @brief Ожидает нажатия клавиши Enter.
 *
 * Функция очищает остаток строки во входном потоке, затем ожидает
 * дополнительный ввод символа новой строки.
 */
void waitForEnter() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
/**
 * @brief Ожидает подтверждения перед ходом компьютера.
 *
 * Используется в режиме "Computer vs Computer", чтобы пользователь
 * мог последовательно наблюдать за каждым ходом.
 */
void waitForComputerMove() {
    std::cout << "Press Enter to make computer move.";
    waitForEnter();
}
/**
 * @brief Выводит игровое поле в консоль в виде ASCII-графики.
 *
 * Размер поля определяется автоматически по размеру переданного объекта `Board`.
 * Функция поддерживает поля 3x3, 4x4 и 5x5.
 *
 * @param board Игровое поле.
 */
void printBoard(const Board& board) {
    int size = static_cast<int>(board.size());

    std::cout << "\n    ";
    for (int col = 1; col <= size; col++) {
        std::cout << "  " << col << " ";
    }
    std::cout << "\n";

    std::cout << "   +";
    for (int col = 0; col < size; col++) {
        std::cout << "---+";
    }
    std::cout << "\n";

    for (int row = 0; row < size; row++) {
        std::cout << " " << row + 1 << " |";

        for (int col = 0; col < size; col++) {
            std::cout << " " << board[row][col] << " |";
        }

        std::cout << "\n";

        std::cout << "   +";
        for (int col = 0; col < size; col++) {
            std::cout << "---+";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
}
/**
 * @brief Выводит игровое поле в консоль в виде ASCII-графики.
 *
 * Размер поля определяется автоматически по размеру переданного объекта `Board`.
 * Функция поддерживает поля 3x3, 4x4 и 5x5.
 *
 * @param board Игровое поле.
 */
int playRound(
    int roundNumber,
    int playersCount,
    int boardSize,
    int gameMode,
    std::mt19937& generator
) {
    Board board(boardSize, std::vector<char>(boardSize, ' '));

    int currentPlayer = 0;

    while (true) {
        clearScreen();
        printLogo();

        std::cout << "Round " << roundNumber << "\n";
        std::cout << "Board size: " << boardSize << "x" << boardSize << "\n";

        if (gameMode == 1) {
            std::cout << "Mode: Players\n";
            std::cout << "Player " << currentPlayer + 1
                      << " turn [" << getPlayerSymbol(currentPlayer) << "]\n";
        } else {
            std::cout << "Mode: Computer vs Computer\n";
            std::cout << "Computer " << currentPlayer + 1
                      << " turn [" << getPlayerSymbol(currentPlayer) << "]\n";
        }

        printBoard(board);

        int row;
        int col;

        if (gameMode == 1) {
            std::cout << "Enter row number: ";
            row = getNumberInput();

            std::cout << "Enter column number: ";
            col = getNumberInput();

            row--;
            col--;

            if (!isCellInside(board, row, col)) {
                std::cout << "\nCell does not exist. Press Enter to try again.";
                waitForEnter();
                continue;
            }

            if (!isCellFree(board, row, col)) {
                std::cout << "\nCell is already occupied. Press Enter to try again.";
                waitForEnter();
                continue;
            }
        } else {
            waitForComputerMove();

            std::pair<int, int> move = getRandomMove(board, generator);
            row = move.first;
            col = move.second;
        }

        char symbol = getPlayerSymbol(currentPlayer);
        makeMove(board, row, col, symbol);

        if (checkWin(board, symbol)) {
            clearScreen();
            printLogo();

            std::cout << "Round " << roundNumber << "\n";
            std::cout << "Board size: " << boardSize << "x" << boardSize << "\n";

            if (gameMode == 1) {
                std::cout << "Mode: Players\n";
            } else {
                std::cout << "Mode: Computer vs Computer\n";
            }

            printBoard(board);

            if (gameMode == 1) {
                std::cout << "Player " << currentPlayer + 1 << " wins this round!\n";
            } else {
                std::cout << "Computer " << currentPlayer + 1 << " wins this round!\n";
            }

            std::cout << "Press Enter to continue.";
            waitForEnter();

            return currentPlayer;
        }

        if (isDraw(board)) {
            clearScreen();
            printLogo();

            std::cout << "Round " << roundNumber << "\n";
            std::cout << "Board size: " << boardSize << "x" << boardSize << "\n";

            if (gameMode == 1) {
                std::cout << "Mode: Players\n";
            } else {
                std::cout << "Mode: Computer vs Computer\n";
            }

            printBoard(board);

            std::cout << "This round is a draw!\n";
            std::cout << "Press Enter to continue.";
            waitForEnter();

            return -1;
        }

        currentPlayer = (currentPlayer + 1) % playersCount;
    }
}
/**
 * @brief Выводит итоговые результаты игры.
 *
 * Функция показывает количество очков каждого игрока или компьютера,
 * количество ничьих и определяет общего победителя по итогам всех раундов.
 *
 * Если несколько участников набрали одинаковое максимальное количество очков,
 * выводится сообщение о ничьей между ними.
 *
 * @param scores Вектор очков участников.
 * @param draws Количество раундов, завершившихся ничьёй.
 * @param gameMode Режим игры: `1` — игроки, `2` — компьютер против компьютера.
 */
void printFinalResults(const std::vector<int>& scores, int draws, int gameMode) {
    clearScreen();
    printLogo();

    std::cout << "Final results\n";
    std::cout << "=============\n\n";

    for (int i = 0; i < static_cast<int>(scores.size()); i++) {
        if (gameMode == 1) {
            std::cout << "Player " << i + 1;
        } else {
            std::cout << "Computer " << i + 1;
        }

        std::cout << " [" << getPlayerSymbol(i) << "]: "
                  << scores[i] << " point(s)\n";
    }

    std::cout << "Draws: " << draws << "\n\n";

    int bestScore = -1;

    for (int score : scores) {
        if (score > bestScore) {
            bestScore = score;
        }
    }

    std::vector<int> winners;

    for (int i = 0; i < static_cast<int>(scores.size()); i++) {
        if (scores[i] == bestScore) {
            winners.push_back(i);
        }
    }

    if (winners.size() == 1) {
        if (gameMode == 1) {
            std::cout << "Winner: Player " << winners[0] + 1;
        } else {
            std::cout << "Winner: Computer " << winners[0] + 1;
        }

        std::cout << " [" << getPlayerSymbol(winners[0]) << "]\n";
    } else {
        std::cout << "The game ended in a tie between: ";

        for (int winner : winners) {
            if (gameMode == 1) {
                std::cout << "Player " << winner + 1 << " ";
            } else {
                std::cout << "Computer " << winner + 1 << " ";
            }
        }

        std::cout << "\n";
    }
}
/**
 * @brief Точка входа в программу.
 *
 * Функция запрашивает у пользователя режим игры, количество игроков,
 * количество раундов и размер игрового поля. Затем запускает указанное
 * количество раундов и выводит итоговый результат.
 *
 * @return Код завершения программы.
 */
int main() {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    clearScreen();
    printLogo();

    std::cout << "Enter the game mode:\n";
    std::cout << "1 - Players\n";
    std::cout << "2 - Computer vs Computer\n";
    std::cout << "Your choice: ";

    int gameMode = getNumberInput();

    while (!isValidGameMode(gameMode)) {
        std::cout << "Game mode must be 1 or 2.\n";
        std::cout << "Your choice: ";
        gameMode = getNumberInput();
    }

    int playersCount;

    if (gameMode == 1) {
        std::cout << "Enter the number of players: ";
        playersCount = getNumberInput();

        while (!isValidPlayersCount(playersCount)) {
            std::cout << "The number of players must be from 2 to 9.\n";
            std::cout << "Enter the number of players: ";
            playersCount = getNumberInput();
        }
    } else {
        playersCount = 2;
    }

    std::cout << "Enter the number of rounds: ";
    int roundsCount = getNumberInput();

    while (!isValidRoundsCount(roundsCount)) {
        std::cout << "The number of rounds must be at least 1.\n";
        std::cout << "Enter the number of rounds: ";
        roundsCount = getNumberInput();
    }

    std::cout << "Enter the board size (3, 4 or 5): ";
    int boardSize = getNumberInput();

    while (!isValidBoardSize(boardSize)) {
        std::cout << "Board size must be 3, 4 or 5.\n";
        std::cout << "Enter the board size (3, 4 or 5): ";
        boardSize = getNumberInput();
    }

    std::vector<int> scores(playersCount, 0);
    int draws = 0;

    for (int round = 1; round <= roundsCount; round++) {
        int winner = playRound(round, playersCount, boardSize, gameMode, generator);

        if (winner == -1) {
            draws++;
        } else {
            scores[winner]++;
        }
    }

    printFinalResults(scores, draws, gameMode);

    return 0;
}