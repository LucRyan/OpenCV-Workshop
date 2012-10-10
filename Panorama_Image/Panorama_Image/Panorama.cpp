//
//  main.cpp
//  Harris Detector
//
//  Created by WANG Yang on 3/3/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include "Panorama.h"

/** Constructor */
Panorama::Panorama(string img)
{
    /* Load the source image and covert it to gray */ 
    src = imread(img);
    cvtColor(src, srcGray, CV_BGR2GRAY );
    srcCopy = src.clone();
    
    if(src.empty())
	{
		printf("Can't read one of the images\n");
        
	}
    assert(!src.empty());
}

Panorama::Panorama()
{
}



/** Destructor */
Panorama::~Panorama()
{
    src.release();
    srcGray.release();
    srcCopy.release();
}


/** @function doHarris */
int Panorama::doHarris()
{

    Mat R; Mat Rtemp; // Corner Response function
    Mat Ix, Iy, Ixy, Ix2, Iy2; // the second moment eigenvalues function
    
    double maxValtemp, minValtemp;
    double minVal; double maxVal; 
    int sigma = 3; // Gaussian sigma
    float k = 0.04; // the alpha of Response function
    int aperture_size =3, block_size = 3; double scale =  1.0; // parameters of sobel first order derivative.
    char* window = "Harris."; // the name of Harris result

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
            
    cout << "Processing Harris Corner Detector..." << endl;
    /* Initialize the corner response function and the temp mat */
    R = Mat::zeros( srcGray.size(), CV_32FC1 );
    Rtemp = Mat::zeros( srcGray.size(), CV_32FC1 );
    
    /* Use Sobel function to calculate the first order derivative of both x and y */
    Sobel( srcGray, Ix, CV_32F, 1, 0, aperture_size, scale, 0, BORDER_DEFAULT );
    Sobel( srcGray, Iy, CV_32F, 0, 1, aperture_size, scale, 0, BORDER_DEFAULT );
    
    /* Calculate the Gaussian Derivative */
    GaussianBlur(Iy, Iy, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    GaussianBlur(Ix, Ix, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    
    /* Calculate the square of each intensity */
    pow(Ix,2,Ix2);
    pow(Iy,2,Iy2);
    
    /* Calculate the Gaussian Derivative */
    GaussianBlur(Iy2, Iy2, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    GaussianBlur(Ix2, Ix2, Size(block_size, block_size), sigma, 0 ,BORDER_DEFAULT);
    
    /* Calculate the Corner Response function */
    for( int j = 0; j < srcGray.rows; j++ )
    { for( int i = 0; i < srcGray.cols; i++ )
    {
        float lambda_1 = Ix2.at<float>( j, i, 0);
        float lambda_2 = Iy2.at<float>( j, i, 0);
        Rtemp.at<float>(j, i, 0) = lambda_1 * lambda_2 - k * pow( ( lambda_1 + lambda_2 ), 2 );
    }
    }
    
    minMaxLoc( Rtemp, &minValtemp, &maxValtemp, 0, 0, Mat() );
    
    /* Normalize Corner Response function as the maxium value is 1 */
    for( int j = 0; j < srcGray.rows; j++ )
    { for( int i = 0; i < srcGray.cols; i++ )
    {  
        R.at<float>(j, i) =  1 / maxValtemp * Rtemp.at<float>(j, i, 0);
    }
    }

    /* Find local maxima of response function (nonmaximum suppression)*/
    minMaxLoc( R, &minVal, &maxVal, 0, 0, Mat() );
    
    /* Create Window  */
    namedWindow( window, CV_WINDOW_AUTOSIZE );
    int currentLevel = 5;
    int maxLevel = 100;
    
    
    double threshold = ( maxVal - minVal ) * currentLevel/maxLevel ;
    
    for( int j = 0; j < srcGray.rows; j++ )
    { 
        for( int i = 0; i < srcGray.cols; i++ )
        {   
            if( R.at<float>(j,i) >  threshold)
            {
                circle( srcCopy, Point(i,j), 4, Scalar(255,255,255), 0, 0, 0 ); 
            
            }
        
        }
    }
    imshow( window, srcCopy );
    /*
    delete &minVal; delete &maxVal;
    delete &maxValtemp; delete &minValtemp; 
    delete &R; delete &Rtemp;
    delete &Ix; delete &Iy; delete &Ix2; delete &Iy2; delete &Ixy;
    */
    return(0);
}


vector<KeyPoint> Panorama::getKeypoints(){
    
	cout << "Detecting keypoints..." << endl;
	SiftFeatureDetector detector(0.18,0.9);
	vector<KeyPoint> keypoints;
	detector.detect(srcGray, keypoints);
    
    return keypoints;
}

Mat Panorama::getDescriptors(vector<KeyPoint> kp){

    cout << "Computing descriptors..." << endl;

	SiftDescriptorExtractor extractor;
	Mat descriptors;
	extractor.compute(srcGray, kp, descriptors);
    
    return descriptors;
}

vector< DMatch > Panorama::matchingGoodPoint(Mat descriptors1, Mat descriptors2){
    
    cout << "Matching descriptors..." << endl;
	BruteForceMatcher<L2<float> > matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
    
    cout << "Total Matches: " << descriptors1.rows << endl;

    double max_dist = 0; double min_dist = 100;
    
    cout << "Eliminate Bad Matches..." << endl;
    // Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors1.rows; i++ )
    { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    
//    printf("-- Max dist : %f \n", max_dist );
//    printf("-- Min dist : %f \n", min_dist );
    
    // Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
    vector< DMatch > good_matches;
    int count = 0;
    for( int i = 0; i < descriptors1.rows; i++ )
    { 
        if( matches[i].distance < 2*min_dist )
        {
            good_matches.push_back( matches[i]); 
            count++;
        }
    }
    
    cout << "Good Matches: " << count << endl;

    return good_matches;
}

Mat Panorama::getSrcCopy(){
    return srcCopy;
}

int Panorama::test(){
    int i = 100;
    return i;
}

void Panorama::showMatches(Mat img1, Mat img2, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, vector<DMatch> good_matches){
    
    cout << "Drawing the results..." << endl;
	namedWindow("matches", 1);
	Mat img_matches;
	drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("matches", img_matches);
    
    char* filename = new char[200];
    sprintf(filename, "MatchesImage.jpg");
    IplImage saveImage = img_matches;
    cvSaveImage(filename, &saveImage);
}

Mat Panorama::findFmat(Mat img1, Mat img2, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, vector<DMatch> good_matches){
    
    cout << "Finding the Fundamental Matrix..." << endl << endl;

    vector<Point2f> points1, points2;
    
    for( int i = 0; i < good_matches.size(); i++ )
    {
        points1.push_back( keypoints1[ good_matches[i].queryIdx ].pt );
        points2.push_back( keypoints2[ good_matches[i].trainIdx ].pt );
    }
    
    Mat fundamental_matrix = findFundamentalMat(Mat(points2), Mat(points1), FM_RANSAC, 3, 0.99);
    
    return fundamental_matrix;
}

void Panorama::makePanoramaImage(Mat img1, Mat img2, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, vector<DMatch> good_matches){
    Mat warpImage2;
     
    cout << "Generating the PanoramaImage..." << endl;
    
    vector<Point2f> points1, points2;
    for( int i = 0; i < good_matches.size(); i++ )
    {
        points1.push_back( keypoints1[ good_matches[i].queryIdx ].pt );
        points2.push_back( keypoints2[ good_matches[i].trainIdx ].pt );
    }
    Mat H = findHomography( Mat(points2), Mat(points1), CV_RANSAC );
    //Mat fundamental_matrix = findFundamentalMat(Mat(points2), Mat(points1), FM_RANSAC, 3, 0.99);
    
    warpPerspective(img2, warpImage2, H, Size(img2.cols*2, img2.rows*2), INTER_CUBIC);
    
    Mat final(Size(img2.cols + img1.cols, img2.rows*2),CV_8UC3);
    
    Mat roi1(final, Rect(0, 0,  img1.cols, img1.rows));
    Mat roi2(final, Rect(0, 0, warpImage2.cols, warpImage2.rows));
    warpImage2.copyTo(roi2);
    img1.copyTo(roi1);
    imshow("final", final);
    
    char* filename = new char[200];
    sprintf(filename, "PanoramaImage.jpg");
    IplImage saveImage = final;
    cvSaveImage(filename, &saveImage);
}
