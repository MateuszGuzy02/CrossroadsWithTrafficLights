#pragma once

struct TrafficLightState {
    bool red, yellow, green;
};

extern TrafficLightState lights[4];
extern int currentPhase;

void drawSingleTrafficLight(float x, float z, float rotationY, TrafficLightState state);
void drawAllTrafficLights();
void updateLights(int value);