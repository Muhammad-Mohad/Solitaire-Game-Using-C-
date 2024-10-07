#include <iostream>
#include <windows.h>
#pragma once
#include "card.cpp"
using namespace std;

void SetColor2(int textColor) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, textColor);
}

template <typename T>
class Stack
{
    private:
        struct Node
        {
            T data;
            Node* next;

            Node(T value)
            {
                data = value;
                next = nullptr;
            }
        };

        Node* top;
        int size;

    public:
        Stack()
        {
            top = nullptr;
            size = 0;
        }
        Stack(int maxSize)
        {
            top = nullptr;
            size = maxSize;
        }
        bool IsEmpty()
        {
            return top == nullptr;
        }
        ~Stack()
        {
            while(!IsEmpty())
                Pop();
        }
        void Push(T data)
        {
            Node* newNode = new Node(data);
            newNode->next = top;
            top = newNode;
            size++;
        }
        T Pop()
        {
            if (IsEmpty())
                throw out_of_range("Stack is empty");

            Node* temp = top;
            T poppedCard = temp->data;  
            top = top->next;
            delete temp;
            size--;

            return poppedCard;  
        }
        T Top()
        {
            if(IsEmpty())
            {
                throw out_of_range("Stack is empty");
            }
            return top->data;
        }
        int GetSize()
        {
            return size;
        }
        int Count()
        {
            int count = 0;
            Node* current = top; 

            while (current != nullptr)
            {
                count++;
                current = current->next; 
            }

            return count;
        }
        string GetSuit()
        {
            return top->data.suit;
        }
        void Print()
        {
            Node* current = top;
            while (current != nullptr)
            {
                if(current->data.suit == "H" || current->data.suit == "D")
                    SetColor2(12);
                else
                    SetColor2(7);
                cout << current->data.rank << current->data.GetSuitSymbol() << " ";
                current = current->next;   
                SetColor2(7);    
            }
            cout << endl;
        }
        void CopyStack(const Stack& other)
        {
            if (other.top == nullptr)
            {
                top = nullptr;
                size = 0;
                return;
            }

            Node* temp = other.top;
            top = new Node(temp->data);
            Node* current = top;
            temp = temp->next;

            while (temp != nullptr)
            {
                current->next = new Node(temp->data);
                current = current->next;
                temp = temp->next;
            }
            size = other.size;
        }
        int CurrentSize()
        {
            int count = 0;
            Node* current = top;

            while (current != nullptr)
            {
                count++;
                current = current->next;
            }

            return count;
        }
        Stack(const Stack& other)
        {
            CopyStack(other);
        }
        Stack& operator=(const Stack& other)
        {
            if (this != &other)
            {
                while (!IsEmpty())
                    Pop();

                CopyStack(other);
            }
            return *this;
        }
};

