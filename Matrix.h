#pragma once
#define _USE_MATH_DEFINES
#define SQ(x) (x) * (x)
#include <cmath>
#include <iostream>
#define max(a,b) ((a) > (b) ? (a) : (b))

using namespace std;
template<typename T>
static T lerp(const T a, const T b, float t)
{
    return a * (1.0f - t) + (b * t);
}

class Vec3
{
public:
    union
    {
        float v[3];
        struct { float x, y, z; };
    };

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& pVec) const
    {
        return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
    }

    Vec3& operator+=(const Vec3& pVec)
    {
        v[0] += pVec.v[0];
        v[1] += pVec.v[1];
        v[2] += pVec.v[2];
        return *this;
    }

    Vec3 operator-(const Vec3& pVec) const
    {
        return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
    }

    Vec3& operator-=(const Vec3& pVec)
    {
        v[0] -= pVec.v[0];
        v[1] -= pVec.v[1];
        v[2] -= pVec.v[2];
        return *this;
    }

    Vec3 operator*(const Vec3& pVec) const
    {
        return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
    }

    Vec3& operator*=(const Vec3& pVec)
    {
        v[0] *= pVec.v[0];
        v[1] *= pVec.v[1];
        v[2] *= pVec.v[2];
        return *this;
    }

    Vec3 operator*(const float val) const
    {
        return Vec3(v[0] * val, v[1] * val, v[2] * val);
    }

    Vec3& operator*=(const float val)
    {
        v[0] *= val;
        v[1] *= val;
        v[2] *= val;
        return *this;
    }

    Vec3 operator/(const Vec3& pVec) const
    {
        return Vec3(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);
    }

    Vec3& operator/=(const Vec3& pVec)
    {
        v[0] /= pVec.v[0];
        v[1] /= pVec.v[1];
        v[2] /= pVec.v[2];
        return *this;
    }

    Vec3 operator/(const float val) const
    {
        return Vec3(v[0] / val, v[1] / val, v[2] / val);
    }

    Vec3& operator/=(const float val)
    {
        v[0] /= val;
        v[1] /= val;
        v[2] /= val;
        return *this;
    }

    Vec3 operator-() const
    {
        return Vec3(-v[0], -v[1], -v[2]);
    }

    Vec3 normalize(void) const
    {
        float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        return Vec3(x * len, y * len, z * len);
    }

    float normalize_GetLength()
    {
        float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        float len = 1.0f / length;
        v[0] *= len; v[1] *= len; v[2] *= len;
        return length;
    }

    float Dot(const Vec3& pVec) const
    {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
    }

    Vec3 Cross(const Vec3& v1)
    {
        return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
            v1.v[2] * v[0] - v1.v[0] * v[2],
            v1.v[0] * v[1] - v1.v[1] * v[0]);
    }

    Vec3 Max(const Vec3& v1, const Vec3& v2)
    {
        return Vec3(max(v1.v[0], v2.v[0]),
            max(v1.v[1], v2.v[1]),
            max(v1.v[2], v2.v[2]));
    }

    float Max() const
    {
        return max(x, max(y, z));
    }

};


class Vec4 {
public:
    float x, y, z, w;

