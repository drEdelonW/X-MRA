#pragma once
#include <stdint.h>
#include "Triangle3d.hpp"
#include "Matrix4x4.hpp"


#define IDX_t   uint32_t
#define DIMOF(original) (sizeof(original) / sizeof(original[0]))

class Mesh {
private:
    const IDX_t     _vertCnt;
    const Vector3D* _origVertices;
          Vector3D*   _trVertices;
    const IDX_t     _triCnt;
    const IDX_t   (*_triangles)[3];

public:
    float drawColors [3][3];    // GL specific!!!

    Mesh(
        const IDX_t vertexCount,
        const Vector3D* vertices,
              Vector3D* verticesTrBuf,
        const IDX_t triangleCount,
        const IDX_t (*triangles)[3]
    );
    Mesh(
        const IDX_t vertexCount,
        const Vector3D* vertices,
        const IDX_t triangleCount,
        const IDX_t (*triangles)[3]
    );

    const Vector3D& getVertex(IDX_t index) const;
         Triangle3D getTriangle(IDX_t index) const;
    IDX_t getVertexCount()   const { return _vertCnt; }
    IDX_t getTriangleCount() const { return _triCnt; }

    bool applyTransformation(const Matrix4x4& matrix);
    // Method to calculate the bounding box (min and max)
    void getBoundingBox(Vector3D& min, Vector3D& max) const;
};
