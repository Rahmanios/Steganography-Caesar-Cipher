/**
*@file image_encoder.h
*@brief functions for encoding text into an image using the LSB method
* This file contains the encode_text_into_image function, which encodes a given text into an image
* by modifying the LSB of each pixel's color channels
* the text can be encrypted if the shift value is provided and not zero
*/
#ifndef IMAGE_ENCODER_H
#define IMAGE_ENCODER_H
#include <opencv2/opencv.hpp>
#include <string>
#include <bitset>
#include "caesar_cipher.h"

/**
 * @brief Encoding the text into an image using the LSB method.
 * This function takes an input image, encodes the provided text into the image by modifying the LSB of each pixel's
 * color channels. and then saves the encoded image into a new file.
 * @param image_path path to the input image file which it must be only a png file
 * @param text the text to be encoded into the image
 * @param output_path path to save the encoded image file
 * @param pixel_offset the number of pixels to be escaped befor start encoding (100)
 * @param shiftvalue the shift value for caesar cipher encryption (0 for no encryption)
 * @return true if the encoding is successful, otherwise it returns false
 * variables
 * ----------
 */

bool encode_text_into_image(const std::string& image_path, const std::string& text, const std::string& output_path,int pixel_offset = 100, int shiftvalue = 0) {
    // opening the image and checking if its exist
    cv::Mat image = cv::imread("../" + image_path, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not open or find the image." << std::endl;
        return false;
    }
    // Encrypt the input text using caesar cipher if shift value is entered
    std::string processed_text = text;
    if (shiftvalue != 0) {
        processed_text = caesar_encrypt(text, shiftvalue);
    }
    //adding a unique symbol to the end of the text
    processed_text += "%%%";

    std::cout << "processed_text -> " << processed_text << std::endl;
    //converting the processed_text into binary string
    std::string binary_text;
    for (char c : processed_text) {
        binary_text += std::bitset<8>(c).to_string();
    }
    std::cout << "binary_text + %%% -> " << binary_text << std::endl;

    //checking if the image has enough pixels to be encoded
    int total_pixels = image.rows * image.cols * 3; //considering BGR
    if (binary_text.length() > total_pixels - pixel_offset * 3) {
        std::cerr << "Image is too small to encode the text." << std::endl;
        return false;
    }

    //Encoding section
    int index = 0;
    for (int i = pixel_offset; i < image.rows; ++i) {        //loop into rows start from offset
        for (int j = 0; j < image.cols; ++j) {              //loop into cols
            for (int k = 0; k < 3; ++k) {                   //loop over BGR channels 0=blue 1=green 2=red
                if (index < binary_text.length()) {         //0xFF = 11111110, Vec3b => 3byte -> 3*8 bit
                    // Modify the LSB of the pixel
                    image.at<cv::Vec3b>(i, j)[k] = (image.at<cv::Vec3b>(i, j)[k] & 0xFE) | (binary_text[index] - '0');
                    index++;
                } else {
                    break;
                }
            }
            if (index >= binary_text.length()) break;
        }
        if (index >= binary_text.length()) break;
    }
    std::cout << "binary_text.length() -> " << binary_text.length() << std::endl;
    std::cout << "procesed_text.length() -> " << processed_text.length() << std::endl;
    std::cout << "processed_text -> " << processed_text << std::endl;
    std::cout << "binary_text -> " << binary_text << std::endl;

    /*
    for (int i = pixel_offset; i < image.rows ; ++i) {
        for (int j = 0; j < image.cols ; ++j) {
            if (index < binary_text.length()) {
                int channel = j % 3 ;
                cout << channel ;
                int col = j / 3 ;
                image.at<cv::Vec3b>(i, col)[channel] = (image.at<cv::Vec3b>(i, col)[channel] >> shiftvalue) & 0xFF;
                index ++ ;

            }else {
                break;
            }
        }
        if (index >= binary_text.length()) break;
    }
    */

    //saving the image
    if (!cv::imwrite("../" + output_path, image)) {     // the ../ and the error
        std::cerr << "Could not save the encoded image." << std::endl;
        return false;
    }
    //std::cout << std::endl ;
    std::cout << "Image encoded successfully into: " << output_path << std::endl;
    return true;
}

#endif //IMAGE_ENCODER_H

//
// Created by User on 11/03/2025.
//