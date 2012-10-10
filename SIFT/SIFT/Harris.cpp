//
//  main.cpp
//  Harris Detector
//
//  Created by WANG Yang on 3/3/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat src, srcGray; // input image 
Mat srcCopy; // the background of result
Mat R; Mat Rtemp; // Corner Response function
Mat Ix, Iy, Ixy, Ix2, Iy2; // the second moment eigenvalues function


float dx[9] =  {-1.0, 0.0, 1.0,  // sobel kernel
                -2.0, 0.0, 2.0,  // x derivative
                -1.0, 0.0, 1.0};

float dy[9] =  { 1.0,  2.0,  1.0, // soble kernel
                 0.0,  0.0,  0.0, // y derivative
                -1.0, -2.0, -1.0};

int currentLevel = 50;
int maxLevel = 100;

double maxValtemp, minValtemp;
double minVal; double maxVal; 
int sigma = 3; // Gaussian sigma
float k = 0.04; // the alpha of Response function
int aperture_size =3, block_size = 3; double scale =  1.0; // parameters of sobel first order derivative.
char* window = "Harris."; // the name of Harris result

/// Function headers
void  HarrisThreshold( int, void* );

/** @function main */
int main( int argc, char** argv )
{
    /* Load the source image and covert it to gray */ 
    src = imread("image.jpg" );
    cvtColor( src, srcGray, CV_BGR2GRAY );
    
    
    /* Initialize the corner response function and the temp mat */
    R = Mat::zeros( srcGray.size(), CV_32FC1 );
    Rtemp = Mat::zeros( srcGray.size(), CV_32FC1 );
    
    /* Use Sobel function to calculate the first order derivative of both x and y */
    Sobel( srcGray, Ix, CV_32F, 1, 0, aperture_size, scale, 0, BORDER_DEFAULT );
    Sobel( srcGray, Iy, CV_32F, 0, 1, aperture_size, scale, 0, BORDER_DEFAULT );
    
    /* Calculate the Gaussian Derivative */
    GaussianBlur(Iy, Iy, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    GaussianBlur(Ix, Ix, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    
//    namedWindow( "Ix", CV_WINDOW_AUTOSIZE );
//    imshow("Ix",Ix);
//    namedWindow( "Iy", CV_WINDOW_AUTOSIZE );
//    imshow("Iy",Iy);
    
    /* Calculate the square of each intensity */
    pow(Ix,2,Ix2);
    pow(Iy,2,Iy2);
    
    /* Calculate the Gaussian Derivative */
    GaussianBlur(Iy2, Iy2, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    GaussianBlur(Ix2, Ix2, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    
//    namedWindow( "Ix2", CV_WINDOW_AUTOSIZE );
//    imshow("Ix2",Ix2);
//    namedWindow( "Iy2", CV_WINDOW_AUTOSIZE );
//    imshow("Iy2",Iy2);
    
    /* Calculate the Corner Response function */
    for( int j = 0; j < srcGray.rows; j++ )
    { for( int i = 0; i < srcGray.cols; i++ )
    {
        float lambda_1 = Ix2.at<float>( j, i, 0);
        float lambda_2 = Iy2.at<float>( j, i, 0);
        Rtemp.at<float>(j, i, 0) = lambda_1 * lambda_2 - k * pow( ( lambda_1 + lambda_2 ), 2 );
    }
    }

    namedWindow( "Rtemp", CV_WINDOW_AUTOSIZE );
    imshow("Rtemp",Rtemp);
    
    minMaxLoc( Rtemp, &minValtemp, &maxValtemp, 0, 0, Mat() );
//    cout << "Max: " << maxValtemp << "  Min: " << minValtemp << "\n";
    
    /* Normalize Corner Response function as the maxium value is 1 */
    for( int j = 0; j < srcGray.rows; j++ )
    { for( int i = 0; i < srcGray.cols; i++ )
    {  
        R.at<float>(j, i) =  1 / maxValtemp * Rtemp.at<float>(j, i, 0);
    }
    }
    
    /* print out R */
    /*
     for( int j = 0; j < srcGray.rows; j++ )
     { for( int i = 0; i < srcGray.cols; i++ )
     {
     cout << R.at<float>(j,i,0) << ", ";
     }
     cout << "\n";
     }
     */
    
//    namedWindow( "R", CV_WINDOW_AUTOSIZE );
//    imshow("R", R);

    /* Find local maxima of response function (nonmaximum suppression)*/
    minMaxLoc( R, &minVal, &maxVal, 0, 0, Mat() );

    cout << "Max: " << maxVal << "  Min: " << minVal ;
    
    /* Create Window and Trackbar */
    namedWindow( window, CV_WINDOW_AUTOSIZE );
    createTrackbar( " Quality Level:", window, &currentLevel, maxLevel,  HarrisThreshold );
    HarrisThreshold( 0, 0 );
    
    waitKey(0);
    return(0);
}

/** @function HarrisThreshold */
void HarrisThreshold( int, void* )
{
    srcCopy = src.clone();
    if( currentLevel < 1 ) { currentLevel = 1; } 
    
    double threshold = ( maxVal - minVal ) * currentLevel/maxLevel ;
    char buffer[256];  // make sure this is big enough!!!
    snprintf(buffer, sizeof(buffer), "Threshold: %g", threshold);
    putText(srcCopy, buffer, Point(1,17), 2, 0.4, 0);
    
    for( int j = 0; j < srcGray.rows; j++ )
    { for( int i = 0; i < srcGray.cols; i++ )
    {
        if( R.at<float>(j,i) >  threshold)
        { circle( srcCopy, Point(i,j), 4, Scalar(0,255,0), 0, 0, 0 ); }

    }
    }
    imshow( window, srcCopy );
}
