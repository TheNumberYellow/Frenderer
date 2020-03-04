#pragma once

#include "Math.h"

struct Camera {
	Vec3f pos = Vec3f(0, 0, -100);
	Vec3f dir = Vec3f(0, 0, 1);
	Vec3f up = Vec3f(0, 1, 0);

	double focalLength;
	Mat<float> m = Mat<float>(4, 4);

	float mouseSensitivity = 0.0005f;

	// TODO: This should not be here (Put lookAt function in Math.h)
	void lookAt(const Vec3f& from, const Vec3f& to, const Vec3f& tmp = Vec3f(0, 1, 0)) {
		Vec3f forward = Vec3f::normalize(from - to);
		Vec3f up = Vec3f::normalize(tmp);
		Vec3f right = Vec3f::normalize(Vec3f::crossProduct(forward, up));
		up = Vec3f::crossProduct(right, forward);

		m.mat[0][0] = right.x;
		m.mat[1][0] = right.y;
		m.mat[2][0] = right.z;
		m.mat[0][1] = up.x;
		m.mat[1][1] = up.y;
		m.mat[2][1] = up.z;
		m.mat[0][2] = -forward.x;
		m.mat[1][2] = -forward.y;
		m.mat[2][2] = -forward.z;

		m.mat[3][0] = -Vec3f::dotProduct(right, from);
		m.mat[3][1] = -Vec3f::dotProduct(up, from);
		m.mat[3][2] = Vec3f::dotProduct(forward, from);

	}

	void update() {
		lookAt(pos, pos + dir, up);
	}

	// shiftX and shiftY are in pixel coordinates
	void rotateCamFromMouseMovement(int shiftX, int shiftY) {
		float rotX = -shiftX * mouseSensitivity;
		float rotY = -shiftY * mouseSensitivity;

		dir = dir.rotateAroundAxis(rotY, Vec3f::crossProduct(dir, up));
		dir = dir.rotateAroundAxis(rotX, up);
		dir = Vec3f::normalize(dir);
	}
};