#include <iostream>
#pragma once
#include "card.cpp"
using namespace std;

class DLL
{
private:
    struct Node;  
    class ListIterator;
    Node* head;
    Node* tail;

public:
    typedef ListIterator Iter;

    DLL()
    {
        head = nullptr;
        tail = nullptr;
    }

    ~DLL()
    {
        Node* ptr = head;
        while(head != nullptr)
        {
            ptr = head->next;
            delete head;
            head = ptr;
        }
    }

    void AddToTail(Card value)
    {
        Node* newNode = new Node(value); 

        if (tail == nullptr) 
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode; 
            newNode->prev = tail; 
            tail = newNode; 
        }
    }
    int GetCount() 
    {
        int count = 0;
        Node* current = head;
        while(current) 
        {
            count++;
            current = current->next;
        }
        return count;
    }
    Card& GetTail() const
    {
        if(tail == nullptr)
            throw runtime_error("List is Empty");
        return tail->data;
    }
    void DeleteTail() 
    {
        if (tail == nullptr)
            return;
        
        Node* temp = tail;
        if(tail == head) 
        {
            head = tail = nullptr; 
        } 
        else
        {
            tail = tail->prev; 
            tail->next = nullptr;
        }
        delete temp;
    }
    Iter Begin()
    {
        return Iter(head);
    }

    Iter End()
    {
        return Iter(nullptr);
    }

private:
    struct Node 
    {
        public:
            Card data;
            Node* next;
            Node* prev;

            Node(Card value, Node* pptr = nullptr, Node* nptr = nullptr)
            {
                data = value;
                prev = pptr;
                next = nptr;
            }
    };

    class ListIterator
    {
        private:
            Node* iptr;

        public:
            friend class DLL;

            ListIterator(Node* ptr = nullptr)
            {
                iptr = ptr;
            }

            Card& operator*()
            {
                return iptr->data;
            }

            bool operator==(const ListIterator& l) const
            {
                return iptr == l.iptr;
            }

            bool operator!=(const ListIterator& l) const
            {
                return !(iptr == l.iptr);
            }

            ListIterator operator++(int)
            {
                ListIterator old = *this;
                ++(*this);
                return old;
            }

            ListIterator& operator++()
            {
                if(iptr)
                    iptr = iptr->next;
                return (*this);
            }

            ListIterator operator--(int)
            {
                ListIterator old = *this; 
                if(iptr)
                    --(*this);  
                return old; 
            }

            ListIterator& operator--()
            {
                if(iptr)
                    iptr = iptr->prev;
                return (*this);
            }
    };
};
