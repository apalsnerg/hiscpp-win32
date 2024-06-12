#include <iostream>
#include <time.h>
#include "player.h"
#include "roulette.h"
using namespace std;

/*
 * Create a new Roulette object with a Player
 * 
 * @param string name the name of the Player
 */
Roulette::Roulette(string name) 
     : bet(0), playerNumber(0), pot(10000), rolledNumber(0) {
    if (name != "") {
        player = Player(name);
    } else {
        player = Player();
    }
}

/*
 * Get input from player on how much they want to bet
 */
void Roulette::getDesiredBet() {
    cout << "How much would you like to bet?" << "\n" << ">> ";
    int userBet = 0;
    try {
        cin >> userBet;
    } catch (invalid_argument) {
		cout << "Not a valid number! Try again!" << "\n" << ">> ";
		getDesiredBet();
	}
}

/*
 * Forcefully set the player's bet
 */
void Roulette::setBet(int newBet) {
	bet = newBet;
}

/*
 * Get the player of the game
 */
Player& Roulette::getPlayer() {
	return player;
}

/*
 * Get the player's bet
 */
int Roulette::getBet() const {
    return bet;
}

/*
* Get the pot
*/
int Roulette::getPot() const {
	return pot;
}


/*
 * Spin the roulette wheel, i.e. returning a number between 0 and 36
 */
int Roulette::spin() {
    rolledNumber = rand() % 37;
	return rolledNumber;
}


/*
* Set the pot to a new value
*/
void Roulette::setPot(int newPot) {
	pot = newPot;
}

/*
* Reduce pot by value
* 
* @param int potReduceVal the value to reduce pot by
*/
void Roulette::reducePot(int potReduceVal) {
	pot -= potReduceVal;
}

/*
* Increase pot by value
* 
* @param int potAddVal the value to add to pot
*/
void Roulette::increasePot(int potAddVal) {
	pot += potAddVal;
}

/*
 * Return the player's chosen number
 */
int Roulette::getPlayerNumber() const {
	return playerNumber;
}

/*
 * Return the randomly generated number
 */
int Roulette::getRolledNumber() const {
	return rolledNumber;
}