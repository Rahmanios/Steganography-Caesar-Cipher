/**
*@file main.cpp
*@brief main program for encoding and decoding text into/from images using LSB steganography
*and Caesar Cipher encryption
*
*@usage : This program allows user to:
*1-encode a text into an image using the Least significant bit (LSB) method
*2-decode a text from an image that was previously encoded using the LSB method
*3-Optionally encrypt or decrypy the text with caesar cipher
*
*@headerfile caesar_cipher.h : for encrypting and decrypting the text using caesar cipher
*@headerfile image_encoder.h : for encoding the text into the image
*@headerfile image_decoder.h : for decoding the text from the image
*
*/
#include <bitset>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "caesar_cipher.h"
#include "image_encoder.h"
#include "image_decoder.h"
//using namespace cv;
//using namespace std;
/**
 *main_menu for the program
 * Variables
 * ---------
 * choice : integer
 * -the menu choice number 1 for encrypt and 2 for decrypt.
 * image_path : string
 * -The image path
 * text : string
 * -the text to be encrypted into the image
 * output_path : string
 * -the path for saving the encrypted image + the suffix of _encoded
 * decoded_text : string
 * -the decoded string to be returned from decode_text_from_image function and to be shown to the user
 * @return saves the encoded image
 */
int main() {
    int choice;
    std::cout<<"Choose an Option:\n1. Encode a Text Into an Image\n2. Decode a Text From an Image\n";
    std::cin>>choice;

    if(choice==1) {
        std::string image_path, text, output_path;
        int shiftvalue = 0 ;

        std::cout<<"Enter Image Path (PNG Only):  ";
        std::cin>>image_path;

        //check if the image is PNG
        if (image_path.substr(image_path.find_last_of(".") + 1) != "png") {
            std::cerr<<"Error: Only PNG Images Are Allowed!\n";
            return 1;
        }
        std::cout<<"Enter a Text to Encode: ";
        std::cin.ignore(); //clear the input buffer
        std::getline(std::cin, text);


        std::cout<<"Enter Shift Value 0 to 20. (0 for no encryption): ";
        std::cin>>shiftvalue;

        if( shiftvalue >= 0 && shiftvalue <= 20) {
            output_path =image_path.substr(0,image_path.find_last_of(".")) + "_encoded.png";
            if (!encode_text_into_image(image_path, text, output_path, 100 , shiftvalue)) {
                return -1;
            }
        }else {
            std::cout<<"The shift value must be a number between 0 to 20";
        }
        //output path
        //output_path =image_path.substr(0,image_path.find_last_of(".")) + "_encoded.png";

        //testing the encode function.
        //std::string encoded_text = (caesar_encrypt(text, shiftvalue));
        //std::cout<<"encrypted text is: "<<encoded_text<<"\n";
        //std::cout<<"decrypted text is: "<<caesar_decrypt(encoded_text, shiftvalue)<<"\n";

        //if (!encode_text_into_image(image_path, text, "../" + output_path, 100 , shiftvalue)) {
        //    return -1;
        //}
    }else if(choice==2) {
        std::string image_path;
        int shiftvalue = 0;

        std::cout<<"Enter The Encoded Image Path: ";
        std::cin>>image_path;

        std::cout<<"Was The Text Encrypted Using CIPHER ? (Enter Shift Value): ";
        std::cin>>shiftvalue;

        //cout<<decode_text_from_image(image_path, shiftvalue);
        std::string decoded_text = decode_text_from_image(image_path, 100, shiftvalue);
        if (!decoded_text.empty()) {
            std::cout << "Decoded text is: " << decoded_text << std::endl;
        }else {
            std::cerr<<"Error: Image Decode Failed!\n";
        }
    }else {
        std::cerr<<"Error: Invalid Choice.\n";
        return -1;
    }
    return 0;
}

//
// Created by User on 01/03/2025.
//

