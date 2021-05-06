#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include <cmath>

using namespace std;
using namespace cv;
using namespace cv::ximgproc;

Mat_<uchar> src;
int k_ = 8;
int blockSize_ = 11;
int type_ = THRESH_BINARY;
int method_ = BINARIZATION_NIBLACK;

RNG rng(12345);

void on_trackbar(int, void*);

int main(int argc, char** argv) {
    //read gray-scale image
    static const char* names[] = {"0200.jpg", "0161.jpg", "test.jpg", 0};

    for(int i = 0; names[i] != 0; i++ ) {
        string filename = samples::findFile(names[i]);
        src = imread(filename, IMREAD_GRAYSCALE);
        //imshow("Source", src);
    
        namedWindow("Niblack", WINDOW_AUTOSIZE);
        createTrackbar("k", "Niblack", &k_, 20, on_trackbar);
        createTrackbar("blockSize", "Niblack", &blockSize_, 30, on_trackbar);
        createTrackbar("method", "Niblack", &method_, 3, on_trackbar);
        createTrackbar("threshType", "Niblack", &type_, 4, on_trackbar);
        on_trackbar(0, 0);
        waitKey(0);
    }

    return 0;
}

void on_trackbar(int, void*) {
    double k = static_cast<double>(k_-10)/10;                 // [-1.0, 1.0]
    int blockSize = 2*(blockSize_ >= 1 ? blockSize_ : 1) + 1; // 3,5,7,...,61
    int type = type_;                                         // THRESH_BINARY, THRESH_BINARY_INV, THRESH_TRUNC, THRESH_TOZERO, THRESH_TOZERO_INV
    int method = method_;                                     // BINARIZATION_NIBLACK, BINARIZATION_SAUVOLA, BINARIZATION_WOLF, BINARIZATION_NICK                                    
                                                              

    Mat dst, dstBlur, dstBlurCanny;                                        
    
    niBlackThreshold(src, dst, 255, type, blockSize, k, method);
    GaussianBlur(dst, dstBlur, Size(3,3), 0, 0);
    Canny(dstBlur, dstBlurCanny, 50, 200, 3);

    vector<vector<Point>> contours;
    vector<vector<Point>> squares;
    vector<Vec4i> hierarchy;

    findContours(dstBlurCanny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    imshow("Niblack", dst);
    imshow("Blur", dstBlurCanny);

    Mat drawing = Mat::zeros(dstBlur.size(), CV_8UC3);
    for(size_t i = 0; i < contours.size(); i++ ) {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256));
        drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
    }
    imshow("Contours", drawing);
    imshow("Original Image", src);
    
    //Compiler Command:
    //g++ $(pkg-config --cflags --libs opencv4) -std=c++11 -o niBlack niBlack.cpp
}