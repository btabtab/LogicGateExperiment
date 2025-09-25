#pragma once
#include <string>
#include <vector>
#include <iostream>


#include "Loggable.hpp"

class Wire : public Loggable
{
private:
	bool signal;
public:
	Wire()
	{
		signal = false;
	}
	bool get() { return signal; }
	void set(bool new_value) { signal = new_value; }
	void send() { signal = true; }
	void stop() { signal = false; }
	void flip() { signal = !signal; }
};