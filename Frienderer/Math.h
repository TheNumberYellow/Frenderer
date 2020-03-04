#pragma once

#include <vector>
#include <assert.h>
#include <iomanip>

inline float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

template<typename T> T Minimum(T a, T b) { return a < b ? a : b; }
template<typename T> T Maximum(T a, T b) { return a > b ? a : b; }

template<typename T>
class Vec3;

// TODO: Make static functions for stuff like normalize etc.
// TODO: Make subscript operator direct from Matrix
template<typename T>
class Mat {
private:

	Mat multiply(Mat lhs, Mat rhs) {
		assert(lhs.cols == rhs.rows);
		Mat result(lhs.rows, rhs.cols);

		for (int i = 0; i < result.rows; i++) {

			for (int j = 0; j < result.cols; j++) {
				T r = 0;
				for (int k = 0; k < lhs.cols; k++) {
					r += lhs.mat[i][k] * rhs.mat[k][j];
				}
				result.mat[i][j] = r;
			}
		}
		return result;
	}

	void scalarMultiply(T s) {
		for (auto& outer : this->mat) {
			for (auto& inner : outer) {
				inner *= s;
			}
		}
	}

public:
	unsigned int rows, cols;
	std::vector<std::vector<T>> mat;

	Mat() {
		rows = 4;
		cols = 4;
		
		resize(4, 4);

		int shortestDim = rows < cols ? rows : cols;
		for (unsigned int i = 0; i < shortestDim; i++) {
			mat[i][i] = 1;
		}
	}

	Mat(unsigned int _rows, unsigned int _cols) {
		rows = _rows;
		cols = _cols;

		resize(_rows, _cols);

		int shortestDim = rows < cols ? rows : cols;
		for (unsigned int i = 0; i < shortestDim; i++) {
			mat[i][i] = 1;
		}
	}

	Mat(std::initializer_list<std::initializer_list<T>> list) {
		resize(list.size(), list.begin()->size());
		int i = 0, j = 0;
		for (auto itOuter : list) {
			assert(itOuter.size() == cols);
			for (auto itInner : itOuter) {
				mat[i][j] = itInner;
				j++;
			}
			j = 0;
			i++;
		}
	}
	void resize(int n, int m) {
		mat.resize(n);
		for (unsigned int i = 0; i < mat.size(); i++) {
			mat[i].resize(m, 0);
		}
		rows = n;
		cols = m;
	}
	Mat transpose() {
		Mat result(this->cols, this->rows);

		for (unsigned int i = 0; i < result.rows; i++) {
			for (unsigned int j = 0; j < result.cols; j++) {
				result.mat[i][j] = this->mat[j][i];
			}
		}
		return result;
	}

	Mat operator* (const T& s) {
		this->scalarMultiply(s);
		return *this;
	}

	Mat operator* (const Mat& v) {
		Mat result = multiply(*this, v);
		return result;
	}

	Mat& operator*= (const Mat& rhs) {
		Mat result = multiply(*this, rhs);

		mat = result.mat;
		rows = result.rows;
		cols = result.cols;

		return *this;
	}

