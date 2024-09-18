#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <queue>
#include <unordered_set>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool(int nThreads);
	~ThreadPool();
	int addTask(std::function<void()> task);
private:
	int nextTaskID = 0;

	struct Task
	{
		Task(std::function<void()> func, int id) : func(func), id(id) {}
		Task() {}
		std::function<void()> func;
		int id;
	};

	std::vector<std::thread> threads;
	std::mutex tasksMutex;
	std::condition_variable taskCondition;
	std::queue<Task> tasks;
	std::unordered_set<int> taskIDs;

	bool stopThreads = false;
};