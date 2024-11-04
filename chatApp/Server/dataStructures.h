#pragma once
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <map>
#include <vector>
#include <algorithm>

namespace DataStructures
{
	template <class T>
	class DQueue
	{
	public:
		void push(T item)
		{
			std::unique_lock <std::mutex> lock(m_mutex);
			m_queue.push(item);
			m_cv.notify_one();
		}

		T pop()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this]() {return !m_queue.empty();});
			T elem = m_queue.front();
			m_queue.pop();

			return elem;
		}

		std::vector<T> getAll()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			std::queue<T> tempQueue = m_queue;
			std::vector<T> elements;
			while (!tempQueue.empty())
			{
				elements.push_back(tempQueue.front());
				tempQueue.pop();
			}
			return elements;
		}
		int getSize()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_queue.size();
		}
	private:
		std::queue<T> m_queue;
		std::mutex m_mutex;
		std::condition_variable m_cv;
	};

	template <class T>
	class DMap
	{
	public:
		void insertItem(std::string key, T t)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_map.insert({ key, t });
			m_cv.notify_one();
		}

		T getItem(std::string key)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this]() {
				return !m_map.empty();
				});
			auto iter = m_map.find(key);
			if (iter != m_map.end())
			{
				return iter->second;
			}
			return nullptr;
		}

		std::vector<std::pair<std::string, T>> getAllItems()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			std::vector<std::pair<std::string, T>> items(m_map.begin(), m_map.end());
			return items;
		}

		int getSize()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return m_map.size();
		}

		T getValue(std::string key)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			auto iter = m_map.find(key);
			if (iter != m_map.end())
			{
				return iter->second;
			}
			return nullptr;
		}
		void printMapByKey()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			for (auto& iter : m_map)
			{
				std::cout << iter.first << " ";
			}

			std::cout << std::endl;
		}
	private:
		std::map<std::string, T> m_map;
		std::mutex m_mutex;
		std::condition_variable m_cv;
	};
}