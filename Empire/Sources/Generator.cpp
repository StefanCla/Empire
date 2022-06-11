#include "Headers/Generator.h"
#include "Headers/Random.h"
#include <stdlib.h>
#include <stdio.h>

Generator::Generator() :
	m_Seed(1),
	m_Arms(1),
	m_ArmOffset(0.0f),
	m_RotateFactor(0.0f)
{}

Generator::~Generator()
{}

void Generator::Init(std::unordered_map<std::string, std::shared_ptr<Planet>>& planets, char* seed, unsigned int arms, float armOffset, float rotateFac)
{
#ifdef WINDOWS
	sscanf_s(seed, "%i", &m_Seed); //Windows specific
#elif LINUX
	sscanf(seed, "%i", &m_Seed);
#endif
	Random::Instance()->SetSeed(m_Seed);

	m_Arms = arms;
	m_ArmOffset = armOffset;
	m_RotateFactor = rotateFac;

	Create(planets);
}

void Generator::Create(std::unordered_map<std::string, std::shared_ptr<Planet>>& planets)
{
	planets.reserve(PLANETNUMBER);

	for (int i = 0; i < PLANETNUMBER; i++)
	{
		std::shared_ptr<Planet> planet = std::make_shared<Planet>();
		planet->SetName(GetRandomName());
		NameDupeChecker(planets, planet);
		planet->SetPosition(GetRandomGalaxyPosition());
		planet->InitBullet();

		planets.insert({ planet->GetName(), planet });

		/*planets.insert(new Planet());
		planets[i]->SetName(Name());
		NameDupeChecker(planets);
		Galaxy(planets[i]);
		planets[i]->InitBullet();*/
	}

	//Create the SoI vector
	//Expensive loop, might wanna rework this
	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator firstIt;
	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator secondIt;

	for (firstIt = planets.begin(); firstIt != planets.end(); firstIt++)
	{
		std::vector<std::shared_ptr<Planet>> sphereList;

		for (secondIt = planets.begin(); secondIt != planets.end(); secondIt++)
		{
			//Avoid adding yourself to the vector
			if (firstIt != secondIt)
			{
				float distance = glm::distance(firstIt->second->GetPosition(), secondIt->second->GetPosition());
				if (firstIt->second->GetMaxSOIRadius() > distance) //Is in radius //15 = max tech level and 0.5 = multiplier
				{
					sphereList.push_back(secondIt->second);
					firstIt->second->SetSphereList(sphereList);
				}
			}
		}
	}
}

//Not my code: https://itinerantgames.tumblr.com/post/78592276402/a-2d-procedural-galaxy-with-c
glm::vec3 Generator::GetRandomGalaxyPosition()
{
	const float randomOffSetXY = 0.02f;
	const float armSeperate = 2 * PI / m_Arms;

	float distance = 0.2f + Random::Instance()->FloatZeroOne() * 1.2f; //Thansk to Wessel Frijters for mentioning the gap offset
	distance = pow(distance, 2); //pow to powf

	float angle = Random::Instance()->FloatZeroOne() * 2 * PI;
	float armOffset = Random::Instance()->FloatZeroOne() * m_ArmOffset;
	armOffset = armOffset - m_ArmOffset / 2;
	armOffset = armOffset * (1 / distance);

	float squaredArmOffset = pow(armOffset, 2); //pow to powf
	if (armOffset < 0)
		squaredArmOffset = squaredArmOffset * -1;
	armOffset = squaredArmOffset;

	float rotation = distance * m_RotateFactor;

	angle = (int)(angle / armSeperate) * armSeperate + armOffset + rotation;

	float posX = cos(angle) * distance;
	float posY = sin(angle) * distance;

	float randomOffsetX = Random::Instance()->FloatZeroOne() * randomOffSetXY;
	float randomOffsetY = Random::Instance()->FloatZeroOne() * randomOffSetXY;

	posX += randomOffsetX;
	posY += randomOffsetY;

	//Add pos to planets
	return glm::vec3(posX * 150.0f, posY * 150.0f, 0.0f);
}

std::string Generator::GetRandomName()
{
	//Names
	//https://www.dyslexia-reading-well.com/44-phonemes-in-english.html
	const char syllable[5] =
	{
		'a', 'e', 'i', 'o', 'u'
	};

	const std::string BigPhoneme[19] =
	{
		"dd", "ed", "gg", "gh", "gu",
		"ge", "di", "ll", "mm", "mb",
		"mn", "lm", "gn", "kn", "nn",
		"rr", "wr", "ve", "wh"
	};

	const char MediumPhoneme[18] =
	{
		'b', 'd', 'f', 'g', 'h',
		'k', 'l', 'm', 'n', 'p',
		'r', 's', 't', 'v', 'w',
		'z', 'j', 'y'
	};

	int length = Random::Instance()->IntRange(3, 8);
	std::string name = "";
	bool lever = false;
	int count = 0;

	int num1 = Random::Instance()->IntRange(0, 3);
	if (num1 == 0)
	{
		int rand = Random::Instance()->IntRange(0, 4);
		name.push_back(syllable[rand]);
	}
	else
	{
		int ran = Random::Instance()->IntRange(0, 17);
		name.push_back(MediumPhoneme[ran]);
	}

	for (int j = 0; j < (length - 1); j++)
	{
		int num2 = Random::Instance()->IntRange(0, 1); 
		if (j == 0)
		{
			int ran = Random::Instance()->IntRange(0, 4);
			name.push_back(syllable[ran]);
			lever = true;
		}
		else if (lever == false)
		{
			int ran = Random::Instance()->IntRange(0, 4);
			name.push_back(syllable[ran]);
			lever = true;
			count = 0;
		}
		else if (num2 == 0)
		{
			int ran = Random::Instance()->IntRange(0, 18);
			name.push_back(BigPhoneme[ran][0]);
			name.push_back(BigPhoneme[ran][1]);
			lever = false;
		}
		else
		{
			int ran = Random::Instance()->IntRange(0, 17);
			name.push_back(MediumPhoneme[ran]);
			count++;
			lever = false;
		}
	}

	return name;
}

//Inefficient, but only needs to be ran once on start up
void Generator::NameDupeChecker(std::unordered_map<std::string, std::shared_ptr<Planet>>& planets, std::shared_ptr<Planet> planet)
{
	std::unordered_map<std::string, std::shared_ptr<Planet>>::iterator it;
	for (it = planets.begin(); it != planets.end(); it++)
	{
		if (it->second != planet && it->second->GetName() == planet->GetName())
		{
			planet->SetName(GetRandomName());
			NameDupeChecker(planets, planet);
		}
	}
}