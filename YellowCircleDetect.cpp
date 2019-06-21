/*
 * YellowCircleDetect.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: techsri
 */
	
	#include "YellowcircleDetect.h"
	
	using namespace cv;
	using namespace std;
	// we'll be using the HSV colour space, instead of the more common RGB colour space. In HSV, each "tint" of colour is assigned a particular number (the Hue).
	//The "amount" of colour is assigned another number (the Saturation) and the brightness of the colour is assigned another number (the Intensity or Value).
	//This gives us the advantage of having a single number (hue) for the yellow circle.
	IplImage* get_thresholded_image(IplImage* img)
	{
		// Convert the image into an HSV image
		IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
		cvCvtColor(img, imgHSV, CV_BGR2HSV);
		IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
	   //
		cvInRangeS(imgHSV, cvScalar(20, 100, 100), cvScalar(30, 255, 255), imgThreshed);
	
		cvReleaseImage(&imgHSV);
		return imgThreshed;
	}
	
	/** @function main */
	int main(int argc, char** argv)
	{
	  Mat src;
	
	  /// Read the image
	  const char* filename = argc >=2 ? argv[1] : "../data/computer-vision-test-image.png";
		 // Loads an image
	  src = imread( filename, IMREAD_COLOR );
		 // Check if image is loaded fine
	  if(src.empty()){
			 printf(" Error opening image\n");
			 printf(" Program Arguments: [image_name -- default %s] \n", filename);
			 return -1;
	   }
	  //౩ Channels conversion of mat to IpIImage.
	  IplImage* ptr_image = cvCreateImage(cvSize(src.cols, src.rows), 8, 3);
	  IplImage ipltemp = src;
	  cvCopy(&ipltemp, ptr_image);
	
	  
	//  IplImage* image = cvCloneImage(&(IplImage)src); 
	  
	  //Threshold to yellow hue. Yellow becomes white. Rest becomes blackened.
	 IplImage*  img_yellow_thresh =  get_thresholded_image(ptr_image);


	 //Convert the thresholded image to an cv::Mat structure for use in HoughCircles function.
	 cv::Mat src_gray = cv::cvarrToMat(img_yellow_thresh);

	 // Reduce the noise so we avoid false circle detection
	 GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

	 cvShowImage("Noise reduced grey Image",img_yellow_thresh);
	 std::vector<cv::Vec3f> circles;
	  // Apply the Hough Transform to find just the circles
	 //gray: Input image (grayscale).
     //HOUGH_GRADIENT: Define the detection method. Currently this is the only one available in OpenCV.
     //dp = 1: The inverse ratio of resolution.
     //min_dist = gray.rows/16: Minimum distance between detected centers.
     //param_1 = 200: Upper threshold for the internal Canny edge detector.
     //param_2 = 100*: Threshold for center detection.
     //min_radius = 0: Minimum radius to be detected. If unknown, put zero as default.
     //max_radius = 0: Maximum radius to be detected. If unknown, put zero as default.
     HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/16, 200, 100, 0, 0 );

	  /// Draw the circles detected
	  for( size_t i = 0; i < circles.size(); i++ )
	  {
		  Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		  int radius = cvRound(circles[i][2]);
		  // circle center
		  circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
		  // circle outline
		  circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
	   }
	
	  /// Show your results
  namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  imshow( "Hough Circle Transform Demo", src );
	
	  waitKey(0);
	  return 0;
	}
