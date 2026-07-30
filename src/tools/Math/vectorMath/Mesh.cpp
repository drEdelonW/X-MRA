#include "Mesh.hpp"
#include <cstring>

Mesh::Mesh(
    IDX_t vertexCount,
    const Vector3D_p verticesArray,
          Vector3D_p verticesCashArray,
    IDX_t triangleCount,
    const IDX_t (*trianglesArray)[3]
) :
    _vertCnt(vertexCount),
    _origVertices(verticesArray),
    _trVertices(verticesCashArray),
    _triCnt(triangleCount),
    _triangles(trianglesArray)
{
    float defaultColors[3][3] = {   // GL specific!!!
        {1.0f, 0.75f, 0.75f},  // Red
        {0.75f, 1.0f, 0.75f},  // Green
        {0.75f, 0.75f, 1.0f}   // Blue
    };

    memcpy(drawColors, defaultColors, sizeof(drawColors));

    if (_trVertices != nullptr) {
        Matrix4x4 identityMatrix;  // Единичная матрица
        applyTransformation(identityMatrix);
    }
}

Mesh::Mesh(
    IDX_t vertexCount,
    const Vector3D_p verticesArray,
    IDX_t triangleCount,
    const IDX_t (*trianglesArray)[3]
) : Mesh(
        vertexCount,
        verticesArray,
        nullptr,
        triangleCount,
        trianglesArray
    ){}

const Vector3D& Mesh::getVertex(IDX_t index) const {
    if (index < _vertCnt) {
        return (_trVertices != nullptr)?
            _trVertices[index]:
            _origVertices[index];
    }
    static Vector3D dummyVertex;
    return dummyVertex;
}

Triangle3D Mesh::getTriangle(IDX_t index) const {
    if (index < _triCnt) {
        const IDX_t* triIndices = _triangles[index];
        if (_trVertices != nullptr) {
            return
                Triangle3D(
                    _trVertices[triIndices[0]],
                    _trVertices[triIndices[1]],
                    _trVertices[triIndices[2]]
                );
        } else {
            return
                Triangle3D(
                    _origVertices[triIndices[0]],
                    _origVertices[triIndices[1]],
                    _origVertices[triIndices[2]]
                );
        }
    }

    return Triangle3D();
}

bool Mesh::applyTransformation(const Matrix4x4& matrix) {
    if (_trVertices != nullptr) {
        for (IDX_t i = 0; i < _vertCnt; ++i) {
            _trVertices[i] = matrix.applyTransform(_origVertices[i]);
        }
        return true;
    }
    return false;
}

void Mesh::getBoundingBox(Vector3D& min, Vector3D& max) const {
        if (_vertCnt == 0) {
            return;
        }

        // Initialize min and max with the first vertex
        min = _origVertices[0];
        max = _origVertices[0];

        // Iterate over all vertices and update min and max
        for (IDX_t i = 1; i < _vertCnt; ++i) {
            const Vector3D& v = _origVertices[i];
            if (v.x < min.x) min.x = v.x;
            if (v.y < min.y) min.y = v.y;
            if (v.z < min.z) min.z = v.z;

            if (v.x > max.x) max.x = v.x;
            if (v.y > max.y) max.y = v.y;
            if (v.z > max.z) max.z = v.z;
        }
    }