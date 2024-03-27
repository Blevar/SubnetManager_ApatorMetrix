#include <iostream>
#include <unordered_map>

/*
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

Autor: Jan Prusinowski
Rev: 1
*/

class SubnetManager {
private:
    std::unordered_map<unsigned int, char> subnets;

public:
    /*
        Dodaje adres do zbioru.
        Argumenty: unsigned int base: IP
                           char mask: maska
        Return: 0
    */
    int add(unsigned int base, char mask) {
        if (mask < 0 || mask > 32) return -1; // Check    
        subnets[base] = mask;
        return 0;
    }

    /*
        Usuwa adres ze zbioru.
        Argumenty: unsigned int base: IP
                           char mask: maska
        Return: 0
    */
    int del(unsigned int base, char mask) {
        // Sprawdzenie czy prefiks istnieje w zbiorze
        auto it = subnets.find(base);
        if (it == subnets.end()) return -1; // Prefiks nie istnieje

        // Sprawdzenie, czy maska zgadza się z istniejącą
        if (it->second != mask) return -1; // Niezgodna maska

        // Usunięcie prefiksu ze zbioru
        subnets.erase(it);
        return 0;
    }

    /*
        Sprawdza czy ip znajduje się w zakresie listy.
        Argumenty: unsigned int base: IP
        Return: char longersMask: Zwraca maskę najmniejszego prefiksu w którym znajduje się IP
                             -1 : jeżeli IP poza zbiorem 
    */
    char check(unsigned int ip) {
        char longestMask = -1; // Domyślna wartość dla braku prefiksu

        // Iteracja przez prefiksy w zbiorze
        for (const auto& subnet : subnets) {
            unsigned int subnetBase = subnet.first;
            char subnetMask = subnet.second;

            // Sprawdzenie czy adres IP zawiera się w prefiksie
            unsigned int maskedBase = subnetBase >> (32 - subnetMask);
            unsigned int maskedIP = ip >> (32 - subnetMask);
            if (maskedBase == maskedIP) {
                // Aktualizacja najdłuższego prefiksu
                if (subnetMask > longestMask) {
                    longestMask = subnetMask;
                }
            }
        }
        return longestMask;
    }
};

int main() {
    SubnetManager manager;

    // Dodanie prefiksów
    manager.add(0x0A140000, 16); // 10.20.0.0/16
    manager.add(0x20408000, 20); // 32.64.128.0/20

    // Sprawdzenie prefiksów
    std::cout << "Check 10.20.128.10: " << (int)manager.check(0x0A14800A) << std::endl; // Should output 16
    std::cout << "Check 32.64.139.200: " << (int)manager.check(0x20408BC8) << std::endl; // Should output 20
    std::cout << "Check 192.168.1.1: " << (int)manager.check(0xC0A80101) << std::endl; // Should output -1

    // Usunięcie prefiksu
    manager.del(0x0A140000, 16);

    // Sprawdzenie ponownie
    std::cout << "Check 10.20.128.10: " << (int)manager.check(0x0A14800A) << std::endl; // Should output -1

    return 0;
}