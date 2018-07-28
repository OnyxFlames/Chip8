#include "ArgumentHandler.hpp"



ArgumentHandler::ArgumentHandler(int argc, char * argv[])
{
	prog_name = argv[0];
	for (int i = 1; i < argc; i++)
		arguments.push_back(argv[i]);
}

ArgumentHandler::~ArgumentHandler()
{
}
