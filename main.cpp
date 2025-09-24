#include <string>
#include <vector>
#include "TwilightEngineCore/TwilightEngine.hpp"
#include "CoffeeInstructions.h"

typedef unsigned char byte;

//Links two IO ports together.
class Cable
{
private:
	byte buffer;
	bool tick_sent;
public:
	void flipTick()
	{
		tick_sent = !tick_sent;
	}
	void send(byte data)
	{
		buffer = data;
	}
	byte recieve()
	{
		return buffer;
	}
	void sendTick()
	{
		tick_sent = true;
	}
	bool wasTickSent()
	{
		return tick_sent;
	}
	void clear()
	{
		buffer = 0;
	}
	void resetTick()
	{
		tick_sent = false;
	}
	void query()
	{
		std::cout << "Querying Cable:\n";
		std::cout << "Tick\t|";
		if(tick_sent)
		{
			std::cout << "HIGH\n";
		}
		else
		{
			std::cout << "LOW\n";
		}
		std::cout << "Data\t|" << std::to_string(buffer) << "\n";
	}
};

enum class PortDirection
{
	IN,
	OUT
};

//Allows something access to the outside.
class IOPort
{
private:
	Cable* connection;
	std::string name;
	PortDirection direction;

public:
	IOPort(Cable* new_cable, std::string new_name, PortDirection new_direction):
	connection(new_cable),
	name(new_name),
	direction(new_direction)
	{
		if(new_cable == nullptr)
		{
			std::cout << "CABLE IS NULLPTR\n";
		}
	}
	PortDirection getDirection()
	{
		return direction;
	}
	void setCable(Cable* cable)
	{
		connection = cable;
	}
	std::string getName()
	{
		return name;
	}
	void transferData(byte new_data)
	{
		if(direction == PortDirection::IN)
		{
			std::cout << "ERROR > Port direction is IN, but is trying to send OUT...\n";
			return;
		}
		if(connection == nullptr)
		{
			return;
		}
		connection->send(new_data);
	}

	byte getTransferredData()
	{
		if(direction == PortDirection::OUT)
		{
			std::cout << "ERROR > Port direction is OUT, but is trying to read IN...\n";
			return 0x00;
		}
		if(connection == nullptr)
		{
			return 0x00;
			std::cout << "IOPort connection is null\n";
		}
		return connection->recieve();
	}

	void sendTick()
	{
		if(connection == nullptr)
		{
			return;
		}
		connection->sendTick();
	}
	void flipTick()
	{
		connection->flipTick();
	}
	bool checkTick()
	{
		return connection->wasTickSent();
	}
	void clearTick()
	{
		connection->resetTick();
	}

	void query()
	{
		std::cout << "\n--------------------------------------\n";
		std::cout << "Querying IOPort:\n";
		std::cout << "name\t|" << name << "\n";
		std::cout << "dir\t|";
		if(direction == PortDirection::IN)
		{
			std::cout << "IN\n";
		}
		else
		{
			std::cout << "OUT\n";
		}
		std::cout << "ptr\t|" << connection << "\n";
		connection->query();
		std::cout << "\n--------------------------------------\n";
	}
};

class Component
{
private:
	//Allows for connecting to other components.
	std::vector<IOPort> io_ports;

protected:
	std::vector<IOPort>* getIOPorts()
	{
		return &io_ports;
	}
public:
	Component()
	{
	}

	void addIOPort(Cable* cable, std::string name, PortDirection direction)
	{
		io_ports.push_back(IOPort(cable, name, direction));
	}

	IOPort getIOPort(int address)
	{
		if(io_ports.size() < address)
		{
			return io_ports.back();
		}
		return io_ports.at(address);
	}
};

class MemoryBlock : public Component, public RenderObject2D
{
private:
	byte data[0xff];
	Point<float> position;
	byte address;

	void sendDataToOutputs()
	{
		for(auto port : *getIOPorts())
		{
			if(port.getDirection() == PortDirection::OUT)
			{
				port.transferData(data[address]);
			}
		}
	}
	void getDataFromInputs()
	{
	}
public:
	MemoryBlock():
	position(400, 400)
	{
		for(byte i = 0; i != 0xff - 10; i++)
		{
			data[i] = 1;
		}
	}
	void draw() override
	{
		DrawRectangle(position.getX(), position.getY(), 100, 100, GREEN);
	}
	std::string getType() override
	{
		return "MemoryBlock";
	}
	void update() override
	{
		sendDataToOutputs();
		address = getIOPort(1).getTransferredData();
		// getIOPort(1).query();
		// std::cout << "\n| MEM BLOCK | ADDRESS SET TO 0x" << std::to_string(address) << "\n\n";
	}

	void write(byte address, byte data)
	{
	}
	void writeExtended(byte offset, byte instruction, byte parameter_1, byte parameter_2, byte operand)
	{
		data[offset + 0] = instruction;
		data[offset + 1] = parameter_1;
		data[offset + 2] = parameter_2;
		data[offset + 3] = operand;

	}
	byte* getDataPointer()
	{
		return data;
	}
};

