#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool
{
public:
	ThreadPool(const ThreadPool& tp) = delete;
	ThreadPool& operator=(const ThreadPool& tp) = delete;

	static ThreadPool* Instance();

	void Idle();
	void QueueTask(const std::function<void(void)>& task);
	void WaitFinished();

private:
	ThreadPool();
	~ThreadPool();

	std::vector<std::thread> m_Threads;
	std::queue<std::function<void(void)>> m_Tasks;

	std::condition_variable m_FinishedCondition;
	std::condition_variable m_QueueCondition;
	std::mutex m_QueueMutex;

	unsigned int m_ThreadCount = 0;
	unsigned int m_BusyThreads = 0;
	
	bool m_Running = false;
};
