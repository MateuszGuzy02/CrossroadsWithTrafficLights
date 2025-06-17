#pragma once

struct PedestrianLightState {
    bool red, green, blinking;
};

extern PedestrianLightState pedestrianLights[8];
extern bool pedestrianRequest[8];

void drawPedestrianLight(float x, float z, float rotationY, PedestrianLightState state, int idx);
void drawAllPedestrianLights();
void setPedestrianRed(int idx);
void startPedestrianBlinking(int idx);
void blinkPedestrianLight(int idx);
