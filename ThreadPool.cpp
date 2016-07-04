/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	threadpool.cpp
Description	:	threadpool implementation file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#include <iostream>
#include <thread>
#include <string>
//Local Includes
#include "WorkQueue.h"
#include "Task.h"

//This Include
#include "ThreadPool.h"

//Global mutex
std::mutex MUTEX;

/***********************
* Constructor: constructs threadpool, loads INI 
* @author: Joshua Tanner
* @parameter: none
* @return:
********************/
ThreadPool::ThreadPool()
{
	m_bFinished = false;   //Set out finished to false to begin
	m_pParser = new CINIParser();
	m_pParser->setFileName("MandelbrotINI.txt");
	m_pParser->LoadIniFile("MandelbrotINI.txt");

	//Setting values from INI
	m_pParser->GetFloatValue("[settings]", "ZoomFactor", m_fZoomFactor);
	m_pParser->GetIntValue("[settings]", "NumberOfThreads", m_iNumberOfThreads);
	m_pParser->GetFloatValue("[settings]", "XOffset", m_fXOffset);
	m_pParser->GetFloatValue("[settings]", "YOffset", m_fYOffset);

	int Width = 800 / m_iNumberOfThreads;
	NumberOfSegments = int(pow(800/Width, 2));
	SegmentsCompleted = 0;
	m_iSelectedThread = 0;

}

/***********************
* Destructor: destroys the thread pool, deallocated memory
* @author: Joshua Tanner
* @parameter: none
* @return:
********************/
ThreadPool::~ThreadPool()
{	
	for (int i = 0; i< m_iNumberOfThreads; i++)
	{
		delete m_threadBackBuffers[i];
		m_threadBackBuffers[i] = 0;
	}

	delete m_pMainBackBuffer;
	m_pMainBackBuffer = 0;

	delete m_pParser;
	m_pParser = 0;

	delete m_pWorkQueue;
	m_pWorkQueue = 0;
}

/***********************
* Draw: bitblts all backbuffers together
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void ThreadPool::Draw()
{
	//Clear the main backbuffer
	m_pMainBackBuffer->Clear();
	if (m_iSelectedThread == 0)
	{
		for (unsigned int i = 0; i < m_threadBackBuffers.size(); i++)
		{
			//Bitblt all backbuffers to main BFDC
			BitBlt(m_pMainBackBuffer->GetBFDC(), 0, 0, 800, 800, m_threadBackBuffers[i]->GetBFDC(), 0, 0, SRCPAINT);
		}
		std::wstring wsOutputString = L"All threads active";//Concatenating
		TextOut(m_pMainBackBuffer->GetBFDC(), 10, 30, wsOutputString.c_str(), wsOutputString.length());//Output to BFDC
	}
	else
	{
		//Bitblt a specific backbuffer to main BFDC
		BitBlt(m_pMainBackBuffer->GetBFDC(), 0, 0, 800, 800, m_threadBackBuffers[m_iSelectedThread-1]->GetBFDC(), 0, 0, SRCPAINT);
		std::wstring wsOutputString = L"Active Thread: " + std::to_wstring(m_iSelectedThread);//Concatenating
		TextOut(m_pMainBackBuffer->GetBFDC(), 10, 30, wsOutputString.c_str(), wsOutputString.length());//Output to BFDC
	}
	
	//Time to compute is draw to BFDC
	DrawTime();	
	m_pMainBackBuffer->Present();
}

/***********************
* Initialise: create workqueue, back buffers and start timer
* @author: Joshua Tanner
* @parameter: handle to window, width and height of window
* @return: void
********************/
void ThreadPool::Initialize(HWND hWindow, int Width, int Height)
{
	//Create a new Work Queue of Mandelbrot Segments
	m_pWorkQueue = new WorkQueue<MandelbrotSegment>();

	//Initialising the main backbuffer
	m_pMainBackBuffer = new CBackBuffer();	
	m_pMainBackBuffer->Initialise(hWindow, Width, Height);		

	for (int i = 0; i< m_iNumberOfThreads; i++)
	{
		//For each thread, create an associated back buffer
		CBackBuffer* threadBackBuffer = new CBackBuffer();
		threadBackBuffer->Initialise(hWindow, Width, Height);
		m_threadBackBuffers.push_back(threadBackBuffer);
		m_workerThreads.push_back(std::thread (&ThreadPool::DoWork, this, std::ref(*threadBackBuffer)));		
	}

	//Start time
	StartTime = clock();
}

/***********************
* Set the end time: create workqueue, back buffers and start timer
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void ThreadPool::SetEndTime()
{
	EndTime = clock();
}

/***********************
* Submit: add item to work queue
* @author: Joshua Tanner
* @parameter: Mandelbrot Segment
* @return: void
********************/
void ThreadPool::Submit(MandelbrotSegment _fItem)
{
	m_pWorkQueue->push(_fItem);
}

/***********************
* DrawTime: output the time to calculate
* @author: Joshua Tanner
* @parameter: none
* @return: void
********************/
void ThreadPool::DrawTime()
{
	double ProcessTime = double((EndTime - StartTime)/(CLOCKS_PER_SEC));//find elapsed time
	std::wstring wsOutputString = L"Time to calculate: " + std::to_wstring(ProcessTime);//Concatenating
	TextOut(m_pMainBackBuffer->GetBFDC(), 10, 10, wsOutputString.c_str(), wsOutputString.length());//Output to BFDC
}

/***********************
* DoWork: threads take off segments to draw
* @author: Joshua Tanner
* @parameter: reference to thread's backbuffer
* @return: void
********************/
void ThreadPool::DoWork(CBackBuffer& threadBackbuffer)
{
	//Continue until there is nothing in work queue
	while (!m_bFinished)
	{
		//The task
		MandelbrotSegment _WorkItem;
		//Pop off from work queue
		if (m_pWorkQueue->pop(_WorkItem))
		{			
			//Do the work 
			_WorkItem(threadBackbuffer, m_fZoomFactor, m_fXOffset, m_fYOffset);
			MUTEX.lock();
			SegmentsCompleted++;//Lock mutex so segment can be incremented by multiple threads
			if (SegmentsCompleted == NumberOfSegments){
				m_bFinished = true;				
			}
			MUTEX.unlock();			
		}
		
	}
}

