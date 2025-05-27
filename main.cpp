#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
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
  _  __     _        _       _     _            
 | |/ /    (_)      (_)     (_)   | |           
 | ' / _ __ _ ______ _ _ __  _  __| | ___  _ __  
 |  < | '__| |_  / _` | '_ \| |/ _` |/ _ \| '_ \ 
 | . \| |  | |/ / (_| | | | | | (_| | (_) | | | |
 |_|\_\_|  |_/___\__,_|_| |_|_|\__,_|\___/|_| |_|                                       
         _____           _     _               
        /  __ \         (_)   (_)              
        | /  \/ ___ _ __ _ ___ _  ___  _ __     
        | |    / _ \ '__| / __| |/ _ \| '_ \    
        | \__/\  __/ |  | \__ \ | (_) | | | |   
         \____/\___|_|  |_|___/_|\___/|_| |_|   
    )" << RESET
         << "\n";
    cout << YELLOW << "                          Dobrodošli u igru Križić-Kružić! 🎮\n"
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
        cin >> player1.name;
        player1.symbol = 'X';

        cout << MAGENTA << "👤 Unesite ime igrača 2: " << RESET;
        cin >> player2.name;
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
        cout << GREEN << "💾 Igra je snimljena!\n"
             << RESET;
    }

    bool loadGame()
    {
        ifstream file("savegame.txt");
        if (!file.is_open())
        {
            cout << RED << "⚠️  Nema snimljene igre.\n"
                 << RESET;
            return false;
        }

        file >> player1.name >> player1.wins;
        file >> player2.name >> player2.wins;
        file >> player1Turn;

        char c;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                file >> board[i][j];

        if (file.peek() == EOF)
        {
            cout << RED << "⚠️  Datoteka s igrom je prazna ili oštećena.\n"
                 << RESET;
            return false;
        }

        file.close();
        gameLoaded = true;
        cout << GREEN << "📂 Prethodna igra je učitana!\n"
             << RESET;
        return true;
    }

    void play()
    {
        if (!gameLoaded && (player1.name.empty() || player2.name.empty()))
        {
            cout << RED << "⚠️  Prvo dodajte igrače!\n"
                 << RESET;
            return;
        }
        int move;
        if (!gameLoaded)
            resetBoard();

        while (true)
        {
            displayBoard();
            cout << YELLOW << "🎲 " << getCurrentPlayer().name << " (" << getCurrentPlayer().symbol << "), unesite potez (1-9): " << RESET;
            cin >> move;
            if (!makeMove(move))
            {
                cout << RED << "❌ Neispravan potez!\n"
                     << RESET;
                continue;
            }
            if (checkWin())
            {
                displayBoard();
                cout << GREEN << "🏆 Čestitke! 🎉 Igrač " << getCurrentPlayer().name << " je pobijedio! 🥳\n"
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
        cout << YELLOW << "Pritisnite Enter za povratak u izbornik..." << RESET;
        cin.ignore();
        cin.get();
    }

    void showRules()
    {
        cout << BLUE << "\n📘 PRAVILA IGRE KRIŽIĆ-KRUŽIĆ 🎮\n"
             << RESET;
        cout << GREEN << "=====================================\n"
             << RESET;
        cout << YELLOW << "🎯 Cilj: " << RESET << "Složiti 3 znaka (X ili O) u red, stupac ili dijagonalu.\n";
        cout << CYAN << "🔁 Naizmjenično igranje: " << RESET << "Igrači biraju pozicije od 1 do 9.\n";
        cout << MAGENTA << "❌❌❌ Kraj igre: " << RESET << "Pobjeda, neriješeno ili puna ploča.\n";
        cout << RED << "💾 Snimanje igre: " << RESET << "Igru možete spremiti i kasnije učitati.\n";
        cout << GREEN << "=====================================\n"
             << RESET;
    }

    void showLeaderboard()
    {
        cout << MAGENTA << "\n🏆 LEADERBOARD - Najbolji igrači:\n"
             << RESET;
        ifstream file("leaderboard.txt");
        if (!file)
        {
            cout << RED << "⚠️  Nema dostupnih podataka.\n"
                 << RESET;
            return;
        }

        vector<pair<string, int>> entries;
        string name;
        int wins;
        while (file >> name >> wins)
            entries.push_back({name, wins});
        file.close();

        sort(entries.begin(), entries.end(), [](auto &a, auto &b)
             { return b.second < a.second; });

        int rank = 1;
        for (auto &entry : entries)
        {
            cout << YELLOW << rank++ << ". " << entry.first << " - " << entry.second << " pobjeda 🎖️\n"
                 << RESET;
        }
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

        scores[player1.name] = max(scores[player1.name], player1.wins);
        scores[player2.name] = max(scores[player2.name], player2.wins);

        vector<pair<string, int>> sorted(scores.begin(), scores.end());
        sort(sorted.begin(), sorted.end(), [](auto &a, auto &b)
             { return b.second < a.second; });

        ofstream outfile("leaderboard.txt");
        for (auto &pair : sorted)
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
        cout << YELLOW << "\n📋 GLAVNI IZBORNIK:\n"
             << RESET;
        cout << "1️⃣  Dodaj igrače 👥\n";
        cout << "2️⃣  Pokreni novu igru 🎮\n";
        cout << "3️⃣  Učitaj prethodnu igru 💾\n";
        cout << "4️⃣  Prikaži pravila 📘\n";
        cout << "5️⃣  Prikaži leaderboard 🏆\n";
        cout << "6️⃣  Izlaz ❌\n";
        cout << CYAN << "Unesite izbor: " << RESET;
        cin >> choice;

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
            cout << RED << "⚠️  Neispravan unos.\n"
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