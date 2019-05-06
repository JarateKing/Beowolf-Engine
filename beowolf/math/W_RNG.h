namespace wolf
{
	class RNG
	{
	public:
		static bool Bool();
		static bool Bool(float trueChance);
		static int GetRandom(int min, int max);
		static float GetRandom(float min, float max);
		static double GetRandom(double min, double max);
	};
}