#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main() {
    using namespace cv;
    using namespace std;
    int choice;
    cout<<"Choose an Option:\n1. Encode a Text Into an Image\n2. Decode a Text From an Image\n";
    cin>>choice;
    if(choice==1) {
        string image_path, text, output_path;
        int shiftvalue = 0;
        cout<<"Enter Image Path (PNG Only): ";
        cin>>image_path;
        cout<<"Enter a Text to Encode: ";
        getline(cin, text);
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

