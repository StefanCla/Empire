#include "Headers/Threading/ThreadPool.h"

ThreadPool* ThreadPool::Instance()
{
	static ThreadPool* s_Instance = new ThreadPool();
	return s_Instance;
}

ThreadPool::ThreadPool()
{
	m_ThreadCount = std::thread::hardware_concurrency();
	
	for (unsigned int i = 0; i < m_ThreadCount; i++)
		m_Threads.push_back(std::thread(&ThreadPool::Idle, this));

	m_Running = true;
}

ThreadPool::~ThreadPool()
{
	m_Running = false;
	m_QueueCondition.notify_all();

	for (std::thread& thread : m_Threads)
		if (thread.joinable())
			thread.join();
}

void ThreadPool::Idle()
{
	std::function<void()> task;
	
	while (m_Running)
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_QueueCondition.wait(lock, [this]() { return !m_Tasks.size() == 0 || !m_Running; });

		m_BusyThreads++;
		task = m_Tasks.front();
		m_Tasks.pop();

		lock.unlock();
		task();
		lock.lock();

		m_BusyThreads--;
		m_FinishedCondition.notify_one();
	}
}

void ThreadPool::QueueTask(const std::function<void(void)>& task)
{
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_Tasks.emplace(task);
	}
	m_QueueCondition.notify_one();
}

void ThreadPool::WaitFinished()
{
	std::unique_lock<std::mutex> lock(m_QueueMutex);
	m_FinishedCondition.wait(lock, [this]() { return m_Tasks.size() == 0 && m_BusyThreads == 0; });
}
