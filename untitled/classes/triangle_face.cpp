#include "triangle_face.h"
#include <algorithm>

Triangle_face::Triangle_face(int a,int b,int c){
    id[0] = a;
    id[1] = b;
    id[2] = c;
    std::sort(id,id+3);
}
