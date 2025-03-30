#include <gtest/gtest.h>
#include "../caesar_cipher.h"
#include "../image_encoder.h"
#include <opencv2/opencv.hpp>

#include "../image_decoder.h"

using namespace std;
using namespace cv;
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
// testing the large shift value
TEST(caesar_test, L_shift) {
    string text = "Hello, World" ;
    int shift = 26;
    string encrypted_text = caesar_encrypt(text, shift);
    EXPECT_EQ(encrypted_text,text);
}
    // /CAESAR ENCODE DECODE TESTS
    // ENCODING TESTS
TEST(image_encoder_test, encode_text) {
    string image_path = "../boat.png" ;
    string text = "Hello World" ;
    string output_path = "test_image_encode.png" ;
    int shift = 0;
    // creating a blank test image
    //cv::Mat image(300, 300, CV_8UC3, cv::Scalar(255, 255, 255));
    //cv::imwrite(image_path, image);
    // encoding text into the image
    bool result = encode_text_into_image(image_path, text, output_path, 100, shift);
    EXPECT_TRUE(result);
}
TEST(image_encoder_test, encode_shifted_text) {
    string image_path = "../boat.png" ;
    string text = "Hello World" ;
    string output_path = "test_image_encode.png" ;
    int shift = 3;
    // creating a blank test image
    //cv::Mat image(300, 300, CV_8UC3, cv::Scalar(255, 255, 255));
    //cv::imwrite(image_path, image);
    // encoding text into the image
    bool result = encode_text_into_image(image_path, text, output_path, 100, shift);
    EXPECT_TRUE(result);
}
    // /ENCODING TESTS
    // DECODING TESTS
//decoding a text with no shift
TEST(image_decoder_test, decode_text_from_image) {
    string image_path = "boat.png";
    string text = "Hello World" ;
    string output_path = "test_image_encode.png" ;
    int shift = 0;

    bool encode_result = encode_text_into_image(image_path, text, output_path, 100, shift);
    ASSERT_TRUE(encode_result);

    string decoded_text = decode_text_from_image(output_path, 100, shift);
    EXPECT_EQ(decoded_text, text);
}

//decoding a text with shift value
/*
TEST(image_decoder_test, decode_text_from_image_with_shift) {
    string image_path = "boat.png";
    string text = "Hello World" ;
    string output_path = "test_image_encode.png" ;
    int shift = 5;

    bool encode_result = encode_text_into_image(image_path, text, output_path, 100, shift);
    ASSERT_TRUE(encode_result);

    string decoded_text = decode_text_from_image(output_path, 100, shift);
    //cout<<"decoded_text is :----------> " << decoded_text ;
    EXPECT_EQ(decoded_text, text);
}
*/





int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}





//
// Created by kali on 3/18/25.
//
