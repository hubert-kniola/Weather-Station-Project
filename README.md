# Weather Station

## Instrukcja użytkowania
#### Instrukcja obsługi urządzenia
//TODO
#### Instrukcja obsługi strony internetowej
1. Aby poprawnie uzyskać dostęp do danych udostępnianych przez stację pogodową niezbędne jest posiadanie sprawnego połączenia internetowego. Bez niego wiadomość strona internetowa będzie wyświetlana jako czysty html.
1. Użytkownik po podłączeniu do sieci bezprzewodowej wchodząc pod adres IP nadany stacji w podsieci routera znajdzie się na stronie głównej.
1. Główna strona zawiera umieszone pod napisem `Weather Station` odnośniki do następujących podstron: `data`, `about`. Obecne poniżej pole `Current Weather` po pobraniu danych z urządzenia wyświetli aktualną pogodę oraz zsynchronizuje czas urządzenia z czasem systemu stacji.
1. Wciśnięcie przez użytkownika odnośnika `data` skutkować będzie przeniesieniem na podstronę, gdzie została udostępniona możliwość podglądu starszych odczytów pogody znajdujących się w pamięci urządzenia. Po pobraniu ilości dostępnych danych użytkownik zostanie poproszony o wpisanie zakresu danych do pobrania.
1. Wciśnięcie przez  użytkownika odnośnika `about` powoduję przejście na podstronę poświęconą autorom projektu oraz informacjom ogólnym na temat stacji pogodowej.
## O projekcie
### Celem projektu było własnoręczne utworzenie stacji pogodowej przy zastosowaniu następujących elementów:
* Moduł WiFi (mikrokontroler ESP8266)
* Moduł karty SD
* Czujnik temperatury i wilgotności powietrza DHT11
* Dioda RGB
* Wyświetlacz LCD 4x20 ze sterownikiem HD44780
* Przyciski Tact-Switch
* Powerbank
* Mikrokontroler STM32F407VG Discovery

### Technologia zastosowana w projekcie:
* Język C - oprogramowanie płytki STM
* Język JavaScript - skryptowa obsługa strony internetowej
* HTML 5 - struktura strony internetowej
* CSS 3 - wygląd strony internetowej
* RESTlike API - struktura działania serwera HTTP
* Fetch API - pobieranie informacji z serwera HTTP
* Bootstrap 4 - responsywność strony internetowej

### Biblioteki języka C użyte w projekcie:
* FatFs by elm-chan

### Prezentacja [elementów](https://github.com/PUT-PTM-2020/P14/tree/master/stm/stmphotos) hardware projektu:

<p align="center">
  <img src="/stm/stmphotos/hardware.jpg" width="500" title="Aktualna pogoda">
</p>

### [Strona](https://github.com/PUT-PTM-2020/P14/tree/master/website/wwwphotos) internetowa została stworzona w celu odczytywania danych na temat pogody z serwera HTTP mikrokontrolera:

<p align="center">
  <img src="/website/wwwphotos/currentweather.png" width="600" title="Aktualna pogoda">
</p>
<p align="center">
  <img src="/website/wwwphotos/data.png" width="600" title="Dane">
</p>
<p align="center">
  <img src="/website/wwwphotos/phonecurrent.jpg" width="200" title="Wersja mobilna1">
  <img src="/website/wwwphotos/phonedata.jpg" width="200" title="Wersja mobilna2">
  <img src="/website/wwwphotos/phoneabout.jpg" width="200" title="Wersja mobilna3">
</p>

### Obudowa stacji pogodowej została wykonana przy użyciu programu Autodesk Fusion 360.  Fragmenty [obudowy](https://github.com/PUT-PTM-2020/P14/tree/master/model3d/Photo3D): 

<p align="center">
  <img src="/model3d/Photo3D/Obudowa.PNG" width="400" title="Obudowa">
</p>
<p align="center">
  <img src="/model3d/Photo3D/Czujniki.PNG" width="400" title="Czujniki">
</p>

## Autorzy
* Hubert Knioła
* Bartosz Kosmala
> 2020 PUT
