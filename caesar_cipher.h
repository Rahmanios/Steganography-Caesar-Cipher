//
// Created by User on 11/03/2025.
//
#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H
#include <string>
#include <gtest/gtest.h>

/*
* @file caesar_cipher.h
* @ functions for encrypt and decrypy the text using Caesar Cipher method.
* This file contains two functions:
* - caesar_encrypt: encrypt the text using caesar cipher
* - caesar_decrypt: decrypt the text using caesar cipher
*
* Caesar Cipher is a substitution cipher that shifts each character by a fixed number in the alphabet order
*
 */
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
