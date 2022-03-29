#pragma once
#include "includes.h"
template<class T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() = default;
	ThreadSafeQueue(const ThreadSafeQueue<T>& ref) = delete;
	virtual ~ThreadSafeQueue()
	{
		clear();
	}

	const T& peekFront()
	{
		std::scoped_lock lock(queueLock);
		return mainDeque.front();
	}

	const T& peekBack()
	{
		std::scoped_lock lock(queueLock);
		return mainDeque.back();
	}

	T popFront()
	{
		std::scoped_lock lock(queueLock);
		auto t = std::move(mainDeque.front());
		mainDeque.pop_front();
		return t;
	}

	T popBack()
	{
		std::scoped_lock lock(queueLock);
		auto t = std::move(mainDeque.back());
		mainDeque.pop_back();
		return t;
	}

	void pushBack(const T& obj)
	{
		std::scoped_lock lock(queueLock);
		mainDeque.push_back(std::move(obj));
		std::unique_lock block(mutexBlocker);
		condBlock.notify_one();
	}

	void pushFront(const T& obj)
	{
		std::scoped_lock lock(queueLock);
		mainDeque.push_front(std::move(obj));
		std::unique_lock block(mutexBlocker);
		condBlock.notify_one();
	}

	bool checkEmpty()
	{
		std::scoped_lock lock(queueLock);
		return mainDeque.empty();
	}

	void clear()
	{
		std::scoped_lock lock(queueLock);
		mainDeque.clear();
	}

	int size()
	{
		std::scoped_lock lock(queueLock);
		return mainDeque.size();
	}

	void waitTilFree()
	{
		while (checkEmpty())
		{
			std::unique_lock block(mutexBlocker);
			condBlock.wait(block);
		}
	}

protected:
	std::deque<T> mainDeque;
	std::mutex queueLock;
	std::mutex mutexBlocker;
	std::condition_variable condBlock;
};
