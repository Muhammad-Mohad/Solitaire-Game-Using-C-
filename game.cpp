#include <iostream>
#pragma once
#include <ctime>
#include "card.cpp"
#include "linked_list.cpp"
#include "stack.cpp"
#include <windows.h>
using namespace std;

const int totalCards = 52;
Card allCards[totalCards];

void GenerateAllCards() 
{
    const string suits[] = {"H", "D", "C", "S"};
    const string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    
    int index = 0;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 13; j++) 
        {
            allCards[index] = Card(ranks[j], suits[i], true);
            index++;
        }
}

void SetConsoleColor(int textColor) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, textColor);
}

class Game
{
    friend class Commands;
    private:
        DLL cardsList[7];
        Stack<Card> stock;
        Stack<Card> waste;
        Stack<Card> foundationHearts;
        Stack<Card> foundationSpades;
        Stack<Card> foundationClubs;
        Stack<Card> foundationDiamonds;

    public:
        Game() : foundationHearts(13), foundationClubs(13), foundationDiamonds(13), foundationSpades(13)
        {
            GenerateAllCards();
            InitializeCards();
            PopulateStock();
        }
        void ShuffleCards() 
        {
            unsigned int seed = static_cast<unsigned int>(time(0)) + rand(); 
            srand(seed);

            for(int i = totalCards - 1; i > 0; --i) 
            {
                int j = rand() % (i + 1);
                swap(allCards[i], allCards[j]);
            }
        }
        void InitializeCards() 
        {
            ShuffleCards(); 

            int index = 0;  
            for(int i = 0; i < 7; i++) 
            {
                int numOfCards = i + 1;  
                
                for(int j = 0; j < numOfCards; j++) 
                {
                    bool face = (j == numOfCards - 1) ? true : false;
                    cardsList[i].AddToTail(Card(allCards[index].rank, allCards[index].suit, face));  
                    index++;  
                }
            }
        }
        void PopulateStock() 
        {
            for(int i = 28; i < totalCards; i++) 
                stock.Push(Card(allCards[i].rank, allCards[i].suit, false));
        }
        void DisplayCards() 
        {
            DLL::Iter itr[7];

            SetConsoleColor(14);
            cout << "Column " << 1 << "\t" << "Column " << 2 << "\t" << "Column " << 3 << "\t" << "Column " << 4 << "\t" << "Column " << 5 << "\t" << "Column " << 6 << "\t" << "Column " << 7 << "\t" << endl;
            SetConsoleColor(8);
            for(int i = 0; i < 7; i++) 
            {
                itr[i] = cardsList[i].Begin();
                cout << "(" << GetCount(i + 1) << " Cards)" << "\t";
            }
            cout << endl;
            SetConsoleColor(7);
            for(int i = 0; i < 7; i++)
                itr[i] = cardsList[i].Begin();

            while(itr[0] != cardsList[0].End() || itr[1] != cardsList[1].End() || itr[2] != cardsList[2].End() || itr[3] != cardsList[3].End() || itr[4] != cardsList[4].End() || itr[5] != cardsList[5].End() || itr[6] != cardsList[6].End())
            {
                for(int i = 0; i < 7; i++)
                {
                    if(itr[i] != cardsList[i].End())
                    {
                        Card& card = *itr[i];

                        if(card.suit == "H" || card.suit == "D")
                            SetConsoleColor(12);  
                        else 
                            SetConsoleColor(7);
                        if(card.face)
                            cout << card.rank << card.GetSuitSymbol() << "\t\t";
                        else
                        {
                            SetConsoleColor(7);
                            cout << "[ ]" << "\t\t";
                        }

                        ++itr[i];
                    }
                    else 
                    {
                        cout << "\t\t";  
                    }
                }
                cout << endl; 
            }
            SetConsoleColor(7);
            cout << endl << endl;
        }
        void DisplayStock()
        {
            SetConsoleColor(9);
            cout << "Stock Pile:\t\t";
            SetConsoleColor(7);
            Stack tempStack = stock;

            int count = 0;
            while (!tempStack.IsEmpty())
            {
                Card card = tempStack.Pop();
                if(card.face)
                    cout << card.rank << card.GetSuitSymbol() << " ";
                else
                    cout << "[ ]" << " ";
                count++;
            }
            cout << endl;
            SetConsoleColor(8);
            cout << "(" << count << " Cards)" << endl << endl;
            SetConsoleColor(7);
        }
        void PopFromStock()
        {
            if(!stock.IsEmpty())
            {
                Card card = stock.Pop();
                waste.Push(card);  
            }
            else if(stock.IsEmpty())
            {
                while(!waste.IsEmpty())
                {
                    Card card = waste.Pop();
                    stock.Push(card);
                }
            }
        }
        void PopFromWaste()
        {
            if(!waste.IsEmpty())
            {
                Card card = waste.Pop();

                Stack<Card>* targetFoundation = nullptr;
                if(card.suit == "H") 
                    targetFoundation = &foundationHearts;
                else if(card.suit == "C")
                    targetFoundation = &foundationClubs;
                else if(card.suit == "D")
                    targetFoundation = &foundationDiamonds;
                else if(card.suit == "S")
                    targetFoundation = &foundationSpades;

                if(targetFoundation != nullptr)
                {
                    if(targetFoundation->IsEmpty() && card.rank == "A")
                        targetFoundation->Push(card);
                    else if(!targetFoundation->IsEmpty())
                    {
                        Card topCard = targetFoundation->Top();
                        
                        if(RankToValue(card.rank) == RankToValue(topCard.rank) + 1)
                            targetFoundation->Push(card);
                        else
                            waste.Push(card);
                    }
                    else
                        waste.Push(card);
                }
            }
        }
        void DisplayWaste()
        {
            SetConsoleColor(9);
            cout << "Waste Pile:\t\t";
            SetConsoleColor(7);

            Stack tempStack = waste;
            int count = 0;
            
            if(tempStack.IsEmpty())
                cout << "";
            else 
            {
                while(!tempStack.IsEmpty())
                {
                    Card card = tempStack.Pop();
                    count++;
                    if(card.suit == "H" || card.suit == "D")
                        SetConsoleColor(12);
                    else
                        SetConsoleColor(7);
                    if(count == 1)
                        cout << card.rank << card.GetSuitSymbol() << " "; 
                    else
                    {
                        SetConsoleColor(7);
                        cout << "[ ]" << " ";
                    }
                }
            }
            SetConsoleColor(7);
            cout << endl;
            SetConsoleColor(8);
            cout << "(" << count << " Cards)" << endl << endl;
            SetConsoleColor(7);
        }
        int RankToValue(const string& rank) 
        {
            if(rank == "A") return 1;
            else if(rank == "2") return 2;
            else if(rank == "3") return 3;
            else if(rank == "4") return 4;
            else if(rank == "5") return 5;
            else if(rank == "6") return 6;
            else if(rank == "7") return 7;
            else if(rank == "8") return 8;
            else if(rank == "9") return 9;
            else if(rank == "10") return 10;
            else if(rank == "J") return 11;
            else if(rank == "Q") return 12;
            else if(rank == "K") return 13;
            return 0;
        }

