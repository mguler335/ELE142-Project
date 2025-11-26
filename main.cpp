#include <iostream>
#include <string>
#include <limits> // std::numeric_limits icin

#include "canvas.cpp"
#include "gravity.cpp"

// ------------------------------------------------------------
//  GORSEL YARDIMCI FONKSIYONLAR
// ------------------------------------------------------------

void printHeader() {
    std::cout << "\n";
    std::cout << "############################################################\n";
    std::cout << "#                                                          #\n";
    std::cout << "#          UZAY SIMULASYONU PROGRAMI (ELE142)              #\n";
    std::cout << "#                                                          #\n";
    std::cout << "############################################################\n\n";
}

void printSection(const std::string& title) {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  >>> " << title << " \n";
    std::cout << "============================================================\n";
}

// ------------------------------------------------------------
//  GIRIS FONKSIYONLARI (INPUT)
// ------------------------------------------------------------

Body* getBodyFromUser_UI(int index) {
    printSection(std::to_string(index) + ". CISIM (BODY) TANIMLAMA");

    std::cout << "   Lutfen degerleri aralarinda bosluk birakarak giriniz:\n";
    std::cout << "   -----------------------------------------------------\n";
    std::cout << "   FORMAT : [Kutle]  [Konum X]  [Konum Y]  [Hiz X]  [Hiz Y]\n";
    std::cout << "   ORNEK  : 100      0          0          0.5      0.5\n\n";

    double m, x, y, vx, vy;

    while (true) {
        std::cout << "-> Giris Yapiniz: ";
        if (std::cin >> m >> x >> y >> vx >> vy) {

            // Veri Dogrulama (Kutle Kontrolu)
            if (m <= 0) {
                std::cout << "   [!] HATA: Kutle 0 veya negatif olamaz. Tekrar deneyin.\n";
                continue;
            }
            break;
        }

        // Hatali karakter girilirse temizle
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "   [!] HATA: Gecersiz format. Lutfen sayisal degerler girin.\n";
    }

    return new Body(m, Vector(x, y), Vector(vx, vy));
}

