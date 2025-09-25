#pragma once
#include <string>
#include <vector>
#include <iostream>


#include "Component.hpp"

//Used to send timing pulses to other Components.
class Clock : public Component
{
private:
	bool has_ticked;
public:
	Clock(std::string label):
	Component(label)
	{
	}
	/*
		Sends a alternating pulse through all
		of the ports based on the current state.
	*/
	void tick()
	{
		has_ticked = !has_ticked;
		for(auto port : *getPortList())
		{
			port.clearPulse();
			if(has_ticked)
			{
				port.sendPulse();
			}
		}
	}
	void query() override
	{
		Component::query();
		std::string ticked_message[2] =
		{
			"HIGH",
			"LOW"
		};
		std::cout << "Tick\t|" << ticked_message[has_ticked] << "\n";
	}
};