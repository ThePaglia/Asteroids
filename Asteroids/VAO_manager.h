#pragma once
#include "geometry.h"
#include "entity.h"

void initShapeVAO(Entity* entity);

void updateShapeVAO(Entity* entity);

void initTextVAO(GLuint* textVAO, GLuint* textVBO);