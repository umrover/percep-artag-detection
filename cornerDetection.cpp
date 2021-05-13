#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <vector>
#include <string>
using namespace cv;
using namespace std;

void cornerDetection(const Mat &image) {
    imshow("Original Image", image);

    Mat imgGray, whiteCopy;
    whiteCopy = image.clone();
    whiteCopy.setTo(Scalar(255, 255, 255));
    cvtColor( image, imgGray, COLOR_BGR2GRAY );

    Mat filter;
    bilateralFilter(imgGray, filter, 15, 80, 80, BORDER_DEFAULT);
    imshow("Filtering", filter);
    
    int maxCorners = 50;
    vector<Point2f> corners;
    double qualityLevel = 0.1;
    double minDistance = 10;
    int blockSize = 3, gradientSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;
    goodFeaturesToTrack( filter,
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
    int clusterCount = 10;
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
    vector<string> jpgs = {"0167.jpg", "0151.jpg", "0155.jpg", "0160.jpg", "0164.jpg", "0170.jpg", 
                            "0177.jpg", "0183.jpg", "0191.jpg", "0197.jpg", "0200.jpg",
                            "0204.jpg", "0210.jpg"};
    for (size_t i = 0; i < jpgs.size(); ++i) {
        Mat src = imread(jpgs[i]);
        cornerDetection(src);
        waitKey();
    }
    return 0;
}