enum class CoffeeInstructions
{
	LOAD,
	JUMP,
	RETURN
};

std::string getInstructionByName(CoffeeInstructions instruction)
{
	switch (instruction)
	{
	case CoffeeInstructions::LOAD:
		return "LOAD";
		break;
	case CoffeeInstructions::JUMP:
		return "JUMP";
		break;
	case CoffeeInstructions::RETURN:
		return "RETURN";
		break;
	default:
		std::cout << "\n\t::>::>INVALID instruction \"Switched\" in:\n\tstd::string getInstructionByName(CoffeeInstructions instruction)\n";
		return "INVALID";
		break;
	}
}

class CoffeeLine
{
private:
	CoffeeInstructions instruction;
	byte parameter_1;
	byte parameter_2;
	byte operand;
	int offset;
public:
	CoffeeLine(int offset, CoffeeInstructions instruction, byte parameter_1, byte parameter_2, byte operand):
	offset(offset),
	instruction(instruction),
	parameter_1(parameter_1),
	parameter_2(parameter_2),
	operand(operand)
	{
	}
	byte getOffset() { return offset; }
	byte getinstruction() { return (byte)instruction; }
	byte getparameter_1() { return parameter_1; }
	byte getparameter_2() { return parameter_2; }
	byte getoperand() { return operand; }
	void query(int line)
	{
		std::cout << line << " | [0x" << offset << "] { " << getInstructionByName(instruction) << "(";
		std::cout << " " << std::to_string(parameter_1) << ", ";
		std::cout << " " << std::to_string(parameter_2) << ") <";
		std::cout << std::to_string(operand) << "> }";
		std::cout << "\n";
	}
};

class CoffeeEditor
{
private:
	std::vector<CoffeeLine> lines;
public:
	CoffeeEditor()
	{
	}
	void query()
	{
		int line_number = 0;
		for(auto line : lines)
		{
			line_number++;
			line.query(line_number);
		}
	}
	void addLine(CoffeeLine new_line)
	{
		lines.push_back(new_line);
	}
	void writeOut(MemoryBlock* data)
	{
		for(auto line : lines)
		{
			data->writeExtended(
								line.getOffset(),
								line.getinstruction(),
								line.getparameter_1(),
								line.getparameter_2(),
								line.getoperand()
								);
		}
	}
};

enum class CoffeeCorePortIndeces
{
	ROM_READ,
	ROM_PTR_OUT,
	RAM_READ,
	RAM_PTR_OUT
};

class CoffeeCore : public Component, public RenderObject2D
{
private:
	/*
		Registers:
		0x00 - A
		0x01 - B
		0x02 - COPY
		0x03 - PTR
	*/
	byte data_registers[4];
	byte instruction_registers[4];
	unsigned int program_counter;
	int current_instruction_register;
	Point<float> position;

