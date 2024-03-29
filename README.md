Zadanie Python + C 

Proszę napisać w Pythonie moduł do testowania biblioteki napisanego w języku C. 

C:
Proszę napisać prostą, przykładową implementację w języku C służącą do zarządzania zbiorem maksymalnie  64 prefiksów podsieci IPv4.
Prefiks określa zakres adresów wyrażony za pomocą wartości bazowej oraz maski, przy czym maska posiada ustawione tylko najbardziej znaczące bity w ilości wyrażonej dla uproszczenia liczbą.
Dla przykładu prefiks 10.20.0.0/16 oznacza zakres adresów 10.20.0.0 - 10.20.255.255, a prefiks 32.64.128.0/20 oznacza zakres 32.64.128.0 - 32.64.143.255. 

Struktura danych powinna przechowywać zbiór prefiksów czyli par - baza IP (32 bity) i maska (wartości 0 - 32).
Muszą zostać zaimplementowane następujące funkcje:
int add(unsigned int base, char mask)
Dodanie prefiksu do zbioru. Zwraca 0 lub -1 dla błędnych argumentów wywołania.
int del(unsigned int base, char mask)
Usunięcie prefiksu ze zbioru. Zwraca 0 lub -1 dla błędnych argumentów wywołania.
char check(unsigned int ip)
Sprawdzenie czy adres IP zawiera się w zbiorze prefiksów. Zwraca maskę najmniejszego prefiksu (o największej wartości maski) w zbiorze, który zawiera wskazany adres. Jeżeli IP nie zawiera się w zbiorze prefiksów zwraca -1. 

Prefiksy w zbiorze są unikalne czyli dodanie istniejącego już prefiksu nie wywołuje żadnej akcji. Usunięcie prefiksu zawierającego w sobie krótszą maskę nie powoduje ich usunięcia. 

Python:
Proszę napisać skrypt w Pythonie wywołujący program napisany w C. Skrypt powinien kompleksowo testować opisane wyżej funkcjonalności. Należy przewidzieć rozszerzanie przypadków testowych o nowe scenariusze
Skrypt powinien generować raport z przeprowadzonych testów.
