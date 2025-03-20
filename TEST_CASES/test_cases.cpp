#include <gtest/gtest.h>
#include "../caesar_cipher.h"
using namespace std;
    // CAESAR ENCODE DECODE TESTS
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
    // Arrange
    string encrypted_text = "Mjqqt, Btwqi";
    int shift = 5;
    // Act
    string decrypted_text = caesar_decrypt(encrypted_text, shift);
    // Assert
    EXPECT_EQ(decrypted_text, "Hello, World");
}

// testing the encrypt result if no shift value provided
TEST(shift_test, encrypt_text) {
    // Arrange
    string text = "Hello, world" ;
    int shift = 0 ;
    // Act
    string no_encrypt = caesar_encrypt(text, shift) ;
    // Assert
    EXPECT_EQ(no_encrypt, "Hello, world");
}
    // /CAESAR ENCODE DECODE TESTS






int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}





//
// Created by kali on 3/18/25.
//
