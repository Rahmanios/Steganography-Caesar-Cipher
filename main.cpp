#include <bitset>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <iso646.h>
#include <string>

using namespace cv;
using namespace std;

string caesar_encrypt(const string& text, int shift) {
    string encrypted_text = text;
    for (char& c : encrypted_text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = base + (c - base + shift) % 26;
        }
    }
    return encrypted_text;
}

string caesar_decrypt(const string& text, int shift) {
    return caesar_encrypt(text,26 - shift); //just shifting back
}

bool encode_text_into_image(const string& image_path, const string& text, const string& output_path, int shiftvalue = 0) {
    int pixel_offset = 100;
    // openig the image and checking if its exist
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Could not open or find the image." << endl;
        return false;
    }
    // Encrypt the input text using caesar cipher if shift value is entered
    string processed_text = text;
    if (shiftvalue != 0) {
        processed_text = caesar_encrypt(text, shiftvalue);
    }
    //adding a unique symbol to the end of the text
    processed_text += "%%%";

    //converting the processed_text into binary string
    string binary_text;
    for (char c : processed_text) {
        binary_text += bitset<8>(c).to_string();
    }
    //checking if the image has enough pixels to be encoded
    int total_pixels = image.rows * image.cols * 3; //considering BGR
    if (binary_text.length() > total_pixels - pixel_offset * 3) {
        cerr << "Image is too small to encode the text." << endl;
        return false;
    }

    //Encoding section
    int index = 0;
    for (int i = pixel_offset; i < image.rows; ++i) {    //loop into rows
        for (int j = 0; j < image.cols; ++j) {          //loop into cols
            for (int k = 0; k < 3; ++k) {               //loop over BGR channels 0=blue 1=green 2=red
                if (index < binary_text.length()) {     //0xFF = 11111110, Vec3b => 3byte -> 3*8 bit
                    image.at<cv::Vec3b>(i, j)[k] = (image.at<cv::Vec3b>(i, j)[k] >> shiftvalue) & 0xFF;
                    index++;
                }else {
                    break;
                }
            }
            if (index >= binary_text.length()) break;
        }
        if (index >= binary_text.length()) break;
    }
    //saving the image
    if (!cv::imwrite(output_path, image)) {
        cerr << "Could not open or find the image." << endl;
        return false;
    }
    cout << "Image encoded successfully into: " << output_path << endl;
    return true;

}

string decode_text_from_image(const string& image_path, int shiftvalue = 0) {
    int pixel_offset = 100;
    //loading the encoded image
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Could not open or load the image." << endl;
        return "";
    }

    string binary_text ;
    bool end_symbol = false;
    //extracting the binary text from image
    for (int i=pixel_offset; i<image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            for (int k=0; k<3; ++k) {
                char bit = (image.at<cv::Vec3b>(i, j)[k] & 1) + '0';
                binary_text += bit;

                //cout<<"binary_text is --->> "<<binary_text;
                //finding the ending symbol we used before %%%
                if (binary_text.length() % 8 == 0) {
                    string byte = binary_text.substr(binary_text.length() - 8, 8) ;//extracting the last 8 bits
                    //char c = static_cast<char>(bitset<8>(byte).to_ullong()) ;
                    char c = static_cast<char>(bitset<8>(byte).to_ulong()) ;//converting the 8bit into character
                    if (c == '%' && binary_text.length() >= 24) {
                        //checking for the last 3 character is %%%
                        string last_3_byte;
                        for (int l = 1; l<= 3; ++l) {
                            //extracting the last byte if l=1, l=2 or 3 second or third from last
                            byte = binary_text.substr(binary_text.length() - 8 * l,8) ;
                            //last_3_byte = static_cast<byte>(bitset<8>.to_ulong());
                            last_3_byte = static_cast<char>(bitset<8>(byte).to_ulong()) + last_3_byte;
                        }
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
    //return binary_text;

}


int main() {
    int choice;
    cout<<"Choose an Option:\n1. Encode a Text Into an Image\n2. Decode a Text From an Image\n";
    cin>>choice;
    if(choice==1) {
        string image_path, text, output_path;
        int shiftvalue = 0;
        cout<<"Enter Image Path (PNG Only):  ";
        cin>>image_path;
        //check if the image is PNG
        if (image_path.substr(image_path.find_last_of(".") + 1) != "png") {
            cerr<<"Error: Only PNG Images Are Allowed!\n";
            return 1;
        }
        cout<<"Enter a Text to Encode: ";
        cin.ignore(); //clear the input buffer
        getline(cin, text);
        cout<<"Enter Shift Value (0 for no encryption): ";
        cin>>shiftvalue;
        //output path
        output_path =image_path.substr(0,image_path.find_last_of("."))+"_encoded.png";
        //testing the encode function.
        string encoded_text = (caesar_encrypt(text, shiftvalue));
        cout<<"encrypted text is: "<<encoded_text<<"\n";
        cout<<"decrypted text is: "<<caesar_decrypt(encoded_text, shiftvalue)<<"\n";
        if (!encode_text_into_image(image_path, text, output_path, shiftvalue)) {
            return -1;
        }
    }else if(choice==2) {
        string image_path;
        int shiftvalue = 0;
        cout<<"Enter The Encoded Image Path: ";
        cin>>image_path;
        cout<<"Was The Text Encrypted Using CIPHER ? (Enter Shift Value): ";
        cin>>shiftvalue;
        cout<<decode_text_from_image(image_path, shiftvalue);
    }else {
        cerr<<"Error: Invalid Choice.\n";
        return -1;
    }


    return 0;
}

//
// Created by User on 01/03/2025.
//

