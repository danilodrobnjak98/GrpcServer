#pragma once

#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>

class CCollection
{
public:
	std::string findFirst(std::string second);
	std::string findSecond(std::string first);
	void pushPair(std::string first, std::string second);
	void popFirst();
private:
	std::vector<std::pair<std::string, std::string>> m_vector;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};