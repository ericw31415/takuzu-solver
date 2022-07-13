#include <fstream>
#include <iostream>
#include <vector>

#include <cstdlib>
#include <cstring>

void print_usage()
{
    std::cout << "Usage: takuzu-solve [options] [size] [file]\n";
    std::cout << "Options:\n";
    std::cout << "  --help\t\tDisplay this help\n\n";
    std::cout << "The [size] parameter is the width of the board.\n";
}

std::vector<std::vector<char>> read_board(int size, std::ifstream& file)
{
    std::vector<std::vector<char>> board(size, std::vector<char>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j] = file.get();
        }
        file.get();
    }
    return board;
}

// Return true if a valid move is found
bool move(std::vector<std::vector<char>>& board)
{
    bool found_move = false;
    for (std::size_t i = 0; i < board.size(); ++i) {
        for (std::size_t j = 0; j < board.size(); ++j) {
            if (board[i][j] != '.')
                continue;

            // Solve blocks of 2
            if ((i > 1 && board[i - 1][j] == '0'
                    && board[i - 1][j] == board[i - 2][j])
                    || (i < board.size() - 2 && board[i + 1][j] == '0'
                    && board[i + 1][j] == board[i + 2][j])
                    || (j > 1 && board[i][j - 1] == '0'
                    && board[i][j - 1] == board[i][j - 2])
                    || (j < board.size() - 2 && board[i][j + 1] == '0'
                    && board[i][j + 1] == board[i][j + 2])) {
                board[i][j] = '1';
                found_move = true;
            } else if ((i > 1 && board[i - 1][j] == '1'
                    && board[i - 1][j] == board[i - 2][j])
                    || (i < board.size() - 2 && board[i + 1][j] == '1'
                    && board[i + 1][j] == board[i + 2][j])
                    || (j > 1 && board[i][j - 1] == '1'
                    && board[i][j - 1] == board[i][j - 2])
                    || (j < board.size() - 2 && board[i][j + 1] == '1'
                    && board[i][j + 1] == board[i][j + 2])) {
                board[i][j] = '0';
                found_move = true;
            }

            // Solve blocks with a middle gap
            if ((0 < i && i < board.size() - 1 && board[i - 1][j] == '0'
                    && board[i - 1][j] == board[i + 1][j])
                    || (0 < j && j < board.size() - 1 && board[i][j - 1] == '0'
                    && board[i][j - 1] == board[i][j + 1])) {
                board[i][j] = '1';
                found_move = true;
            } else if ((0 < i && i < board.size() - 1 && board[i - 1][j] == '1'
                    && board[i - 1][j] == board[i + 1][j])
                    || (0 < j && j < board.size() - 1 && board[i][j - 1] == '1'
                    && board[i][j - 1] == board[i][j + 1])) {
                board[i][j] = '0';
                found_move = true;
            }
        }
    }

    // Solve equal number of 0s and 1s
    for (std::size_t i = 0; i < board.size(); ++i) {
        int zeros = 0;
        int ones = 0;
        for (std::size_t j = 0; j < board.size(); ++j) {
            if (board[i][j] == '0')
                ++zeros;
            else if (board[i][j] == '1')
                ++ones;
        }
        if (zeros == static_cast<int>(board.size() / 2) && ones < zeros) {
            for (std::size_t j = 0; j < board.size(); ++j) {
                if (board[i][j] == '.')
                    board[i][j] = '1';
            }
            found_move = true;
        } else if (ones == static_cast<int>(board.size() / 2) && zeros < ones) {
            for (std::size_t j = 0; j < board.size(); ++j) {
                if (board[i][j] == '.')
                    board[i][j] = '0';
            }
            found_move = true;
        }
    }
    for (std::size_t j = 0; j < board.size(); ++j) {
        int zeros = 0;
        int ones = 0;
        for (std::size_t i = 0; i < board.size(); ++i) {
            if (board[i][j] == '0')
                ++zeros;
            else if (board[i][j] == '1')
                ++ones;
        }
        if (zeros == static_cast<int>(board.size() / 2) && ones < zeros) {
            for (std::size_t i = 0; i < board.size(); ++i) {
                if (board[i][j] == '.')
                    board[i][j] = '1';
            }
            found_move = true;
        } else if (ones == static_cast<int>(board.size() / 2) && zeros < ones) {
            for (std::size_t i = 0; i < board.size(); ++i) {
                if (board[i][j] == '.')
                    board[i][j] = '0';
            }
            found_move = true;
        }
    }

    // Solve uniqueness of rows and columns
    for (std::size_t i1 = 0; i1 < board.size(); ++i1) {
        int zeros1 = 0;
        for (std::size_t j = 0; j < board.size(); ++j) {
            if (board[i1][j] == '0')
                ++zeros1;
        }
        if (zeros1 != static_cast<int>(board.size() / 2))
            continue;

        for (std::size_t i2 = 0; i2 < board.size(); ++i2) {
            if (i1 == i2)
                continue;

            int matches = 0;
            int blanks = 0;
            for (std::size_t j = 0; j < board.size(); ++j) {
                if (board[i1][j] == '0' && board[i2][j] == '0')
                    ++matches;
                else if (board[i2][j] == '.')
                    ++blanks;
            }
            if (matches == zeros1 - 1 && blanks == 2) {
                for (std::size_t j = 0; j < board.size(); ++j) {
                    if (board[i1][j] == '0' && board[i2][j] == '.') {
                        board[i2][j] = '1';
                        break;
                    }
                }
                found_move = true;
            }
        }
    }
    for (std::size_t j1 = 0; j1 < board.size(); ++j1) {
        int zeros1 = 0;
        for (std::size_t i = 0; i < board.size(); ++i) {
            if (board[i][j1] == '0')
                ++zeros1;
        }
        if (zeros1 != static_cast<int>(board.size() / 2))
            continue;

        for (std::size_t j2 = 0; j2 < board.size(); ++j2) {
            if (j1 == j2)
                continue;

            int matches = 0;
            int blanks = 0;
            for (std::size_t i = 0; i < board.size(); ++i) {
                if (board[i][j1] == '0' && board[i][j2] == '0')
                    ++matches;
                else if (board[i][j2] == '.')
                    ++blanks;
            }
            if (matches == zeros1 - 1 && blanks == 2) {
                for (std::size_t i = 0; i < board.size(); ++i) {
                    if (board[i][j1] == '0' && board[i][j2] == '.') {
                        board[i][j2] = '1';
                        break;
                    }
                }
                found_move = true;
            }
        }
    }

    return found_move;
}

void solve(std::vector<std::vector<char>>& board)
{
    while (move(board))
        continue;
}

void print_board(const std::vector<std::vector<char>>& board)
{
    for (const auto& row : board) {
        for (auto j : row)
            std::cout.put(j);
        std::cout.put('\n');
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1 || argc == 2 && strcmp("--help", argv[1]) == 0) {
        print_usage();
    } else if (argc == 3) {
        int size = 0;
        std::ifstream file{argv[2]};

        for (std::size_t i = 0; argv[1][i]; ++i) {
            if ('0' <= argv[1][i] && argv[1][i] <= '9') {
                size = size * 10 + argv[1][i] - '0';
            } else {
                std::cerr << "Error: invalid board size\n";
                return EXIT_FAILURE;
            }
        }
        if (size % 2 != 0) {
            std::cerr << "Error: board size must be even\n";
            return EXIT_FAILURE;
        }

        if (!file) {
            std::cerr << "Error: file does not exist\n";
            return EXIT_FAILURE;
        }

        std::vector<std::vector<char>> board{read_board(size, file)};
        solve(board);
        print_board(board);
    }

    return EXIT_SUCCESS;
}
