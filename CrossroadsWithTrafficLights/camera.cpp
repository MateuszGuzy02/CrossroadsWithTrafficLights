#include "camera.h"
#include <GL/glut.h>
#include <cmath>

float cameraAngle = 0.0f;
float cameraHeight = 10.0f;
float cameraDistance = 20.0f;
bool autoRotate = false;
float rotationSpeed = 0.01f;

void handleCameraKeyboard(unsigned char key) {
    switch (key) {
    case 'a':
    case 'A':
        cameraAngle -= 0.1f;
        glutPostRedisplay();
        break;
    case 'd':
    case 'D':
        cameraAngle += 0.1f;
        glutPostRedisplay();
        break;
    case 'w':
    case 'W':
        cameraHeight += 0.5f;
        glutPostRedisplay();
        break;
    case 's':
    case 'S':
        cameraHeight -= 0.5f;
        if (cameraHeight < 2.0f) cameraHeight = 2.0f;
        glutPostRedisplay();
        break;
    case 'q':
    case 'Q':
        cameraDistance -= 1.0f;
        if (cameraDistance < 5.0f) cameraDistance = 5.0f;
        glutPostRedisplay();
        break;
    case 'e':
    case 'E':
        cameraDistance += 1.0f;
        if (cameraDistance > 50.0f) cameraDistance = 50.0f;
        glutPostRedisplay();
        break;
    case 'r':
    case 'R':
        autoRotate = !autoRotate;
        break;
    case '+':
        rotationSpeed += 0.005f;
        if (rotationSpeed > 0.1f) rotationSpeed = 0.1f;
        break;
    case '-':
        rotationSpeed -= 0.005f;
        if (rotationSpeed < 0.001f) rotationSpeed = 0.001f;
        break;
    }
}

void updateCamera() {
    if (autoRotate) {
        cameraAngle += rotationSpeed;
    }
}
