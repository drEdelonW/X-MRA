#pragma once
#include "Vector3d.hpp"

#define TRI_NUM (3)
struct Triangle3D {
  public:
    Vector3D vertices[TRI_NUM];

    Triangle3D()
        : vertices{Vector3D(), Vector3D(), Vector3D()} {}

    Triangle3D(
        const Vector3D& v1,
        const Vector3D& v2,
        const Vector3D& v3
    ) : vertices{v1, v2, v3} {}

    bool intersectsWith(const Triangle3D& other) const;
    bool isPointInProjectionZ(const Vector3D& point) const;
    Vector3D getEdge(int index) const; // Helper functions for the intersection algorithm

    // Triangle3D(const Vector3D verts[TRI_NUM]);

    const Vector3D& operator[](int index) const;
          Vector3D& operator[](int index);
    void toFloatArray(float* arr) const;
    void printVertices() const;

  private:
    void projectOntoAxis(const Triangle3D& triangle, const Vector3D& axis, float& min, float& max) const;
    bool overlapOnAxis(const Triangle3D& t2, const Vector3D& axis) const;
    bool boundingBoxOverlap(const Triangle3D& other) const;
};
