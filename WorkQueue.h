/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	WorkQueue.h
Description	:	WorkQueue header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once 

//includes
#include <queue>
#include <mutex>

//Templated workqueue to handle mandelbrot
template<typename T>
class WorkQueue
{
public:
	WorkQueue() {}
	~WorkQueue() {}

	
	void push(const T& item)	{
		//Add task to the queue
		std::lock_guard<std::mutex> _lock(m_WorkQMutex);
		workQueue.push(item);
	}

	bool pop(T& _workItem)	{
		//remove item from queue if available
		std::lock_guard<std::mutex> _lock(m_WorkQMutex);
		if (workQueue.empty())
		{
			return false;
		}

		//Pop front item
		_workItem = workQueue.front();
		workQueue.pop();
		return true;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> _lock(m_WorkQMutex);
		return workQueue.empty();
	}

private:

	std::queue<T> workQueue;
	mutable std::mutex m_WorkQMutex;

};
