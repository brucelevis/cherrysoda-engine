#ifndef _CHERRYSODA_UTIL_CALC_H_
#define _CHERRYSODA_UTIL_CALC_H_

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Random
{
public:
	Random() {}
	Random(type::UInt32 seed) { Seed(seed); }

	inline void Seed(type::UInt32 seed) { STL::RandomSeed(m_random, seed); }
	inline type::UInt32 Next() { return STL::RandomNext(m_random); }
	inline type::UInt32 Next(type::UInt32 max) { return static_cast<type::UInt32>(max * NextDouble()) % max; }
	inline type::UInt32 Next(type::UInt32 min, type::UInt32 max) { return min + Next(max - min + 1); }
	inline type::Int32 NextInt(type::Int32 max) { return static_cast<type::Int32>(max * NextDouble()) % max; }
	inline type::Int32 NextInt(type::Int32 min, type::Int32 max) { return min + NextInt(max - min + 1); }
	inline float NextFloat() { return static_cast<float>(NextDouble()); }
	inline float NextFloat(float max) { return NextFloat() * max; }
	inline float NextFloat(float min, float max) { return min + NextFloat(max - min); }
	inline double NextDouble() { return Next() / static_cast<double>(STL::RandomMax(m_random)); }
	inline float NextAngle() { return NextFloat(Math::Pi2); }

	static type::UInt32 TrueRandomNext();

private:
	STL::Random m_random;
};

class Calc
{
public:
	static void PushRandom(type::UInt32 seed) { STL::Push(ms_randomStack, Random(seed)); }
	static void PushRandom() { STL::Push(ms_randomStack, Random()); }
	static void PopRandom() { STL::Pop(ms_randomStack); }
	static Random& GetRandom() { return STL::TopRef(ms_randomStack); }

private:
	static STL::Stack<Random> ms_randomStack;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CALC_H_
 