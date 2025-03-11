#include <bitset>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

//using namespace cv;
//using namespace std;

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

bool encode_text_into_image(const std::string& image_path, const std::string& text, const std::string& output_path,int pixel_offset = 100, int shiftvalue = 0) {
    // openig the image and checking if its exist
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
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
    if (!cv::imwrite(output_path, image)) {
        std::cerr << "Could not save the encoded image." << std::endl;
        return false;
    }
    //std::cout << std::endl ;
    std::cout << "Image encoded successfully into: " << output_path << std::endl;
    return true;
}

std::string decode_text_from_image(const std::string& image_path, int pixel_offset = 100, int shiftvalue = 0) {
    //loading the encoded image
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not open or load the image." << std::endl;
        return "";
    }

    std::string binary_text ;
    std::cout<<"binary text is: " <<binary_text<<std::endl;
    bool end_symbol = false;

    //extracting the binary text from image with LSB method
    for (int i=pixel_offset; i<image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            for (int k = 0 ; k < 3 ; ++k ) {
                char bit = (image.at<cv::Vec3b>(i, j)[k] & 1) + '0';
                binary_text += bit;
                std::cout<<"binary_text is --->> "<<binary_text << std::endl;

                //finding the ending symbol we used before %%%
                if (binary_text.length() % 8 == 0) {
                    std::string byte = binary_text.substr(binary_text.length() - 8, 8) ;//extracting the last 8 bits
                    //char c = static_cast<char>(bitset<8>(byte).to_ullong()) ;
                    char c = static_cast<char>(std::bitset<8>(byte).to_ulong()) ;//converting the 8bit into character
                    std::cout << "------------char c ------------> " << c << std::endl;
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
                        std::cout << "last_3_byte -> " << last_3_byte <<std::endl;
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
    std::cout << "binary_text.length() -> " << binary_text.length() << std::endl;
    std::cout << "binary_text -> " << binary_text << std::endl;

    //Removing the end_symbol
    std::cout << "removing the end symbol"<<std::endl;
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
    std::cout << "decrypt the encoded_text if there is a shift value"<<std::endl;
    // decrypt the encoded_text if there is a shift value
    if (shiftvalue != 0) {
        decoded_text = caesar_decrypt(decoded_text, shiftvalue);
    }


    return decoded_text ;
}

int main() {
    int choice;
    std::cout<<"Choose an Option:\n1. Encode a Text Into an Image\n2. Decode a Text From an Image\n";
    std::cin>>choice;

    if(choice==1) {
        std::string image_path, text, output_path;
        int shiftvalue = 0;

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

        std::cout<<"Enter Shift Value (0 for no encryption): ";
        std::cin>>shiftvalue;

        //output path
        output_path =image_path.substr(0,image_path.find_last_of(".")) + "_encoded.png";

        //testing the encode function.
        //std::string encoded_text = (caesar_encrypt(text, shiftvalue));
        //std::cout<<"encrypted text is: "<<encoded_text<<"\n";
        //std::cout<<"decrypted text is: "<<caesar_decrypt(encoded_text, shiftvalue)<<"\n";

        if (!encode_text_into_image(image_path, text, output_path, 100 , shiftvalue)) {
            return -1;
        }
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

