#ifndef ROULETTE_H
#define ROULETTE_H

#include <iostream>
#include "player.h"
using namespace std;

#pragma once
/**
 * Handles a game of Roulette.
 * 
*/
class Roulette {
    int bet;
    int pot;
    Player player;
    int rolledNumber;
    int playerNumber;

    public:
        Roulette(string name = "George Smith");

        void getDesiredBet();

        void setBet(int newBet);

        Player& getPlayer();

        void setPot(int newPot);

        void increasePot(int potAddVal);

		void reducePot(int potReduceVal);

		int getPot() const;

        int getBet() const;

        int spin();

        int getRolledNumber() const;

        int getPlayerNumber() const;
};

#endif
