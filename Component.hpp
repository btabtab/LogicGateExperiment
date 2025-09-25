#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Loggable.hpp"
class Component : public Loggable
{
private:
	//A vector of ports so that there can be lots of communication.
	std::vector<IOPort> ports;
	//What the component is referred to in the system.
protected:
	std::vector<IOPort>* getPortList() { return &ports; }
public:
	Component(std::string new_label):
	Loggable(new_label)
	{
	}
	//Add a new port to the vector.
	void connect(Wire* output_wire, Component* output_component)
	{
		ports.push_back(IOPort(output_wire, true));
		output_component->getPortList()->push_back(IOPort(output_wire, false));
	}
	//Get a specified port.
	IOPort* getPort(int index)
	{
		if(ports.size() < index) { return nullptr; }
		return &ports.at(index);
	}
	static void onPulse();

	void query() override
	{
		std::cout << "\nQuery: <" << getLabel() << ">\n";
		std::cout << "ports |\t" << ports.size() << "\n";
		int out_ports = 0, in_ports = 0;
		for(auto port : ports)
		{
			out_ports += port.isOutput();
			in_ports += port.isInput();
		}
		std::cout << "\tout |\t" << out_ports << "\n";
		std::cout << "\tin |\t" << in_ports << "\n";
	}
};