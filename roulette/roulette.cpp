#include <iostream>
#include <time.h>
#include "player.h"
#include "roulette.h"
using namespace std;


Roulette::Roulette(string name) {
    if (name != "") {
        player = Player(name);
    } else {
        player = Player();
    }
}

void Roulette::setBet() {
    cout << "How much would you like to bet?" << "\n" << ">> ";
    int userBet = 0;
    try {
        cin >> userBet;
    } catch (std::invalid_argument) {
		cout << "Not a valid number! Try again!" << "\n" << ">> ";
		setBet();
	}
}

int Roulette::getBet() {
    return bet;
}

int Roulette::spin() {
    return rand() % 37; // 0 to 36
}
