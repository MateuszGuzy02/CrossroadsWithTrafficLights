#pragma once

extern float cameraAngle;
extern float cameraHeight;
extern float cameraDistance;
extern bool autoRotate;
extern float rotationSpeed;

void handleCameraKeyboard(unsigned char key);
void updateCamera();
