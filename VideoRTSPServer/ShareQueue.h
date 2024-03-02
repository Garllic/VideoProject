#pragma once
#include <queue>
#include <mutex>

template<typename T>
class ShareQueue
{
public:
	ShareQueue() {}
	~ShareQueue() {}
	void Push(T t) {
		m_mutex.lock();
		m_queue.push(t);
		m_mutex.unlock();
	}
	T Front() {
		T t;
		if (!m_queue.empty()) {
			t = m_queue.front();
		}
		return t;
	}
	void Pop() {
		m_mutex.lock();
		if (!m_queue.empty()) {
			m_queue.pop();
		}
		m_mutex.unlock();
	}
	T GetFront() {
		T t;
		t = Front();
		Pop();
		return t;
	}
	int Size() {
		return m_queue.size();
	}
private:
	std::queue<T> m_queue;
	std::mutex m_mutex;
};

