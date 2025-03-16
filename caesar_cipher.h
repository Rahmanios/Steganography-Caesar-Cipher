//
// Created by User on 11/03/2025.
//
#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include <string>
std::string caesar_encrypt(const std::string& text, int shift) {
    std::string encrypted_text = text;
    for (char& c : encrypted_text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = base + (c - base + shift) % 26;
        }
    }
    return encrypted_text;
}

std::string caesar_decrypt(const std::string& text, int shift) {
    return caesar_encrypt(text, 26 - shift); //just shifting back
}
#endif //CAESAR_CIPHER_H
