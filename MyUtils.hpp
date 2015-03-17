#pragma once
#include <tyga/Math.hpp>
#include <math.h> //pi
#include <random>

namespace utilAyre
{

	/*----------------------------------------OLD OGRE IMPLEMENTATION-------------------------------------------------------------------
					   [Note to self - Bezier maths could be done in util rather than in badger]

					   //matrix and math stuff
					   Ogre::Matrix3 EulerOrientation(float x, float y, float z);
					   Ogre::Quaternion QuatOrientation(Ogre::Matrix3 eulerMat);
					   Ogre::Quaternion RotatingQuat(float x, float y, float z, float dt);
					   Ogre::Radian EulerConversion(Ogre::Matrix3 mat);
					   Ogre::Matrix3 QuatConversion(Ogre::Quaternion quat);

					   Ogre::Radian Deg2Rad(float deg);
					   float Rad2Deg(Ogre::Radian rad);

					   //setup stuff
					   const Ogre::String initMaterial(Ogre::String materialName, Ogre::String texturePath);

	 */

#pragma region ConversionFunctions & Clamp

	float DegToRad(float degrees);

	float Dot2Vec3(tyga::Vector3 a, tyga::Vector3 b);

	tyga::Vector3 GetPos(tyga::Matrix4x4 source_xform);

	float Dot3Vec3(tyga::Vector3 a, tyga::Vector3 b, tyga::Vector3 c);

	float Clamp(float value, float min, float max);

#pragma endregion

#pragma region Bezier & Euler Functions

	tyga::Vector3 CalcBezier(const tyga::Vector3 p0, const tyga::Vector3 p1, tyga::Vector3 p2, tyga::Vector3 p3, float s); //refactored version of the simple bezier calculations I used in OGRE, float between 0 and 1 for positon along a bezier curve

	inline tyga::Vector3 EulerVec(tyga::Vector3 rootVec, float timeStep, tyga::Vector3 derivative){ return rootVec + timeStep * derivative; }


#pragma endregion

#pragma region NormaliseQuaternion/Magnitude, Direction Unit Vector

	tyga::Quaternion NormQuat(tyga::Quaternion quat, float magnitude);

	tyga::Vector3 DirectionVec(tyga::Matrix4x4 mat4); //DEPRECATED, use tyga::unit instead

	float MagnitudeOfQuaternion(tyga::Quaternion q, tyga::Quaternion qc);

#pragma endregion

#pragma region TranslationRotation

	tyga::Matrix4x4 QuatToRotateMat(tyga::Quaternion q);

	tyga::Matrix4x4 Translate(float x, float y, float z);

	tyga::Matrix4x4 Translate(tyga::Vector3 pos);

#pragma endregion

#pragma region InterpolationFunctions

	float Smoothstep(float t);

	float Lerp(float initPos, float endPos, float time);

	tyga::Vector3 Lerp(tyga::Vector3 initPos, tyga::Vector3 endPos, float time);

	float LinStep(float min, float max, float x);

#pragma endregion

#pragma region RandomFunctions

	float RandomScalar(float min, float max);

	int RandomScalar(int min, int max);

	tyga::Vector3 RandomVec3();

	tyga::Vector3 RandomDirVecSphere(float radius);

#pragma endregion

	tyga::Vector3 ApplyGravity(tyga::Vector3 initForce);

}





