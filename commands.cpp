#include <iostream>
#include <string>
#include "game.cpp"
using namespace std;

Stack<char> history;

void DisplayGame()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void DisplayEnd()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    cout << "\n\nGame Won\n\n";
}
void GameCheck(Game& game)
{
    if(game.WinCheck())
    {
        DisplayEnd();
        exit(EXIT_SUCCESS);
    }
}

void CommandHistory(const string& command)
{
    while(!history.IsEmpty()) 
        history.Pop();

    if(command[2] == 'c' && command[5] == 'c')
    {
        history.Push(command[2]);
        history.Push(command[5]);
        if(isdigit(command[3]))
            history.Push(command[3]);
        if(isdigit(command[6]))
            history.Push(command[6]);
        if(isdigit(command[8]))
            history.Push(command[8]);       
    }
    else if(command[2] == 'w' && command[4] == 'c')
    {
        history.Push(command[2]);
        history.Push(command[4]);
        if(isdigit(command[5]))
            history.Push(command[5]);
    }
    else if(command[2] == 'c' && command[5] == 'f')
    {
        if(isdigit(command[6]))
            history.Push(command[6]);
        if(isdigit(command[3]))
            history.Push(command[3]);
        history.Push(command[5]);
        history.Push(command[2]);
    }
    else if(command[2] == 'f' && command[5] == 'c')
    {
        if(isdigit(command[6]))
            history.Push(command[6]);
        if(isdigit(command[3]))
            history.Push(command[3]);
        history.Push(command[5]);
        history.Push(command[2]);
    }
}

void ExecuteCommand(const string& command, Game& game)
{   
    if(command == "s")
        game.PopFromStock();
    else if(command == "w")
        game.PopFromWaste();
    else if(command[0] == 'm' && command[2] == 'c' && command[5] == 'f') 
        game.MoveFromColumnToFoundations(command);
    else if(command[0] == 'm' && command[2] == 'c' && command[5] == 'c')
        game.MoveBetweenColumns(command);
    else if(command[0] == 'm' && command[2] == 'f' && command[5] == 'c')
        game.MoveFoundationToColumn(command);
    else if(command[0] == 'm' && command[2] == 'w' && command[4] == 'c')
        game.MoveFromWasteToColumn(command);
    else if(command == "exit")
    {
        cout << "\nGame Terminated\n";
        exit(EXIT_SUCCESS);
    }
    else if(command[0] == 'z' && !history.IsEmpty())
    {
        if(history.Count() == 5)
        {
            int numOfCards = history.Pop() - '0';
            int dest = history.Pop() - '0';
            int src = history.Pop() - '0';
            game.UndoColumntoColumn(src, dest, numOfCards);
        }
        else if(history.Count() == 3)
        {
            int column = history.Pop() - '0';
            game.UndoWasteToColumn(column);
        }
        else if(history.Count() == 4)
        {
            char src = history.Pop();
            char dest = history.Pop();

            if(src == 'c' && dest == 'f')
            {
                int col = history.Pop() - '0';
                int foundation = history.Pop() - '0';
                col--;
                game.UndoColumnToFoundation(col, foundation);
            }
            else if(src == 'f' && dest == 'c')
            {
                int foundation = history.Pop() - '0';
                int col = history.Pop() - '0';
                col--;
                game.UndoFoundationToColumn(foundation, col);
            }
        }
    }
    else if(command[0] == 'z' && history.IsEmpty())
        cout << "";
    else
    {
        cout << "Invalid Command\n";
        system("pause");
    }

    DisplayGame();
}
