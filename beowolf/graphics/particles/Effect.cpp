#include "Effect.h"
#include "AffectorScale.h"
#include "AffectorRecolor.h"
#include "AffectorFade.h"
#include "AffectorVelocity.h"
#include "AffectorRandomPos.h"

#include "tinyxml.h"
#include <sstream>

Effect::Effect()
{
	Emitter* emitter = new Emitter(1, -1, 0.1f, "../resources/textures/particles/particle.dds");

	m_emitters.push_back(emitter);
}

Effect::Effect(std::string xmlPath)
{
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
}

Effect::~Effect()
{
	for (int i = 0; i < m_emitters.size(); i++)
	{
		delete m_emitters[i];
	}
}

void Effect::Update(float delta, glm::mat3 view)
{
	for (int i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Update(delta, view);
	}
}

void Effect::Render(glm::mat4 projview)
{
	for (int i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Render(projview);
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