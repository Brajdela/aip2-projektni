#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

// ANSI boje
const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string YELLOW = "\033[1;33m";
const string BLUE = "\033[1;34m";
const string MAGENTA = "\033[1;35m";
const string CYAN = "\033[1;36m";
const string RESET = "\033[0m";

// ASCII naslov
void showTitle()
{
    cout << CYAN;
    cout << R"(

    
    )" << RESET
         << "\n";
    cout << YELLOW << "                          Dobrodo\u0161li u igru Kri\u017ei\u0107-Kru\u017ei\u0107! \U0001F3AE\n"
         << RESET;
}

struct Player
{
    string name;
    char symbol;
    int wins = 0;
};

class Game
{
private:
    char board[3][3];
    Player player1, player2;
    bool player1Turn;
    bool gameLoaded = false;

public:
    Game()
    {
        resetBoard();
    }

    void addPlayers()
    {
        cout << MAGENTA << "\U0001F464 Unesite ime igra\u010da 1: " << RESET;
        cin >> player1.name;
        player1.symbol = 'X';

        cout << MAGENTA << "\U0001F464 Unesite ime igra\u010da 2: " << RESET;
        cin >> player2.name;
        player2.symbol = 'O';

        player1Turn = true;
        cout << GREEN << "\u2705 Igra\u010di su dodani!\n"
             << RESET;
    }

    void resetBoard()
    {
        char start = '1';
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                board[i][j] = start++;
    }

    void displayBoard()
    {
        cout << "\n";
        for (int i = 0; i < 3; ++i)
        {
            cout << " ";
            for (int j = 0; j < 3; ++j)
            {
                cout << CYAN << board[i][j] << RESET;
                if (j < 2)
                    cout << " | ";
            }
            if (i < 2)
                cout << "\n-----------\n";
        }
        cout << "\n";
    }

