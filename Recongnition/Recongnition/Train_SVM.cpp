//
//  Train_SVM.cpp
//  Recongnition
//
//  Created by WANG Yang on 4/20/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include "Train_SVM.h"

Train_SVM::Train_SVM(){}

Train_SVM::~Train_SVM(){}


void Train_SVM::extractTrainingSamples(String directory, String postFix) {
    
    cout << "-------- Preparing Train SVM --------- " << endl;
    
    clock_t start, finish;   
    double elapsed_time; 
    
    start = time(NULL);
    int total_samples = 0;
    vector<string> classes_names;
	map<string,Mat> classes_training_data;
    
    SiftFeatureDetector detector(0.14,10);
    DescriptorMatcher* matcher= new BruteForceMatcher<L2<float> >();
    //DescriptorExtractor* extractor =  new SiftDescriptorExtractor();
    Ptr<DescriptorExtractor > extractor(
                                        new OpponentColorDescriptorExtractor(
                                                                             Ptr<DescriptorExtractor>(new SiftDescriptorExtractor())
                                                                             )
                                        );
    BOWImgDescriptorExtractor bowide(extractor, matcher);
    String filename;
    string dir;
    
    //#pragma omp parallel for schedule(dynamic,4)
    for(int i = 0; i < 4; i++){
        switch (i) {
            case 3:
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
            case 0:
                filename = "none";
                dir = directory+ "/" + filename;
                break;
            default:
                break;
        }
        cout << "Loading Vocabulary..." << endl;
        FileStorage fs1("vocabulary_1000_"+postFix+".txt", FileStorage::READ);
        Mat vocabulary;
        fs1["vocabulary"] >> vocabulary;
        fs1.release();
        
        bowide.setVocabulary(vocabulary);
        
        string filepath;
        DIR *dp;
        struct dirent *dirp;
        struct stat filestat;
        
        dp = opendir( dir.c_str() );
        cout << "Compute Response Histograms..." << endl;
        while ((dirp = readdir( dp )))
        {
            filepath = dir + "/" + dirp->d_name;
            Mat img;
            Mat response_hist;
            string class_ = "Class_" + filename;
            
            vector<KeyPoint> keypoints;
            // If the file is a directory (or is in some way invalid) we'll skip it 
            if (stat( filepath.c_str(), &filestat )) continue;
            if (S_ISDIR( filestat.st_mode ))         continue;
            
            img = imread(filepath);
            if (!img.data) {
                continue;
            }
            
            detector.detect(img,keypoints);
            bowide.compute(img, keypoints, response_hist);
            
            cout << "."; cout.flush();
            
            #pragma omp critical
            {
                if(classes_training_data.count(class_) == 0) { //not yet created...
                    classes_training_data[class_].create(0,response_hist.cols,response_hist.type());
                    classes_names.push_back(class_);
                }
                classes_training_data[class_].push_back(response_hist);
            }
            total_samples++;
            
            //waitKey(0);
        }
        cout << endl;
    }
    
        
    finish=time(NULL);
    elapsed_time = finish-start;
    int hours = (int) elapsed_time / 3600;
    int minutes = (int) (elapsed_time - hours * 3600) / 60;
    int seconds = (int) elapsed_time - hours * 3600 - minutes * 60;
    cout << "Elapsed Time for Computing: " << hours << ":" << minutes << ":" << seconds << endl << endl;

    
	cout << "Save Training Samples to File..."<<endl;
	{
		FileStorage fs("training_samples_" + postFix + ".txt",FileStorage::WRITE);
		for (map<string,Mat>::iterator it = classes_training_data.begin(); it != classes_training_data.end(); ++it) {
            fs << (*it).first << (*it).second;
            cout << (*it).first << " Saved."<< endl;
		}
        fs.release();
	}
    
}

