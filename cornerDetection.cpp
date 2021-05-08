#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

void cornerDetection(const Mat &image) {
    imshow("Original Image", image);

    int maxCorners = 60;
    vector<Point2f> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3, gradientSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    Mat imgGray, whiteCopy;
    whiteCopy = image.clone();
    whiteCopy.setTo(Scalar(255, 255, 255));
    cvtColor( image, imgGray, COLOR_BGR2GRAY );

    int morph_size = 2;
    Mat original = imgGray;
    Mat dst;
    Mat element = getStructuringElement(MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size + 1 ), 
                                            Point( morph_size, morph_size ));
    morphologyEx(original, dst, MORPH_OPEN, element);
    imshow("Filtering", dst);

    Mat blur;
    GaussianBlur(dst, blur, Size(9,9), 0);
    imshow("Blur", blur);
    goodFeaturesToTrack( blur,
                         corners,
                         maxCorners,
                         qualityLevel,
                         minDistance,
                         Mat(),
                         blockSize,
                         useHarrisDetector,
                         k );


    for (size_t j = 0; j < corners.size(); ++j) {
        circle(whiteCopy, corners[j], 4, Scalar(255, 0, 0), FILLED);
    }
    imshow("Points", whiteCopy);
    Mat labels;
    Mat centers;
    int clusterCount = 11;
    kmeans(corners, clusterCount, labels, TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
            3, KMEANS_PP_CENTERS, centers);

    vector<Point2f> centerPoints;
    for (size_t k = 0; k < clusterCount; ++k) {
        Point2f pt = centers.at<Point2f>(k);
        centerPoints.push_back(pt);
    } 
    
    Mat copy = image.clone();
    for (size_t i = 0; i < centerPoints.size(); ++i) {
        circle( copy, centerPoints[i], 40, Scalar(0, 255, 0), 1, FILLED );
    }
    imshow("Cluster Centers", copy);
}

int main() {
    Mat src = imread("0182.jpg");
    cornerDetection(src);
    waitKey();
    return 0;
}