//
//  Build_Vocabulary.cpp
//  Recongnition
//
//  Created by WANG Yang on 4/18/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include "Build_Vocabulary.h"

/** Constructor */
Build_Vocabulary::Build_Vocabulary()
{
}

/** Destructor */
Build_Vocabulary::~Build_Vocabulary()
{

}



void Build_Vocabulary::extract(String filename, double threshold ,double thresholdCorner){
    
    clock_t start, finish;   
    double elapsed_time;   

    string dir = "Validation/" + filename, filepath;
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;
	
	dp = opendir( dir.c_str() );
	
	// detecting keypoints
	SiftFeatureDetector detector(threshold, thresholdCorner);
	vector<KeyPoint> keypoints;	
	
	// computing descriptors
    //SiftDescriptorExtractor() extractor;
    Ptr<DescriptorExtractor > extractor(
                                        new OpponentColorDescriptorExtractor(
                                                                             Ptr<DescriptorExtractor>(new SiftDescriptorExtractor())
                                                                             )
                                        );
	Mat descriptors;
	Mat training_descriptors;
	Mat img;
	
	cout << "------- Build Vocabulary ---------\n";
	int count = 0;
	cout << "Extract descriptors.."<<endl;
    start=time(NULL);       
	while ((dirp = readdir( dp )))
    {
		filepath = dir + "/" + dirp->d_name;
		
		// If the file is a directory (or is in some way invalid) we'll skip it 
		if (stat( filepath.c_str(), &filestat )) continue;
		if (S_ISDIR( filestat.st_mode ))         continue;
		
		img = imread(filepath);
		if (!img.data) {
			continue;
		}

		detector.detect(img, keypoints);
        {
            Mat out; img.copyTo(out);
            drawKeypoints(img, keypoints, out, Scalar(255));
            imshow("fg",img);
            imshow("keypoints", out);
            finish=time(NULL);
            elapsed_time = finish-start;
            int hours = (int) elapsed_time / 3600;
            int minutes = (int) (elapsed_time - hours * 3600) / 60;
            int seconds = (int) elapsed_time - hours * 3600 - minutes * 60;
            cout << "Elapsed Time: " << hours << ":" << minutes << ":" << seconds << endl;
            waitKey(0);
        }
		extractor->compute(img, keypoints, descriptors);
		training_descriptors.push_back(descriptors);
		cout << ".";
        count++;
    }
	cout << endl;
	closedir( dp );

    cout << "Total Images Input: " << count << endl;
	cout << "Total descriptors: " << training_descriptors.rows << endl;
    

    finish=time(NULL);
    elapsed_time = finish-start;
    int hours = (int) elapsed_time / 3600;
    int minutes = (int) (elapsed_time - hours * 3600) / 60;
    int seconds = (int) elapsed_time - hours * 3600 - minutes * 60;
    cout << "Elapsed Time for Extracting: " << hours << ":" << minutes << ":" << seconds << endl << endl;
	
	FileStorage fs("training_descriptors_" + filename + "_color.txt", FileStorage::WRITE);
	fs << "training_descriptors" << training_descriptors;
	fs.release();
}

void Build_Vocabulary::doKmeans(String filename, String itemname){
    
    clock_t start, finish;   
    double elapsed_time; 
    
    start = time(NULL);
    cout << "Loading Descriptor Files..." << endl;
	FileStorage fs("training_descriptors_" + filename + ".txt", FileStorage::READ);
    Mat training_descriptors;
	fs["training_descriptors"] >> training_descriptors;
	fs.release();	
    
    /*    */
	BOWKMeansTrainer bowtrainer(1000); //num clusters
	bowtrainer.add(training_descriptors);
	cout << "Clustering Bag-of-Words features..." << endl;
    
	Mat vocabulary = bowtrainer.cluster();
    FileStorage fs1("vocabulary_" + filename + "_1000.txt", FileStorage::WRITE);
	fs1 << "vocabulary" << vocabulary;
	fs1.release();
    
    /*
     Mat Label, kMeanCenter;
     TermCriteria KmeanTerm(CV_TERMCRIT_EPS, 100, 0.001);
     kmeans(training_descriptors, 1000, Label , KmeanTerm, 2, KMEANS_PP_CENTERS, kMeanCenter);
     */
    
    /*
    FileStorage fs2("kMeans_1000.txt", FileStorage::WRITE);
	fs2 << "label" << Label;
    fs2 << "kMeanCenter" << kMeanCenter;
	fs2.release();
    */
    
    /*    */
    finish=time(NULL);
    elapsed_time = finish-start;
    int hours = (int) elapsed_time / 3600;
    int minutes = (int) (elapsed_time - hours * 3600) / 60;
    int seconds = (int) elapsed_time - hours * 3600 - minutes * 60;
    cout << "Elapsed Time for Clustering: " << hours << ":" << minutes << ":" << seconds << endl << endl;
    cout << "------- Finished Build Vocabulary ---------\n" << endl;
    
}


