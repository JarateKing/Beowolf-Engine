#include "W_ResourceLoader.h"
#include <fstream>

namespace wolf
{


	bool ResourceLoader::checkFileExists(std::string filename) {
		std::ifstream file(filename.c_str());
		return file.good();
	}
}