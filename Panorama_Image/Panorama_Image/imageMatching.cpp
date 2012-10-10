//
//  imageMatching.cpp
//  Panorama_Image
//
//  Created by WANG Yang on 3/21/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include <iostream>
// include the necessary libraries
#include <iostream>
#include <opencv2/opencv.hpp>

void switch_callback_h( int position );
void switch_callback_l( int position );


int high_switch_value = 0;
int highInt = 0;
int low_switch_value = 0;
int lowInt = 0;

void switch_callback_h( int position ){
    highInt = position;
}
void switch_callback_l( int position ){
    lowInt = position;
}


// initialize the main function
int imageMatching()
{
    CvCapture* capture =capture = cvCaptureFromCAM(-1); // capture from video device (Macbook iSight)
    
    cvNamedWindow("Window1", CV_WINDOW_AUTOSIZE); // setup window
    cvNamedWindow("Window2", CV_WINDOW_AUTOSIZE); // setup window
    cvNamedWindow("Window3", CV_WINDOW_AUTOSIZE); // setup window
    
    IplImage* newImg; // original image
    IplImage* flipImg;
    IplImage* templateImg;
    IplImage* tempImg;
    IplImage* ftmp[6];
    
    
    
    while(1) {
        newImg = cvQueryFrame( capture );
        if( !newImg ) break;
        // flip image
        flipImg = cvCreateImage(cvGetSize(newImg), IPL_DEPTH_8U, 1);
        flipImg = newImg;
        cvFlip(flipImg, flipImg, 1);
        
        
        int i;
        
        // Read in the template to be used for matching:
        tempImg = cvLoadImage("/Users/chet/Desktop/Yellow_Ball.jpg");        
        templateImg = cvCreateImage(cvGetSize(tempImg), tempImg->depth, tempImg->nChannels);
        //        templateImg = cvCreateImage(cvSize(tempImg->width/10,tempImg->height/10), tempImg->depth, tempImg->nChannels);
        //        templateImg= cvCreateImage(cvGetSize(tempImg), IPL_DEPTH_8U, 1);
        cvResize(tempImg, templateImg);
        
        
        // Allocate Output Images:
        int iwidth = flipImg->width - templateImg->width + 1;
        int iheight = flipImg->height - templateImg->height + 1;
        for(i = 0; i < 6; ++i){
            ftmp[i]= cvCreateImage( cvSize( iwidth, iheight ), 32, 1 );
        }
        
        // Do the matching of the template with the image
        for( i = 0; i < 6; ++i ){
            cvMatchTemplate( flipImg, templateImg, ftmp[i], i );
            cvNormalize( ftmp[i], ftmp[i], 1, 0, CV_MINMAX );
        }
        // ftmp[0] = square difference
        // ftmp[1] = square difference normalized
        // ftmp[2] = correlation matching
        // ftmp[3] = correlation matching normalized
        // ftmp[4] = correlation coefficient
        // ftmp[5] = correlation coefficient normalized
        
        // DISPLAY
        cvShowImage( "Window1", templateImg );
        cvShowImage( "Window2", flipImg );
        cvShowImage( "Window3", ftmp[5] );        
        
        
        char c = cvWaitKey(33); // press escape to quit
        if( c == 27 ) break;
        
    }
    
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Window1" );
    cvDestroyWindow( "Window2" );
    cvDestroyWindow( "Window3" );
    
    cvReleaseImage( &newImg );
    cvReleaseImage( &templateImg );
    cvReleaseImage( &flipImg );
    
    cvReleaseImage( &ftmp[0] );
    cvReleaseImage( &ftmp[1] );
    cvReleaseImage( &ftmp[2] );
    cvReleaseImage( &ftmp[3] );
    cvReleaseImage( &ftmp[4] );
    cvReleaseImage( &ftmp[5] );
    
    
    
    
    return 0;
}