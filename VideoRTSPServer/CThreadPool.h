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
		int m_id;				//����id
		CThreadPool* m_pool;	//�����̳߳�
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
					m_pool->m_cond_lock->wait(lock);	//�ȴ���������֪ͨ
					if (m_pool->m_shutdown) {
						lock.unlock();
						break;
					}
				}
				task = m_pool->m_taskQ->GetFront();	//ȡ����������е�����
				lock.unlock();
				if (task.func != nullptr) {
					ATLTRACE("�߳�%d��ʼ����\r\n", m_id);
					task.func(task.args);
					ATLTRACE("�߳�%d��������\r\n", m_id);
				}
			}
			std::cout <<m_id<< " exit" << std::endl;
		}
	};
	bool m_shutdown;		//�߳��Ƿ�ر�
	ShareQueue<Task>* m_taskQ;		//�������
	std::vector<std::thread*> m_threads;	//�����̶߳���
	std::mutex* m_cond_mutex;		//�߳��������������
	std::condition_variable* m_cond_lock; //�̻߳��������������̴߳������߻��߻���
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
			m_threads[i] = new std::thread(ThreadWorker(this, i));	//���乤���߳�
		}
	}

	void Shutdown() {
		m_shutdown = true;
		m_cond_lock->notify_all();	//�������й����߳�

		for (int i = 0; i < m_threads.size(); i++) {
			if (m_threads[i]->joinable()) {	//�ж��߳��Ƿ��ڵȴ�����
				m_threads[i]->join();		//���������н�����������ȴ�����
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
		m_taskQ->Push(task);		//���һ������
		m_cond_lock->notify_one();	//����һ���ȴ�����
		lock.unlock();
	}

	size_t GetThreadNum() {
		return m_threads.size();
	}
};

