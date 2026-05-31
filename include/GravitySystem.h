#ifndef GRAVITY_SYSTEM_H
#define GRAVITY_SYSTEM_H

#include "Character.h"
#include "TileMap.h"

// Mantiene a los personajes apoyados sobre tiles solidos y corrige casos donde
// una entidad queda metida en el suelo por movimiento o salto.
class GravitySystem {
public:
    // Aplica caida, detecta suelo y corrige penetracion con tiles solidos.
    void applyGravity(Character* character, TileMap* map);
};

#endif
