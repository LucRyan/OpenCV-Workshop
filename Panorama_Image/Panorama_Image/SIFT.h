//
//  SIFT.h
//  Panorama_Image
//
//  Created by WANG Yang on 3/21/12.
//  Copyright 2012 Stevens Institute of Technology. All rights reserved.
//

#include <opencv2/opencv.hpp>

#include "keypoint.h"
#include "descriptor.h"

class mySIFT
{
public:
	mySIFT(IplImage* img, int octaves, int intervals);
	mySIFT(const char* filename, int octaves, int intervals);
	~mySIFT();

	void DoSift();

	void ShowKeypoints();
	void ShowAbsSigma();
    void displayDecs();
    CvMat getDescriptors();

private:
	void GenerateLists();
	void BuildScaleSpace();
	void DetectExtrema();
	void AssignOrientations();
	void ExtractKeypointDescriptors();

	unsigned int GetKernelSize(double sigma, double cut_off=0.001);
	CvMat* BuildInterpolatedGaussianTable(unsigned int size, double sigma);
	double gaussian2D(double x, double y, double sigma);


private:
	IplImage* m_srcImage;			// The image we're working on
	unsigned int m_numOctaves;		// The desired number of octaves
	unsigned int m_numIntervals;	// The desired number of intervals
	unsigned int m_numKeypoints;	// The number of keypoints detected

	IplImage***	m_gList;		// A 2D array to hold the different gaussian blurred images
	IplImage*** m_dogList;		// A 2D array to hold the different DoG images
	IplImage*** m_extrema;		// A 2D array to hold binary images. In the binary image, 1 = extrema, 0 = not extrema
	double**	m_absSigma;		// A 2D array to hold the sigma used to blur a particular image

	vector<Keypoint> m_keyPoints;	// Holds each keypoint's basic info
	vector<Descriptor> m_keyDescs;	// Holds each keypoint's descriptor
};