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
				std::unique_lock<std::mutex> lock(m_pool->m_cond_mutex);
				if (m_pool->m_taskQ->GetTaskNum() == 0) {
					m_pool->m_cond_lock.wait(lock);	//�ȴ���������֪ͨ
				}
				task = m_pool->m_taskQ->GetTask();	//ȡ����������е�����
				lock.unlock();
				if (task.func != nullptr) {
					task.func(task.args);
				}
			}
		}
	};
	bool m_shutdown;		//�߳��Ƿ�ر�
	TaskQueue* m_taskQ;		//�������
	std::vector<std::thread> m_threads;	//�����̶߳���
	std::mutex m_cond_mutex;		//�߳��������������
	std::condition_variable m_cond_lock; //�̻߳��������������̴߳������߻��߻���
public:
	CThreadPool(const int n_threads = 4) :m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false)
	{
		m_taskQ = new TaskQueue();
	}

	void Init() {
		for (int i = 0; i < m_threads.size(); i++) {
			m_threads[i] = std::thread(ThreadWorker(this, i));	//���乤���߳�
		}
	}

	void Shutdown() {
		m_shutdown = true;
		m_cond_lock.notify_all();	//�������й����߳�

		for (int i = 0; i < m_threads.size(); i++) {
			if (m_threads[i].joinable()) {	//�ж��߳��Ƿ��ڵȴ�����
				m_threads[i].join();		//���������н�����������ȴ�����
			}
		}
	}

	void SubmitTask(Task task) {
		std::unique_lock<std::mutex> lock(m_cond_mutex);
		if (m_shutdown) {
			lock.unlock();
			return;
		}
		m_taskQ->AddTask(task);		//���һ������
		m_cond_lock.notify_one();	//����һ���ȴ�����
		lock.unlock();
	}

	size_t GetThreadNum() {
		return m_threads.size();
	}
};

