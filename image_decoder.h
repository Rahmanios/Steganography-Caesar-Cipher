/**
*@file image_decoder.h
*@brief functions for decoding text into an image using the LSB method
*this file contains decode_text_from_image function, which extracts text from an image that was previously encoded
*using the LSB method.
*/
#ifndef IMAGE_DECODER_H
#define IMAGE_DECODER_H
#include <opencv2/opencv.hpp>
#include <string>
#include <bitset>
#include "caesar_cipher.h"


/**
 *@brief decodes the text from an image using the LSB method
 * @param image_path path to the encoded image file
 * @param pixel_offset the number of pixels to be skipped before starting the encoding (100)
 * @param shiftvalue the shift value for the caesar cipher decryption
 * @return decoded_text
 * variables
 *-----------
 *image_path : string
 * - The image path
 * pixel_offset : integer
 * - an integer number to determine the start point (pixel) for encryption
 * shiftvalue : int
 * - the number for shifting each character
 * binary_text : string
 * - the variable to hold the strings of binary values of the processed (shifted) text for encryption
 * end_symbol : string
 * - the symbol (%%%) as a delimiter to know where is the end of encoded string
 * bit : char
 * - the variable to hold the extracted LSB of the image
 * c : char
 * - converting the 8 bit to character in order to find the end symbol
 * last_3_byte : string
 * - the variable to find the delimiter %%%
 */

std::string decode_text_from_image(const std::string& image_path, int pixel_offset = 100, int shiftvalue = 0) {
    //loading the encoded image
    cv::Mat image = cv::imread("../" + image_path, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not open or load the image." << std::endl;
        return "";
    }

    std::string binary_text ;
    //std::cout<<"binary text is: " <<binary_text<<std::endl;
    bool end_symbol = false;

    //extracting the binary text from image with LSB
    // looping over rows of the image starting from pixel_offset
    for (int i=pixel_offset; i<image.rows; ++i) {
        // looping over the columns of the image
        for (int j = 0; j < image.cols; ++j) {
            // loops over the three color channel BGR
            for (int k = 0 ; k < 3 ; ++k ) {
                // extracting the LSB of the pixel. &1 isolates the last bit of the 8 bit value
                char bit = (image.at<cv::Vec3b>(i, j)[k] & 1) + '0';
                binary_text += bit;
                //std::cout<<"binary_text is --->> "<<binary_text << std::endl;

                //finding the ending symbol we used before %%%
                if (binary_text.length() % 8 == 0) {
                    //extracting the last 8 bits
                    std::string byte = binary_text.substr(binary_text.length() - 8, 8) ;
                    //char c = static_cast<char>(bitset<8>(byte).to_ullong()) ;
                    //converting the 8bit into character
                    char c = static_cast<char>(std::bitset<8>(byte).to_ulong()) ;
                    //std::cout << "------------char c ------------> " << c << std::endl;
                    if (c == '%' && binary_text.length() >= 24) {
                        //checking for the last 3 character is %%%
                        std::string last_3_byte;
                        for (int l = 1; l<= 3; ++l) {
                            //extracting the last byte if l=1, l=2 or 3 second or third from last
                            byte = binary_text.substr(binary_text.length() - 8 * l,8) ;
                            //last_3_byte = static_cast<byte>(bitset<8>.to_ulong());
                            last_3_byte = static_cast<char>(std::bitset<8>(byte).to_ulong()) + last_3_byte;
                            //std::cout << "last_3_byte -> " << last_3_byte <<std::endl;
                            //std::cout << "byte -> " << byte <<std::endl;
                        }
                        //std::cout << "last_3_byte -> " << last_3_byte <<std::endl;
                        if (last_3_byte == "%%%") {
                            end_symbol = true;
                            break;
                        }
                    }
                }
            }
            if (end_symbol) break;
        }
        if (end_symbol) break;
    }
    //std::cout << "binary_text.length() -> " << binary_text.length() << std::endl;
    //std::cout << "binary_text -> " << binary_text << std::endl;

    //Removing the end_symbol
    //std::cout << "removing the end symbol"<<std::endl;
    binary_text = binary_text.substr(0, binary_text.length() - 24 );
    //std::cout << "binary_text without %%% is: -> " << binary_text << std::endl;

    //converting binary_text to string
    std::cout << "converting binary_text to string"<<std::endl;
    std::string decoded_text ;
    // size_t for getting the size of object in memory
    for (size_t i = 0 ; i < binary_text.length(); i += 8) {
        std::string byte = binary_text.substr(i, 8) ;
        char c = static_cast<char>(std::bitset<8>(byte).to_ulong()) ;
        decoded_text += c;
    }
    //std::cout << "decrypt the encoded_text if there is a shift value"<<std::endl;
    // decrypt the encoded_text if there is a shift value
    std::cout << "ShiftValue -------> "<<shiftvalue<<std::endl;
    if (shiftvalue != 0) {
        std::cout << "<------- Processing the IF statement -------> "<<shiftvalue<<std::endl;
        decoded_text = caesar_decrypt(decoded_text, shiftvalue);
    }
    //std::cout << "SHIFT--------------->>> "<<shiftvalue<<std::endl;
    return decoded_text ;
}


#endif //IMAGE_DECODER_H
//
// Created by User on 11/03/2025.
//