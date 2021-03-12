#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    Mat output1, output2, mask, total_output, src_hsv;

    CommandLineParser parser( argc, argv, "{@input | input image}" );
    Mat src = imread( samples::findFile( parser.get<String>( "@input" ) ) );
    if( src.empty() )
    {
      cout << "Could not open or find the image!\n" << endl;
      cout << "Usage: " << argv[0] << " <Input image>" << endl;
      return -1;
    }

    cvtColor(src, src_hsv, COLOR_BGR2HSV);

    //vertical band
    inRange(src_hsv, Scalar(0, 0, 151), cv::Scalar(180, 90, 255), output1);

    //horizontal band
    inRange(src_hsv, Scalar(110, 0, 0), cv::Scalar(255, 255, 51), output2);

    mask = output1 | output2;

    src.copyTo(total_output, mask);

    namedWindow( "output1", WINDOW_AUTOSIZE );
    namedWindow( "output2", WINDOW_AUTOSIZE );
    namedWindow("total", WINDOW_AUTOSIZE);
    
 	imshow("output1", output1);
    imshow("output2", output2);
    imshow("total", total_output);
    
    waitKey(0);

    return 0;
}