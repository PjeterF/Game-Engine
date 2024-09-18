#include "ThreadPool.hpp"

ThreadPool::ThreadPool(int nThreads)
{
	for (int i = 0; i < nThreads; i++)
	{
		threads.push_back(std::thread([this]() {
			while (1)
			{
				std::function<void()> task;
				int id;

				{
					std::unique_lock<std::mutex> lock(tasksMutex);

					taskCondition.wait(lock, [this]() {
						return !tasks.empty() || stopThreads;
						});

					if (tasks.empty() && stopThreads)
						return;

					task = std::move(tasks.front().func);
					id = tasks.front().id;

					tasks.pop();
				}

				task();

				{
					std::unique_lock<std::mutex> lock(tasksMutex);
					taskIDs.erase(id);
				}
			}
			}));
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(tasksMutex);
		stopThreads = true;
	}

	taskCondition.notify_all();

	for (auto& thread : threads)
		thread.join();
}

int ThreadPool::addTask(std::function<void()> task)
{
	int result;
	{
		std::unique_lock<std::mutex> lock(tasksMutex);
		tasks.push(Task(task, nextTaskID));
		taskIDs.insert(nextTaskID);
		result = nextTaskID++;
	}
	taskCondition.notify_one();

	return result;
}
