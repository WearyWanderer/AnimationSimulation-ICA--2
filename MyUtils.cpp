#include "MyUtils.hpp"


namespace utilAyre
{

#pragma region deprecatedOgreFunctions
	/*
	Ogre::Matrix3 EulerOrientation(float x, float y, float z)
	{
		Ogre::Matrix3 eulerMat;

		Ogre::Radian xRad = (Ogre::Radian) x * (Ogre::Math::PI / 180.0f);
		Ogre::Radian yRad = (Ogre::Radian) y * (Ogre::Math::PI / 180.0f);
		Ogre::Radian zRad = (Ogre::Radian) z * (Ogre::Math::PI / 180.0f);

		eulerMat.FromEulerAnglesXYZ(xRad, yRad, zRad);

		return eulerMat;
	}


	Ogre::Quaternion QuatOrientation(Ogre::Matrix3 eulerMat)
	{
		Ogre::Quaternion quat;

		quat.FromRotationMatrix(eulerMat);

		return quat;
	}


	Ogre::Quaternion RotatingQuat(float x, float y, float z, float dt)
	{
		Ogre::Matrix3 currentRot = EulerOrientation(x + dt, y + dt, z + dt);

		Ogre::Quaternion currentQuat = QuatOrientation(currentRot);

		return currentQuat;

	}


	Ogre::Radian Deg2Rad(float deg)
	{
		Ogre::Radian result =(Ogre::Radian) deg * (Ogre::Math::PI / 180.0f);
		return result;
	}

	float Rad2Deg(Ogre::Radian rad)
	{
		float result = rad.valueRadians() * (180.0f / Ogre::Math::PI);

		return result;
	}

	//------line break for the setup stuff

	const Ogre::String initMaterial(Ogre::String materialName, Ogre::String texturePath)
	{
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::Technique* firstTechnique = material->getTechnique(0);
		Ogre::Pass* firstPass = firstTechnique->getPass(0);

		Ogre::TextureUnitState* textureUnit = firstPass->createTextureUnitState();
		textureUnit->setTextureName(texturePath, Ogre::TEX_TYPE_2D);
		textureUnit->setTextureCoordSet(0);

		const Ogre::String& materialDef = materialName;

		return materialDef;
	}
	*/
#pragma endregion

#pragma region newTygaFunctions

	tyga::Vector3 DirectionVec(tyga::Matrix4x4 mat4)
		{
			tyga::Vector3 dv;

			dv = tyga::Vector3(mat4._20, mat4._21, mat4._22);

			return dv;
		}

	tyga::Vector3 CalcBezier(const tyga::Vector3 p0, const tyga::Vector3 p1, tyga::Vector3 p2, tyga::Vector3 p3, float s) //refactored version of the simple bezier calculations I used in OGRE, float between 0 and 1 for positon along a bezier curve
		{
			return tyga::Vector3(((1 - s)*(1 - s)*(1 - s)*p0) + (3 * s*(1 - s)*(1 - s)*p1) + (3 * s*s*(1 - s)*p2) + (s*s*s*p3));
		}

	tyga::Vector3 GetPos(tyga::Matrix4x4 source_xform)
		{
			return tyga::Vector3(source_xform._30, source_xform._31, source_xform._32);
		}

	tyga::Vector3 ApplyGravity(tyga::Vector3 initForce)
		{
			return initForce + tyga::Vector3(0, -9.81f, 0); //using real gravity value as base
		}

	float Dot2Vec3(tyga::Vector3 a, tyga::Vector3 b)
		{
			return a.x*b.x + a.y * b.y + a.z + b.z;
		}

	float Dot3Vec3(tyga::Vector3 a, tyga::Vector3 b, tyga::Vector3 c)
		{
			return a.x*b.x*c.x + a.y*b.y*c.y + a.z*b.z*c.z;
		}

	float DegToRad(float degrees)
		{
			return degrees * (float)(M_PI / 180);
		}

	tyga::Quaternion NormQuat(tyga::Quaternion quat, float magnitude)
		{
			tyga::Quaternion normal;
			normal.w = quat.w / magnitude;
			normal.x = quat.x / magnitude;
			normal.y = quat.y / magnitude;
			normal.z = quat.z / magnitude;

			return normal;
		}