        bool IsFoundationEmpty(int foundationIndex) 
        {
            if(foundationIndex == 0) 
                return foundationHearts.IsEmpty();
            else if(foundationIndex == 1) 
                return foundationClubs.IsEmpty();
            else if(foundationIndex == 2) 
                return foundationDiamonds.IsEmpty();
            else if(foundationIndex == 3) 
                return foundationSpades.IsEmpty();
            else 
                return true; 
        }

        void DisplayFoundations()
        {
            SetConsoleColor(2);
            cout << "Foundation " "\u2665"":\t\t"; 
            SetConsoleColor(7);
            foundationHearts.Print();
            SetConsoleColor(8);
            cout << "(" << foundationHearts.Count() << " Cards)";
            SetConsoleColor(7);
            cout << endl << endl;

            SetConsoleColor(2);
            cout << "Foundation " "\u2663"":\t\t"; 
            SetConsoleColor(7);
            foundationClubs.Print();
            SetConsoleColor(8);
            cout << "(" << foundationClubs.Count() << " Cards)";
            SetConsoleColor(7);
            cout << endl << endl;

            SetConsoleColor(2);
            cout << "Foundation " "\u2666"":\t\t"; 
            SetConsoleColor(7);
            foundationDiamonds.Print();
            SetConsoleColor(8);
            cout << "(" << foundationDiamonds.Count() << " Cards)";
            SetConsoleColor(7);
            cout << endl << endl;

            SetConsoleColor(2);
            cout << "Foundation " "\u2660"":\t\t"; 
            SetConsoleColor(7);
            foundationSpades.Print();
            SetConsoleColor(8);
            cout << "(" << foundationSpades.Count() << " Cards)";
            SetConsoleColor(7);
            cout << endl << endl;
        }
        int GetCount(int columnIndex) 
        {
            return cardsList[columnIndex - 1].GetCount();
        }
        void MoveFromColumnToFoundations(string command) 
        {
            int column = command[3] - '0';
            int foundation = command[6] - '0';
            column--;

            if(column >= 0 && column < 7) 
            {
                if (cardsList[column].GetCount() > 0) 
                {
                    Card cardToMove = cardsList[column].GetTail();

                    Stack<Card>* targetFoundation = nullptr;

                    switch(foundation) 
                    {
                        case 1: targetFoundation = &foundationHearts; break;
                        case 2: targetFoundation = &foundationClubs; break;
                        case 3: targetFoundation = &foundationDiamonds; break;
                        case 4: targetFoundation = &foundationSpades; break;
                        default: break;
                    }

                    if(targetFoundation != nullptr && IsValidMove(cardToMove, targetFoundation))
                    {
                        cardsList[column].DeleteTail();
                        targetFoundation->Push(cardToMove);
                    } 
                }
            }
            if(cardsList[column].GetCount() > 0) 
            {
                Card& newTopCard = cardsList[column].GetTail();
                newTopCard.face = true; 
            }
        }

