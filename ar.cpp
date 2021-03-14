#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int NUM_IMAGES = 211;

void pipeline(Mat src) {
    Mat src_gray;
    int thresh = 450;


    //threshold image
    /*Mat src_thresh;
    int threshold_value = 150;
    int threshold_type = 1;
    int const max_value = 255;
    int const max_type = 4;
    int const max_binary_value = 255;
    threshold( src_gray, src_thresh, threshold_value, max_binary_value, threshold_type );*/

    //apply color filtering
    Mat src_hsv, mask, mask1, mask2, src_color_filtered;
    cvtColor(src, src_hsv, COLOR_BGR2HSV);

    inRange(src_hsv, Scalar(55, 0, 151), cv::Scalar(180, 151, 255), mask1);
    //inRange(src_hsv, Scalar(110, 0, 0), cv::Scalar(255, 255, 51), mask2);

    //mask = mask1 | mask2;

    src.copyTo(src_color_filtered, mask1);

    //convert image to gray
    cvtColor(src_color_filtered, src_gray, COLOR_BGR2GRAY );

    //use canny edge detection to find edges in the image
    Mat canny_output;
    Canny(src_gray, canny_output, 300, 900);
    
    //use findContours to find contours based on canny edge detection
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //filter contours based on bounding rectangle aspect ratio; must be between .75 and 2
    vector< vector<Point> > contours_accepted;
    /*vector<Rect> boundRect(contours.size());
    vector<Rect> accepted_rect_aspect(contours.size()); */

    /*for(size_t i = 0; i < contours.size(); ++i) {
        boundRect.at(i) =  boundingRect(contours.at(i));
        if((double)(boundRect.at(i).width)/(double)(boundRect.at(i).height) < 2 && 
        (double)(boundRect.at(i).width)/(double)(boundRect.at(i).height) > .5 && 
        contourArea(contours.at(i)) > 200) {
            contours_accepted.push_back(contours.at(i));
            accepted_rect_aspect.push_back(boundRect.at(i));
        }
    } */
 
    //Filter bounding boxes based on whether or not they overlap
    // vector<Rect> accepted_rect(contours.size());
    // Rect temp;

    // for(int i = 0; i < accepted_rect_aspect.size(); ++i) {
    //     for(int j = 0; j < accepted_rect_aspect.size(); ++j) {
    //         if(i != j) {
    //             temp = accepted_rect_aspect.at(i) & accepted_rect_aspect.at(j);
    //             if(temp.area() > 50) {
    //                 accepted_rect.push_back(accepted_rect_aspect.at(i));
    //                 accepted_rect.push_back(accepted_rect_aspect.at(j));
    //             }
    //         }
    //     }
    // }

    //draw bounding rects
    Mat bounding_mat = Mat::zeros(canny_output.size(), CV_8UC3);

    //for(int i = 0; i < accepted_rect_aspect.size(); i++) {
        //rectangle(src, accepted_rect_aspect.at(i), Scalar(0,0,255),2);
    //}

    //draw contours
    Mat contour_mat = Mat::zeros(canny_output.size(), CV_8UC3);

    // Detecting corners
    cornerHarris(src_gray, bounding_mat, 7, 5, 0.05, BORDER_DEFAULT );

    // Normalizing
    Mat bounding_mat_norm;
    Mat bounding_mat_scaled;
    normalize(bounding_mat, bounding_mat_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(bounding_mat_norm, bounding_mat_scaled);

    // Drawing a circle around corners
    int circleCounter;

    for( int j = 0; j < bounding_mat_norm.rows ; j++ )
    { for( int i = 0; i < bounding_mat_norm.cols; i++ )
    {
        if( (int) bounding_mat_norm.at<float>(j,i) > thresh )
        {
           circle(bounding_mat_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0);
           ++circleCounter;
        }
    }
    }


    //for(int i = 0; i < contours_accepted.size(); i++) {
        //drawContours(src, contours_accepted, (int)i, Scalar(94,223,95), 2, LINE_8, hierarchy, 0 );
    //}

    //display results
    //namedWindow( "Source w/ Bounding Rects", WINDOW_AUTOSIZE );
    //namedWindow("hsv", WINDOW_AUTOSIZE);
 	//namedWindow("Contours", WINDOW_AUTOSIZE);
    
 	//imshow("Source w/ Bounding Rects", src);
    //imshow("hsv", src_color_filtered);
 	//imshow("Contours", contour_mat);
    
 	// Showing the result
    namedWindow("corners_window", CV_WINDOW_AUTOSIZE);
    imshow("corners_window", bounding_mat_scaled);

    waitKey(0);
}

int main( int argc, char** argv )
{
    //get image name from command line for testing
    /*CommandLineParser parser( argc, argv, "{@input | input image}" );
    Mat src = imread(samples::findFile( parser.get<String>( "@input" ) ) );
    if( src.empty() )
    {
      cout << "Could not open or find the image!\n" << endl;
      cout << "Usage: " << argv[0] << " <Input image>" << endl;
      return -1;
    }*/
    string filename;
    Mat temp;
    for(int i = 0; i <= NUM_IMAGES; ++i) {
        cout << i << endl;
        if(i < 10) {
            filename = "000" + to_string(i) + ".jpg";
        } else if (i < 100) {
            filename = "00" + to_string(i) + ".jpg";
        } else {
            filename = "0" + to_string(i) + ".jpg";
        }
        temp = imread(filename);
        pipeline(temp);
    }

 	return 0;
}


//g++ $(pkg-config --cflags --libs opencv4) -std=c++11 -o ar ar.cpp