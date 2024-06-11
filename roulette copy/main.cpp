#include <iostream>
#include <windows.h>
#include "roulette.h"
#include "player.h"
#include "window.h"
using namespace std;

int main() {
    Roulette roulette;
    Player player;

    player.printName();
    player.setName();
    player.printName();

    roulette.setBet();
    cout << "Your bet was: " << roulette.getBet() << "\n";
    srand(time(NULL));
    cout << "Your spin was: " << roulette.spin() << "\n"; 
    srand(time(NULL));

    Window* pWindow = new Window();
    bool running = true;
    while (running) {
        if (!pWindow->processMessages()) {
            cout << "Closing. Bye bye!" << "\n";
            running = false;
        }

        Sleep(10);
    }

    delete pWindow;
    return 1;
}
