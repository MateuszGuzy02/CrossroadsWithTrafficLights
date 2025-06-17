#include "pedestrianlights.h"
#include <GL/glut.h>

PedestrianLightState pedestrianLights[8] = {
    {true, false}, {true, false}, {true, false}, {true, false},
    {true, false}, {true, false}, {true, false}, {true, false}
};

bool pedestrianRequest[8] = { false, false, false, false, false, false, false, false };

static bool blinkState[8] = { false, false, false, false, false, false, false, false };
static int blinkCount[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void drawPedestrianLight(float x, float z, float rotationY, PedestrianLightState state, int idx) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // S³upek
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    glScalef(0.15f, 2.4f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Obudowa œwiate³
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 2.6f, 0.0f);
    glScalef(0.4f, 0.8f, 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Œwiat³a
    glPushMatrix();
    glTranslatef(0.0f, 2.7f, 0.22f);

    // Czerwone œwiat³o
    if (state.red)
        glColor3f(1.0f, 0.0f, 0.0f);
    else
        glColor3f(0.3f, 0.0f, 0.0f);
    glutSolidSphere(0.13, 16, 16);

    // Zielone œwiat³o - z obs³ug¹ migania
    glTranslatef(0.0f, -0.3f, 0.0f);
    if (state.green) {
        if (state.blinking) {
            // Jeœli miga, to zale¿nie od stanu migania œwieci lub nie
            if (blinkState[idx])
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(0.0f, 0.3f, 0.0f);
        }
        else {
            // Normalne œwiecenie
            glColor3f(0.0f, 1.0f, 0.0f);
        }
    }
    else {
        glColor3f(0.0f, 0.3f, 0.0f);
    }
    glutSolidSphere(0.13, 16, 16);
    glPopMatrix();

    // Przycisk
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.15f);
    if (pedestrianRequest[idx])
        glColor3f(1.0f, 0.2f, 0.2f); // czerwony, jeœli zg³oszono ¿¹danie
    else
        glColor3f(0.8f, 0.8f, 0.2f); // ¿ó³ty normalnie
    glutSolidSphere(0.12, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

void drawAllPedestrianLights() {
    // Przejœcie pó³nocne (N/S traffic - indeksy 0,1)
    drawPedestrianLight(-6.0f, -12.8f, 90.0f, pedestrianLights[0], 0);
    drawPedestrianLight(6.0f, -12.8f, -90.0f, pedestrianLights[1], 1);

    // Przejœcie po³udniowe (N/S traffic - indeksy 2,3)
    drawPedestrianLight(-6.0f, 12.8f, 90.0f, pedestrianLights[2], 2);
    drawPedestrianLight(6.0f, 12.8f, -90.0f, pedestrianLights[3], 3);

    // Przejœcie zachodnie (E/W traffic - indeksy 4,5)
    drawPedestrianLight(-12.8f, -6.0f, 0.0f, pedestrianLights[4], 4);
    drawPedestrianLight(-12.8f, 6.0f, 180.0f, pedestrianLights[5], 5);

    // Przejœcie wschodnie (E/W traffic - indeksy 6,7)
    drawPedestrianLight(12.8f, -6.0f, 0.0f, pedestrianLights[6], 6);
    drawPedestrianLight(12.8f, 6.0f, 180.0f, pedestrianLights[7], 7);
}

void setPedestrianRed(int idx) {
    // ZnajdŸ parê dla danego sygnalizatora
    int pair;
    if (idx < 4) {
        // Przejœcia N/S: 0-1, 2-3
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }
    else {
        // Przejœcia E/W: 4-5, 6-7
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }

    // Ustaw czerwone œwiat³o dla obu sygnalizatorów w parze
    pedestrianLights[idx] = { true, false };
    pedestrianLights[pair] = { true, false };

    glutPostRedisplay();
}

void startPedestrianBlinking(int idx) {
    // ZnajdŸ parê dla danego sygnalizatora
    int pair;
    if (idx < 4) {
        // Przejœcia N/S: 0-1, 2-3
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }
    else {
        // Przejœcia E/W: 4-5, 6-7
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }

    // Ustaw stan migania dla pary sygnalizatorów
    pedestrianLights[idx].blinking = true;
    pedestrianLights[pair].blinking = true;

    // Zresetuj licznik migania
    blinkCount[idx] = 0;
    blinkCount[pair] = 0;

    // Rozpocznij miganie
    blinkPedestrianLight(idx);
}

void blinkPedestrianLight(int idx) {
    // ZnajdŸ parê
    int pair;
    if (idx < 4) {
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }
    else {
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }

    // Jeœli nie miga ju¿, zakoñcz
    if (!pedestrianLights[idx].blinking) {
        return;
    }

    // Prze³¹cz stan migania
    blinkState[idx] = !blinkState[idx];
    blinkState[pair] = !blinkState[pair];

    blinkCount[idx]++;
    blinkCount[pair]++;

    glutPostRedisplay();

    // Miga przez 3 sekundy (6 prze³¹czeñ co 0.5s)
    if (blinkCount[idx] < 20) {
        glutTimerFunc(250, [](int i) { blinkPedestrianLight(i); }, idx);
    }
    else {
        // Zakoñcz miganie i ustaw czerwone œwiat³o
        setPedestrianRed(idx);
    }
}
