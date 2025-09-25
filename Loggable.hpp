#pragma once
#include <string>
#include <vector>
#include <iostream>

class Loggable
{
private:
	std::string label;
public:
	Loggable(std::string label = "Broken/No Label"):
	label(label)
	{
	}
	std::string getLabel()
	{
		return label;
	}
	virtual void query()
	{
	}
	void logMessage(std::string message)
	{
		std::cout << label << "::>" << message << "\n";
	}
	void error(std::string message)
	{
		std::cout << "ERROR: ";
		logMessage(message);
		exit(1);
	}
	void warning(std::string message)
	{
		std::cout << "WARNING: ";
		logMessage(message);
		return;
	}
};
