#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Component.hpp"

class Gate : public Component
{
protected:
	bool is_HIGH;
public:
	Gate(std::string name):
	Component(name)
	{
		is_HIGH = false;
	}
    virtual void update();
	void query() override
	{
		Component::query();
		std::cout << "status |\t" << is_HIGH << "\n";
	}
};

class AndGate : public Gate
{
private:
public:
	AndGate():
	Gate("And Gate")
	{
	}
	void update() override
	{
		int input_high_count = 0;
		int amount_of_inputs = 0;
		for(auto port : *getPortList())
		{
			if(port.isInput())
			{
				amount_of_inputs++;
				input_high_count += port.getPulse();
			}
		}
		if(input_high_count == amount_of_inputs)
		{
			is_HIGH = true;
			for(auto port : *getPortList())
			{
				if(port.isOutput())
				{
					port.sendPulse();
				}
			}
			logMessage("AND is true : " + std::to_string(input_high_count) + " / " + std::to_string(amount_of_inputs));
		}
		else
		{
			logMessage("AND is false : " + std::to_string(input_high_count) + " / " + std::to_string(amount_of_inputs));
		}
	}
};

class ORGate : public Gate
{
private:
public:
	ORGate():
	Gate("OR gate")
	{
	}
	void update() override
	{
		for(auto port : *getPortList())
		{
			if(port.isInput())
			{
				if(port.getPulse())
				{
					is_HIGH = true;
					break;
				}
			}
		}
	}
};

class NotGate : public Gate
{
public:
	NotGate():
	Gate("Not Gate")
	{
		is_HIGH = true;
	}
	void update() override
	{
		for(auto port : *getPortList())
		{
			if(port.isInput())
			{
				is_HIGH = !port.getPulse();
				return;
			}
		}
	}
};