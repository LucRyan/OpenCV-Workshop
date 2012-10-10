//
//  Train_SVM.h
//  Recongnition
//
//  Created by WANG Yang on 4/20/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#ifndef Recongnition_Train_SVM_h
#define Recongnition_Train_SVM_h

#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <omp.h>


using namespace cv;
using namespace std;

class Train_SVM{

public:

    void extractTrainingSamples(String dir, String postFix);
    void StartTrainingSVM(String postFix);
    void optimizeHistogram(String postFix);
    Train_SVM();
    ~Train_SVM();
};

#endif
