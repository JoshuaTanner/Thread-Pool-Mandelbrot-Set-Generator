/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	Task.cpp
Description	:	Task cpp implementaion file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/


#include <iostream>
#include <thread>

#include "Task.h"


/***********************
* findMandelBrot: calculate the iterations
* @author: Joshua Tanner
* @parameter: Y value, upper boundary, lower boundary
* @return: float
********************/
int findMandelBrot(float X, float Y, int iMaxIterations)
{
	//Begin iterations at 0
	int i = 0;
	float fReal = 0.0f, fImag = 0.0f;
	//Iterate through to find val
	while (i < iMaxIterations && (fReal * fReal + fImag + fImag) < 4.0f)
	{
		float fTemp = fReal*fReal - fImag*fImag + X;
		fImag = 2.0f * fReal * fImag + Y;
		fReal = fTemp;
		i++;
	}

	if (i < iMaxIterations)
	{
		//Using log to smooth colours
		float LogN = log(fReal*fReal + fImag*fImag) / 2.0f;
		float NU = log(LogN / log(2.0f)) / log(2.0f);
		i = int(i + 1 - NU);
	}

	return i;
}



/***********************
* RealScale: scale X value
* @author: Joshua Tanner
* @parameter: X value, upper boundary, lower boundary
* @return: float
********************/
float RealScale(int x, float minReal, float maxReal)
{
	float fDiff = maxReal - minReal;
	return(x * (fDiff / 800) + minReal);
}

/***********************
* ImagScale: scale Y value
* @author: Joshua Tanner
* @parameter: Y value, upper boundary, lower boundary
* @return: float
********************/
float ImagScale(int y, float minImag, float maxImag)
{
	float fDiff = maxImag - minImag;
	return(y*(fDiff / 800) + minImag);

}


/***********************
* operator(): overloaded operator to calculate segment
* @author: Joshua Tanner
* @parameter: Y value, upper boundary, lower boundary
* @return: float
********************/
void MandelbrotSegment::operator()(CBackBuffer& threadBackbuffer, float fZoom, float fXOffset, float fYOffset) const
{	
	//Iterate through each pixel in the segment
	for (int i = StartY; i < EndY; i++)
	{
		for (int j = StartX; j < EndX; j++)
		{
			//Scale the values
			float fReal = RealScale(j, -2.5f*(1 / fZoom) + fXOffset, 1.0f*(1 / fZoom) + fXOffset);
			float fImag = ImagScale(i, -1.0f*(1 / fZoom) - fYOffset, 1.0f*(1 / fZoom) - fYOffset);

			int iMand = findMandelBrot(fReal, fImag, 1000);

			//Setting the colours
			int r = (iMand*iMand % 256);
			int g = (int(log(iMand)) % 256);
			int b = ((iMand) % 256);
		
			//Setpixel in thread's backbuffer
			SetPixel(threadBackbuffer.GetBFDC(), j, i, RGB(r, g, b));
			
		}
	}
}