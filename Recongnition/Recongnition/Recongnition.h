//
//  Recongnition.h
//  Recongnition
//
//  Created by WANG Yang on 4/25/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#ifndef Recongnition_Recongnition_h
#define Recongnition_Recongnition_h

#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <set>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <omp.h>

using namespace cv;
using namespace std;


class Recongnition{
    
public:
    void predict(String filename);
    void predictAll(String directory);
    void initSVMs();
    Recongnition();    
    ~Recongnition();
    
private:
    map<string,CvSVM> classes_classifiers;
    
};

#endif
