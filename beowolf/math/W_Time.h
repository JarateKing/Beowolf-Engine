#ifndef W_TIME_H
#define W_TIME_H

namespace wolf
{
	class Time
	{
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
		// singleton pattern get instance
		static Time& Instance()
		{
			static Time m_instance;
			return m_instance;
		}
		// remove methods that break the singleton pattern
		Time(Time const&) = delete;
		void operator=(Time const&) = delete;

		void update();
		double deltaTime();
	private:
		//-------------------------------------------------------------------------
		// PRIVATE METHODS
		//-------------------------------------------------------------------------
		// private constructor
		Time() {}

		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		double lastTime = 1.0;
		double currentTime = 1.0;
	};
}

#endif