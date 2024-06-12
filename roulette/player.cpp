#include <iostream>
#include <string>
#include "player.h"
using namespace std;

Player::Player(string playerName, int playerBalance) {
    name = playerName;
    balance = playerBalance;
}

void Player::setName() {
    cout << "What is your name?" << "\n" << ">> ";
    string newName;
    getline(cin, newName);
    name = newName;
}

/*
 * Get player's balance
 */
int Player::getBalance() const {
    return balance;
}

/*
 * Set player's balance to new value
 * 
 * @param int newBalance the new balance
 */
void Player::setBalance(int newBalance) {
    balance = newBalance;
}

/*
 * Update player's balance by subtracting newBalance
 * 
 * @param int newBalance the value to subtract from balance
 */
void Player::updateBalance(int newBalance) {
    balance -= newBalance;
}

/*
 * Print player's name (used only in testing for now)
 */
void Player::printName() const {
    cout << "Your name is " << name << "\n";
}

/*
* Get player's name
*/
string Player::getName() const {
	return name;
}