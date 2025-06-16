#include <GL/glut.h>
#include "road.h"
#include "trafficlights.h"
#include "pedestrianlights.h"
#include "camera.h"
#include "utils.h"
#include <cmath>
#include <corecrt_math.h>

void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    // Pobierz macierze i viewport
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Przelicz współrzędne ekranu na świat na wysokości przycisku (y = 1.0f)
    float winX = (float)x;
    float winY = (float)viewport[3] - (float)y;

    // Rzutuj dwa punkty: na bliskiej i dalekiej płaszczyźnie
    GLdouble pos1[3], pos2[3];
    gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &pos1[0], &pos1[1], &pos1[2]);
    gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &pos2[0], &pos2[1], &pos2[2]);

    // Wyznacz parametry promienia
    double t = (1.0 - pos1[1]) / (pos2[1] - pos1[1]); // y=1.0f
    double clickX = pos1[0] + t * (pos2[0] - pos1[0]);
    double clickZ = pos1[2] + t * (pos2[2] - pos1[2]);

    // Pozycje przycisków (prawy bok słupka)
    struct Button {
        float x, z, rot;
    } buttons[8] = {
        {-6.0f + 0.09f, -12.8f, 90.0f},
        {6.0f + 0.09f, -12.8f, -90.0f},
        {-6.0f + 0.09f, 12.8f, 90.0f},
        {6.0f + 0.09f, 12.8f, -90.0f},
        {-12.8f + 0.09f, -6.0f, 0.0f},
        {-12.8f + 0.09f, 6.0f, 180.0f},
        {12.8f + 0.09f, -6.0f, 0.0f},
        {12.8f + 0.09f, 6.0f, 180.0f}
    };

    // Sprawdź kliknięcie dla każdego przycisku z większym obszarem
    for (int i = 0; i < 8; ++i) {
        float bx = buttons[i].x;
        float bz = buttons[i].z;
        // Zwiększony obszar kliknięcia z 0.12f na 0.18f (odpowiada większemu przyciskowi)
        if (std::fabs(clickX - bx) < 0.18f && std::fabs(clickZ - bz) < 0.18f) {
            // Znajdź parę dla tego przycisku
            int pair;
            if (i < 4) {
                // Przejścia N/S: 0↔1, 2↔3
                pair = (i % 2 == 0) ? i + 1 : i - 1;
            }
            else {
                // Przejścia E/W: 4↔5, 6↔7
                pair = (i % 2 == 0) ? i + 1 : i - 1;
            }

            // Aktywuj żądanie dla obu sygnalizatorów w parze
            pedestrianRequest[i] = true;
            pedestrianRequest[pair] = true;

            glutPostRedisplay();
            break;
        }
    }
}


// Funkcja wyświetlania sceny
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    updateCamera();

    float camX = cameraDistance * sinf(cameraAngle);
    float camZ = cameraDistance * cosf(cameraAngle);
    gluLookAt(camX, cameraHeight, camZ, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);

    drawCompleteRoad();
    drawAllTrafficLights();
    drawAllPedestrianLights();

    glutSwapBuffers();
}

// Obsługa klawiatury (kamera + światła)
void keyboard(unsigned char key, int, int) {
    if (key == 27) { // ESC
        exit(0);
    }

    // Kamera
    handleCameraKeyboard(key);

    // Światła drogowe (przykład: 1-4)
    switch (key) {
    case '1':
        currentPhase = 0;
        for (int i = 0; i < 4; ++i)
            lights[i] = { true, false, false };
        glutPostRedisplay();
        break;
    case '2':
        currentPhase = 1;
        for (int i = 0; i < 4; ++i)
            lights[i] = { true, true, false };
        glutPostRedisplay();
        break;
    case '3':
        currentPhase = 2;
        for (int i = 0; i < 4; ++i)
            lights[i] = { false, false, true };
        glutPostRedisplay();
        break;
    case '4':
        currentPhase = 3;
        for (int i = 0; i < 4; ++i)
            lights[i] = { false, true, false };
        glutPostRedisplay();
        break;
    case '5': // Przejście północne (0 i 1)
        pedestrianRequest[0] = true;
        pedestrianRequest[1] = true;
        glutPostRedisplay();
        break;
    case '6': // Przejście południowe (2 i 3)
        pedestrianRequest[2] = true;
        pedestrianRequest[3] = true;
        glutPostRedisplay();
        break;
    case '7': // Przejście zachodnie (4 i 5)
        pedestrianRequest[4] = true;
        pedestrianRequest[5] = true;
        glutPostRedisplay();
        break;
    case '8': // Przejście wschodnie (6 i 7)
        pedestrianRequest[6] = true;
        pedestrianRequest[7] = true;
        glutPostRedisplay();
        break;
    }
}

// Inicjalizacja OpenGL i sceny
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // Ciemnoniebieski jak wieczorne niebo
    setupLighting();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printControls();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("Sygnalizacja Swietlna 3D z Droga - OpenGL");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutTimerFunc(0, updateLights, 0);

    glutMainLoop();
    return 0;
}
