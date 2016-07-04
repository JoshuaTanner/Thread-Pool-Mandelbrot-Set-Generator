/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	Task.h
Description	:	Task header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once
#ifndef __TASK_H__
#define __TASK_H__
#include <Windows.h>
#include <mutex>

#include "Backbuffer.h"


//Functor responsible for creating one part
//of mandelbrot fractal set
class MandelbrotSegment
{
public:
	MandelbrotSegment() {}
	MandelbrotSegment(int X1, int X2, int Y1, int Y2)
		:StartX(X1), EndX(X2), StartY(Y1), EndY(Y2) {}//Constructor which sets boundaries
	void operator()(CBackBuffer& threadBackbuffer, float fZoom, float fXOffset, float fYOffset) const;
	int StartX;
	int EndX;
	int StartY;
	int EndY;
};

#endif