Rocket* getRocketFromUser_UI(int index) {
    printSection(std::to_string(index) + ". ROKET (ROCKET) TANIMLAMA");

    std::cout << "   Roket parametrelerini sirasiyla giriniz:\n";
    std::cout << "   -----------------------------------------------------------------------\n";
    std::cout << "   FORMAT : [Kutle] [Konum X/Y] [Hiz X/Y] [Gaz Hizi] [Atim Orani] [Motor]\n";
    std::cout << "   ORNEK  : 100     0   0       0.1 0.2   15         0.5          On\n\n";

    double m, x, y, vx, vy, eVel, eRate;
    std::string motorStr;
    bool motorOn = true;

    while (true) {
        std::cout << "-> Giris Yapiniz: ";
        if (std::cin >> m >> x >> y >> vx >> vy >> eVel >> eRate >> motorStr) {

            // Veri Dogrulama
            if (m <= 0) {
                std::cout << "   [!] HATA: Kutle pozitif olmalidir.\n";
                continue;
            }
            if (eRate < 0) {
                std::cout << "   [!] HATA: Yakit atim orani (Exhaust Rate) negatif olamaz.\n";
                continue;
            }

            // Motor Durumu Kontrolu
            if (motorStr == "On" || motorStr == "on" || motorStr == "ON")
                motorOn = true;
            else if (motorStr == "Off" || motorStr == "off" || motorStr == "OFF")
                motorOn = false;
            else {
                std::cout << "   [!] HATA: Motor durumu icin 'On' veya 'Off' yaziniz.\n";
                continue;
            }
            break;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "   [!] HATA: Gecersiz format. Lutfen tekrar deneyin.\n";
    }

    return new Rocket(m, Vector(x, y), Vector(vx, vy), eVel, eRate, motorOn);
}

Gezegen* getGezegenFromUser_UI(int index) {
    // Burada basligi yazdiriyoruz ama input icin Body fonksiyonunu cagiriyoruz.
    printSection(std::to_string(index) + ". GEZEGEN (PLANET) TANIMLAMA");
    std::cout << "   (Not: Gezegen verileri Cisim formatinda alinacaktir)\n";

    // Body fonksiyonunu cagirip veriyi al
    Body* b = getBodyFromUser_UI(index);

    // Yeni Gezegen nesnesini olustur
    auto* p = new Gezegen(
            b->getMass(),
            Vector(b->getPosX(), b->getPosY()),
            Vector(b->getVelX(), b->getVelY())
    );

    delete b; // Gecici nesneyi temizle
    return p;
}

Asteroid* getAsteroidFromUser_UI(int index) {
    printSection(std::to_string(index) + ". ASTEROID (ASTEROID) TANIMLAMA");
    std::cout << "   (Not: Asteroid verileri Cisim formatinda alinacaktir)\n";

    Body* b = getBodyFromUser_UI(index);

    auto* a = new Asteroid(
            b->getMass(),
            Vector(b->getPosX(), b->getPosY()),
            Vector(b->getVelX(), b->getVelY())
    );

    delete b;
    return a;
}

// ------------------------------------------------------------
//  MAIN (ANA PROGRAM)
// ------------------------------------------------------------

int main() {

    printHeader();

    // 1. Dosya Adi
    std::string filename;
    printSection("CIKTI DOSYASI AYARLARI");
    std::cout << "   Olusturulacak HTML dosyasinin adini giriniz.\n";
    std::cout << "   (Ornek: yorunge_deneme)\n\n";
    std::cout << "-> Dosya Adi: ";
    std::cin >> filename;

    Universe U(1.0);

    // 2. Nesne Sayilari
    printSection("SIMULASYON NESNE ADETLERI");
    std::cout << "   Simulasyona eklenecek nesne sayilarini giriniz:\n\n";

    int bCount, rCount, pCount, aCount;

    std::cout << "-> Cisim (Body) Sayisi     : ";
    std::cin >> bCount;

    std::cout << "-> Roket (Rocket) Sayisi   : ";
    std::cin >> rCount;

    std::cout << "-> Gezegen (Planet) Sayisi : ";
    std::cin >> pCount;

    std::cout << "-> Asteroid Sayisi         : ";
    std::cin >> aCount;

    // 3. Nesnelerin Olusturulmasi
    // Donguler sirayla calisir: Body -> Rocket -> Planet -> Asteroid

    for (int i = 1; i <= bCount; i++)
        U.addBody(getBodyFromUser_UI(i));

    for (int i = 1; i <= rCount; i++)
        U.addBody(getRocketFromUser_UI(i));

    for (int i = 1; i <= pCount; i++)
        U.addBody(getGezegenFromUser_UI(i));

    for (int i = 1; i <= aCount; i++)
        U.addBody(getAsteroidFromUser_UI(i));

    // 4. Grafik Baslatma
    canvas graphic(filename);
    graphic.startDoc();
    graphic.drawFrame();

    const char* colors[] = {
            "red","green","blue","orange","magenta","cyan","brown"
    };
    const int COLOR_COUNT = 7;

    // 5. Simulasyon Dongusu
    printSection("SIMULASYON BASLATILIYOR");
    std::cout << "   Hesaplamalar yapiliyor ve dosya olusturuluyor...\n";
    std::cout << "   Lutfen bekleyiniz.\n\n";

    // Ilerleme cubugu efekti
    std::cout << "   Ilerleme: [";

    for (int t = 0; t < 2000; t++) {

        // Her 100 adimda bir noktayla ilerleme goster
        if (t % 100 == 0) std::cout << "#" << std::flush;

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
    std::cout << "] Tamamlandi!\n";

    graphic.finishDoc();

    printSection("ISLEM BASARILI");
    std::cout << "   Cikti dosyasi olusturuldu: " << filename << ".html\n";
    std::cout << "   Dosyayi tarayicinizda acarak yorungeleri inceleyebilirsiniz.\n";
    std::cout << "############################################################\n\n";

    return 0;
}