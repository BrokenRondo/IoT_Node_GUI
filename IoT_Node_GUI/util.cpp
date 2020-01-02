#include "util.h"
#include <iomanip>
#include <sstream>
#include <chrono>
#include <cmath>
std::string Util::utilRandomString(const unsigned len)
{
	int i;
	char* str = new char[len + 1];
	for (i = 0; i < len; ++i)
	{
		switch ((rand() % 3))
		{
		case 1:
			str[i] = 'A' + rand() % 26;
			break;
		case 2:
			str[i] = 'a' + rand() % 26;
			break;
		default:
			str[i] = '0' + rand() % 10;
			break;
		}
	}
	str[i] = '\0';
	return std::string(str);
}
std::string Util::utilLocalTimeStr()
{
	//auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 	//convert to string
	//std::stringstream ss;
	////show as different types
	//ss << std::put_time(std::localtime(&t), "%Y-%m-%d-%H-%M-%S");
	////ss << std::put_time(std::localtime(&t), "%Y年%m月%d日%H时%M分%S秒");
	////ss << std::put_time(std::localtime(&t), "%Y%m%d%H%M%S");
	//std::string str_time = ss.str();
	time_t timer = time(NULL);
	char szBuf[30]={0};
	strftime(szBuf, sizeof(szBuf), "%Y-%m-%d %H:%M:%S", localtime(&timer));
	std::string str_time = szBuf;
	return str_time;
}
time_t Util::utilTimeStr2LocalTime(char* str)
{
	tm tm_;
	int year, month, day, hour, minute, second;
	sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
    time_t t_ = mktime(&tm_); //已经减了8个时区  
	 return t_; //秒时间  
}

float Util::calculateDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
std::time_t Util::getTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	std::time_t timestamp = tmp.count();
	//std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);
	return timestamp;

}


