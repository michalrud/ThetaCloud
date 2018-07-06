#pragma once
#include <list>
#include <string>

struct WireT
{
	std::list<std::string> history;
	void begin(int a, int b)
	{
		history.push_back(std::string("begin(") + std::to_string(a)
			+ "," + std::to_string(b) + ");");
	}
};

extern WireT Wire;
