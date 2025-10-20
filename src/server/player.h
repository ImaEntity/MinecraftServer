#ifndef HH_SERVER_PLAYER
#define HH_SERVER_PLAYER

#include "../types.h"
#include "entity.h"

typedef struct {
    Entity entity;
    boolean isSneaking;
    boolean isSprinting;
} Player;

#endif