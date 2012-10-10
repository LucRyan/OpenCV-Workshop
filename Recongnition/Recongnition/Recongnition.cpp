//
//  Recongnition.cpp
//  Recongnition
//
//  Created by WANG Yang on 4/25/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include <iostream>
#include "Recongnition.h"

Recongnition::Recongnition(){};


Recongnition::~Recongnition(){}

void Recongnition::predict(String filename){
    cout << "------- Test Images with SVM ---------\n";
    
    FileStorage fs1("vocabulary_1000_with_colors", FileStorage::READ);
    Mat vocabulary;
    fs1["vocabulary"] >> vocabulary;
    fs1.release();
    
    SiftFeatureDetector detector(0.10, 10);
    DescriptorMatcher* matcher= new BruteForceMatcher<L2<float> >();
    //DescriptorExtractor* extractor =  new SiftDescriptorExtractor();
    Ptr<DescriptorExtractor > extractor(
                                        new OpponentColorDescriptorExtractor(
                                                                             Ptr<DescriptorExtractor>(new SiftDescriptorExtractor())
                                                                             )
                                        );

    BOWImgDescriptorExtractor bowide(extractor, matcher);
    bowide.setVocabulary(vocabulary);
    
	//evaluate
    string dir = "Test/" + filename, filepath;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
	dp = opendir( dir.c_str() );
    map<string,map<string,int> > confusion_matrix; // confusionMatrix[classA][classB] = number_of_times_A_voted_for_B;
	vector<string> classes; //load up with the respective classes
    double count = 0.0;
    double total = 0.0;
    while ((dirp = readdir( dp )))
    {
		filepath = dir + "/" + dirp->d_name;
        Mat img;
        Mat response_hist;
        
		// If the file is a directory (or is in some way invalid) we'll skip it 
		if (stat( filepath.c_str(), &filestat )) continue;
		if (S_ISDIR( filestat.st_mode ))         continue;
		if (dirp->d_name[0] == '.')					 continue; //hidden file!
		
		cout << "Evaluate File " << filepath << endl;
        vector<KeyPoint> keypoints;
        
        img = imread(filepath);
        detector.detect(img,keypoints);
		bowide.compute(img, keypoints, response_hist);
		
		//test vs. SVMs
        float minf = FLT_MAX; string minclass;
		for (map<string,CvSVM>::iterator it = classes_classifiers.begin(); it != classes_classifiers.end(); ++it) {
			float res = (*it).second.predict(response_hist, true);
            if (res < minf) {
                minf = res;
                minclass = (*it).first;
            }
			cout << "class: " << (*it).first << ", response: " << res << endl;
		}
        cout << "This image belongs to " << minclass << "." << endl << endl;
        total++;
        if (minclass == filename){
            count++;
        }
        //cout << ".";
    }
	cout << endl;
    stringstream ss; 
    ss << (double)count/total*100.00;
    cout << ss.str().c_str() << "%" << endl;
	closedir( dp );

}


