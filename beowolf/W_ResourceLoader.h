#ifndef W_RESOURCELOADER_H
#define W_RESOURCELOADER_H

#include <string>
#include <utility>

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

		std::string getTexture(const std::string& name);
		std::string getModelTexture(const std::string& name);
		std::pair<std::string, std::string> getShaders(const std::string& name);
		std::string getVertexShader(const std::string& name);
		std::string getPixelShader(const std::string& name);
		std::string getModel(const std::string& name);
		std::string getFont(const std::string& name);
		std::string getJSONObject(const std::string& name);

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// private constructor
		ResourceLoader() {}

		bool checkFileExists(const std::string& filename);
	};
}

#endif