    // Constructors
    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Operations
    Vec4 operator+(const Vec4& v) const {
        return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    Vec4 operator-(const Vec4& v) const {
        return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    Vec4 operator*(float scalar) const {
        return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Vec4 operator/(float scalar) const {
        return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // Dot product
    float Dot(const Vec4& v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    // Length
    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    // Normalize
    Vec4 Normalize() const {
        float len = Length();
        return Vec4(x / len, y / len, z / len, w / len);
    }
};


class Matrix
{
private:
    // Helper to check if the matrix is a projection matrix
    bool isProjection() const
    {
        // Simple check for the 4th row
        return m[12] != 0.0f || m[13] != 0.0f || m[14] != 0.0f || m[15] != 1.0f;
    }

public:
    union
    {
        float a[4][4];
        float m[16];
    };

    // Default constructor (Identity matrix)
    Matrix()
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                a[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    //initialization
    Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
        a[0][0] = m00;
        a[0][1] = m01;
        a[0][2] = m02;
        a[0][3] = m03;
        a[1][0] = m10;
        a[1][1] = m11;
        a[1][2] = m12;
        a[1][3] = m13;
        a[2][0] = m20;
        a[2][1] = m21;
        a[2][2] = m22;
        a[2][3] = m23;
        a[3][0] = m30;
        a[3][1] = m31;
        a[3][2] = m32;
        a[3][3] = m33;
    }

    //create an identity matrix
    static Matrix Identity()
    {
        return Matrix();
    }

    // Multiply a point by this matrix (auto-detect if projection is needed)
    Vec3 mulPoint(const Vec3& v) const
    {
        if (isProjection())
        {
            // Projection matrix logic
            Vec3 v1 = Vec3(
                (v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
                (v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
                (v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
            float w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
            w = 1.0f / w;
            return (v1 * w);
        }
        else
        {
            // Non-projection matrix logic
            return Vec3(
                (v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
                (v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
                (v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
        }
    }

    //Sometimes we only want rotation:
    Vec3 mulVec(const Vec3& v)
    {
        return Vec3(
            (v.x * m[0] + v.y * m[1] + v.z * m[2]),
            (v.x * m[4] + v.y * m[5] + v.z * m[6]),
            (v.x * m[8] + v.y * m[9] + v.z * m[10]));
    }

    Matrix mul(const Matrix& matrix) const
    {
        Matrix ret;
        ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
        ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
        ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
        ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
        ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
        ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
        ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
        ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
        ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
        ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
        ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
        ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
        ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
        ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
        ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
        ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
        return ret;
    }

    Matrix operator*(const Matrix& matrix)
    {
        return mul(matrix);
    }

    // Add rotation around X axis
    Matrix RotationX(float angle)
    {
        float c = cos(angle);
        float s = sin(angle);
        Matrix rot;
        rot.m[5] = c;
        rot.m[6] = -s;
        rot.m[9] = s;
        rot.m[10] = c;
        return rot;
    }

    // Add rotation around Y axis
    Matrix RotationY(float angle)
    {
        float c = cos(angle);
        float s = sin(angle);
        Matrix rot;
        rot.m[0] = c;
        rot.m[2] = s;
        rot.m[8] = -s;
        rot.m[10] = c;
        return rot;
    }

    // Add rotation around Z axis
    Matrix RotationZ(float angle)
    {
        float c = cos(angle);
        float s = sin(angle);
        Matrix rot;
        rot.m[0] = c;
        rot.m[1] = -s;
        rot.m[4] = s;
        rot.m[5] = c;
        return rot;
    }

    // Add translation
    static Matrix Translation(const Vec3& v)
    {
        Matrix mat;
        mat.a[0][3] = v.x;
        mat.a[1][3] = v.y;
        mat.a[2][3] = v.z;
        return mat;
    }

    // Add scaling
    static Matrix Scaling(const Vec3& v)
    {
        Matrix mat;
        mat.m[0] = v.x;
        mat.m[5] = v.y;
        mat.m[10] = v.z;
        return mat;
    }

    // Method to create transformation matrix from orthonormal basis and position
    static Matrix Transform(const Vec3& u, const Vec3& n, const Vec3& v, const Vec3& p)
    {
        Matrix transform = Matrix::Identity();
        transform.m[0] = u.x; transform.m[1] = n.x; transform.m[2] = v.x; transform.m[3] = p.x;
        transform.m[4] = u.y; transform.m[5] = n.y; transform.m[6] = v.y; transform.m[7] = p.y;
        transform.m[8] = u.z; transform.m[9] = n.z; transform.m[10] = v.z; transform.m[11] = p.z;
        transform.m[12] = 0.0f; transform.m[13] = 0.0f; transform.m[14] = 0.0f; transform.m[15] = 1.0f;

        return transform;
    }

    // Transpose
    Matrix Transpose()
    {
        return Matrix(a[0][0], a[1][1], a[2][0], a[3][0],
            a[0][1], a[1][1], a[2][1], a[3][1],
            a[0][2], a[1][2], a[2][2], a[3][2],
            a[0][3], a[1][3], a[2][3], a[3][3]);
    }

    float& operator[](int index) {
        return m[index];
    }

    // Invert
    Matrix invert() const
    {
        Matrix inv;
        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        if (det == 0) {
            throw std::runtime_error("Matrix is singular and cannot be inverted.");
        }
        det = 1.0f/ det;
        for (int i = 0; i < 16; i++) {
            inv[i] = inv[i] * det;
        }
        return inv;
    }

    Matrix operator*(const Matrix& other) const {
        return mul(other);
    }

    Matrix lookAt(Vec3& from, Vec3& to, Vec3& up) {
        Vec3 dir = (from - to).normalize();
        Vec3 right = up.Cross(dir).normalize();
        Vec3 upPrime = dir.Cross(right);
        Matrix result;

        result.m[0] = right.x;
        result.m[1] = right.y;
        result.m[2] = right.z;

        result.m[4] = upPrime.x;
        result.m[5] = upPrime.y;
        result.m[6] = upPrime.z;

        result.m[8] = dir.x;
        result.m[9] = dir.y;
        result.m[10] = dir.z;

        result.m[3] = -from.Dot(right);
        result.m[7] = -from.Dot(upPrime);
        result.m[11] = -from.Dot(dir);

        result.m[12] = 0.f;
        result.m[13] = 0.f;
        result.m[14] = 0.f;
        result.m[15] = 1.f;

        return result;
    }

      Matrix Perspective(float fovY, float aspectRatio, float nearPlane, float farPlane) {
        float tanHalfFovy = tan(fovY / 2.0f);

        Matrix result;
        for (int i = 0; i < 16; ++i) result.m[i] = 0.0f;

        result.m[0] = 1.0f / (aspectRatio * tanHalfFovy);
        result.m[5] = 1.0f / tanHalfFovy;
        result.m[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result.m[11] = -1.0f;
        result.m[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

        return result;
    }

};



class SphericalCoordinates {
public:
    float theta; // Polar angle (radians)
    float phi;   // Azimuthal angle (radians)
    float r;     // Radius (default to 1 if not specified)

    // Default constructor
    SphericalCoordinates() : theta(0.0f), phi(0.0f), r(1.0f) {}

    // Constructor from spherical coordinates
    SphericalCoordinates(float t, float p, float radius = 1.0f) : theta(t), phi(p), r(radius) {}

    // Static method: Convert Cartesian (x, y, z) to Spherical (theta, phi, r)
    static SphericalCoordinates FromCartesian(float x, float y, float z) {
        float radius = std::sqrt(x * x + y * y + z * z);
        float polar = std::acos(z / radius); // theta: angle from z-axis
        float azimuth = std::atan2(y, x);    // phi: angle in xy-plane
        return SphericalCoordinates(polar, azimuth, radius);
    }

    // Static method: Convert Spherical (theta, phi, r) to Cartesian (x, y, z)
    static void ToCartesian(const SphericalCoordinates& sc, float& x, float& y, float& z) {
        x = sc.r * std::sin(sc.theta) * std::cos(sc.phi);
        y = sc.r * std::sin(sc.theta) * std::sin(sc.phi);
        z = sc.r * std::cos(sc.theta);
    }
};



class Quaternion {
public:
    union {
        struct {
            float x; // Represents the i component
            float y; // Represents the j component
            float z; // Represents the k component
            float w; // Scalar component
        };
        float q[4]; // Array representation
    };

    // Default constructor (identity quaternion)
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

    // Constructor with explicit values
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Quaternion addition
    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
    }

    // Quaternion multiplication
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }

    // Subtract two quaternions
    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
    }

    // Multiply quaternion by a scalar
    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Normalize the quaternion
    void Normalize() {
        float magnitude = std::sqrt(x * x + y * y + z * z + w * w);
        if (magnitude > 0.0f) {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            w /= magnitude;
        }
    }

    // Spherical linear interpolation (SLERP)
    static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        Quaternion q1_norm = q1;
        Quaternion q2_norm = q2;
        q1_norm.Normalize();
        q2_norm.Normalize();

        float dot = q1_norm.x * q2_norm.x + q1_norm.y * q2_norm.y + q1_norm.z * q2_norm.z + q1_norm.w * q2_norm.w;

        // If the dot product is negative, slerp won't take the shorter path.
        if (dot < 0.0f) {
            q2_norm.x = -q2_norm.x;
            q2_norm.y = -q2_norm.y;
            q2_norm.z = -q2_norm.z;
            q2_norm.w = -q2_norm.w;
            dot = -dot;
        }

        const float DOT_THRESHOLD = 0.9995f;
        if (dot > DOT_THRESHOLD) {
            // Linear interpolation for very close quaternions
            Quaternion result = q1_norm + (q2_norm - q1_norm) * t;
            result.Normalize();
            return result;
        }

        float theta_0 = std::acos(dot);      // Initial angle between q1 and q2
        float theta = theta_0 * t;          // Interpolated angle
        float sin_theta = std::sin(theta);
        float sin_theta_0 = std::sin(theta_0);

        float s1 = std::cos(theta) - dot * sin_theta / sin_theta_0;
        float s2 = sin_theta / sin_theta_0;

        return (q1_norm * s1) + (q2_norm * s2);
    }

    // Convert quaternion to rotation matrix
    Matrix ToMatrix() const {
        float xx = q[0] * q[0], xy = q[0] * q[1], xz = q[0] * q[2];
        float yy = q[1] * q[1], zz = q[2] * q[2], yz = q[1] * q[2];
        float wx = q[3] * q[0], wy = q[3] * q[1], wz = q[3] * q[2];

        Matrix matrix;
        matrix[0] = 1 - 2 * (yy + zz);
        matrix[1] = 2 * (xy - wz);
        matrix[2] = 2 * (xz + wy);
        matrix[3] = 0;

        matrix[4] = 2 * (xy + wz);
        matrix[5] = 1 - 2 * (xx + zz);
        matrix[6] = 2 * (yz - wx);
        matrix[7] = 0;

        matrix[8] = 2 * (xz - wy);
        matrix[9] = 2 * (yz + wx);
        matrix[10] = 1 - 2 * (xx + yy);
        matrix[11] = 0;

        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        matrix[15] = 1;

        return matrix;
    }
};

class ShadingFrame {
public:
    Vec3 u; // Tangent
    Vec3 v; // Bitangent
    Vec3 w; // Normal

    // 构造函数：使用 Gram-Schmidt 方法从一个向量构建基
    explicit ShadingFrame(const Vec3& normal) {
        // 创建临时对象以调用 normalize
        Vec3 tempNormal = normal;
        w = tempNormal.normalize(); // 设置法线向量为归一化结果

        // 找到一个与 w 不共线的向量
        Vec3 temp = (fabs(w.x) > 0.9f) ? Vec3(0.0f, 1.0f, 0.0f) : Vec3(1.0f, 0.0f, 0.0f);

        // 使用 Gram-Schmidt 正交化计算 u 和 v
        u = (temp.Cross(w)).normalize(); // u 与 w 垂直并归一化
        v = w.Cross(u);                  // v 与 u、w 都垂直
    }

    // 前向变换：将局部坐标转换为世界坐标
    Vec3 ToWorld(const Vec3& local) const {
        return u * local.x + v * local.y + w * local.z;
    }

    // 逆向变换：将世界坐标转换为局部坐标
    Vec3 ToLocal(const Vec3& world) const {
        return Vec3(world.Dot(u), world.Dot(v), world.Dot(w));
    }
};

class Colour {
public:
    float r, g, b, a;

    Colour() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}


    Colour(float red, float green, float blue, float alpha = 1.0f) : r(red), g(green), b(blue), a(alpha) {}

 
    Colour(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) {
        r = red / 255.0f;
        g = green / 255.0f;
        b = blue / 255.0f;
        a = alpha / 255.0f;
    }

    Colour operator+(const Colour& colour) const {
        return Colour(r + colour.r, g + colour.g, b + colour.b, a + colour.a);
    }

    // 乘法运算符（颜色与颜色）
    Colour operator*(const Colour& colour) const {
        return Colour(r * colour.r, g * colour.g, b * colour.b, a * colour.a);
    }

    // 乘法运算符（颜色与标量）
    Colour operator*(const float scalar) const {
        return Colour(r * scalar, g * scalar, b * scalar, a * scalar);
    }

    // 除法运算符（颜色与标量）
    Colour operator/(const float scalar) const {
        return Colour(r / scalar, g / scalar, b / scalar, a / scalar);
    }
};

class HomogeneousVector : public Vec4 {
public:
    // 默认构造函数
    using Vec4::Vec4;
    HomogeneousVector() : Vec4() {}

    // 带参数的构造函数
    HomogeneousVector(float x, float y, float z, float w) : Vec4(x, y, z, w) {}

    // 实现透视除法
    void PerspectiveDivide() {
        if (w != 0.0f) {
            x /= w;
            y /= w;
            z /= w;
            w = 1.0f; // 通常在除法后将 w 设置为 1.0
        }
        else {
            throw std::runtime_error(" PerspectiveDivide error");
        }
    }

    // 重载除法运算符，用于支持标量除法
    HomogeneousVector operator/(float scalar) const {
        return HomogeneousVector(x / scalar, y / scalar, z / scalar, w / scalar);
    }
};