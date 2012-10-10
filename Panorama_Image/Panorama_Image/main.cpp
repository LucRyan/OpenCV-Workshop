//
//  main.cpp
//  Panorama_Image
//
//  Created by WANG Yang on 3/20/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Panorama.h"
#include "SIFT.h"


int main (int argc, const char * argv[])
{
    //Initial the instance.
    Panorama* panorama = new Panorama();
    Panorama left("left.jpg");
    Panorama right("right.jpg");
    
    //Detecting keypoints.
    vector<KeyPoint> keypoints1, keypoints2;
    keypoints1 = left.getKeypoints();
    keypoints2 = right.getKeypoints();
    
    //Computing descriptors.
    Mat descriptors1, descriptors2;
    descriptors1 = left.getDescriptors(keypoints1);
    descriptors2 = right.getDescriptors(keypoints2);

    cout << endl;
    
    //Get the goodmatches.
    vector<DMatch> goodMatches = panorama -> matchingGoodPoint(descriptors1, descriptors2);
    
    cout << endl;
    
    //Show and save Matched image.
    panorama -> showMatches(left.getSrcCopy(), right.getSrcCopy(), keypoints1, keypoints2, goodMatches);
    
    //Find the Fundamental Matrix.
    Mat Fundamental_Mat = panorama -> findFmat(left.getSrcCopy(), right.getSrcCopy(), keypoints1, keypoints2, goodMatches);
    cout << Fundamental_Mat << endl << endl;
    
    //Show and save Panarama Image.
    panorama -> makePanoramaImage(left.getSrcCopy(), right.getSrcCopy(), keypoints1, keypoints2, goodMatches);

	waitKey(0);
    
    left.~Panorama();
    right.~Panorama();
    delete panorama;
    
	return 0;
}

