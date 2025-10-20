#ifndef HH_SERVER_ENTITY
#define HH_SERVER_ENTITY

#include "../types.h"
#include "vector.h"

typedef struct {
    int id;
    Vec3d pos;
    Vec3d vel;
} Entity;

#endif