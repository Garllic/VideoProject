#pragma once
#include <thread>
#include <mutex>
#include "ShareQueue.h"
#include <atltrace.h>

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
	~Task() {
		func = nullptr;
		args = nullptr;
	}
	callback func = nullptr;
	void* args = nullptr;
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
				std::unique_lock<std::mutex> lock(*m_pool->m_cond_mutex);
				if (m_pool->m_taskQ->Size() == 0) {
					m_pool->m_cond_lock->wait(lock);	//等待条件变量通知
					if (m_pool->m_shutdown) {
						lock.unlock();
						break;
					}
				}
				task = m_pool->m_taskQ->GetFront();	//取出任务队列中的任务
				lock.unlock();
				if (task.func != nullptr) {
					ATLTRACE("线程%d开始工作\r\n", m_id);
					task.func(task.args);
					ATLTRACE("线程%d结束工作\r\n", m_id);
				}
			}
			std::cout <<m_id<< " exit" << std::endl;
		}
	};
	bool m_shutdown;		//线程是否关闭
	ShareQueue<Task>* m_taskQ;		//任务队列
	std::vector<std::thread*> m_threads;	//工作线程队列
	std::mutex* m_cond_mutex;		//线程休眠锁互斥变量
	std::condition_variable* m_cond_lock; //线程环境锁，可以让线程处于休眠或者唤醒
public:
	CThreadPool& operator=(const CThreadPool& pool) {
		if (this != &pool) {
			m_shutdown = pool.m_shutdown;
			m_taskQ = pool.m_taskQ;
			m_threads = pool.m_threads;
			m_cond_mutex = pool.m_cond_mutex;
			m_cond_lock = pool.m_cond_lock;
		}
		return *this;
	}
	CThreadPool(const int n_threads = 8) :m_threads(std::vector<std::thread*>(n_threads)), m_shutdown(false)
	{
		ATLTRACE("create pool\r\n");
		m_taskQ = new ShareQueue<Task>();
		m_cond_mutex = new std::mutex();
		m_cond_lock = new std::condition_variable();
	}

	void Init() {
		for (int i = 0; i < m_threads.size(); i++) {
			m_threads[i] = new std::thread(ThreadWorker(this, i));	//分配工作线程
		}
	}

	void Shutdown() {
		m_shutdown = true;
		m_cond_lock->notify_all();	//唤醒所有工作线程

		for (int i = 0; i < m_threads.size(); i++) {
			if (m_threads[i]->joinable()) {	//判断线程是否在等待队列
				m_threads[i]->join();		//将工作队列结束任务后加入等待队列
			}
			delete m_threads[i];
		}
		delete m_taskQ;
		delete m_cond_mutex;
		delete m_cond_lock;
	}

	void SubmitTask(Task task) {
		std::unique_lock<std::mutex> lock(*m_cond_mutex);
		if (m_shutdown) {
			lock.unlock();
			return;
		}
		m_taskQ->Push(task);		//添加一个任务
		m_cond_lock->notify_one();	//唤醒一个等待队列
		lock.unlock();
	}

	size_t GetThreadNum() {
		return m_threads.size();
	}
};