void Recongnition::predictAll(String directory){
    cout << "------- Test Images with SVM ---------\n";
    
    FileStorage fs1("vocabulary_1000_with_colors.txt", FileStorage::READ);
    Mat vocabulary;
    fs1["vocabulary"] >> vocabulary;
    fs1.release();
    
    SiftFeatureDetector detector(0.12, 10);
    DescriptorMatcher* matcher= new BruteForceMatcher<L2<float> >();
    //DescriptorExtractor* extractor =  new SiftDescriptorExtractor();
    Ptr<DescriptorExtractor > extractor(
                                        new OpponentColorDescriptorExtractor(
                                                                             Ptr<DescriptorExtractor>(new SiftDescriptorExtractor())
                                                                             )
                                        );
    
    BOWImgDescriptorExtractor bowide(extractor, matcher);
    bowide.setVocabulary(vocabulary);
    
    String dir, filepath,filename;

    
    map<string,map<string,int> > confusion_matrix; // confusionMatrix[classA][classB] = number_of_times_A_voted_for_B;
	vector<string> classes; //load up with the respective classes
    double count = 0.0;
    double total = 0.0;
    
    
	//evaluate
    #pragma omp parallel for schedule(dynamic,4)
    for(int i = 0; i < 4; i++){
        switch (i) {
            case 0:
                filename = "bike";
                dir = directory+ "/" + filename;
                break;
            case 1:
                filename = "cars";
                dir = directory+ "/"+ filename;
                break;
            case 2:
                filename = "person";
                dir = directory+ "/" + filename;
                break;
            case 3:
                filename = "none";
                dir = directory+ "/" + filename;
                break;
            default:
                break;
        }

        
        DIR *dp;
        struct dirent *dirp;
        struct stat filestat;
        dp = opendir( dir.c_str() );
        
    while ((dirp = readdir( dp )))
    {
		filepath = dir + "/" + dirp->d_name;
        Mat img;
        Mat response_hist;
        
		// If the file is a directory (or is in some way invalid) we'll skip it 
		if (stat( filepath.c_str(), &filestat )) continue;
		if (S_ISDIR( filestat.st_mode ))         continue;
		if (dirp->d_name[0] == '.')					 continue; //hidden file!
		
		cout << "Evaluate File " << filepath << endl;
        vector<KeyPoint> keypoints;
        
        img = imread(filepath);
        detector.detect(img,keypoints);
		bowide.compute(img, keypoints, response_hist);
		
		//test vs. SVMs
        float minf = FLT_MAX; string minclass;
		for (map<string,CvSVM>::iterator it = classes_classifiers.begin(); it != classes_classifiers.end(); ++it) {
			float res = (*it).second.predict(response_hist, true);
            /*if ((*it).first == "none" && res > 0.9) {
                continue;
            }*/
            if (res < minf) {
                minf = res;
                minclass = (*it).first;
            }
            
            /*
            if (res < 0.7 && res > 0.6) {
                minclass = "bike";
            }else if ( res > 0.4 && res < 0.51){
                minclass = "person";
            }else if ( res > 0.51 && res < 0.6){
                minclass = "cars";
            }else{
                minclass = "none";
            }
            */
			cout << "class: " << (*it).first << ", response: " << res << endl;
		}
        cout << "This image belongs to " << minclass << "." << endl << endl;
        total++;
        if (minclass == filename){
            count++;
        }
        
		confusion_matrix[minclass][filename]++;
        //cout << confusion_matrix[minclass][filename];
        //cout << ".";
    }
        
    closedir( dp );
        
}  
	cout << endl;
    
    cout << "         Bike         Cars         Person         None" << endl;
    
    cout << "Bike:     " << confusion_matrix["bike"]["bike"] <<  "           " << confusion_matrix["cars"]["bike"] << "            " 
         << confusion_matrix["person"]["bike"] << "            " << confusion_matrix["none"]["bike"] << "            " << endl;
    
    cout << "Cars:     " << confusion_matrix["bike"]["cars"] << "           " << confusion_matrix["cars"]["cars"] << "            " 
         << confusion_matrix["person"]["cars"] << "            " << confusion_matrix["none"]["cars"] << "            " << endl;
     
    cout << "Person:   " << confusion_matrix["bike"]["person"] << "           " << confusion_matrix["cars"]["person"] << "            " 
         << confusion_matrix["person"]["person"] << "            " << confusion_matrix["none"]["person"] << "            " << endl;    
    
    cout << "None:     " << confusion_matrix["bike"]["none"] << "           " << confusion_matrix["cars"]["none"] << "            " 
         << confusion_matrix["person"]["none"] << "            " << confusion_matrix["none"]["none"] << "             " << endl;
    
}



void Recongnition::initSVMs() {
	string dir, filepath;
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;
	
	cout << "Load SVM classifiers" << endl;
	dir = ".";
	dp = opendir( dir.c_str() );
	
	while ((dirp = readdir( dp )))
    {
		filepath = dir + "/" + dirp->d_name;
		
		// If the file is a directory (or is in some way invalid) we'll skip it 
		if (stat( filepath.c_str(), &filestat )) continue;
		if (S_ISDIR( filestat.st_mode ))         continue;
		if (filepath.find("SVM_classifier_with_colors") != string::npos)
		{
			string class_ = filepath.substr(filepath.rfind('_')+1,filepath.rfind('.')-filepath.rfind('_')-1);
			cout << "load " << filepath << ", class: " << class_ << endl;
			classes_classifiers.insert(pair<string,CvSVM>(class_,CvSVM()));
			classes_classifiers[class_].load(filepath.c_str());
		}
	}
	closedir(dp);
}