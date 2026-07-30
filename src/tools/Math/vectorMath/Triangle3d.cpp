#include "Triangle3d.hpp"
// #include <math.h>
#include "common_tools.h"

// Function to project a triangle onto an axis and find min/max projections
void Triangle3D::projectOntoAxis(
    const Triangle3D& triangle,
    const Vector3D& axis,
    float& min, float& max
) const {
    min = max = triangle.vertices[0].dot(axis);
    for (int idx = 1; idx < TRI_NUM; ++idx) {
        float projection = triangle.vertices[idx].dot(axis);
        /* */if (min > projection) { min = projection; }
        else if (max < projection) { max = projection; }
    }
}

// Function to check if two projections overlap
bool Triangle3D::overlapOnAxis(const Triangle3D& t2, const Vector3D& axis) const {
    float min1, max1, min2, max2;
    projectOntoAxis(*this, axis, min1, max1);
    projectOntoAxis(t2, axis, min2, max2);
    return !(
        (max1 < min2) ||
        (max2 < min1)
        );
}

bool Triangle3D::boundingBoxOverlap(const Triangle3D& other) const {
    {
        float t1_max = vertices[0].x;
        float t1_min = t1_max;
        float t2_max = other.vertices[0].x;
        float t2_min = t2_max;

        for (int idx = 1; idx < TRI_NUM; ++idx) {
            /**/ if (t1_min > vertices[idx].x) { t1_min = vertices[idx].x; }
            else if (t1_max < vertices[idx].x) { t1_max = vertices[idx].x; }

            /**/ if (t2_min > other.vertices[idx].x) { t2_min = other.vertices[idx].x; }
            else if (t2_max < other.vertices[idx].x) { t2_max = other.vertices[idx].x; }
        }

        if ((t1_max < t2_min) ||
            (t2_max < t1_min)
            )   return false;
    }

    {
        float t1_max = vertices[0].y;
        float t1_min = t1_max;
        float t2_max = other.vertices[0].y;
        float t2_min = t2_max;

        for (int idx = 1; idx < TRI_NUM; ++idx) {
            /**/ if (t1_min > vertices[idx].y) { t1_min = vertices[idx].y; }
            else if (t1_max < vertices[idx].y) { t1_max = vertices[idx].y; }

            /**/ if (t2_min > other.vertices[idx].y) { t2_min = other.vertices[idx].y; }
            else if (t2_max < other.vertices[idx].y) { t2_max = other.vertices[idx].y; }
        }

        if ((t1_max < t2_min) ||
            (t2_max < t1_min)
            )   return false;

    }
    {
        float t1_max = vertices[0].z;
        float t1_min = t1_max;
        float t2_max = other.vertices[0].z;
        float t2_min = t2_max;

        for (int idx = 1; idx < TRI_NUM; ++idx) {
            /**/ if (t1_min > vertices[idx].z) { t1_min = vertices[idx].z; }
            else if (t1_max < vertices[idx].z) { t1_max = vertices[idx].z; }

            /**/ if (t2_min > other.vertices[idx].z) { t2_min = other.vertices[idx].z; }
            else if (t2_max < other.vertices[idx].z) { t2_max = other.vertices[idx].z; }
        }

        if ((t1_max < t2_min) ||
            (t2_max < t1_min)
            )   return false;
    }
    return true;
}

// Method to check if triangles intersect using SAT
bool Triangle3D::intersectsWith(const Triangle3D& other) const {
    if (!boundingBoxOverlap(other))
        return false;

    Vector3D normal1 = getEdge(0).cross(getEdge(1));  // normal of this triangle
    Vector3D normal2 = other.getEdge(0).cross(other.getEdge(1));  // normal of the other triangle
    int axisCount = 2;
#if 0
    Vector3D axes[TRI_NUM * TRI_NUM + 2];

    // Triangle face normals
    axes[axisCount++] = normal1;
    axes[axisCount++] = normal2;

    // Cross products of edges from both triangles
    for (int idx1 = 0; idx1 < TRI_NUM; ++idx1) {
        for (int idx2 = 0; idx2 < TRI_NUM; ++idx2) {
            axes[axisCount++] =
                getEdge(idx1).cross(
                    other.getEdge(idx2)
                );
        }
    }
#else
    Vector3D axes[7];

    if (!normal1.isZero()) { axes[axisCount++] = normal1; }
    if (!normal2.isZero()) { axes[axisCount++] = normal2; }

    // Cross products of edges from both triangles
    for (int idx1 = 0; idx1 < TRI_NUM; ++idx1) {
        for (int idx2 = 0; idx2 < TRI_NUM; ++idx2) {
            Vector3D axis =
                getEdge(idx1).cross(
                    other.getEdge(idx2)
                );
            if (!axis.isZero()) {
                axes[axisCount++] = axis;
                if (axisCount == 7) break; // Max 7 Axes
            }
        }
    }

#endif
    // Check for overlap on each axis
    for (int idx = 0; idx < axisCount; ++idx) {
        if (!overlapOnAxis(other, axes[idx])) {
            return false;  // Separating axis found, no intersection
        }
    }
    return true;  // No separating axis found, triangles intersect
}

