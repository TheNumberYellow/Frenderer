#include "Render.h"
#include <iostream>

void Buffer::placePixel(int x, int y, Colour c) {
	if (x > width - 1 || x < 0 || y > height - 1 || y < 0) return;

	uint32_t * pixel = (uint32_t*)memory;

	pixel += y * width;
	pixel += x;

	*pixel = (c.b | (c.g << 8) | (c.r << 16));
}

float Buffer::edgeFunction(const Vec2i& a, const Vec2i& b, const Vec2i& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

// TODO: return float vecs (Device normalized coordinates), change to pixel coordinates in another step 
inline Vec2<int> Buffer::projectTo2D(Vec3<float> point, Mat<float> mat) {

	Vec3<float> p = point;
	p = p * mat;

	Vec2<int> res = Vec2<int>((width * (p.x / p.z)) + (width / 2), (height * (p.y / p.z)) + (height / 2));
	return res;
}

void Buffer::drawLine(Vec3f p0, Vec3f p1, Colour colour0, Colour colour1, Mat<float> mat) {
	Vec2i pixel0, pixel1;
	pixel0 = projectTo2D(p0, mat);
	pixel1 = projectTo2D(p1, mat);

	for (float t = 0.0; t < 1.0; t += 0.001) {
		int x = std::round(pixel0.x * (1.0 - t) + pixel1.x * (double)t);
		int y = std::round(pixel0.y * (1.0 - t) + pixel1.y * (double)t);
		if ((x < 0 && y < 0) || (x > 500 && y > 500)) {
			continue;
		}
		Colour newColour = Colour(colour0.r + (t * (colour1.r - colour0.r)), colour0.g + (t * (colour1.g - colour0.g)), colour0.b + (t * (colour1.b - colour0.b)));
		placePixel(x, y, newColour);
	}
}

inline void Buffer::drawTri(Tri t, Mat<float> projMat, BITMAP texture) {
	
	Vec2i p0 = projectTo2D(t.p[0], projMat);
	Vec2i p1 = projectTo2D(t.p[1], projMat);
	Vec2i p2 = projectTo2D(t.p[2], projMat);


	Vec2i min = Vec2i(Minimum(Minimum(p0.x, p1.x), p2.x), Minimum(Minimum(p0.y, p1.y), p2.y));
	Vec2i max = Vec2i(Maximum(Maximum(p0.x, p1.x), p2.x), Maximum(Maximum(p0.y, p1.y), p2.y));

	for (int y = min.y; y != max.y + 1; y++) {
		for (int x = min.x; x != max.x + 1; x++) {
			float area = edgeFunction(p0, p1, p2);

			float w0 = edgeFunction(p1, p2, Vec2i(x, y));
			float w1 = edgeFunction(p2, p0, Vec2i(x, y));
			float w2 = edgeFunction(p0, p1, Vec2i(x, y));

			if (w0 > 0 && w1 > 0 && w2 > 0) {
				w0 /= area;
				w1 /= area;
				w2 /= area;

				Vec2f texCoords = (w0 * t.uv[0]) + (w1 * t.uv[1]) + (w2 * t.uv[2]);
				
				texCoords.x *= texture.bmWidth;
				texCoords.y *= texture.bmHeight;

				unsigned int index = ((texture.bmBitsPixel / 8) * (unsigned int)texCoords.x) + ((unsigned int)texCoords.y * texture.bmWidthBytes);

				uint8_t* pixel = (uint8_t*)texture.bmBits;

				pixel += index;

				Colour texColour = Colour(*(pixel + 2), *(pixel + 1), *(pixel));

				texColour.r *= (w0 * t.colour[0].r / 255) + (w1 * t.colour[1].r / 255) + (w2 * t.colour[2].r / 255);
				texColour.g *= (w0 * t.colour[0].g / 255) + (w1 * t.colour[1].g / 255) + (w2 * t.colour[2].g / 255);
				texColour.b *= (w0 * t.colour[0].b / 255) + (w1 * t.colour[1].b / 255) + (w2 * t.colour[2].b / 255);

				placePixel(x, y, texColour);
			}
		}
	}


	//drawLine(t.p[0], t.p[1], t.colour[0], t.colour[1], projMat);
	//drawLine(t.p[1], t.p[2], t.colour[1], t.colour[2], projMat);
	//drawLine(t.p[2], t.p[0], t.colour[2], t.colour[0], projMat);

}

void Buffer::drawMesh(Mesh m, Camera cam)
{
	for (Tri t : m.tris) {
		Mat<float> transformationMatrix = m.modelMat * cam.m;
		drawTri(t, transformationMatrix, m.texture);
	}
}

void Buffer::clearBuffer(Colour c) {
	uint32_t* pixel = (uint32_t*)memory;
	size_t size = width * height;
	for (int i = 0; i < size; i++) {
		*pixel++ = (c.b | (c.g << 8) | (c.r << 16));
	}
}

void Buffer::drawToScreen(HDC hdc) {
	StretchDIBits(hdc,
		0, 0, width, height,
		0, 0, width, height,
		memory, &info,
		DIB_RGB_COLORS, SRCCOPY);
}

void Mesh::rotateAroundAxis(float radians, Vec3f axis)
{
	modelMat *= Mat<float>::getRotationMatrix(radians, axis);
}

void Mesh::setRotation(float radians, Vec3f axis)
{
	modelMat = Mat<float>::getRotationMatrix(radians, axis);
}
