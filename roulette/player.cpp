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

void Player::setBalance(int newBalance) {
    balance = newBalance;
}

void Player::updateBalance(int newBalance) {
    balance -= newBalance;
}
void Player::printName() {
    cout << "Your name is " << name << "\n";
}