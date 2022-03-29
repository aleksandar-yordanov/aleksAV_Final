#pragma once
#include "framework.h"
template<class T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() = default;
	explicit ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
	~ThreadSafeQueue()
	{
		clear();
	}

	const T& peekFront()
	{
		std::scoped_lock lock(this->queueLock);
		return this->mainDeque.front();
	}

	const T& peekBack()
	{
		std::scoped_lock lock(this->queueLock);
		return this->mainQueue.back();
	}

	T popFront()
	{
		std::scoped_lock lock(this->queueLock);
		auto t = std::move(this->mainDeque.front());
		this->mainDeque.pop_front();
		return t;
	}

	T popBack()
	{
		std::scoped_lock lock(this->queueLock);
		auto t = std::move(this->mainDeque.back());
		this->mainDeque.pop_back();
		return t;
	}

	void pushBack(const T& obj)
	{
		std::scoped_lock lock(this->queueLock);
		this->mainDeque.push_back(std::move(obj));
		std::unique_lock<std::mutex> block(this->mutexBlocker);
		this->condBlock.notify_one();
	}

	void pushFront(const T& obj)
	{
		std::scoped_lock lock(this->queueLock);
		this->mainDeque.push_front(std::move(obj));
		std::unique_lock<std::mutex> block(this->mutexBlocker);
		this->condBlock.notify_one();
	}

	bool checkEmpty()
	{
		std::scoped_lock lock(this->queueLock);
		return this->mainQueue.empty();
	}

	int size()
	{
		std::scoped_lock lock(this->queueLock);
		return this->mainQueue.size();
	}

	void waitTilFree()
	{
		while (empty())
		{
			std::unique_lock<std::mutex> block(this->mutexBlocker);
			this->condBlock.wait(block);
		}
	}


private:
	std::deque<T> mainDeque;
	std::mutex queueLock;
	std::mutex mutexBlocker;
	std::condition_variable_any condBlock;
};