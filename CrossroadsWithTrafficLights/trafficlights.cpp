#include "trafficlights.h"
#include "pedestrianlights.h"
#include <GL/glut.h>

TrafficLightState lights[4] = {
    {true, false, false},
    {false, false, true},
    {true, false, false},
    {false, false, true},
};

int currentPhase = 0;

void drawSingleTrafficLight(float x, float z, float rotationY, TrafficLightState state) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // S³upek
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, 0.0f);
    glScalef(0.3f, 5.0f, 0.3f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Obudowa swiate³
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 5.5f, 0.0f);
    glScalef(1.0f, 3.0f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Œwiat³a z efektem swiecenia
    glPushMatrix();
    glTranslatef(0.0f, 6.5f, 0.6f);

    // Czerwone swiat³o
    if (state.red)
        glColor3f(1.0f, 0.0f, 0.0f);
    else
        glColor3f(0.3f, 0.0f, 0.0f);
    glutSolidSphere(0.3, 20, 20);

    glTranslatef(0.0f, -1.0f, 0.0f);
    // ¯ó³te swiat³o
    if (state.yellow)
        glColor3f(1.0f, 1.0f, 0.0f);
    else
        glColor3f(0.3f, 0.3f, 0.0f);
    glutSolidSphere(0.3, 20, 20);

    glTranslatef(0.0f, -1.0f, 0.0f);
    // Zielone swiat³o
    if (state.green)
        glColor3f(0.0f, 1.0f, 0.0f);
    else
        glColor3f(0.0f, 0.3f, 0.0f);
    glutSolidSphere(0.3, 20, 20);

    glPopMatrix();
    glPopMatrix();
}

void drawAllTrafficLights() {
    drawSingleTrafficLight(-5.0f, -5.0f, 180.0f, lights[0]);
    drawSingleTrafficLight(5.0f, -5.0f, 90.0f, lights[1]);
    drawSingleTrafficLight(5.0f, 5.0f, 0.0f, lights[2]);
    drawSingleTrafficLight(-5.0f, 5.0f, 270.0f, lights[3]);
}

void updateLights(int value) {
    currentPhase = (currentPhase + 1) % 6;

    switch (currentPhase) {
    case 0:
        // Faza A: N/S zielone, E/W czerwone
        lights[0] = lights[2] = { false, false, true };   // N/S
        lights[1] = lights[3] = { true, false, false };   // E/W

        // Obs³uga ¿¹dañ pieszych dla przejœæ E/W (indeksy 4-7) gdy ruch N/S
        for (int i = 4; i < 8; ++i) {
            if (pedestrianRequest[i]) {
                // Parowanie sygnalizatorów: 4-5, 6-7
                int pair = (i % 2 == 0) ? i + 1 : i - 1;
                pedestrianLights[i] = { false, true, false }; // zielone dla pieszych
                pedestrianLights[pair] = { false, true, false }; // zielone dla pary
                pedestrianRequest[i] = false;
                pedestrianRequest[pair] = false;

                // Po 3 sekundach rozpocznij miganie (3s migania + przejœcie na czerwone)
                glutTimerFunc(4000, [](int idx) { startPedestrianBlinking(idx); }, i);
            }
        }

        // Pozosta³e sygnalizatory pieszych maj¹ czerwone
        for (int i = 0; i < 8; ++i) {
            if (!pedestrianLights[i].green) {
                pedestrianLights[i] = { true, false, false };
            }
        }

        glutTimerFunc(6000, updateLights, 0);
        break;

    case 1:
        // Faza B: N/S ¿ó³te
        lights[0] = lights[2] = { false, true, false };   // N/S
        glutTimerFunc(2000, updateLights, 0);
        break;

    case 2:
        // Faza C: N/S czerwone, E/W czerwono-¿ó³te
        lights[0] = lights[2] = { true, false, false };   // N/S
        lights[1] = lights[3] = { true, true, false };    // E/W
        glutTimerFunc(2000, updateLights, 0);
        break;

    case 3:
        // Faza D: E/W zielone, N/S czerwone
        lights[1] = lights[3] = { false, false, true };   // E/W
        lights[0] = lights[2] = { true, false, false };   // N/S

        // Obs³uga ¿¹dañ pieszych dla przejœæ N/S (indeksy 0-3) gdy ruch E/W
        for (int i = 0; i < 4; ++i) {
            if (pedestrianRequest[i]) {
                // Parowanie sygnalizatorów: 0-1, 2-3
                int pair = (i % 2 == 0) ? i + 1 : i - 1;
                pedestrianLights[i] = { false, true, false }; // zielone dla pieszych
                pedestrianLights[pair] = { false, true, false }; // zielone dla pary
                pedestrianRequest[i] = false;
                pedestrianRequest[pair] = false;

                // Po 3 sekundach rozpocznij miganie
                glutTimerFunc(4000, [](int idx) { startPedestrianBlinking(idx); }, i);
            }
        }

        // Pozosta³e sygnalizatory pieszych maj¹ czerwone
        for (int i = 0; i < 8; ++i) {
            if (!pedestrianLights[i].green) {
                pedestrianLights[i] = { true, false, false };
            }
        }

        glutTimerFunc(6000, updateLights, 0);
        break;

    case 4:
        // Faza E: E/W ¿ó³te
        lights[1] = lights[3] = { false, true, false };   // E/W
        glutTimerFunc(2000, updateLights, 0);
        break;

    case 5:
        // Faza F: E/W czerwone, N/S czerwono-¿ó³te
        lights[1] = lights[3] = { true, false, false };   // E/W
        lights[0] = lights[2] = { true, true, false };    // N/S
        glutTimerFunc(2000, updateLights, 0);
        break;
    }

    glutPostRedisplay();
}