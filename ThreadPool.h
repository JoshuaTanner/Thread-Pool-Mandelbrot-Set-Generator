/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	ThreadPool.h
Description	:	Thread pool header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <vector>
#include <thread>
#include <functional>
#include <thread>
#include <time.h>

#include "WorkQueue.h"
#include "Task.h"
#include "Backbuffer.h"
#include "INIParser.h"

//Thread pool class 
class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();	

	//Pass through handle to window for backbuffers 
	void Initialize(HWND hWindow, int Width, int Height);
	//Add item to work queue
	void Submit(MandelbrotSegment _iworkItem);
	void DoWork(CBackBuffer& threadBackbuffer);
	void SetEndTime();
	void DrawTime();
	void Draw();
	
	std::vector<CBackBuffer*> m_threadBackBuffers;
	CBackBuffer* m_pMainBackBuffer;
	std::vector<std::thread> m_workerThreads;

	
private:	
	ThreadPool(const ThreadPool& _kr) = delete;
	ThreadPool& operator= (const ThreadPool& _kr) = delete;


public: 
	int m_iNumberOfThreads;
	bool m_bFinished;
	int m_iSelectedThread;

private:
	CINIParser* m_pParser;	

	//A WorkQueue of tasks which are functors
	WorkQueue<MandelbrotSegment >* m_pWorkQueue;

	//A variable to hold the number of threads we want
	
	float m_fZoomFactor;
	float m_fXOffset;
	float m_fYOffset;
	mutable std::mutex m_PoolQMutex;

	int NumberOfSegments;
	int SegmentsCompleted; 

	double StartTime;
	double EndTime;

	
	
};
#endif