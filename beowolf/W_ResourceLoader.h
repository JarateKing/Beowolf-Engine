#ifndef W_RESOURCELOADER_H
#define W_RESOURCELOADER_H

#include <string>

namespace wolf
{
	class ResourceLoader
	{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		// singleton pattern get instance
		static ResourceLoader& Instance()
		{
			static ResourceLoader m_instance;
			return m_instance;
		}
		// remove methods that break the singleton pattern
		ResourceLoader(ResourceLoader const&) = delete;
		void operator=(ResourceLoader const&) = delete;

		std::string getTexture(std::string name);
		std::string getVertexShader(std::string name);
		std::string getPixelShader(std::string name);
		std::string getModel(std::string name);

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// private constructor
		ResourceLoader() {}

		bool checkFileExists(std::string filename);
	};
}

#endif