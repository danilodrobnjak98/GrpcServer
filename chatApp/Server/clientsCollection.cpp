#include "clientsCollection.h"

std::string CCollection::findFirst(std::string second)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	for (auto& iter : m_vector)
	{
		if (iter.second == second)
		{
			return iter.first;
		}
	}

	return std::string("");
}

std::string CCollection::findSecond(std::string first)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	for (auto& iter : m_vector)
	{
		if (iter.first == first)
		{
			return iter.second;
		}
	}

	return std::string("");
}

void CCollection::pushPair(std::string first, std::string second)
{
	std::unique_lock <std::mutex> lock(m_mutex);
	auto pair = std::make_pair(first, second);
	m_vector.push_back(pair);
	m_cv.notify_one();
}

void CCollection::popFirst()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock, [this]() {return !m_vector.empty();});
	m_vector.erase(m_vector.begin());
}