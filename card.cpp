#include <iostream>
#pragma once
using namespace std;

struct Card
{
    string suit;
    string rank;
    bool face;

    Card()
    {
        suit = "";
        rank = "";
        face = true;
    }
    Card(string r, string s, bool f)
    {
        rank = r;
        suit = s;
        face = f;
    }
    string GetSuitSymbol()
    {
        if(suit == "H") 
            return "\u2665"; 
        if(suit == "D") 
            return "\u2666";
        if(suit == "C") 
            return "\u2663"; 
        if(suit == "S")
            return "\u2660";
        return "";
    }
};