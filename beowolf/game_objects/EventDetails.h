#ifndef EVENTDETAILS
#define EVENTDETAILS

#include "GameObject.h"
#include <map>

class EventDetails
{
public:
	EventDetails(std::string id);
	~EventDetails();

	void AddInt(std::string id, int val);
	void AddFloat(std::string id, float val);
	void AddObj(std::string id, Common::GameObject* val);
	void AddStr(std::string id, std::string val);

	int GetInt(std::string id);
	float GetFloat(std::string id);
	Common::GameObject* GetObj(std::string id);
	std::string GetStr(std::string id);

	double time;
	std::string id;

private:
	std::map<std::string, int> mapInt;
	std::map<std::string, float> mapFloat;
	std::map<std::string, Common::GameObject*> mapObj;
	std::map<std::string, std::string> mapStr;
};

#endif