	bool operator==(const Mat& rhs) const {
		if (this->cols != rhs.cols || this->rows != rhs.rows) {
			return false;
		}
		for (unsigned int i = 0; i < this->rows; i++) {
			for (unsigned int j = 0; j < this->cols; j++) {
				if (this->mat[i][j] != rhs.mat[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	friend std::ostream& operator << (std::ostream& s, const Mat& m) {
		for (unsigned int i = 0; i < m.rows; i++) {
			s << "[ ";
			s << std::fixed << std::setprecision(4);
			for (unsigned int j = 0; j < m.cols; j++) {
				s << m.mat[i][j];

				if (j < m.cols - 1) s << ", ";
			}
			s << " ]\n";
		}
		s << '\n';
		return s;
	}

	static Mat<float> getRotationMatrix(float radians, Vec3<T> axis) {
		Mat<float> rotMat({
			{cos(radians) + (axis.x * axis.x) * (1 - cos(radians)), axis.x * axis.y * (1 - cos(radians)) - axis.z * sin(radians), axis.x * axis.z * (1 - cos(radians)) + axis.y * sin(radians), 0},
			{axis.y * axis.x * (1 - cos(radians)) + axis.z * sin(radians), cos(radians) + (axis.y * axis.y) * (1 - cos(radians)), axis.y * axis.z * (1 - cos(radians)) - axis.x * sin(radians), 0},
			{axis.z * axis.x * (1 - cos(radians)) - axis.y * sin(radians), axis.z * axis.y * (1 - cos(radians)) + axis.x * sin(radians), cos(radians) + (axis.z * axis.z) * (1 - cos(radians)), 0},
			{0, 0, 0, 1}
			});
		return rotMat;

	}
};

template<typename T>
class Vec2 {
public:
	Vec2() : x(0), y(0) {}
	Vec2(T _x) : x(_x), y(_x) {}
	Vec2(T _x, T _y) : x(_x), y(_y) {}
	
	Vec2 operator + (const Vec2 &v) const {
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator / (const T &s) const {
		return Vec2(x / s, y / s);
	}
	Vec2 operator * (const T &s) const {
		return Vec2(x * s, y * s);
	}

	Vec2 operator /= (const T &s) {
		x /= s, y /= s; return *this;
	}
	Vec2 operator *= (const T &s) {
		x *= s, y *= s; return *this;
	}
	friend std::ostream& operator << (std::ostream& s, const Vec2<T> &v) {
		return s << '[' << v.x << ' ' << v.y << ']';
	}
	friend Vec2 operator * (const T &s, const Vec2<T> &v) {
		return Vec2(v.x * s, v.y * s);
	}
	T x, y;

};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template<typename T>
class Vec3 {
public:
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T _x) : x(_x), y(_x), z(_x) {}
	Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	Vec3 operator + (const Vec3& v) const {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}
	Vec3 operator - (const Vec3 & v) const {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}
	Vec3 operator - () const {
		return Vec3(-x, -y, -z);
	}
	Vec3 operator * (const T & s) const {
		return Vec3(x * s, y * s, z * s);
	}
	Vec3 operator / (const T & s) const {
		return Vec3(x / s, y / s, z / s);
	}

	Vec3 operator * (const Vec3 & v) const {
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3 operator * (const Mat<T> & m) const {
		if (m.cols == 4) {
			Mat<float> vecAsMat({ {x, y, z, 1} });
			Mat<float> result = vecAsMat * m;
			return Vec3(result.mat[0][0] / result.mat[0][3], result.mat[0][1] / result.mat[0][3], result.mat[0][2] / result.mat[0][3]);
		}
		else {
			Mat<float> vecAsMat({ {x, y, z} });

			Mat<float> result = vecAsMat * m;

			return Vec3(result.mat[0][0], result.mat[0][1], result.mat[0][2]);

		}
	}

	static T dotProduct(const Vec3<T> &left, const Vec3<T> &right) {
		return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
	}
	Vec3& operator /= (const T & s) {
		x /= s, y /= s, z /= s; return *this;
	}
	Vec3& operator *= (const T & s) {
		x *= s, y *= s, z *= s; return *this;
	}


	static Vec3 crossProduct(const Vec3<T> &left, const Vec3<T> &right) {
		return Vec3<T>((left.y * right.z) - (left.z * right.y), (left.z * right.x) - (left.x * right.z), (left.x * right.y) - (left.y * right.x));
	}
	T norm() const {
		return (x * x) + (y * y) + (z * z);
	}
	T length() const {
		return sqrt(norm());
	}
	const T& operator [] (uint8_t i) const { return (&x)[i]; }
	T& operator [] (uint8_t i) { return (&x)[i]; }

	static Vec3 normalize(const Vec3<T> &vec) {
		T n = vec.norm();
		Vec3 result = vec;

		if (n > 0) {
			T factor = 1 / sqrt(n);
			result.x *= factor, result.y *= factor, result.z *= factor;
		}

		return result;
	}

	friend Vec3 operator * (const T & s, const Vec3 & v) {
		return Vec3<T>(v.x * s, v.y * s, v.z * s);
	}
	friend Vec3 operator / (const T & s, const Vec3 & v) {
		return Vec3<T>(s / v.x, s / v.y, s / v.z);
	}

	bool operator==(const Vec3 & rhs) const {
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}

	friend std::ostream& operator << (std::ostream & s, const Vec3<T> & v) {
		return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
	}

	Vec3 rotateAroundX(float radians) {
		Mat<float> rotMat({
			{1, 0, 0},
			{0, cos(radians), -sin(radians)},
			{0, sin(radians), cos(radians)},
		});
		return *this * rotMat;
	}
	Vec3 rotateAroundY(float radians) {
		Mat<float> rotMat({
			{cos(radians), 0, sin(radians)},
			{0, 1, 0},
			{-sin(radians), 0, cos(radians)},
		});
		return *this * rotMat;
	}
	Vec3 rotateAroundZ(float radians) {
		Mat<float> rotMat({
			{cos(radians), -sin(radians), 0},
			{sin(radians), cos(radians), 0},
			{0, 0, 1},
		});
		return *this * rotMat;
	}
	Vec3 rotateAroundAxis(float radians, Vec3 axis) {
		Mat<T> rotMat({
			{cos(radians) + (axis.x * axis.x) * (1 - cos(radians)), axis.x * axis.y * (1 - cos(radians)) - axis.z * sin(radians), axis.x * axis.z * (1 - cos(radians)) + axis.y * sin(radians)},
			{axis.y * axis.x * (1 - cos(radians)) + axis.z * sin(radians), cos(radians) + (axis.y * axis.y) * (1 - cos(radians)), axis.y * axis.z * (1 - cos(radians)) - axis.x * sin(radians)},
			{axis.z * axis.x * (1 - cos(radians)) - axis.y * sin(radians), axis.z * axis.y * (1 - cos(radians)) + axis.x * sin(radians), cos(radians) + (axis.z * axis.z) * (1 - cos(radians))}
		});
		return *this * rotMat;
	}



	T x, y, z;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;