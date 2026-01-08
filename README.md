# 🪐 Space Simulation: Multi-Body Gravity System
<img width="792" height="902" alt="örnek1" src="https://github.com/user-attachments/assets/de17cd47-b38c-47ca-9a4a-af2499bd499e" />

<p align="center">
  <img src="gorseller/simulation_preview.png" width="600" alt="Simulation Preview">
</p>

Bu proje, uzaydaki cisimlerin (gezegenler, roketler, asteroidler) birbirlerine uyguladıkları kütleçekim kuvvetlerini hesaplayan ve hareket yörüngelerini HTML Canvas üzerinde görselleştiren bir **C++** simülasyonudur.

## 🚀 Özellikler
* [cite_start]**Kütleçekim Modellemesi:** Newton'un evrensel kütleçekim yasasını kullanarak tüm cisimlerin birbirine olan etkisini hesaplar[cite: 70].
* [cite_start]**OOP Yapısı:** Cisimler, roketler ve gezegenler için kalıtım (inheritance) ve polimorfizm (polymorphism) içeren genişletilebilir bir sınıf yapısı kullanılmıştır[cite: 70].
* [cite_start]**Roket Dinamiği:** Roketler için yakıt tüketimi (kütle azalması) ve motor itki kuvveti (thrust) simüle edilmiştir[cite: 70].
* **HTML/Canvas Görselleştirme:** Hesaplanan koordinatlar, `canvas.cpp` kütüphanesi aracılığıyla bir HTML dosyasına çizim komutları olarak aktarılır.

## 🛠️ Teknik Detaylar

### Sınıf Hiyerarşisi
* [cite_start]**Vector:** 2D vektör işlemleri (toplama, çıkarma, büyüklük hesaplama)[cite: 70].
* [cite_start]**Body (Temel Sınıf):** Kütle, pozisyon ve hız verilerini tutar, temel hareket denklemlerini (Euler yöntemi) uygular[cite: 70].
* [cite_start]**Rocket (Body'den türetilmiş):** Motor durumuna göre itki üretir ve kütle kaybeder[cite: 70].
* [cite_start]**Gezegen & Asteroid:** Farklı gök cisimlerini temsil eden alt sınıflar[cite: 70].
* [cite_start]**Universe:** Tüm cisimleri bir `BodyList` içinde yönetir ve her adımda (`run`) kütleçekim kuvvetlerini günceller[cite: 70].

### Görselleştirme (Canvas)
Simülasyon çıktıları `1000x1000` piksellik bir koordinat sisteminde çizilir. 
* `drawFrame()`: Koordinat eksenlerini ve ölçeklendirmeyi çizer.
* `drawPoint()`: Cisimlerin anlık konumlarını renkli noktalar olarak işaretler.

## 💻 Kullanım

1.  **Derleme:** Tüm dosyaları (`main.cpp`, `canvas.cpp`, `gravity.cpp`) aynı dizinde bulundurun ve derleyin:
    ```bash
    g++ main.cpp -o SpaceSim
    ```
2.  **Çalıştırma:** Programı çalıştırdıktan sonra sizden şu bilgileri isteyecektir:
    * Çıktı dosyası adı (örn: `yörünge`)
    * Eklenecek cisim sayıları
    * Her cisim için: Kütle, Başlangıç Pozisyonu (X, Y) ve Başlangıç Hızı (VX, VY)
3.  **Sonuç:** Program tamamlandığında bir `.html` dosyası oluşturur. Bu dosyayı herhangi bir modern web tarayıcısıyla açarak yörüngeleri izleyebilirsiniz.

## 📊 Örnek Giriş Formatı
```text
Mass: 500
PosX: 0, PosY: 0
VelX: 0.1, VelY: 0.0
