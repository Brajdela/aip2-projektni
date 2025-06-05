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
********************************************************
* __                  __            __                 *
*|  \                |  \          |  \                *
*| $$   __   ______   \$$ ________  \$$  _______       *
*| $$  /  \ /      \ |  \|        \|  \ /       \      *
*| $$_/  $$|  $$$$$$\| $$ \$$$$$$$$| $$|  $$$$$$$      *
*| $$   $$ | $$   \$$| $$  /    $$ | $$| $$            *
*| $$$$$$\ | $$      | $$ /  $$$$_ | $$| $$_____       *
*| $$  \$$\| $$      | $$|  $$    \| $$ \$$     \      *
* \$$   \$$ \$$       \$$ \$$$$$$$$ \$$  \$$$$$$$      *
*                                                      *
*                                                      *
*                                                      *
* __                                      __           *
*|  \                                    |  \          *
*| $$   __   ______   __    __  ________  \$$  _______ *
*| $$  /  \ /      \ |  \  |  \|        \|  \ /       \*
*| $$_/  $$|  $$$$$$\| $$  | $$ \$$$$$$$$| $$|  $$$$$$$*
*| $$   $$ | $$   \$$| $$  | $$  /    $$ | $$| $$      *
*| $$$$$$\ | $$      | $$__/ $$ /  $$$$_ | $$| $$_____ *
*| $$  \$$\| $$       \$$    $$|  $$    \| $$ \$$     \*
* \$$   \$$ \$$        \$$$$$$  \$$$$$$$$ \$$  \$$$$$$$*
********************************************************
    )" << RESET
         << "\n";
    cout << YELLOW << "                         Dobrodošli u igru Križić-Kružić! \n\n David Brajdić i David Lisec 🎮\n"
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
        cout << MAGENTA << "👤 Unesite ime igrača 1: " << RESET;
        getline(cin, player1.name);
        player1.symbol = 'X';

        cout << MAGENTA << "👤 Unesite ime igrača 2: " << RESET;
        getline(cin, player2.name);
        player2.symbol = 'O';

        player1Turn = true;
        cout << GREEN << "✅ Igrači su dodani!\n"
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

    Player &getCurrentPlayer() { return player1Turn ? player1 : player2; }
    Player &getOtherPlayer() { return player1Turn ? player2 : player1; }

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

    void switchTurn() { player1Turn = !player1Turn; }

    void saveGame()
    {
        ofstream file("savegame.dat", ios::binary);
        size_t len1 = player1.name.size(), len2 = player2.name.size();
        file.write((char *)&len1, sizeof(len1));
        file.write(player1.name.c_str(), len1);
        file.write((char *)&player1.wins, sizeof(player1.wins));

        file.write((char *)&len2, sizeof(len2));
        file.write(player2.name.c_str(), len2);
        file.write((char *)&player2.wins, sizeof(player2.wins));
        file.write((char *)&player1Turn, sizeof(player1Turn));
        file.write((char *)board, sizeof(board));
        file.close();
        cout << GREEN << "💾 Igra je spremljena!\n"
             << RESET;
    }

    bool loadGame()
    {
        ifstream file("savegame.dat", ios::binary);
        if (!file)
        {
            cout << RED << "⚠️ Nema spremljene igre.\n"
                 << RESET;
            return false;
        }

        size_t len1, len2;
        file.read((char *)&len1, sizeof(len1));
        player1.name.resize(len1);
        file.read(&player1.name[0], len1);
        file.read((char *)&player1.wins, sizeof(player1.wins));

        file.read((char *)&len2, sizeof(len2));
        player2.name.resize(len2);
        file.read(&player2.name[0], len2);
        file.read((char *)&player2.wins, sizeof(player2.wins));
        file.read((char *)&player1Turn, sizeof(player1Turn));
        file.read((char *)board, sizeof(board));

        file.close();
        gameLoaded = true;
        cout << GREEN << "📂 Igra je učitana!\n"
             << RESET;
        return true;
    }

    void play()
    {
        if (!gameLoaded && (player1.name.empty() || player2.name.empty()))
        {
            cout << RED << "⚠️ Prvo dodajte igrače!\n"
                 << RESET;
            return;
        }
        if (!gameLoaded)
            resetBoard();

        int move;
        while (true)
        {
            displayBoard();
            cout << YELLOW << "🎲 " << getCurrentPlayer().name << " (" << getCurrentPlayer().symbol << "), potez (1-9) ili 0 za spremiti i izaći: " << RESET;
            cin >> move;

            if (move == 0)
            {
                saveGame();
                cout << BLUE << "🔁 Povratak u izbornik.\n"
                     << RESET;
                return;
            }

            if (!makeMove(move))
            {
                cout << RED << "❌ Neispravan potez!\n"
                     << RESET;
                continue;
            }

            if (checkWin())
            {
                displayBoard();
                cout << GREEN << "🏆 Čestitke! " << getCurrentPlayer().name << " je pobijedio!\n"
                     << RESET;
                getCurrentPlayer().wins++;
                updateLeaderboard();
                break;
            }
            if (isDraw())
            {
                displayBoard();
                cout << YELLOW << "🤝 Neriješeno!\n"
                     << RESET;
                break;
            }

            switchTurn();
        }

        saveGame();
        gameLoaded = false;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << CYAN << "🔁 Enter za povratak u izbornik..." << RESET;
        cin.get();
    }

    void showRules()
    {
        cout << BLUE << "\n📘 Pravila Križić-Kružić\n"
             << RESET;
        cout << "1️⃣ Dva igrača naizmjenično biraju pozicije 1-9.\n";
        cout << "2️⃣ Cilj je složiti 3 ista znaka u liniji.\n";
        cout << "3️⃣ Ako je ploča puna bez pobjednika – igra je neriješena.\n";
        cout << "4️⃣ Igru možete spremiti u bilo kojem trenutku.\n";
    }

    void showLeaderboard()
    {
        cout << MAGENTA << "\n📊 Leaderboard:\n"
             << RESET;
        ifstream file("leaderboard.dat", ios::binary);
        if (!file)
            return;

        map<string, int> scores;
        size_t len;
        string name;
        int wins;
        while (file.read((char *)&len, sizeof(len)))
        {
            name.resize(len);
            file.read(&name[0], len);
            file.read((char *)&wins, sizeof(wins));
            scores[name] = wins;
        }

        vector<pair<int, string>> sorted;
        for (auto &s : scores)
            sorted.push_back({s.second, s.first});
        sort(sorted.rbegin(), sorted.rend());

        for (auto &entry : sorted)
            cout << "🥇 " << entry.second << " - " << entry.first << " pobjeda\n";
    }

    void updateLeaderboard()
    {
        map<string, int> scores;
        ifstream in("leaderboard.dat", ios::binary);
        size_t len;
        string name;
        int wins;
        while (in.read((char *)&len, sizeof(len)))
        {
            name.resize(len);
            in.read(&name[0], len);
            in.read((char *)&wins, sizeof(wins));
            scores[name] = wins;
        }
        in.close();

        scores[player1.name] = player1.wins;
        scores[player2.name] = player2.wins;

        ofstream out("leaderboard.dat", ios::binary);
        for (auto &pair : scores)
        {
            len = pair.first.size();
            out.write((char *)&len, sizeof(len));
            out.write(pair.first.c_str(), len);
            out.write((char *)&pair.second, sizeof(pair.second));
        }
        out.close();
    }
};

void showMenu(Game &game)
{
    int choice;
    do
    {
        showTitle();
        cout << YELLOW << "\n📋 GLAVNI IZBORNIK:\n"
             << RESET;
        cout << "1️⃣ Dodaj igrače 👥\n";
        cout << "2️⃣ Nova igra 🎮\n";
        cout << "3️⃣ Učitaj igru 💾\n";
        cout << "4️⃣ Pravila 📘\n";
        cout << "5️⃣ Leaderboard 🏆\n";
        cout << "6️⃣ Izlaz ❌\n";
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
            cout << GREEN << "👋 Hvala što ste igrali!\n"
                 << RESET;
            break;
        default:
            cout << RED << "⚠️ Neispravan unos.\n"
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
