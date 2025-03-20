#include <gtest/gtest.h>
#include "../caesar_cipher.h"
using namespace std;

// testing if it encrypts and if the shift value is right
TEST(caesar_test, encrypt_text) {
    // Arrange
    string text = "Hello, World";
    int shift = 5;
    // Act
    string encrypted_text = caesar_encrypt(text, shift);
    // Assert
    EXPECT_EQ(encrypted_text, "Mjqqt, Btwqi");
}

// testing if it decrypts the text with shift provided
TEST(caesar_test, decrypt_text) {
    string encrypted_text = "Mjqqt, Btwqi";
    int shift = 5;
    string decrypted_text = caesar_decrypt(encrypted_text, shift);
    EXPECT_EQ(decrypted_text, "Hello, World");
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

//testing::Test::



















//
// Created by kali on 3/18/25.
//
