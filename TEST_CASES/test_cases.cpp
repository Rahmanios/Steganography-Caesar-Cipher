#include <gtest/gtest.h>
#include "../caesar_cipher.h"
#include "../image_encoder.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "../image_decoder.h"

using namespace std;
using namespace cv;
    // CAESAR ENCODE DECODE TESTS
// testing if it encrypts and if the shift value works right
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
// testing the large shift value
// the large shift value is passed in this test but its limited in the menu
//so the menu checks the shift value it accepts the shift between 0 and 20
TEST(caesar_test, L_shift) {
    string text = "Hello, World" ;
    int shift = 26;
    string encrypted_text = caesar_encrypt(text, shift);
    EXPECT_EQ(encrypted_text,text);
}
// testing for nonalphabetic characters
TEST(caesar_test,nonalphabetic) {
    string text = "123!@#";
    int shift = 5;
    EXPECT_EQ(caesar_encrypt(text,shift),"123!@#");
    EXPECT_EQ(caesar_encrypt("Hello, World!",3),"Khoor, Zruog!");
}
    // /CAESAR ENCODE DECODE TESTS
//-------------------------------------------------------------------------
    // ENCODING TESTS
// the function to create an image for testing
void create_test_image(const string& path) {
    Mat test_image(500,500,CV_8UC3, Scalar(255,255,255));
    imwrite("../" + path, test_image);
}
// the function to clean the test files
void cleanup_test_files() {
    if (filesystem::exists("../test.png")) {
        filesystem::remove("../test.png");
    }
    if (filesystem::exists("../test_encoded.png")) {
        filesystem::remove("../test_encoded.png");
    }
}
//------------------------------------------------------------------------
//encoding the basic message with no shift value
TEST(image_encoder_test, basic_encoding) {
    // Arrange
    const string input_image = "test.png";
    const string output_image = "test_encoded.png";
    create_test_image(input_image);

    // Act
    string message = "Hello World";
    bool result = encode_text_into_image(input_image, message, output_image);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_TRUE(filesystem::exists("../" + output_image));

    // Cleanup
    cleanup_test_files();
}
// testing with the empty message
TEST(image_encoder_test, empty_message) {
    const string input_image = "test.png";
    const string output_image = "test_encoded.png";
    create_test_image(input_image);

    string message = "";
    bool result = encode_text_into_image(input_image, message, output_image);

    EXPECT_TRUE(result);

    cleanup_test_files();
}
//testing with the 1000 character long message
TEST(image_encoder_test, large_message) {
    const string input_image = "test.png";
    const string output_image = "test_encoded.png";
    create_test_image(input_image);

    string message(1000, 'x');  // 1000 character message
    bool result = encode_text_into_image(input_image, message, output_image);

    EXPECT_TRUE(result);

    cleanup_test_files();
}
// Test with non-existent file
TEST(image_encoder_test, invalid_image_path) {
    string message = "Test";
    bool result = encode_text_into_image("notexist.png", message, "test_encoded.png");

    EXPECT_FALSE(result);

    cleanup_test_files();
}

//-----------------------------------------------------------------------
// basic message decode
TEST(image_decode_test, basic_decoding) {
    create_test_image("test.png");
    encode_text_into_image("test.png", "Test message", "test_encoded.png");

    std::string decoded = decode_text_from_image("test_encoded.png");
    EXPECT_EQ(decoded, "Test message");

    cleanup_test_files();
}
// decoding a shifted message (5)
TEST(image_decode_test, shifted) {
    create_test_image("test.png");
    encode_text_into_image("test.png", "Hello", "test_encoded.png", 100, 5);

    std::string decoded = decode_text_from_image("test_encoded.png", 100, 5);
    EXPECT_EQ(decoded, "Hello");

    cleanup_test_files();
}
// Test with wrong image path
TEST(image_decode_test, wrong_image_path) {
    string decoded = decode_text_from_image("wrong_image.png");
    EXPECT_TRUE(decoded.empty());
}

TEST(image_decode_test, not_encoded_image) {
    // Setup
    create_test_image("test.png");

    // Test with image that has no encoded message
    string decoded = decode_text_from_image("test.png");
    EXPECT_FALSE(decoded.empty());
    // Cleanup
    cleanup_test_files();
}

//----------------------------------------------------------------

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}





//
// Created by kali on 3/18/25.
//
