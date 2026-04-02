#ifndef TRIANGLE_FACE_H
#define TRIANGLE_FACE_H

#include <cstddef>

#include <bits/functional_hash.h>

class Triangle_face
{
public:
    int id[3];
    Triangle_face(int a,int b,int c);

    bool operator==(const Triangle_face &other) const{
        return
            id[0] == other.id[0] &&
            id[1] == other.id[1] &&
            id[2] == other.id[2];
    }
};

struct Triangle_face_hash {
    std::size_t operator()(const Triangle_face& t) const {
        std::size_t h1 = std::hash<int>{}(t.id[0]);
        std::size_t h2 = std::hash<int>{}(t.id[1]);
        std::size_t h3 = std::hash<int>{}(t.id[2]);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
#endif // TRIANGLE_FACE_H
