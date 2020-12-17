#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>

#include "../math/vectors/vector3.h"

Mesh* CreateMesh(Vector3* vertices, Vector3* normals, const int facesCount)
{
   Mesh* resMesh = (Mesh*)(malloc(sizeof(Mesh)));
   if (!resMesh)
      return NULL;

   resMesh->VerticesCount = facesCount;

   GLuint vbo;
   GLuint nvbo;

   glGenVertexArrays(1, &resMesh->VaoId);

   glGenBuffers(1, &vbo);
   glGenBuffers(1, &nvbo);

   glBindVertexArray(resMesh->VaoId);


   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * facesCount, vertices, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

   
   glBindBuffer(GL_ARRAY_BUFFER, nvbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * facesCount, normals, GL_STATIC_DRAW);

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   return resMesh;
}