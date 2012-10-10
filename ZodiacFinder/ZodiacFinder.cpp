#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std; 

IplImage *inputHigh, *waterMark;
int lowSigma = 10 , highSigma = 10;
char finalWindowName[50];

void setPicture(char inputIm[], char waterIm[], char windowName[], int lowSig, int highSig){ //
    inputHigh = cvLoadImage( inputIm, CV_LOAD_IMAGE_UNCHANGED);
    waterMark = cvLoadImage( waterIm, CV_LOAD_IMAGE_UNCHANGED);
    sprintf(finalWindowName, windowName);
    lowSigma = lowSig; 
    highSigma = highSig;
}
                
int main(int argc, char **argv)
{
    // ============================================================================================================
    // create the variables, include all the used image.       
    // ============================================================================================================
    IplImage *inputLow, *imageCap, *lowImage, *highImage, *output;
//	IplImage *ChannelR,*ChannelG,*ChannelB;
//  IplImage *RedImage,*GreenImage,*BlueImage;
//  IplImage *RedImageLap,*GreenImageLap,*BlueImageLap;
    char fileName[100];
    char key;
    CvMat G_mat; 
    CvSize inputHighSize;
     
    float arr[9] = { 0.0, -1.0, 0.0,
                     -1.0, 4.0, -1.0,
                     0.0, -1.0, 0.0, }; 
    
    G_mat= cvMat(3, 3, CV_32FC1, arr);
    
    // ============================================================================================================
    // Let user input their birth day to identify the image they would use.
    // ============================================================================================================
    int birthYear;
    cout << "Please enter the year you were born:";
    cin >> birthYear;
    
    if(birthYear > 0 && birthYear < 2013 && birthYear != 666){
        
        int select = (birthYear - 1984) >= 0? (birthYear - 1984) % 12 : 12 - (1984 - birthYear) % 12;
        switch(select){
            case 0:
                setPicture("Mouse.jpg", "Shu.jpg", "Your Chinese Zodiac is Rat!", 15, 15);
                break;
            case 1:
                setPicture("Bull.jpg", "Niu.jpg", "Your Chinese Zodiac is Ox!", 15, 10);
                break;
            case 2:
                setPicture("Tiger.jpg", "Hu.jpg", "Your Chinese Zodiac is Tiger!", 15, 5);
                break;
            case 3:
                setPicture("Rabbit.jpg", "Tu.jpg", "Your Chinese Zodiac is Rabbit!", 15, 10);
                break;
            case 4:
                setPicture("Dragon.jpg", "Long.jpg", "Your Chinese Zodiac is Dragon!", 15, 10);
                break;
            case 5:
                setPicture("Snake.jpg", "She.jpg", "Your Chinese Zodiac is Snake!", 15, 3);
                break;
            case 6:
                setPicture("Horse.jpg", "Ma.jpg", "Your Chinese Zodiac is Horse!", 15, 10);
                break;
            case 7:
                setPicture("Goat.jpg", "Yang.jpg", "Your Chinese Zodiac is Ram!", 15, 10);
                break;
            case 8:
                setPicture("Monkey.jpg", "Hou.jpg", "Your Chinese Zodiac is Monkey!", 15, 10);
                break;
            case 9:
                setPicture("Chicken.jpg", "Ji.jpg", "Your Chinese Zodiac is Rooster!", 15, 10);
                break;
            case 10:
                setPicture("Dog.jpg", "Gou.jpg", "Your Chinese Zodiac is Dog!", 15, 20);
                break;
            case 11:
                setPicture("Pig.jpg", "Zhu.jpg", "Your Chinese Zodiac is Pig!", 15, 10);
                break;
        }

    }else if(birthYear == 0){ // Some Easter Egg.
        setPicture("God.jpg", "Shizi.jpg", "At the begining of B.C., are you the Jesus?", 15, 10);
    }else if(birthYear == 666){
        setPicture("Satan.jpg", "Liu.jpg", "All right, I known who you are.", 15, 10);
    }else{
        setPicture("Alien.jpg", "Et.jpg", "You're not belong to this planet!", 15, 15);
    }
    
    if(!inputHigh || !waterMark){
		return -1;
    }

    inputHighSize = cvGetSize(inputHigh); // get the input image's size

    // ============================================================================================================
    // Open the camera and capture user's picture
    // ============================================================================================================
   
    // create all necessary instances
    cvNamedWindow ("Say Cheese! (Press 'C' to Capture, 'ESC' to Exit)", CV_WINDOW_AUTOSIZE); // set window
    CvCapture * camera = cvCreateCameraCapture (CV_CAP_ANY); // Create camera
    cvMoveWindow("Say Cheese! (Press 'C' to Capture, 'ESC' to Exit)", 0, 0); // set the window position
    
    // you do own an iSight, don't you?
    if (! camera)
        abort ();

    while(true)
    {
        imageCap = cvQueryFrame(camera); // Create the image of camera
        cvFlip(imageCap, NULL, 1); //flip the image, like the mirror
        key = cvWaitKey(50); // set the refresh frequency
        if(key == 27) break; //Press ESC to exit
        if(key == 'c')       //press C to capture
        {
            sprintf(fileName, "Picture_%d.jpg", birthYear); //create image file
            cvSaveImage(fileName, imageCap);
            cvXorS(imageCap, cvScalarAll(255), imageCap);// make vision effect about capture
            cvShowImage("Say Cheese! (Press 'C' to Capture, 'ESC' to Exit)",imageCap);
            cvWaitKey(100); //effect lasting time
        }
        cvShowImage("Say Cheese! (Press 'C' to Capture, 'ESC' to Exit)",imageCap); // Show Image 
    }
    // Release memory
    cvReleaseCapture(&camera); 
    cvDestroyWindow("Say Cheese! (Press 'C' to Capture, 'ESC' to Exit)");

    inputLow = cvLoadImage(fileName, CV_LOAD_IMAGE_UNCHANGED); // Create image
    
    
    // ============================================================================================================
    // Process the image.
    // ============================================================================================================
    
//  imageLuv = cvCreateImage(inputHighSize,inputLow->depth,3);
    output = cvCreateImage(inputHighSize,inputLow->depth,3); // Create image
    lowImage = cvCreateImage(inputHighSize,inputLow->depth,3); // Create image
    highImage = cvCreateImage(inputHighSize,inputLow->depth,3); // Create image
    
//    ChannelR = cvCreateImage(inputHighSize,inputLow->depth,1);
//    ChannelG = cvCreateImage(inputHighSize,inputLow->depth,1);
//    ChannelB = cvCreateImage(inputHighSize,inputLow->depth,1);

//    BlueImage=cvCreateImage(inputHighSize,IPL_DEPTH_8U,3);
//    GreenImage=cvCreateImage(inputHighSize, IPL_DEPTH_8U,3);
//    RedImage=cvCreateImage(inputHighSize, IPL_DEPTH_8U,3);
  
//    BlueImageLap=cvCreateImage(inputHighSize,IPL_DEPTH_16S,3);
//    GreenImageLap=cvCreateImage(inputHighSize, IPL_DEPTH_16S,3);
//    RedImageLap=cvCreateImage(inputHighSize, IPL_DEPTH_16S,3);
    
//    cvSplit(inputLow,ChannelR,ChannelG,ChannelB,0);
//    cvMerge(ChannelR,0,0,0,BlueImage);
//    cvMerge(0,ChannelG,0,0,GreenImage);
//    cvMerge(0,0,ChannelB,0,RedImage);
    
//    cvCvtColor(input,imageLuv,CV_RGB2Luv);
    
    cvSmooth(inputLow, lowImage, CV_GAUSSIAN, 35, 35, lowSigma); // Use Gaussian to blur background image (low pass)
    cvSmooth(inputHigh, highImage, CV_GAUSSIAN, 35, 35, highSigma); // Use Gaussian to filter the high frequency

//    cvLaplace(input, RedImageLap, 1);
//    cvLaplace(RedImage, RedImageLap, 3);
//    cvLaplace(GreenImage, GreenImageLap, 3);
//    cvLaplace(BlueImage, BlueImageLap, 3);

    
    cvSub(inputHigh, highImage, highImage); // Use Original image sub the low frequency, then we'll get the high frequence image.
    cvAdd(lowImage, highImage, output); // Add two image together.
    cvAdd(output, waterMark, output); // Add the Watermark for out put image
    
    cvNamedWindow(finalWindowName); // create the window to contain the image
	cvShowImage(finalWindowName, output); // show image
    cvWaitKey(0); // set wait key
   
    // Release memory.
    cvReleaseImage(&highImage);
    cvReleaseImage(&lowImage);
    cvReleaseImage(&inputHigh);
    cvReleaseImage(&inputLow);
    cvReleaseImage(&output);
    cvDestroyWindow(finalWindowName);
    
    
	return 0;
}