bool Triangle3D::isPointInProjectionZ(const Vector3D& point) const {
    if (
        !(
            (vertices[0].z > 0.f) ||
            (vertices[0].z > 0.f) ||
            (vertices[0].z > 0.f)
            )
        )   return false;
    // Project the vertices onto the XOY plane
    float x1 = vertices[0].x, y1 = vertices[0].y;
    float x2 = vertices[1].x, y2 = vertices[1].y;
    float x3 = vertices[2].x, y3 = vertices[2].y;
    float px = point.x, py = point.y;

    // Calculate the area of the full triangle
    float fullArea = 0.5f * fabsf((x1 * (y2 - y3)) + (x2 * (y3 - y1)) + (x3 * (y1 - y2)));

    // Calculate areas with the point
    float area1 = 0.5f * fabsf((px * (y2 - y3)) + (x2 * (y3 - py)) + (x3 * (py - y2)));
    float area2 = 0.5f * fabsf((x1 * (py - y3)) + (px * (y3 - y1)) + (x3 * (y1 - py)));
    float area3 = 0.5f * fabsf((x1 * (y2 - py)) + (x2 * (py - y1)) + (px * (y1 - y2)));

    // If the sum of areas equals the full area, the point lies within or on the boundary
    float sumAreas = area1 + area2 + area3;
    return
        (sumAreas >= (fullArea - 1e-5f)) &&
        (sumAreas <= (fullArea + 1e-5f));
}

Vector3D Triangle3D::getEdge(int index) const {
    return vertices[(index + 1) % TRI_NUM] - vertices[index];
}

#if 0
Triangle3D::Triangle3D(const Vector3D verts[TRI_NUM]) {
    for (int idx = 0; idx < TRI_NUM; ++idx)
        vertices[idx] = verts[idx];
}
#endif

const Vector3D& Triangle3D::operator[](int index) const {
    return vertices[index % TRI_NUM];
}

Vector3D& Triangle3D::operator[](int index) {
    return vertices[index % TRI_NUM];
}

void Triangle3D::toFloatArray(float* arr) const {
    for (int idx = 0; idx < TRI_NUM; ++idx) {
        arr[idx * TRI_NUM] = vertices[idx].x;
        arr[idx * TRI_NUM + 1] = vertices[idx].y;
        arr[idx * TRI_NUM + 2] = vertices[idx].z;
    }
}

void Triangle3D::printVertices() const {
    for (int idx = 0; idx < TRI_NUM; ++idx) {
        LOG(
            "Vertex[%d]: "
            "x[%.3f] "
            "y[%.3f] "
            "z[%.3f]\n",
            idx + 1,
            vertices[idx].x,
            vertices[idx].y,
            vertices[idx].z
        );
    }
}

// Определяем структуру для точки
typedef struct {
    float x;
    float y;
} Point;

// Функция для вычисления знака площади треугольника
float sign(Point p1, Point p2, Point p3) {
    return
        (p1.x - p3.x) * (p2.y - p3.y) -
        (p2.x - p3.x) * (p1.y - p3.y);
}

// Функция для проверки принадлежности точки треугольнику
int is_point_in_triangle(Point pt, Point v1, Point v2, Point v3) {
    float d1 = sign(pt, v1, v2);
    float d2 = sign(pt, v2, v3);
    float d3 = sign(pt, v3, v1);

    int has_neg = (d1 < 0.f) || (d2 < 0.f) || (d3 < 0.f);
    int has_pos = (d1 > 0.f) || (d2 > 0.f) || (d3 > 0.f);

    return !(has_neg && has_pos);
}