    bool makeMove(int position)
    {
        char posChar = position + '0';
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == posChar)
                {
                    board[i][j] = getCurrentPlayer().symbol;
                    return true;
                }
        return false;
    }

    Player &getCurrentPlayer()
    {
        return player1Turn ? player1 : player2;
    }

    Player &getOtherPlayer()
    {
        return player1Turn ? player2 : player1;
    }

    bool checkWin()
    {
        char s = getCurrentPlayer().symbol;
        for (int i = 0; i < 3; ++i)
            if ((board[i][0] == s && board[i][1] == s && board[i][2] == s) ||
                (board[0][i] == s && board[1][i] == s && board[2][i] == s))
                return true;
        if ((board[0][0] == s && board[1][1] == s && board[2][2] == s) ||
            (board[0][2] == s && board[1][1] == s && board[2][0] == s))
            return true;
        return false;
    }

    bool isDraw()
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (isdigit(board[i][j]))
                    return false;
        return true;
    }

    void switchTurn()
    {
        player1Turn = !player1Turn;
    }

    void saveGame()
    {
        ofstream file("savegame.txt");
        file << player1.name << " " << player1.wins << "\n";
        file << player2.name << " " << player2.wins << "\n";
        file << player1Turn << "\n";
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                file << board[i][j];
        file.close();
        cout << GREEN << "\U0001F4BE Igra je snimljena!\n"
             << RESET;
    }

    bool loadGame()
    {
        ifstream file("savegame.txt");
        if (!file.is_open())
        {
            cout << RED << "\u26A0\uFE0F  Nema snimljene igre.\n"
                 << RESET;
            return false;
        }
        file >> player1.name >> player1.wins;
        file >> player2.name >> player2.wins;
        file >> player1Turn;

        char c;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                file >> c;
                board[i][j] = c;
            }

        file.close();
        gameLoaded = true;
        cout << GREEN << "\U0001F4C2 Prethodna igra je u\u010ditana!\n"
             << RESET;
        return true;
    }

    void play()
    {
        if (!gameLoaded && (player1.name.empty() || player2.name.empty()))
        {
            cout << RED << "\u26A0\uFE0F  Prvo dodajte igra\u010de!\n"
                 << RESET;
            return;
        }
        int move;
        if (!gameLoaded)
            resetBoard();

        while (true)
        {
            displayBoard();
            cout << YELLOW << "\U0001F3B2 " << getCurrentPlayer().name << " (" << getCurrentPlayer().symbol << "), unesite potez (1-9): " << RESET;
            cin >> move;
            if (!makeMove(move))
            {
                cout << RED << "\u274C Neispravan potez!\n"
                     << RESET;
                continue;
            }
            if (checkWin())
            {
                displayBoard();
                cout << GREEN << "\U0001F3C6 \u010cestitke! \U0001F389 Igra\u010d " << getCurrentPlayer().name << " je pobijedio! \U0001F973\n"
                     << RESET;
                getCurrentPlayer().wins++;
                updateLeaderboard();
                break;
            }
            if (isDraw())
            {
                displayBoard();
                cout << YELLOW << "\U0001F91D Nerije\u0161eno!\n"
                     << RESET;
                break;
            }
            switchTurn();
        }
        saveGame();
        gameLoaded = false;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << CYAN << "\U0001F501 Pritisnite Enter za povratak u izbornik..." << RESET;
        cin.get();
    }

    void showRules()
    {
        cout << BLUE << "\n\U0001F4D8 Pravila Kri\u017ei\u0107-Kru\u017ei\u0107\n"
             << RESET;
        cout << "1\uFE0F\u20E3  Dva igra\u010da naizmjeni\u010dno biraju pozicije od 1 do 9.\n";
        cout << "2\uFE0F\u20E3  Cilj je slo\u017eiti 3 ista znaka (X ili O) u liniji.\n";
        cout << "3\uFE0F\u20E3  Ako je plo\u010da puna, a nitko nije pobijedio – igra je nerije\u0161ena.\n";
        cout << "4\uFE0F\u20E3  Igra se mo\u017ee spremiti i ponovno u\u010ditati.\n";
    }

    void showLeaderboard()
    {
        cout << MAGENTA << "\n\U0001F4CA Leaderboard:\n"
             << RESET;
        ifstream file("leaderboard.txt");
        string name;
        int wins;
        vector<pair<int, string>> scores;
        while (file >> name >> wins)
        {
            scores.push_back({wins, name});
        }
        sort(scores.rbegin(), scores.rend());
        for (auto &pair : scores)
        {
            cout << "\U0001F3C5 " << pair.second << " - " << pair.first << " pobjeda\n";
        }
        file.close();
    }

    void updateLeaderboard()
    {
        map<string, int> scores;
        ifstream infile("leaderboard.txt");
        string name;
        int wins;
        while (infile >> name >> wins)
            scores[name] = wins;
        infile.close();

        scores[player1.name] = player1.wins;
        scores[player2.name] = player2.wins;

        ofstream outfile("leaderboard.txt");
        for (auto &pair : scores)
            outfile << pair.first << " " << pair.second << "\n";
        outfile.close();
    }
};

void showMenu(Game &game)
{
    int choice;
    do
    {
        showTitle();
        cout << YELLOW << "\n\U0001F4CB GLAVNI IZBORNIK:\n"
             << RESET;
        cout << "1\uFE0F\u20E3  Dodaj igra\u010de \U0001F46E\n";
        cout << "2\uFE0F\u20E3  Pokreni novu igru \U0001F3AE\n";
        cout << "3\uFE0F\u20E3  U\u010ditaj prethodnu igru \U0001F4BE\n";
        cout << "4\uFE0F\u20E3  Prika\u017ei pravila \U0001F4D8\n";
        cout << "5\uFE0F\u20E3  Prika\u017ei leaderboard \U0001F3C6\n";
        cout << "6\uFE0F\u20E3  Izlaz \u274C\n";
        cout << CYAN << "Unesite izbor: " << RESET;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            game.addPlayers();
            break;
        case 2:
            game.play();
            break;
        case 3:
            if (game.loadGame())
                game.play();
            break;
        case 4:
            game.showRules();
            break;
        case 5:
            game.showLeaderboard();
            break;
        case 6:
            cout << GREEN << "\U0001F44B Hvala \u0161to ste igrali!\n"
                 << RESET;
            break;
        default:
            cout << RED << "\u26A0\uFE0F  Neispravan unos.\n"
                 << RESET;
            break;
        }

        cout << "\n"
             << string(60, '=') << "\n";

    } while (choice != 6);
}

int main()
{
    Game game;
    showMenu(game);
    return 0;
}
