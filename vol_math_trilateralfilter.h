#ifndef VOL_MATH_TRILATERLFILTER_H
#define VOL_MATH_TRILATERLFILTER_H
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <assert.h>
#include "vol_math_RawImage.h"
#include "vol_math_filter_Interface.h"
#include <vector>
#include <pthread.h>
#include "vol_math_filter_Interface.h"
#define M_EXP 2.7182818284590452353602874713527
class RawArray;
struct Tri_para 
{
	Raw *src;
	PIXTYPE sigmaC;
	Tri_para()
	{

	}
	Tri_para(Raw *src, PIXTYPE sigmaC)
	{
		this->src = src;
		this->sigmaC = sigmaC;
	}
};
class Trilateralfilter
{
public:
	Raw* src;
	Trilateralfilter(Raw* src,Raw *ret,int iter,void(*ProgressChanged)(int,int,int,bool &));
	Trilateralfilter(Raw* src);
	~Trilateralfilter(void);
	//void TrilateralFilter(float sigmaC); 
	void TrilateralFilter(Raw &src,Raw &ret,float sigmaC); 
	void TrilateralFilter_Multi(float sigmaC,int threadcount);
	void TrilateralFilter(Raw & src,Raw & ret,float sigmaC,float sigmaR,float sigma3,float sigma4,float R,int maxhalfsize);
	void TrilateralFilter(Raw & src,Raw & ret,float sigmaC,float sigma,float sigmab,float sigmc) /*===================================================================================================== */;
private:
	RawImage* img;
	Raw *ret;
	Raw *temp;
	int iter;
	RawArray* rawarraydata;
	void (*ProgressChanged)(int ,int ,int ,bool &);
	//Computes X , Y and Z gradients of the input RawImage
	void ComputeGradients(Raw* pX, Raw* pY,Raw *pZ); 
	//Bilaterally filters  gradients pX and pY 
	void BilateralGradientFilter(Raw* pX, Raw* pY,Raw*pZ, Raw* pSmoothX, 
		Raw* pSmoothY,Raw * pSmoothZ,float sigmaC,float sigmaS, int filterSize); //sigmaC,sugmaS is change  from PIXTYPE to float
	//Builds the stack of min-max RawImage gradients; returns the range variance
	PIXTYPE buildMinMaxImageStack(Raw* pX, Raw* pY, Raw* pZ,RawArray* pMinStack,
		RawArray* pMaxStack , int levelMax, float beta);		// beta is changed from PIXTYPE to float

	//Finds the adaptive neighborhood size (stack level) 
	//from the min-max gradient stack
	void findAdaptiveRegion(RawArray* pMinStack, RawArray* pMaxStack, Raw *ftheta,float R, int levelMax); 

	//Filters the detail signal and computes the final output RawImage	
	void DetailBilateralFilter(Raw* srcImg, Raw* pSmoothX, Raw* pSmoothY, Raw* pSmoothZ, 
		Raw* fTheta, Raw *dest,float sigmaCTheta, float sigmaRTheta); // sigmaCTheta sigmaRTheta is changed from PIXTYPE to float
	void ComputeGradientsSipl(Raw* pX, Raw *pY, Raw* pZ);
	//Bilaterally filters  gradients pX and pY 
	void BilateralGradientFilterSipl(Raw* pX, Raw* pY,Raw*pZ, Raw* pSmoothX, 
		Raw* pSmoothY,Raw * pSmoothZ,float sigmaC,float sigmaS, int filterSize); //sigmaC,sugmaS is change  from PIXTYPE to float

	//Builds the stack of min-max RawImage gradients; returns the range variance
	PIXTYPE buildMinMaxImageStackSipl(Raw* pX, Raw* pY, Raw* pZ,RawArray* pMinStack,
		RawArray* pMaxStack , int levelMax, float beta);		// beta is changed from PIXTYPE to float

	//Finds the adaptive neighborhood size (stack level) 
	//from the min-max gradient stack
	void findAdaptiveRegionSipl(RawArray* pMinStack, RawArray* pMaxStack, float R, int levelMax); 

	//Filters the detail signal and computes the final output RawImage	
	void DetailBilateralFilterSipl(Raw* srcImg, Raw* pSmoothX, Raw* pSmoothY, Raw* pSmoothZ, 
		Raw* fTheta, float sigmaCTheta, float sigmaRTheta); // sigmaCTheta sigmaRTheta is changed from PIXTYPE to float

};
#endif 





