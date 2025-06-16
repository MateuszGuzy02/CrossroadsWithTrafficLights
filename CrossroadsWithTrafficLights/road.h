#pragma once


struct RoadTexture {
    float r, g, b;
};

void drawBox(float w, float h, float d);
void drawTexturedQuad(float w, float h, RoadTexture tex);
void drawRoadSegment(float x, float z, float width, float length, RoadTexture tex);
void drawRoadLine(float x, float z, float width, float length, bool horizontal = true);
void drawCompleteRoad();