#include "EventDetails.h"

EventDetails::EventDetails(std::string identifier)
{
	time = 0.0;
	id = identifier;
}

EventDetails::~EventDetails()
{

}

void EventDetails::AddInt(std::string id, int val)
{
	mapInt[id] = val;
}
void EventDetails::AddFloat(std::string id, float val)
{
	mapFloat[id] = val;
}
void EventDetails::AddObj(std::string id, Common::GameObject* val)
{
	mapObj[id] = val;
}
void EventDetails::AddStr(std::string id, std::string val)
{
	mapStr[id] = val;
}

int EventDetails::GetInt(std::string id)
{
	return mapInt[id];
}
float EventDetails::GetFloat(std::string id)
{
	return mapFloat[id];
}
Common::GameObject* EventDetails::GetObj(std::string id)
{
	return mapObj[id];
}
std::string EventDetails::GetStr(std::string id)
{
	return mapStr[id];
}