void Train_SVM::optimizeHistogram(String postFix){
    cout << "Loading Training Samples..." << endl;
    
	map<string,Mat> classes_training_data;
    vector<string> classes_names;
	FileStorage fs("training_samples_"+postFix+".txt",FileStorage::READ);
	
    
    classes_names.push_back("Class_bike");
    classes_names.push_back("Class_cars");
    classes_names.push_back("Class_person");
    classes_names.push_back("Class_none");
    for (vector<string>::iterator it = classes_names.begin(); it != classes_names.end(); ++it) {
		fs[(*it)] >> classes_training_data[*it];
	}
    fs.release();

    int count[1000];
    String name[1000];
    
    Mat m =  classes_training_data["Class_person"];
    
    int rows = classes_training_data["Class_person"].rows;
    
    for (int i = 0; i < 1000; i++) {
        count[i] = 0;
        //cout << count[i];
    }
    for (int x = 0; x < 111; x++) {
        //for (int a = 0; a < classes_names.size(); a++) {
            
            //#pragma omp parallel for schedule(dynamic)
            //for(int i = 0; i < classes_training_data[classes_names[a]].rows; i++){
                for (int j = 0; j < classes_training_data[classes_names[1]].cols; j++){
                    if (classes_training_data["Class_bike"].at<float>(x,j) != 0.0 &&
                        classes_training_data["Class_cars"].at<float>(x,j) != 0.0 &&
                        classes_training_data["Class_person"].at<float>(x,j) != 0.0 &&
                        classes_training_data["Class_none"].at<float>(x,j) != 0.0){
                        count[j]++;
                        //name[j] = classes_names[a];
                    }
                }
            //}

        //}
    }
    /*
    //for (int a = 0; a < classes_names.size(); a++) {
        for( int i = 0; i<classes_training_data[classes_names[a]]; i++){
            for( int j = 0; j<1000; i++){
                //if(count[i] >= 0){
                    cout << name[i] << "[" << i << "]" << ":   " << count[i] << endl;
                    classes_training_data[classes_names[a]].at<float>(i,j) = 0.0 
                //}
            }
        }
     }
    */
    int total = 165+220+111+175;
    int min = 100000000, minIndex;
    int max = 0, maxIndex;
    for( int i = 0; i<1000; i++){
        if(min > count[i]){
            min = count[i];
            minIndex = i;
        }
        if(max < count[i]){
            max = count[i];
            maxIndex = i;
        }
    }
    
    for(int i = 0; i<1000; i++){
        if(count[i] > (min)){
            cout << "[" << i << "]" << ":   " << count[i] << endl;
        }
    }
    cout << "Max: "<< max << "[" << maxIndex << "] " 
          << "    Min: " << min << "[" << minIndex << "]" <<endl;
}

    

void Train_SVM::StartTrainingSVM(String postFix){
    
    cout << "Loading Training Samples..." << endl;

	map<string,Mat> classes_training_data;
    vector<string> classes_names;
	FileStorage fs("training_samples_"+postFix+".txt",FileStorage::READ);
	
    classes_names.push_back("Class_bike");
    classes_names.push_back("Class_cars");
    classes_names.push_back("Class_person");
    classes_names.push_back("Class_none");
    
	for (vector<string>::iterator it = classes_names.begin(); it != classes_names.end(); ++it) {
		fs[(*it)] >> classes_training_data[*it];
	}
    fs.release();

    //#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < classes_names.size(); i++) {
        //train 1-vs-all SVMs
        //map<string,CvSVM> classes_classifiers;
        string class_ = classes_names[i];
        
        Mat& one_class = (*(classes_training_data.begin())).second;
        
        Mat samples(0,one_class.cols,one_class.type());
        Mat labels(0,1,CV_32FC1);
        
        //copy class samples and label
        cout << "Adding " << classes_training_data[class_].rows << " positive." << endl;
        samples.push_back(classes_training_data[class_]);
        Mat class_label = Mat::ones(classes_training_data[class_].rows, 1, CV_32FC1);
        labels.push_back(class_label);
        
        //copy rest samples and label
        for (map<string,Mat>::iterator it1 = classes_training_data.begin(); it1 != classes_training_data.end(); ++it1) {
            string not_class_ = (*it1).first;
            if(not_class_.compare(class_)==0) continue; //skip class itself
            samples.push_back(classes_training_data[not_class_]);
            class_label = Mat::zeros(classes_training_data[not_class_].rows, 1, CV_32FC1);
            labels.push_back(class_label);
        }
            cout << "Train " << class_ << ".." << endl;
            Mat samples_32f; samples.convertTo(samples_32f, CV_32F);
            if(samples.rows == 0) continue; //phantom class?!
            
            CvSVMParams params;
            params.svm_type    = CvSVM::C_SVC;
            params.kernel_type = CvSVM::RBF;
            params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-9);
            
            CvSVM classifier; 
            classifier.train(samples_32f,labels, Mat(), Mat(), params);
           // cout << labels;
        
            {
                stringstream ss; 
                ss << "SVM_classifier_"; 
                if(postFix.size() > 0) ss << postFix << "_";
                ss << class_ << ".txt";
                cout << "SVM_classifier_" << class_ <<" Saved.." << endl << endl;
                classifier.save(ss.str().c_str());
            }
        }
    
    cout << "-------- SVM Train Finished --------- " << endl;
    
}

