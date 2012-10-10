//
//  main.cpp
//  Recongnition
//
//  Created by WANG Yang on 4/18/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include "Build_Vocabulary.h"
#include "Train_SVM.h"
#include "Recongnition.h"



int main (int argc, const char * argv[])
{
    
    //Build_Vocabulary* buildVocabulary = new Build_Vocabulary();
    //buildVocabulary -> extract("bike", 0.12, 10.0);
    //buildVocabulary -> doKmeans("bike", ".txt");
    //buildVocabulary -> extract("cars", 0.11, 10.0);
    //buildVocabulary -> doKmeans("cars", ".txt");
    //buildVocabulary -> extract("person", 0.09, 10.0);
    //buildVocabulary -> doKmeans("person", ".txt");
    //buildVocabulary -> extract("none", 0.11, 10.0);
    //buildVocabulary -> doKmeans("none", ".txt");
    //buildVocabulary -> doKmeansAll("validation_with_colors");
    
    //Train_SVM* trainSVM = new Train_SVM();
    //trainSVM -> extractTrainingSamples("Train", "with_colors");
    //trainSVM -> optimizeHistogram("with_colors");
    //trainSVM -> StartTrainingSVM("with_colors");
    
    
    Recongnition* Recong = new Recongnition();
    Recong -> initSVMs();
    Recong -> predictAll("Validation");
    //Recong -> predict("person");
    //Recong -> predict("cars");
    //Recong -> predict("bike");
    
    
}