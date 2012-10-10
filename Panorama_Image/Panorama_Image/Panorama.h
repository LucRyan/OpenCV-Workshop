//
//  Panorama.h
//  Panorama_Image
//
//  Created by WANG Yang on 3/20/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#ifndef Panorama_H
#define Panorama_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

class Panorama{
    
private:
    Mat src, srcGray; // input image 
    Mat srcCopy; // the background of result  
    
public:
    int test();
    int doHarris();
    vector<KeyPoint> getKeypoints();
    Mat getDescriptors(vector<KeyPoint>);
    vector< DMatch > matchingGoodPoint(Mat, Mat);
    Mat getSrcCopy();
    void showMatches(Mat img1, Mat img2, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, vector<DMatch> good_matches);
    Mat findFmat(Mat img1, Mat img2, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, vector<DMatch> good_matches);
    void makePanoramaImage(Mat img1, Mat img2, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, vector<DMatch> good_matches);
    
    /* The Constructor and Destructor  */
    Panorama(string);
    Panorama();
    ~Panorama();
};

#endif
