#pragma once
#ifndef UTIL
#define UTIL
#include <string>
#include <ctime>
class Util
{
public :
	std::string utilLocalTimeStr();
	time_t utilTimeStr2LocalTime(char* str);
	std::string utilRandomString(const unsigned int len);
	float calculateDistance(int x1, int y1, int x2, int y2);
	std::time_t getTimeStamp();
};


#endif
