#include "ArgumentHandler.hpp"

#include <filesystem>

namespace fs = std::experimental::filesystem;

ArgumentHandler::ArgumentHandler(int argc, char * argv[])
{
	prog_name = argv[0];
	for (int i = 1; i < argc; i++)
		arguments.push_back(argv[i]);
}

ArgumentHandler::~ArgumentHandler()
{
}

void ArgumentHandler::eval()
{
	const size_t size = arguments.size();
	for (size_t i = 0; i < size; i++)
	{
		const std::string c = arguments[i];
		if (c == "-c")
		{
			_flags.compile = true;
		}
		else if (c == "-o" && i + 1 < size)
		{
			_flags.output_name = arguments[i + 1];
			i++;
		}
		else if (fs::exists(c))
		{
			if (!_flags.compile)
				_flags.execute = true;
			_flags.file_list.push_back(c);
		}
	}
}
