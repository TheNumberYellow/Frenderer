#pragma once
#include "Math.h"
#include "Camera.h"

#include <Windows.h>

#include <vector>

struct Colour {
	uint8_t r, g, b;
	Colour() : r(0), g(0), b(0) {}
	Colour(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

};

struct Tri {
	Tri(Vec3f a, Vec3f b, Vec3f c) {
		p[0] = a;
		p[1] = b;
		p[2] = c;

		colour[0] = Colour(RandomFloat(0, 255), RandomFloat(0, 255), RandomFloat(0, 255));
		colour[1] = Colour(RandomFloat(0, 255), RandomFloat(0, 255), RandomFloat(0, 255));
		colour[2] = Colour(RandomFloat(0, 255), RandomFloat(0, 255), RandomFloat(0, 255));
	}
	Tri(Vec3f a, Vec3f b, Vec3f c, Colour colA, Colour colB, Colour colC) : p{ a, b, c }, colour{ colA, colB, colC }
	{
	}

	Tri(Vec3f a, Vec3f b, Vec3f c, Vec2f uv0, Vec2f uv1, Vec2f uv2) : p{ a, b, c }, uv{ uv0, uv1, uv2 } {
		colour[0] = Colour(RandomFloat(0, 255), RandomFloat(0, 255), RandomFloat(0, 255));
		colour[1] = Colour(RandomFloat(0, 255), RandomFloat(0, 255), RandomFloat(0, 255));
		colour[2] = Colour(RandomFloat(0, 255), RandomFloat(0, 255), RandomFloat(0, 255));
	}

	Tri(Vec3f a, Vec3f b, Vec3f c, Vec2f uv0, Vec2f uv1, Vec2f uv2, Colour colA, Colour colB, Colour colC) : p{ a, b, c }, uv{ uv0, uv1, uv2 }, colour{ colA, colB, colC } {
	}

	Vec3f p[3];
	Vec2f uv[3];
	Colour colour[3];
	
};

struct Mesh {
	Mesh() : modelMat(4, 4)
	{
	}

	std::vector<Tri> tris;

	Vec3f displacement;

	Mat<float> modelMat;
	
	BITMAP texture; 

	void rotateAroundAxis(float radians, Vec3f axis);
	void setRotation(float radians, Vec3f axis);
};

class Buffer {
public:
	int width, height;
	void* memory;
	BITMAPINFO info;
	int bytesPerPixel;
	int pitch;
	
	void inline placePixel(int x, int y, Colour colour);
	
	float edgeFunction(const Vec2i& a, const Vec2i& b, const Vec2i& c);


	Vec2<int> projectTo2D(Vec3<float> point, Mat<float> mat);

	void drawLine(Vec3f p0, Vec3f p1, Colour colour0, Colour colour1, Mat<float> mat);

	void drawTri(Tri t, Mat<float> projMat, BITMAP texture);

	void drawMesh(Mesh m, Camera cam);

	void clearBuffer(Colour c);
	
	void drawToScreen(HDC hdc);
};