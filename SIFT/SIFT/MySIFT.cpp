/****************************************************** 
 *	Code by Utkarsh Sinha
 *	Based on JIFT by Jun Liu
 *	Visit http://aishack.in/ for more indepth articles and tutorials
 *	on artificial intelligence
 * Use, reuse, modify, hack, kick. Do whatever you want with
 * this code :)
 ******************************************************/

// MySIFT.cpp : Defines the entry point for the console application.
//

#include "SIFT.h"

#include <opencv2/opencv.hpp>

// The main function!
int main()
{
	// Create an instance of SIFT
	SIFT *sift = new SIFT("left.jpg", 4, 2);

	sift->DoSift();				// Find keypoints
	sift->ShowAbsSigma();		// Display the sigma table
	sift->ShowKeypoints();		// Show the keypoints
	cvWaitKey(0);				// Wait for a keypress

	// Cleanup and exit
	delete sift;
	return 0;
}