void Build_Vocabulary::doKmeansAll(String postFix){
    
    clock_t start, finish;   
    double elapsed_time; 
    
    start = time(NULL);
    cout << "Loading Descriptor Files..." << endl;
	FileStorage fs("training_descriptors_bike_" +postFix+ ".txt", FileStorage::READ);
    Mat training_descriptors_bike;
	fs["training_descriptors"] >> training_descriptors_bike;
	fs.release();	
    
    FileStorage fs1("training_descriptors_cars_" +postFix+ ".txt", FileStorage::READ);
    Mat training_descriptors_cars;
	fs1["training_descriptors"] >> training_descriptors_cars;
	fs1.release();	
    
    FileStorage fs2("training_descriptors_person_" +postFix+ ".txt", FileStorage::READ);
    Mat training_descriptors_person;
	fs2["training_descriptors"] >> training_descriptors_person;
	fs2.release();	
    
    FileStorage fs3("training_descriptors_none_" +postFix+ ".txt", FileStorage::READ);
    Mat training_descriptors_none;
	fs3["training_descriptors"] >> training_descriptors_none;
	fs3.release();	
    
    /*    */
	BOWKMeansTrainer bowtrainer(1000); //num clusters
	bowtrainer.add(training_descriptors_bike);
    bowtrainer.add(training_descriptors_person);
    bowtrainer.add(training_descriptors_cars);
    bowtrainer.add(training_descriptors_none);
	cout << "Clustering Bag-of-Words features..." << endl;
    
	Mat vocabulary = bowtrainer.cluster();
    FileStorage fs4("vocabulary_1000_" +postFix+ ".txt", FileStorage::WRITE);
	fs4 << "vocabulary" << vocabulary;
	fs4.release();
    
    /*    */
    finish=time(NULL);
    elapsed_time = finish-start;
    int hours = (int) elapsed_time / 3600;
    int minutes = (int) (elapsed_time - hours * 3600) / 60;
    int seconds = (int) elapsed_time - hours * 3600 - minutes * 60;
    cout << "Elapsed Time for Clustering: " << hours << ":" << minutes << ":" << seconds << endl << endl;
    cout << "------- Finished Build Vocabulary ---------\n" << endl;
    
}

void Build_Vocabulary::drawKmeans(String vocabularyfile, String descriptorsfile){// can only draw 2D points.
   
    cout << "Loading Files..." << endl;
	FileStorage fs(descriptorsfile, FileStorage::READ);
    Mat training_descriptors;
	fs["training_descriptors"] >> training_descriptors;
	fs.release();	
    
    FileStorage fs1(vocabularyfile, FileStorage::READ);
    Mat vocabulary;
	fs1["vocabulary"] >> vocabulary;
	fs1.release();	

    IplImage* img = cvCreateImage( cvSize( 500, 500 ), 8, 3 );
    cvZero( img );
    
    for( int i = 0; i < training_descriptors.rows; i++ )
    {
        CvPoint2D32f pt = ((CvPoint2D32f*)training_descriptors.data)[i];
        int cluster_idx = vocabulary.data[i];
        cvCircle( img, cvPointFrom32f(pt), 2, CV_RGB(cluster_idx%255 , cluster_idx%255 , cluster_idx%255), CV_FILLED );
    }
    
    training_descriptors.release(); 
    vocabulary.release();
    
    cvShowImage( "clusters", img );
    
    waitKey(0);
}
