#include <iostream>
#include <windows.h>
#include "game.cpp"
#include "commands.cpp"
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Game game;
    string command;
    DisplayGame();
    
    while(true)
    {
        game.DisplayStock();
        game.DisplayWaste();
        game.DisplayCards();
        game.DisplayFoundations();
        GameCheck(game);
        cout << "\n\nEnter command: ";
        getline(cin, command);
        CommandHistory(command);
        ExecuteCommand(command, game);
    }

    return 0;
}