	float LinStep(float min, float max, float x)
		{
			return (x - min) / (max - min);
		}

	float MagnitudeOfQuaternion(tyga::Quaternion q, tyga::Quaternion qc)
		{
			//Magnitude is used in order to normalise the quoternion
			float magnitude = sqrt((q.w*q.w) + (q.x*q.x) + (q.y*q.y) + (q.z*q.z));
			return magnitude;
		}

	tyga::Matrix4x4 QuatToRotateMat(tyga::Quaternion q)
		{
			return tyga::Matrix4x4(1 - 2 * q.y*q.y - 2 * q.z*q.z, 2 * q.x*q.y + 2 * q.z*q.w, 2 * q.x*q.z - 2 * q.y*q.w, 0,
								   2 * q.x*q.y - 2 * q.z*q.w, 1 - 2 * q.x*q.x - 2 * q.z*q.z, 2 * q.y*q.z + 2 * q.x*q.w, 0,
								   2 * q.x*q.y + 2 * q.y*q.w, 2 * q.y*q.z - 2 * q.x*q.w, 1 - 2 * q.x*q.x - 2 * q.y*q.y, 0,
								   0, 0, 0, 1);
		}

	tyga::Matrix4x4 Translate(float x, float y, float z)
		{
			return tyga::Matrix4x4
						 (1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  x, y, z, 1);
		}

	tyga::Matrix4x4 Translate(tyga::Vector3 pos)
		{
			return tyga::Matrix4x4
						 (1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
					   	  pos.x, pos.y, pos.z, 1);
		}

	float Lerp(float initPos, float endPos, float time)
		{
			return (initPos * (1 - time) + endPos * time);
		}

	tyga::Vector3 Lerp(tyga::Vector3 initPos, tyga::Vector3 endPos, float time)
		{
			tyga::Vector3 result;

			result.x = (initPos.x * (1 - time) + endPos.x * time); //initially called Lerp() but is unecessary additionally overhead from calling function, just do this in here
			result.y = (initPos.y * (1 - time) + endPos.y * time);
			result.z = (initPos.z * (1 - time) + endPos.z * time);

			return result;
		}

	float Clamp(float value, float min, float max)
		{
			if (value < min){
				value = min;
			}
			else if (value>max){
				value = max;
			}

			return value;
		}

	float Smoothstep(float t)
		{
			return ((3 * t*t) - (2 * t*t*t));
		}

#pragma region RandomFunctions

	float RandomScalar(float min, float max)
	{
		std::minstd_rand randomSetup(time(NULL)); //set up the rand for all random functions
		std::uniform_real_distribution<float> random(min, max);
		return random(randomSetup);
	}

	int RandomScalar(int min, int max)
	{
		std::minstd_rand randomSetup(time(NULL)); //set up the rand for all random functions
		std::uniform_int_distribution<int> random(min, max);
		return random(randomSetup);
	}

	tyga::Vector3 RandomDirVecSphere()
	{
		std::minstd_rand randomSetup(time(NULL)); //set up the rand for all random functions
		std::uniform_real_distribution<float> randomPoint(-1, 1);
		std::uniform_real_distribution<float> randomPoint2(0, 1);
		/*std::uniform_real_distribution<float> randomAzimuth(0, M_PI * 2);
		std::uniform_real_distribution<float> randomInclination(0, M_PI);*/
		float randomInclination(acos(randomPoint(randomSetup)));
		float randomAzimuth(2 * M_PI * randomPoint2(randomSetup));

		/*return tyga::Vector3(radius * sin(randomInclination(randomSetup)) * cos(randomAzimuth(randomSetup)), 
							 radius * sin(randomInclination(randomSetup)) * sin(randomAzimuth(randomSetup)),
							 radius * cos(randomInclination(randomSetup)));*/

		return tyga::Vector3(sin(randomInclination) * cos(randomAzimuth),
							 sin(randomInclination) * sin(randomAzimuth),
							 cos(randomInclination));
	}

#pragma endregion

#pragma endregion
}