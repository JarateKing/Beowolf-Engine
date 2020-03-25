#include "Effect.h"
#include "AffectorScale.h"
#include "AffectorRecolor.h"
#include "AffectorFade.h"
#include "AffectorVelocity.h"
#include "AffectorRandomPos.h"
#include "AffectorBounce.h"

#include "tinyxml.h"
#include "JSON/json.hpp"
#include <sstream>
#include <fstream>
#include "W_ResourceLoader.h"

using json = nlohmann::json;

Effect::Effect()
{
	Emitter* emitter = new Emitter(1, -1, 0.1f, wolf::ResourceLoader::Instance().getTexture("particles/particle.dds"));

	m_emitters.push_back(emitter);
}

Effect::Effect(std::string jsonPath)
{
	std::ifstream file(jsonPath);
	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	json j = json::parse(content);

	auto& gameObjects = j["Effect"];
	for (auto& gameObject : gameObjects["Emitters"])
	{
		// emitter setup
		int max = 0;
		glm::vec3 pos;
		float duration = -1;
		float rate = 0.1;
		std::string texture = wolf::ResourceLoader::Instance().getTexture("particles/particle.dds");
		bool additive = false;
		float lifespanMin = 1.0f;
		float lifespanMax = 1.0f;

		std::string maxt = gameObject["max_particles"];
		std::string xt = gameObject["pX"];
		std::string yt = gameObject["pY"];
		std::string zt = gameObject["pZ"];
		std::string durationt = gameObject["duration"];
		std::string ratet = gameObject["rate"];
		std::string texturet = gameObject["texture"];
		std::string additivet = gameObject["additive"];
		std::string lifespanMint = gameObject["lifespanMin"];
		std::string lifespanMaxt = gameObject["lifespanMax"];

		int frames = 1;
		int framerate = 60;
		if (gameObject.contains("animationFrames"))
			frames = gameObject["animationFrames"];
		if (gameObject.contains("animationRate"))
			framerate = gameObject["animationRate"];

		max = std::stoi(maxt);
		pos = glm::vec3(std::stof(xt), std::stof(yt), std::stof(zt));
		duration = std::stof(durationt);
		rate = std::stof(ratet);
		texture = texturet;
		if (additivet == "true")
			additive = true;
		lifespanMin = std::stof(lifespanMint);
		lifespanMax = std::stof(lifespanMaxt);

		Emitter* emitter = new Emitter(max, duration, rate, texture);
		emitter->Translate(pos);
		emitter->SetAdditive(additive);
		emitter->SetLifespan(lifespanMin, lifespanMax);
		emitter->SetAnimatedTexture(frames, framerate);

		auto& affectors = gameObject["affectors"];
		for (auto& affector : affectors)
		{
			if (affector["type"] == "scale")
			{
				float start;
				float end;

				std::string startt = affector["start"];
				start = std::stof(startt);

				std::string endt = affector["end"];
				end = std::stof(endt);

				emitter->AddAffector(new AffectorScale(start, end));
			}
			else if (affector["type"] == "recolor")
			{
				glm::vec3 start;
				glm::vec3 end;

				std::string sRt = affector["sR"];
				std::string sGt = affector["sG"];
				std::string sBt = affector["sB"];
				std::string eRt = affector["eR"];
				std::string eGt = affector["eG"];
				std::string eBt = affector["eB"];

				start = glm::vec3(std::stof(sRt), std::stof(sGt), std::stof(sBt));
				end = glm::vec3(std::stof(eRt), std::stof(eGt), std::stof(eBt));

				emitter->AddAffector(new AffectorRecolor(start, end));
			}
			else if (affector["type"] == "fade")
			{
				float start;
				float end;

				std::string startt = affector["start"];
				start = std::stof(startt);

				std::string endt = affector["end"];
				end = std::stof(endt);

				emitter->AddAffector(new AffectorFade(start, end));
			}
			else if (affector["type"] == "velocity")
			{
				glm::vec3 start;
				glm::vec3 end;

				std::string sXt = affector["sX"];
				std::string sYt = affector["sY"];
				std::string sZt = affector["sZ"];
				std::string eXt = affector["eX"];
				std::string eYt = affector["eY"];
				std::string eZt = affector["eZ"];

				start = glm::vec3(std::stof(sXt), std::stof(sYt), std::stof(sZt));
				end = glm::vec3(std::stof(eXt), std::stof(eYt), std::stof(eZt));

				emitter->AddAffector(new AffectorVelocity(start, end));
			}
			else if (affector["type"] == "randomPos")
			{
				float radius;

				std::string radiust = affector["radius"];
				radius = std::stof(radiust);

				emitter->AddAffector(new AffectorRandomPos(radius));
			}
			else if (affector["type"] == "bounce")
			{
				float magnitude = affector["magnitude"];
				float period = affector["period"];

				emitter->AddAffector(new AffectorBounce(magnitude, period));
			}
		}

		m_emitters.push_back(emitter);
	}
	/*
	// verify things exist
	TiXmlDocument doc(xmlPath.c_str());
	if (doc.LoadFile() == false)
	{
		Effect();
		return;
	}
	TiXmlNode* pNode = doc.FirstChild("effect");
	if (pNode == NULL)
	{
		Effect();
		return;
	}
	
	// loop through each emitter
	TiXmlNode* pComponentNode = pNode->FirstChild();
	while (pComponentNode != NULL)
	{
		// emitter setup
		int max = 0;
		glm::vec3 pos;
		int duration = -1;
		float rate = 0.1;
		std::string texture = "../resources/textures/particles/particle.dds";
		bool additive = false;
		float lifespanMin = 1.0f;
		float lifespanMax = 1.0f;

		TiXmlElement* emitterElement = pComponentNode->ToElement();

		emitterElement->QueryIntAttribute("max_particles", &max);
		emitterElement->QueryIntAttribute("duration", &duration);
		emitterElement->QueryFloatAttribute("rate", &rate);
		emitterElement->QueryFloatAttribute("pX", &pos.x);
		emitterElement->QueryFloatAttribute("pY", &pos.y);
		emitterElement->QueryFloatAttribute("pZ", &pos.z);
		texture = emitterElement->Attribute("texture");
		emitterElement->QueryBoolAttribute("additive", &additive);
		emitterElement->QueryFloatAttribute("lifespanMin", &lifespanMin);
		emitterElement->QueryFloatAttribute("lifespanMax", &lifespanMax);

		Emitter* emitter = new Emitter(max, duration, rate, texture);
		emitter->Translate(pos);
		emitter->SetAdditive(additive);
		emitter->SetLifespan(lifespanMin, lifespanMax);
		

		// affectors setup
		for (TiXmlNode* i = pComponentNode->FirstChild(); i != NULL; i = i->NextSibling())
		{
			const char* componentName = i->Value();
			TiXmlElement* element = i->ToElement();

			if (std::strcmp(componentName, "scale") == 0)
			{
				float start;
				float end;

				element->QueryFloatAttribute("start", &start);
				element->QueryFloatAttribute("end", &end);

				emitter->AddAffector(new AffectorScale(start, end));
			}
			else if (std::strcmp(componentName, "recolor") == 0)
			{
				glm::vec3 start;
				glm::vec3 end;

				element->QueryFloatAttribute("sR", &start.x);
				element->QueryFloatAttribute("sG", &start.y);
				element->QueryFloatAttribute("sB", &start.z);
				element->QueryFloatAttribute("eR", &end.x);
				element->QueryFloatAttribute("eG", &end.y);
				element->QueryFloatAttribute("eB", &end.z);

				emitter->AddAffector(new AffectorRecolor(start, end));
			}
			else if (std::strcmp(componentName, "fade") == 0)
			{
				float start;
				float end;

				element->QueryFloatAttribute("start", &start);
				element->QueryFloatAttribute("end", &end);

				emitter->AddAffector(new AffectorFade(start, end));
			}
			else if (std::strcmp(componentName, "velocity") == 0)
			{
				glm::vec3 start;
				glm::vec3 end;

				element->QueryFloatAttribute("sX", &start.x);
				element->QueryFloatAttribute("sY", &start.y);
				element->QueryFloatAttribute("sZ", &start.z);
				element->QueryFloatAttribute("eX", &end.x);
				element->QueryFloatAttribute("eY", &end.y);
				element->QueryFloatAttribute("eZ", &end.z);

				emitter->AddAffector(new AffectorVelocity(start, end));
			}
			else if (std::strcmp(componentName, "randomPos") == 0)
			{
				float radius;

				element->QueryFloatAttribute("radius", &radius);

				emitter->AddAffector(new AffectorRandomPos(radius));
			}
		}

		// add final emitter to this effect
		m_emitters.push_back(emitter);

		// get next emitter
		pComponentNode = pComponentNode->NextSibling();
	}
	*/
}

Effect::~Effect()
{
	for (int i = 0; i < m_emitters.size(); i++)
	{
		delete m_emitters[i];
	}
	m_emitters.clear();
}

void Effect::Update(float delta, glm::mat3 view)
{
	for (int i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Update(delta, view);
	}
}

void Effect::Render(glm::mat4 projview, wolf::RenderFilterType type)
{
	for (int i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Render(projview, type);
	}
}

glm::vec3 Effect::GetPos()
{
	return m_pos;
}

void Effect::SetPos(glm::vec3 pos)
{
	glm::vec3 difference = pos - m_pos;
	m_pos = pos;

	for (int i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Translate(difference);
	}
}