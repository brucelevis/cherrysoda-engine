#ifndef _CHERRYSODA_UTIL_MATH_H_
#define _CHERRYSODA_UTIL_MATH_H_

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_INTRINSICS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include <type_traits>


#define CHERRYSODA_NONE_OP ((void)0)

#define CHERRYSODA_GETTER_SETTER(NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_OF_TYPE(TYPE,NAME,VALUE) \
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(TYPE,NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_GETTER_SETTER_EX(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const decltype(VALUE) NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(const decltype(VALUE)& v) { EXTRA_SET_OP; VALUE = v; }

#define CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(TYPE,NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const TYPE NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(const TYPE& v) { EXTRA_SET_OP; VALUE = v; }

#define CHERRYSODA_MATH_VEC3_GETTER_SETTER(NAME,VALUE) \
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(NAME,VALUE,CHERRYSODA_NONE_OP,CHERRYSODA_NONE_OP)

#define CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(NAME,VALUE,EXTRA_GET_OP,EXTRA_SET_OP) \
	inline const Math::Vec3 NAME() const { EXTRA_GET_OP; return VALUE; } \
	inline void NAME(const Math::Vec3& v3) { EXTRA_SET_OP; VALUE = v3; } \
	inline void NAME(const Math::Vec2& v2) { NAME(Math::Vec3(v2, NAME()[2])); }


#define CHERRYSODA_DECLARE_ENUM_FLAG(ENUM_T) \
inline ENUM_T operator | (ENUM_T lhs, ENUM_T rhs) \
{ \
    using T = std::underlying_type_t<ENUM_T>; \
    return static_cast<ENUM_T>(static_cast<T>(lhs) | static_cast<T>(rhs)); \
} \
inline ENUM_T& operator |= (ENUM_T& lhs, ENUM_T rhs) \
{ \
    lhs = lhs | rhs; \
    return lhs; \
} \
inline ENUM_T operator & (ENUM_T lhs, ENUM_T rhs) \
{ \
    using T = std::underlying_type_t<ENUM_T>; \
    return static_cast<ENUM_T>(static_cast<T>(lhs) & static_cast<T>(rhs)); \
} \
inline ENUM_T& operator &= (ENUM_T& lhs, ENUM_T rhs) \
{ \
    lhs = lhs & rhs; \
    return lhs; \
}


#define Vec2_Zero       cherrysoda::Math::Vec2(0.f)
#define Vec2_One        cherrysoda::Math::Vec2(1.f)
#define Vec2_XUp        cherrysoda::Math::Vec2(1.f,0.f)
#define Vec2_YUp        cherrysoda::Math::Vec2(0.f,1.f)

#define Vec3_Zero       cherrysoda::Math::Vec3(0.f)
#define Vec3_One        cherrysoda::Math::Vec3(1.f)
#define Vec3_XUp        cherrysoda::Math::Vec3(1.f,0.f,0.f)
#define Vec3_YUp        cherrysoda::Math::Vec3(0.f,1.f,0.f)
#define Vec3_ZUp        cherrysoda::Math::Vec3(0.f,0.f,1.f)

#define IVec2_Zero      cherrysoda::Math::IVec2(0)
#define IVec2_One       cherrysoda::Math::IVec2(1)
#define IVec2_XUp       cherrysoda::Math::IVec2(1,0)
#define IVec2_YUp       cherrysoda::Math::IVec2(0,1)

#define IVec3_Zero      cherrysoda::Math::IVec3(0)
#define IVec3_One       cherrysoda::Math::IVec3(1)
#define IVec3_XUp       cherrysoda::Math::IVec3(1,0,0)
#define IVec3_YUp       cherrysoda::Math::IVec3(0,1,0)
#define IVec3_ZUp       cherrysoda::Math::IVec3(0,0,1)

#define Math_Abs        glm::abs
#define Math_Cross      glm::cross
#define Math_Cos        glm::cos
#define Math_Clamp      glm::clamp
#define Math_Dot        glm::dot
#define Math_Length     glm::length
#define Math_LengthSq   glm::length2
#define Math_Min        glm::min 
#define Math_Max        glm::max 
#define Math_Normalize  glm::normalize
#define Math_Rotate     glm::rotate
#define Math_Radians    glm::radians
#define Math_Sign       glm::sign
#define Math_Sin        glm::sin

#define Math_Identity     glm::identity
#define Math_Inverse      glm::inverse
#define Math_LookAt       glm::lookAt
#define Math_Perspective  glm::perspective
#define Math_Scale        glm::scale
#define Math_Translate    glm::translate

#define Math_BitCount             cherrysoda::Math::BitCount_
#define Math_RotateVector         cherrysoda::Math::RotateVector_
#define Math_GetOrientationMatrix cherrysoda::Math::GetOrientationMatrix_
#define Math_RaycastAABB          cherrysoda::Math::RaycastAABB_


namespace cherrysoda {

class Math
{
public:
	static constexpr float Pi     = 3.14159265357878323846f;
	static constexpr float Pi2    = Pi * 2.0f;
	static constexpr float PiHalf = Pi * 0.5f;

	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat4 = glm::mat4;
	using Quat = glm::quat;

	using IVec2 = glm::i32vec2;
	using IVec3 = glm::i32vec3;
	using IVec4 = glm::i32vec4;

	struct AABB
	{
		Vec3 m_min;
		Vec3 m_max;
	};

	struct IRectangle
	{
		IVec2 m_coord;
		IVec2 m_size;

		inline int X() const { return m_coord.x; }
		inline int Y() const { return m_coord.y; }
		inline IVec2 Size() const { return m_size; }
		inline int Width() const { return m_size.x; }
		inline int Height() const { return m_size.y; }
		inline int Left() const { return X(); }
		inline int Right() const { return X() + Width(); }
		inline int Top() const { return Y() + Height(); }
		inline int Bottom() const { return Y(); }
	};

	static inline const Vec3 RotateVector_(const Vec3& v3, float angle, const Vec3& axis)
	{
		return Vec3(Vec4(v3, 1.f) * Math_Rotate(Math_Identity<Mat4>(), angle, axis));
	}

	static const Mat4 GetOrientationMatrix_(const Mat4& matrix);

	static bool RaycastAABB_(const Vec3& start, const Vec3& direction, const AABB& aabb, float* t1 = nullptr, float* t2 = nullptr);

	template <typename T>
	static inline int BitCount_(T x)
	{
		int ret = 0;
		while (x) { x &= ~(x & (-x)); ++ret; }
		return ret;
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_MATH_H_
