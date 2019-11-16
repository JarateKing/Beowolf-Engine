#ifndef W_RESOURCELOADER_H
#define W_RESOURCELOADER_H

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

	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// private constructor
		ResourceLoader() {}
	};
}

#endif