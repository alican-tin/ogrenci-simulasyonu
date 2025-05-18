
# Öğrenci Simülasyonu Oyunu 

Raylib ve C++ kullanılarak geliştirilmiş 2D bir öğrenci simülasyonu oyunudur. Oyuncu, günlük hayatında enerji, başarı, mutluluk gibi statları dengede tutarak mezun olmaya çalışır.

## Oynanış

- Ders çalış, dışarı çık, yemek ye, uyu gibi aktivitelerle statlarını yönet
- Her hafta sınav, her dönem mezuniyet şartı
- Mini oyun: Dinozor runner
- Rastgele olaylar: sosyal, sağlık, başarı etkileri

## Kullanılan Teknolojiler

- `C++`
- `Raylib`
- `OOP` yapısı (her sınıf ayrı dosyada)
- `Git` ve `GitHub` ile sürüm kontrol

## Ekran Görüntüleri

> Buraya PNG dosyaları eklenmelidir ("assets/screenshots" klasörü)

```markdown
![1 - Menü Ekranı](assets/screenshots/1.png)
![2 - Okul Menüsü](assets/screenshots/2.png)
![3 - Ana Oyun Ekranı](assets/screenshots/3.png)
![4 - Dışarı Aktiviteleri](assets/screenshots/4.png)
![5 - Gün Bitirme Seçeneği](assets/screenshots/5.png)
![6 - Düşük Stat ile Ders Engeli](assets/screenshots/6.png)
![7 - Yemek Menüsü](assets/screenshots/7.png)
![8 - Yemek ve Stat Değişimi](assets/screenshots/8.png)
![9 - Sosyalleşmede Random Event](assets/screenshots/9.png)
![10 - Sınav Günü](assets/screenshots/10.png)
```

## Katkıda Bulunanlar

- **Alican Tin** – Kodlama ve oyun mekaniği
- **Yelda Çoğal** – Tüm görsellerin çizimi ve sprite animasyonlar, kodlama

## 📁 Dosya Yapısı

```
/ (ana klasör)
├── main.cpp
├── PlayerStats.h / PlayerStats.cpp
├── EventManager.h / EventManager.cpp
├── InteractableObject.h / InteractableObject.cpp
├── GameEvent.h
├── ClampUtils.h
├── GameState.h
├── PlayerType.h
├── StatChangePopup.h
├── assets/
│   ├── backgrounds/
│   ├── animations/
│   ├── objects/
│   ├── screenshots/
│   └── (PNG görseller)
```

## Teslim Bilgisi

- Ders: CENG 220 Programlama Dilleri Laboratuvarı
- Teslim tarihi: 18 Mayıs 2025
- Proje linki: [GitHub Linki buraya eklenecek]
