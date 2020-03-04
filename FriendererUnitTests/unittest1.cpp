#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Frienderer\Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<typename T> static std::wstring ToString(const Mat<T>& mat) {
	return L"Yeet";
}
template<typename T> static std::wstring ToString(const Vec2<T>& vec) {
	std::stringstream s;
	s << "{" << vec.x << ", " << vec.y << "}";
	std::string str = s.str();
	std::wstring ws(str.begin(), str.end());
	return ws;
}
template<typename T> static std::wstring ToString(const Vec3<T>& vec) {
	std::stringstream s;
	s << "{" << vec.x << ", " << vec.y << "," << vec.z << "}";
	std::string str = s.str();
	std::wstring ws(str.begin(), str.end());
	return ws;
}

namespace FriendererUnitTests
{	
	TEST_CLASS(MathTests)
	{
	public:
		
		TEST_METHOD(Vec2Init)
		{
			Vec2i testVec = Vec2i(2, 4);

			Assert::AreEqual(testVec.x, 2);
			Assert::AreEqual(testVec.y, 4);
		}

		TEST_METHOD(MatrixMultiplication)
		{
			Mat<int> matA(4, 4);
			Mat<int> matB(4, 4);
			
			matA = {
				{4, 2, 6, 3},
				{6, 1, -3, 4},
				{8, -2, 4, 2},
				{-7, 3, 5, 2}
			};
			
			matB = {
				{8, 4, -2, 5},
				{4, 6, 1, -5},
				{-9, 4, -3, 7},
				{-4, 1, 3, -6}
			};
			
			Mat<int> matC = matA * matB;

			Mat<int> matD(
				{
					{-26, 55, -15, 34},
					{63, 22, 10, -20},
					{12, 38, -24, 66},
					{-97, 12, 8, -27}
				});

			Assert::AreEqual(matC, matD);
		}

		TEST_METHOD(CrossProduct)
		{
			Vec3i vecA(5, -3, 6);
			Vec3i vecB(-2, 8, 4);

			Vec3i result = vecA.crossProduct(vecB);
			
			Assert::AreEqual(result, Vec3i(-60, -32, 34));
		}

		TEST_METHOD(DotProduct) {
			Vec3i vecA(4, 9, -23);
			Vec3i vecB(-17, 3, 90);
			Assert::AreEqual(vecA.dotProduct(vecB), -2111);
		}
	};
}