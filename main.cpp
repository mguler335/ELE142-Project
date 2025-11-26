#include <iostream>
#include <string>

#include "canvas.cpp"
#include "gravity.cpp"

// ------------------------------------------------------------
//  HEADER AND SECTIONS
// ------------------------------------------------------------

void printHeader() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "                 SPACE SIMULATION PROGRAM                  \n";
    std::cout << "============================================================\n\n";
}

void printSection(const std::string& title) {
    std::cout << "\n------------------------------------------------------------\n";
    std::cout << "  " << title << "\n";
    std::cout << "------------------------------------------------------------\n";
}

// ------------------------------------------------------------
//  INPUT FUNCTIONS
// ------------------------------------------------------------

Body* getBodyFromUser_UI(int index) {
    printSection("BODY " + std::to_string(index));

    std::cout <<
              " Enter values for Body " << index << " in the format:\n"
                                                    "   mass   posX   posY   velX   velY\n"
                                                    " Example: 100  0  0  0.1  0.2\n\n";

    double m, x, y, vx, vy;

    while (true) {
        std::cout << " Input: ";
        if (std::cin >> m >> x >> y >> vx >> vy) break;

        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << " Invalid input. Try again.\n";
    }

    return new Body(m, Vector(x, y), Vector(vx, vy));
}

Rocket* getRocketFromUser_UI(int index) {
    printSection("ROCKET " + std::to_string(index));

    std::cout <<
              " Enter values for Rocket " << index << " in the format:\n"
                                                      "   mass posX posY velX velY exhaustVel exhaustRate Motor(On/Off)\n"
                                                      " Example: 100 0 0 0.1 0.2 15 0.2 On\n\n";

    double m, x, y, vx, vy, eVel, eRate;
    std::string motorStr;
    bool motorOn = true;

    while (true) {
        std::cout << " Input: ";
        if (std::cin >> m >> x >> y >> vx >> vy >> eVel >> eRate >> motorStr) {

            if (motorStr == "On" || motorStr == "on" || motorStr == "ON")
                motorOn = true;
            else if (motorStr == "Off" || motorStr == "off" || motorStr == "OFF")
                motorOn = false;
            else {
                std::cout << " Motor must be 'On' or 'Off'. Try again.\n";
                continue;
            }
            break;
        }

        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << " Invalid input. Try again.\n";
    }

    return new Rocket(m, Vector(x, y), Vector(vx, vy), eVel, eRate, motorOn);
}

Gezegen* getGezegenFromUser_UI(int index) {
    printSection("PLANET " + std::to_string(index));
    std::cout << " Planet input is same as Body.\n\n";

    Body* b = getBodyFromUser_UI(index);

    auto* p = new Gezegen(
            b->getMass(),
            Vector(b->getPosX(), b->getPosY()),
            Vector(b->getVelX(), b->getVelY()) // <-- DÜZELTİLDİ: Artık girilen hızı alıyor
    );

    delete b;
    return p;
}

Asteroid* getAsteroidFromUser_UI(int index) {
    printSection("ASTEROID " + std::to_string(index));
    std::cout << " Asteroid input is same as Body.\n\n";

    Body* b = getBodyFromUser_UI(index);

    auto* a = new Asteroid(
            b->getMass(),
            Vector(b->getPosX(), b->getPosY()),
            Vector(b->getVelX(), b->getVelY()) // <-- DÜZELTİLDİ: Artık girilen hızı alıyor
    );

    delete b;
    return a;
}

// ------------------------------------------------------------
//  MAIN
// ------------------------------------------------------------

int main() {

    printHeader();

    // Output name
    std::string filename;
    printSection("OUTPUT FILE");
    std::cout << " Enter output HTML file name (example: orbit): ";
    std::cin >> filename;

    Universe U(1.0);

    // Counts
    printSection("OBJECT COUNTS");

    int bCount, rCount, pCount, aCount;
    std::cout << " Number of Bodies: ";
    std::cin >> bCount;
    std::cout << " Number of Rockets: ";
    std::cin >> rCount;
    std::cout << " Number of Planets: ";
    std::cin >> pCount;
    std::cout << " Number of Asteroids: ";
    std::cin >> aCount;

    // Create objects
    for (int i = 1; i <= bCount; i++)
        U.addBody(getBodyFromUser_UI(i));

    for (int i = 1; i <= rCount; i++)
        U.addBody(getRocketFromUser_UI(i));

    for (int i = 1; i <= pCount; i++)
        U.addBody(getGezegenFromUser_UI(i));

    for (int i = 1; i <= aCount; i++)
        U.addBody(getAsteroidFromUser_UI(i));

    // Canvas
    canvas graphic(filename);
    graphic.startDoc();
    graphic.drawFrame();

    // Static color array (vector removed)
    const char* colors[] = {
            "red","green","blue","yellow","cyan","orange","magenta"
    };
    const int COLOR_COUNT = 7;

    // Simulation
    printSection("SIMULATION RUNNING");
    std::cout << " Please wait...\n\n";

    for (int t = 0; t < 2000; t++) {

        U.run(1);

        BodyNode* node = U.getList().head();
        int idx = 0;

        while (node) {
            graphic.drawPoint(
                    node->obj->getPosX(),
                    node->obj->getPosY(),
                    colors[idx % COLOR_COUNT]
            );
            node = node->next;
            idx++;
        }
    }

    graphic.finishDoc();

    printSection("DONE");
    std::cout << " Output file ready: " << filename << ".html\n\n";

    return 0;
}
