// Unskew image program

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/ximgproc.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/types_c.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    Mat src = imread("skewtest2.jpg");

    //Compute quad point for edge
    Point Q1=Point2f(179,106);
    Point Q2=Point2f(260,86);
    Point Q3=Point2f(259,221);
    Point Q4=Point2f(176,270);

    // compute the size of the card by keeping aspect ratio.
    double ratio = 0.75;
    double cardH = sqrt((Q3.x-Q2.x)*(Q3.x-Q2.x)+(Q3.y-Q2.y)*(Q3.y-Q2.y)); 
    double cardW = ratio * cardH;
    
    Rect R(Q1.x,Q1.y,cardW,cardH);

    Point R1 = Point2f(R.x,R.y);
    Point R2 = Point2f(R.x+R.width,R.y);
    Point R3 = Point2f(Point2f(R.x+R.width,R.y+R.height));
    Point R4 = Point2f(Point2f(R.x,R.y+R.height));

    vector<Point2f> quad_pts;
    vector<Point2f> square_pts;

    quad_pts.push_back(Q1);
    quad_pts.push_back(Q2);
    quad_pts.push_back(Q3);
    quad_pts.push_back(Q4);

    square_pts.push_back(R1);
    square_pts.push_back(R2);
    square_pts.push_back(R3);
    square_pts.push_back(R4);


    Mat transmtx = getPerspectiveTransform(quad_pts,square_pts);
    int offsetSize = 250;
    Mat transformed = Mat::zeros(R.height + offsetSize, R.width + offsetSize, CV_8UC3);
    warpPerspective(src, transformed, transmtx, transformed.size());

    //rectangle(src, R, Scalar(0,255,0),1,8,0);

    line(src,Q1,Q2, Scalar(0,0,255),1,CV_AA,0);
    line(src,Q2,Q3, Scalar(0,0,255),1,CV_AA,0);
    line(src,Q3,Q4, Scalar(0,0,255),1,CV_AA,0);
    line(src,Q4,Q1, Scalar(0,0,255),1,CV_AA,0);

    imshow("unskewed", transformed);
    imshow("original", src);
    waitKey();

    //g++ $(pkg-config --cflags --libs opencv4) -std=c++11 -o perspective perspective.cpp
}