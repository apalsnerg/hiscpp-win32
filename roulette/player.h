#ifndef PLAYER_H
#define PLAYER_H


#include <iostream>
using namespace std;

#pragma once
class Player {
    public:
        string name;
        Player(string name = "George Smith", int balance = 1000);

        void setName();

        void setBalance(int balance);

        void updateBalance(int balance);

        void printName() const;

        string getName() const;

		int getBalance() const;

    private:
        int balance;
};

#endif
