#pragma once
#include <thread>
#include <queue>
#include <mutex>

using callback = void (*)(void* arg);

struct Task
{
	Task() {
		func = nullptr;
		args = nullptr;
	}
	Task(callback f, void* a) {
		func = f;
		args = a;
	}
	callback func = nullptr;
	void* args = nullptr;
};

class TaskQueue
{
public:
	TaskQueue() {}
	~TaskQueue() {}
	void AddTask(Task t) {
		m_mutex.lock();
		m_taskQ.push(t);
		m_mutex.unlock();
	}
	Task GetTask() {
		Task t;
		m_mutex.lock();
		if (!m_taskQ.empty()) {
			t = m_taskQ.front();
			m_taskQ.pop();
		}
		m_mutex.unlock();
		return t;
	}
	int GetTaskNum() {
		return m_taskQ.size();
	}
private:
	std::queue<Task> m_taskQ;
	std::mutex m_mutex;
};

class CThreadPool
{
private:
	class ThreadWorker
	{
	private:
		int m_id;				//工作id
		CThreadPool* m_pool;	//所属线程池
	public:
		ThreadWorker(CThreadPool* pool, const int id) :m_pool(pool), m_id(id)
		{
		}
		void operator()()
		{
			Task task;
			while (!m_pool->m_shutdown) {
				std::unique_lock<std::mutex> lock(m_pool->m_cond_mutex);
				if (m_pool->m_taskQ->GetTaskNum() == 0) {
					m_pool->m_cond_lock.wait(lock);	//等待条件变量通知
				}
				task = m_pool->m_taskQ->GetTask();	//取出任务队列中的任务
				lock.unlock();
				if (task.func != nullptr) {
					task.func(task.args);
				}
			}
		}
	};
	bool m_shutdown;		//线程是否关闭
	TaskQueue* m_taskQ;		//任务队列
	std::vector<std::thread> m_threads;	//工作线程队列
	std::mutex m_cond_mutex;		//线程休眠锁互斥变量
	std::condition_variable m_cond_lock; //线程环境锁，可以让线程处于休眠或者唤醒
public:
	CThreadPool(const int n_threads = 4) :m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false)
	{
		m_taskQ = new TaskQueue();
	}

	void Init() {
		for (int i = 0; i < m_threads.size(); i++) {
			m_threads[i] = std::thread(ThreadWorker(this, i));	//分配工作线程
		}
	}

	void Shutdown() {
		m_shutdown = true;
		m_cond_lock.notify_all();	//唤醒所有工作线程

		for (int i = 0; i < m_threads.size(); i++) {
			if (m_threads[i].joinable()) {	//判断线程是否在等待队列
				m_threads[i].join();		//将工作队列结束任务后加入等待队列
			}
		}
	}

	void SubmitTask(Task task) {
		std::unique_lock<std::mutex> lock(m_cond_mutex);
		if (m_shutdown) {
			lock.unlock();
			return;
		}
		m_taskQ->AddTask(task);		//添加一个任务
		m_cond_lock.notify_one();	//唤醒一个等待队列
		lock.unlock();
	}

	size_t GetThreadNum() {
		return m_threads.size();
	}
};

