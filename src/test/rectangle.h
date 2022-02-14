#ifndef RECTANGLE_H
#define RECTANGLE_H

void createRectangle(float topLeftX, float topLeftY, float topLeftZ,
  float bottomRightX, float bottomRightY, float bottomRightZ, float* vertexData,
  uint32_t* indexData, float* textureCoordsData) {

  vertexData = malloc(16 * sizeof(float));

  vertexData[0] = bottomRightX;
  vertexData[1] = bottomRightY;
  vertexData[2] = bottomRightZ;
  vertexData[3] = 1.0f;
  vertexData[4] = bottomRightX;
  vertexData[5] = topLeftY;
  vertexData[6] = topLeftZ;
  vertexData[7] = 1.0f;
  vertexData[8] = topLeftX;
  vertexData[9] = topLeftY;
  vertexData[10] = topLeftZ;
  vertexData[11] = 1.0f;
  vertexData[12] = topLeftX;
  vertexData[13] = bottomRightY;
  vertexData[14] = bottomRightZ;
  vertexData[15] = 1.0f;

  indexData = malloc(6 * sizeof(uint32_t));

  indexData[0] = 0;
  indexData[1] = 1;
  indexData[2] = 2;
  indexData[3] = 2;
  indexData[4] = 3;
  indexData[5] = 0;

  textureCoordsData = malloc(8 * sizeof(float));
  textureCoordsData[0] = 1.0f;
  textureCoordsData[1] = 1.0f;
  textureCoordsData[2] = 1.0f;
  textureCoordsData[3] = 0.0f;
  textureCoordsData[4] = 0.0f;
  textureCoordsData[5] = 0.0f;
  textureCoordsData[6] = 0.0f;
  textureCoordsData[7] = 1.0f;

}

#endif
