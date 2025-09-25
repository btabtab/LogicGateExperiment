#pragma once
#include <string>
#include <vector>
#include <iostream>


#include "Loggable.hpp"

class IOPort : public Loggable
{
private:
	bool is_output;
	Wire* wire;
public:
	IOPort(Wire* wire, bool is_output):
	wire(wire),
	is_output(is_output)
	{
	}
	//Sends a HIGH pulse through the wire.
	void sendPulse()
	{
		if(!is_output)
		{
			warning("sendPulse() : not an output");
			return;
		}
		wire->send();
	}
	void clearPulse()
	{
		if(!is_output)
		{
			warning("clearPulse() : not an output");
		}
	}
	//Gets TRUE or FALSE based on the wire's state.
	bool getPulse()
	{
		if(is_output)
		{
			warning("getPulse() : not an input");
			return false;
		}
		return wire->get();
	}
	bool isInput()
	{
		return !is_output;
	}
	bool isOutput()
	{
		return is_output;
	}
};
