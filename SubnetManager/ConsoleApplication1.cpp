#include <iostream>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <string>
#include <regex>
#include <vector>

using namespace std;

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
    int add(unsigned int base, char mask) {
        if (mask < 0 || mask > 32) {
            return -1; // Sprawdzenie poprawności
        }
        if (subnets.find(base) != subnets.end()) {
            return -1; // Sprawdzenie czy prefiks już istnieje
        }
        subnets[base] = mask;
        return 0;
    }

    int del(unsigned int base, char mask) {
        // Sprawdzenie czy prefiks istnieje w zbiorze
        auto it = subnets.find(base);
        if (it == subnets.end()) {
            return -1; // Prefiks nie istnieje
        }

        // Sprawdzenie, czy maska zgadza się z istniejącą
        if (it->second != mask) {
            return -1; // Niezgodna maska
        }

        // Usunięcie prefiksu ze zbioru
        subnets.erase(it);
        return 0;
    }

    char check(unsigned int ip) {
        char longestMask = -1; // Domyślna wartość dla braku prefiksu

        // Iteracja przez prefiksy w zbiorze
        for (const auto& subnet : subnets) {
            unsigned int subnetBase = subnet.first;
            char subnetMask = subnet.second;

            // Sprawdzenie czy adres IP zawiera się w prefiksie
            unsigned int maskedBase = subnetBase >> (32 - subnetMask);
            unsigned int maskedIP = ip >> (32 - subnetMask);

            // Aktualizacja najdłuższego prefiksu
            if (maskedBase == maskedIP) {
                if (subnetMask > longestMask) {
                    longestMask = subnetMask;
                }
            }
        }
        return longestMask;
    }

    std::string hexToIPAddress(uint32_t hexValue) {
        // Zamiana liczby heksadecymalnej na adres IP
        std::ostringstream ip;
        ip << ((hexValue >> 24) & 0xFF) << '.'   // Pierwszy bajt
            << ((hexValue >> 16) & 0xFF) << '.'  // Drugi bajt
            << ((hexValue >> 8) & 0xFF) << '.'   // Trzeci bajt
            << (hexValue & 0xFF);                // Czwarty bajt
        return ip.str();
    }

    uint32_t ipAddressToHex(const std::string& ipAddress) {
        // Rozdzielenie adresu IP na części
        std::istringstream iss(ipAddress);
        std::string part;
        uint32_t hexValue = 0;
        int shift = 24;
        while (getline(iss, part, '.')) {
            // Konwersja części adresu IP na liczbę całkowitą
            int value = std::stoi(part);

            // Dodanie wartości do liczby heksadecymalnej
            hexValue |= (value << shift);

            // Przesunięcie bitowe w prawo o 8 bitów
            shift -= 8;
        }
        return hexValue;
    }

    bool isValidIPAddress(std::string ip) {
        std::istringstream iss(ip);
        std::string segment;
        std::vector<int> segments;

        while (std::getline(iss, segment, '.')) {
            if (segment.empty() || segment.size() > 3) {
                return false;
            }

            for (char c : segment) {
                if (!std::isdigit(c)) {
                    return false;
                }
            }

            int value = std::stoi(segment);
            if (value < 0 || value > 255) {
                return false;
            }

            segments.push_back(value);
        }

        return segments.size() == 4;
    }

    std::string menu(std::string command) {
        std::vector<std::string> input = splitCommand(command);
        char mask;

        int option = -1;

        if (input[0] == "add") option = 1;
        else if (input[0] == "del") option = 2;
        else if (input[0] == "check") option = 3;

        if (isValidIPAddress(input[1])) {

            switch (option) {
            case 1: // add
                if (input[2] == "") return "-1";
                mask = static_cast<char>(std::stoi(input[2]));
                return std::to_string(add(ipAddressToHex(input[1]), mask));
                break;

            case 2: // del
                if (input[2] == "") return "-1";
                mask = static_cast<char>(std::stoi(input[2]));
                return std::to_string(del(ipAddressToHex(input[1]), mask));
                break;

            case 3: // check
                return std::to_string(check(ipAddressToHex(input[1])));
                break;

            default:
                return "-1";
            }
        }
        else {
            return "-1";
        }
    }

    std::vector<std::string> splitCommand(std::string command) {
        std::vector<std::string> answer(3);
        std::size_t spaceIndex;
        std::size_t slashIndex;

        spaceIndex = command.find(" ");

        // Sprawdź czy znaleziono spację
        if (spaceIndex == std::string::npos) {            
            return {};
        }

        answer[0] = command.substr(0, spaceIndex);

        slashIndex = command.find("/");
        if (slashIndex != std::string::npos) {
            answer[1] = command.substr(spaceIndex + 1, slashIndex - spaceIndex - 1);
            answer[2] = command.substr(slashIndex + 1);
        }
        else {
            answer[1] = command.substr(spaceIndex + 1);
            answer[2] = "";
        }

        return answer;
    }
};

int main() {
    std::string command;
    std::string answer;
    SubnetManager manager;

    std::getline(std::cin, command);

    while (command != "end") {
        answer = manager.menu(command);
        std::cout << answer << endl;
        std::getline(std::cin, command);
    }
    // test(manager);
}

/*
int test(SubnetManager manager) {
    manager.add(0x0A140000, 16); // 10.20.0.0/16
    manager.add(0x20408000, 20); // 32.64.128.0/20
    std::cout << "Check 10.20.128.10: " << (int)manager.check(manager.ipAddressToHex("10.20.128.10")) << std::endl;
    std::cout << "Check " << manager.hexToIPAddress(0x20408BC8) << ": " << (int)manager.check(0x20408BC8) << std::endl;
    std::cout << "Check 192.168.1.1: " << (int)manager.check(0xC0A80101) << std::endl;
    manager.del(0x0A140000, 16);
    std::cout << "Check 10.20.128.10: " << (int)manager.check(0x0A14800A) << std::endl;
    return 0;
}
*/