	int jumps;
	byte function_stack[8];

protected:
	void tickUpCurrentInstruction()
	{
		current_instruction_register++;
		if(current_instruction_register == 4)
		{
			current_instruction_register = 0;
		}
	}
	int cycles_until_execution;
	void executeInstruction()
	{
		std::cout << "PC == " << std::to_string(program_counter) << "\n " << "cycles_until_execution == " << cycles_until_execution << "\n";
		if(cycles_until_execution)
		{
			cycles_until_execution--;
			return;
		}
		query();
		std::cout << "Executing function ";
		cycles_until_execution = 3;
		switch (instruction_registers[0])
		{
		case (int)CoffeeInstructions::LOAD:
			data_registers[0] = getIOPort((int)CoffeeCorePortIndeces::RAM_READ).getTransferredData();
			std::cout << "loading into A\n";
			break;
		
		case (int)CoffeeInstructions::JUMP:
			if(jumps == 8)
			{
				std::cout << "Stack Full...";
				return;
			}
			function_stack[jumps++] = program_counter;
			program_counter = instruction_registers[2];
			std::cout << "Jumping from " << std::to_string(program_counter) << " from " << function_stack[jumps - 1] << "\n";
			break;

		case (int)CoffeeInstructions::RETURN:
			if(jumps == 0)
			{
				return;
			}
			program_counter = function_stack[jumps] - 1;
			// function_stack.
			function_stack[jumps] = 0x00;
			jumps--;
			break;
		default:
			std::cout << "invalid instruction found\n";
			exit(0);
			break;
		}
	}
	void updateInstructionRegister()
	{
		instruction_registers[current_instruction_register] = getIOPort((int)CoffeeCorePortIndeces::ROM_READ).getTransferredData();
	}
public:
	CoffeeCore():
	position(30, 30)
	{
		data_registers[0x03] = 0x0a;
		for(int i = 0; i != 4; i++)
		{
			data_registers[i] = 0x00;
			function_stack[i] = 0x00;
		}
		for(int i = 0; i != 4; i++)
		{
			instruction_registers[i] = 0x00;
			function_stack[4 + i] = 0x00;
		}
		program_counter = 0;
		current_instruction_register = 0;
		cycles_until_execution = 3;
	}
	void draw() override
	{
		DrawRectangle(position.getX(), position.getY(), 150, 150, WHITE);
		DrawRectangle(position.getX() + 5, position.getY() + 5, 140, 140, BLACK);

		DrawText(("A " + std::to_string(data_registers[0])).c_str(), 40, 40, 20, WHITE);
		DrawText(("B " + std::to_string(data_registers[1])).c_str(), 40, 60, 20, WHITE);
		DrawText(("COPY " + std::to_string(data_registers[2])).c_str(), 40, 80, 20, WHITE);
		DrawText(("PTR " + std::to_string(data_registers[3])).c_str(), 40, 100, 20, WHITE);
		DrawText(("PC " + std::to_string(program_counter)).c_str(), 40, 120, 20, WHITE);
	}
	std::string getType() override
	{
		return "CoffeeCore";
	}
	void update() override
	{
		getIOPort((int)CoffeeCorePortIndeces::ROM_READ).flipTick();
		if(getIOPort((int)CoffeeCorePortIndeces::ROM_READ).checkTick())
		{
			updateInstructionRegister();
			executeInstruction();
		}
		// std::cout << "Transferring PC > 0x" << program_counter << "\n";

		if(getIOPort((int)CoffeeCorePortIndeces::ROM_READ).checkTick())
		{
			getIOPort((int)CoffeeCorePortIndeces::ROM_PTR_OUT).sendTick();
		}
		else
		{
			getIOPort((int)CoffeeCorePortIndeces::RAM_PTR_OUT).clearTick();
		}
		if(getIOPort((int)CoffeeCorePortIndeces::ROM_READ).checkTick())
		{
			program_counter++;
			// updateInstructionRegister();
			tickUpCurrentInstruction();
			getIOPort((int)CoffeeCorePortIndeces::ROM_PTR_OUT).transferData(program_counter);
		}
		// system("clear");
		// query();
	}
	void query(bool query_ioports = false)
	{
		std::cout << "\n--------------------------------------\n";
		std::cout << "Querying CoffeeCore\n";
		std::cout << "PC\t| " << program_counter << "\n";
		std::cout << "ins\t| " << getInstructionByName((CoffeeInstructions)instruction_registers[0]) << "\n";
		std::cout << "para 1\t| " << std::to_string(instruction_registers[1]) << "\n";
		std::cout << "para 2\t| " << std::to_string(instruction_registers[2]) << "\n";
		std::cout << "opera\t| " << std::to_string(instruction_registers[3]) << "\n";
		std::cout << "\n--------------------------------------\n";
		if(!query_ioports)
		{
			return;
		}
		for(auto port : *getIOPorts())
		{
			port.query();
		}
	}
};

void TwilightEngine::userSetup()
{
	Cable* ROM_talk = new Cable();
	Cable* ROM_addr_talk = new Cable();

	Cable* RAM_talk = new Cable();
	Cable* RAM_addr_talk = new Cable();
	
	CoffeeCore* coffee_core = new CoffeeCore();
	coffee_core->addIOPort(ROM_talk, "ROM read IN", PortDirection::IN);
	coffee_core->addIOPort(ROM_addr_talk, "ROM PTR Out", PortDirection::OUT);
	coffee_core->getIOPort((int)CoffeeCorePortIndeces::ROM_READ).sendTick();
	coffee_core->addIOPort(RAM_talk, "RAM read IN", PortDirection::IN);
	coffee_core->addIOPort(RAM_addr_talk, "RAM PTR Out", PortDirection::OUT);

	MemoryBlock* rom_memory_block = new MemoryBlock();
	rom_memory_block->addIOPort(ROM_talk, "CoffeeCore Read Out", PortDirection::OUT);
	rom_memory_block->addIOPort(ROM_addr_talk, "CoffeeCore PTR In", PortDirection::IN);

	MemoryBlock* ram_memory_block = new MemoryBlock();

	addRenderObject(coffee_core);
	addRenderObject(rom_memory_block);

	CoffeeEditor editor;
	editor.addLine(CoffeeLine(0, CoffeeInstructions::LOAD, 5, 0, 0));
	editor.addLine(CoffeeLine(4, CoffeeInstructions::JUMP, 16, 0, 0));
	editor.addLine(CoffeeLine(4, CoffeeInstructions::RETURN, 28, 0, 0));

	editor.query();
	editor.writeOut(rom_memory_block);
	// exit(0);
}

void TwilightEngine::userLoop()
{
	int stop = 99;
}

int main(int argc, char* argv[])
{
	TwilightEngine(
					argc,
					argv,
					Point<float>(800, 800),
					std::string("CoffeeCore")
					).enter();
	return 0;
}
