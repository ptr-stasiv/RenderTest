#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>

#include "../math/vectors/vector3.h"

Mesh* CreateMesh(Vector3* vertices, const int verticesCount)
{
   Mesh* resMesh = (Mesh*)(malloc(sizeof(Mesh)));
   if (!resMesh)
      return NULL;

   resMesh->VerticesCount = verticesCount;

   GLuint vbo;

   glGenVertexArrays(1, &resMesh->VaoId);
   glGenBuffers(1, &vbo);

   glBindVertexArray(resMesh->VaoId);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * verticesCount, vertices, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   return resMesh;
}