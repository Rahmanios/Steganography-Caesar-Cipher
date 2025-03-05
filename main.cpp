#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

string caesar_encrypt(const string text, int shift) {
    string encrypted_text = text;
    for (char& c : encrypted_text) {
        char base = islower(c) ? c - 32 : c;
        c = base + shift;
    }
return encrypted_text;
}
/*
string caesar_decrypt(const string& text, int shift) {

}
*/

int main() {
    int choice;
    cout<<"Choose an Option:\n1. Encode a Text Into an Image\n2. Decode a Text From an Image\n";
    cin>>choice;
    if(choice==1) {
        string image_path, text, output_path;
        int shiftvalue = 0;
        cout<<"Enter Image Path (PNG Only): ";
        cin>>image_path;
        cout<<"Enter a Text to Encode: ";
        cin.ignore();
        getline(cin, text);
        cout<<"Enter Shift Value (0 for no encryption): ";
        cin>>shiftvalue;
        output_path =image_path.substr(0,image_path.find_last_of("."))+"_encoded.png";
        //calling the encode function in here.
        cout<<(caesar_encrypt(text, shiftvalue));
    }else if(choice==2) {
        string image_path;
        int shiftvalue = 0;
        cout<<"Enter The Encoded Image Path: ";
        cin>>image_path;
        cout<<"Was The Text Encrypted Using CIPHER ? (Enter Shift Value): ";
        cin>>shiftvalue;
    }else {
        cerr<<"Error: Invalid Choice.\n";
        return -1;
    }


    return 0;
}

//
// Created by User on 01/03/2025.
//

