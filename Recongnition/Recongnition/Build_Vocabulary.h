//
//  Build_Vocabulary.h
//  Recongnition
//
//  Created by WANG Yang on 4/18/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#ifndef Recongnition_FeatureExtract_h
#define Recongnition_FeatureExtract_h


#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <omp.h>

using namespace cv;
using namespace std;

class Build_Vocabulary{
    
private:
 
    
public:
    void extract(String filename, double threshold ,double thresholdCorner);
    void doKmeans(String filename, String itemname);
    void drawKmeans(String vocabularyfile, String descriptorsfile);
    void doKmeansAll(String postFix);
    
    /* The Constructor and Destructor  */
    Build_Vocabulary();
    ~Build_Vocabulary();
};

#endif