        bool IsValidMove(const Card& card, Stack<Card>* targetFoundation) 
        {
            if(targetFoundation->IsEmpty()) 
            {
                if(card.rank == "A") 
                {
                    if(targetFoundation == &foundationHearts) 
                        return card.suit == "H";
                    else if(targetFoundation == &foundationDiamonds) 
                        return card.suit == "D";
                    else if(targetFoundation == &foundationClubs) 
                        return card.suit == "C";
                    else if(targetFoundation == &foundationSpades) 
                        return card.suit == "S";
                }
            } 
            else 
            {
                Card topCard = targetFoundation->Top();

                if(RankToValue(card.rank) == RankToValue(topCard.rank) + 1) 
                {
                    if (targetFoundation == &foundationHearts) 
                        return card.suit == "H";
                    else if (targetFoundation == &foundationDiamonds) 
                        return card.suit == "D";
                    else if (targetFoundation == &foundationClubs) 
                        return card.suit == "C";
                    else if (targetFoundation == &foundationSpades) 
                        return card.suit == "S";
                }
            }
            return false;
        }
        void MoveBetweenColumns(string str)
        {
            int column1 = str[3] - '0';
            int column2 = str[6] - '0';
            int num = stoi(str.substr(8)); 
            column1--;
            column2--;

            if(num == 1 && column1 >= 0 && column1 < 7 && column2 >= 0 && column2 < 7) 
            {
                if(cardsList[column2].GetCount() == 0)
                {
                    Card src = cardsList[column1].GetTail();
                    if(src.rank == "K")
                    {
                        cardsList[column1].DeleteTail();
                        cardsList[column2].AddToTail(src);
                        if(cardsList[column1].GetCount() > 0)
                        {
                            Card& newCard = cardsList[column1].GetTail();
                            newCard.face = true;
                        }
                    }
                }
                else if(cardsList[column2].GetCount() > 0) 
                {
                    Card src = cardsList[column1].GetTail();
                    Card dest = cardsList[column2].GetTail();
                    if((dest.suit == "H" || dest.suit == "D") && (src.suit == "S" || src.suit == "C"))
                    {
                        if(RankToValue(src.rank) + 1 == RankToValue(dest.rank))
                        {
                            cardsList[column1].DeleteTail();
                            cardsList[column2].AddToTail(src);
                            if(cardsList[column1].GetCount() > 0)
                            {
                                Card& newCard = cardsList[column1].GetTail();
                                newCard.face = true;
                            }
                        }
                    }
                    else if((dest.suit == "S" || dest.suit == "C") && (src.suit == "H" || src.suit == "D"))
                    {
                        if(RankToValue(src.rank) + 1 == RankToValue(dest.rank))
                        {
                            cardsList[column1].DeleteTail();
                            cardsList[column2].AddToTail(src);
                            if(cardsList[column1].GetCount() > 0)
                            {
                                Card& newCard = cardsList[column1].GetTail();
                                newCard.face = true;
                            }
                        }
                    }
                }
            }
            else if(column1 >= 0 && column1 < 7 && column2 >= 0 && column2 < 7)
            {

                if(cardsList[column1].GetCount() >= num)
                {
                    Stack<Card> temp;

                    for(int i = 0; i < num; i++)
                    {   
                        Card card = cardsList[column1].GetTail();
                        cardsList[column1].DeleteTail();
                        temp.Push(card);
                    }

                    Card src = temp.Pop();

                    if(cardsList[column2].GetCount() == 0)
                    {
                        if(src.rank == "K")
                        {
                            cardsList[column2].AddToTail(src);
                            while(!temp.IsEmpty())
                            {
                                Card card = temp.Pop();
                                cardsList[column2].AddToTail(card);
                            }
                            if(cardsList[column1].GetCount() > 0)
                            {
                                Card& tempCard = cardsList[column1].GetTail();
                                tempCard.face = true;
                            }
                        }
                        else
                        {
                            cardsList[column1].AddToTail(src);
                            while(!temp.IsEmpty())
                            {
                                Card card = temp.Pop();
                                cardsList[column1].AddToTail(card);
                            }
                        }
                    }
                    else
                    {
                        Card dest = cardsList[column2].GetTail();
                        if(((dest.suit == "S" || dest.suit == "C") && (src.suit == "H" || src.suit == "D")) || ((dest.suit == "H" || dest.suit == "D") && (src.suit == "S" || src.suit == "C")))
                        {
                            if(RankToValue(src.rank) + 1 == RankToValue(dest.rank))
                            {
                                if(cardsList[column1].GetCount() > 0)
                                {
                                    Card& temp = cardsList[column1].GetTail();
                                    temp.face = true;
                                }
                                cardsList[column2].AddToTail(src);
                                while(!temp.IsEmpty())
                                {
                                    Card card = temp.Pop();
                                    cardsList[column2].AddToTail(card);
                                }
                            }
                            else
                            {
                                cardsList[column1].AddToTail(src);
                                while(!temp.IsEmpty())
                                {
                                    Card card = temp.Pop();
                                    cardsList[column1].AddToTail(card);
                                }
                            }
                        }
                        else
                        {
                            cardsList[column1].AddToTail(src);
                            while(!temp.IsEmpty())
                            {
                                Card card = temp.Pop();
                                cardsList[column1].AddToTail(card);
                            }
                        }
                    }
                }
            }
        }
        void MoveFoundationToColumn(string str)
        {
            int src = str[3] - '0';
            int column = str[6] - '0';
            column--;

            Stack<Card>* target = nullptr;
            if(src == 1) 
                target = &foundationHearts;
            else if(src == 2) 
                target = &foundationClubs;
            else if(src == 3) 
                target = &foundationDiamonds;
            else if(src == 4) 
                target = &foundationSpades;

            if(column >= 0 && column < 7)
                if(!target->IsEmpty())
                {
                    Card card1 = target->Top();
                    Card card2 = cardsList[column].GetTail();
                    
                    if((card1.suit == "H" || card1.suit == "D") && (card2.suit == "C" || card2.suit == "S"))
                    {
                        if(RankToValue(card1.rank) + 1 == RankToValue(card2.rank))
                        {
                            target->Pop();
                            card1.face = true;
                            cardsList[column].AddToTail(card1);
                        }
                    }
                    else if((card1.suit == "S" || card1.suit == "C") && (card2.suit == "H" || card2.suit == "D"))
                    {
                        if(RankToValue(card1.rank) + 1 == RankToValue(card2.rank))
                        {
                            target->Pop();
                            card1.face = true; 
                            cardsList[column].AddToTail(card1);
                        }
                    }
                }
        }
        void MoveFromWasteToColumn(string str)
        {
            int column = str[5] - '0';
            column--;
            if(column >=0 && column < 7)
            {
                if(!waste.IsEmpty())
                {
                    Card card1 = waste.Top();

                    if(cardsList[column].GetCount() == 0)
                    {
                        if(card1.rank == "K")
                        {
                            waste.Pop();
                            card1.face = true;
                            cardsList[column].AddToTail(card1);
                        }
                        return;
                    }

                    Card card2 = cardsList[column].GetTail();

                    if((card1.suit == "H" || card1.suit == "D") && (card2.suit == "C" || card2.suit == "S"))
                    {
                        if(RankToValue(card1.rank) + 1 == RankToValue(card2.rank))
                        {
                            waste.Pop();
                            card1.face = true;
                            cardsList[column].AddToTail(card1);
                        }
                    }
                    else if((card1.suit == "S" || card1.suit == "C") && (card2.suit == "H" || card2.suit == "D"))
                    {
                        if(RankToValue(card1.rank) + 1 == RankToValue(card2.rank))
                        {
                            waste.Pop();
                            card1.face = true;
                            cardsList[column].AddToTail(card1);
                        }
                    }
                }
            }
        }
        bool WinCheck()
        {
            return (foundationClubs.CurrentSize() == 13 && foundationDiamonds.CurrentSize() == 13 && foundationHearts.CurrentSize() == 13 && foundationSpades.CurrentSize() == 13);
        }
        void UndoColumntoColumn(int src, int dest, int numOfCards)
        {
            src--;
            dest--;
            if(cardsList[src].GetCount() > 0)
                cardsList[src].GetTail().face = false;
            for(int i = 0; i < numOfCards; i++)
            {
                Card& card = cardsList[dest].GetTail();
                cardsList[dest].DeleteTail();
                cardsList[src].AddToTail(card);
            }
        }
        void UndoWasteToColumn(int col)
        {
            col--;
            Card& card = cardsList[col].GetTail();
            cardsList[col].DeleteTail();
            waste.Push(card);
        }
        void UndoColumnToFoundation(int col, int foundation)
        {
            Card card;
            if(cardsList[col].GetCount() > 0)
                cardsList[col].GetTail().face = false;
            if(foundation == 1)
                card = foundationHearts.Pop();
            else if(foundation == 2)
                card = foundationClubs.Pop();
            else if(foundation == 3)
                card = foundationDiamonds.Pop();
            else if(foundation == 4)
                card = foundationSpades.Pop();
            cardsList[col].AddToTail(card);
        }
        void UndoFoundationToColumn(int foundation, int col)
        {
            Card card = cardsList[col].GetTail();
            cardsList[col].DeleteTail();
            if(foundation == 1)
                foundationHearts.Push(card);
            else if(foundation == 2)
                foundationClubs.Push(card);
            else if(foundation == 3)
                foundationDiamonds.Push(card);
            else if(foundation == 4)
                foundationSpades.Push(card);
        }
};

