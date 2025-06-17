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

    // S�upek
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    glScalef(0.15f, 2.4f, 0.15f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Obudowa �wiate�
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 2.6f, 0.0f);
    glScalef(0.4f, 0.8f, 0.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // �wiat�a
    glPushMatrix();
    glTranslatef(0.0f, 2.7f, 0.22f);

    // Czerwone �wiat�o
    if (state.red)
        glColor3f(1.0f, 0.0f, 0.0f);
    else
        glColor3f(0.3f, 0.0f, 0.0f);
    glutSolidSphere(0.13, 16, 16);

    // Zielone �wiat�o - z obs�ug� migania
    glTranslatef(0.0f, -0.3f, 0.0f);
    if (state.green) {
        if (state.blinking) {
            // Je�li miga, to zale�nie od stanu migania �wieci lub nie
            if (blinkState[idx])
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(0.0f, 0.3f, 0.0f);
        }
        else {
            // Normalne �wiecenie
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
        glColor3f(1.0f, 0.2f, 0.2f); // czerwony, je�li zg�oszono ��danie
    else
        glColor3f(0.8f, 0.8f, 0.2f); // ��ty normalnie
    glutSolidSphere(0.12, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

void drawAllPedestrianLights() {
    // Przej�cie p�nocne (N/S traffic - indeksy 0,1)
    drawPedestrianLight(-6.0f, -12.8f, 90.0f, pedestrianLights[0], 0);
    drawPedestrianLight(6.0f, -12.8f, -90.0f, pedestrianLights[1], 1);

    // Przej�cie po�udniowe (N/S traffic - indeksy 2,3)
    drawPedestrianLight(-6.0f, 12.8f, 90.0f, pedestrianLights[2], 2);
    drawPedestrianLight(6.0f, 12.8f, -90.0f, pedestrianLights[3], 3);

    // Przej�cie zachodnie (E/W traffic - indeksy 4,5)
    drawPedestrianLight(-12.8f, -6.0f, 0.0f, pedestrianLights[4], 4);
    drawPedestrianLight(-12.8f, 6.0f, 180.0f, pedestrianLights[5], 5);

    // Przej�cie wschodnie (E/W traffic - indeksy 6,7)
    drawPedestrianLight(12.8f, -6.0f, 0.0f, pedestrianLights[6], 6);
    drawPedestrianLight(12.8f, 6.0f, 180.0f, pedestrianLights[7], 7);
}

void setPedestrianRed(int idx) {
    // Znajd� par� dla danego sygnalizatora
    int pair;
    if (idx < 4) {
        // Przej�cia N/S: 0-1, 2-3
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }
    else {
        // Przej�cia E/W: 4-5, 6-7
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }

    // Ustaw czerwone �wiat�o dla obu sygnalizator�w w parze
    pedestrianLights[idx] = { true, false };
    pedestrianLights[pair] = { true, false };

    glutPostRedisplay();
}

void startPedestrianBlinking(int idx) {
    // Znajd� par� dla danego sygnalizatora
    int pair;
    if (idx < 4) {
        // Przej�cia N/S: 0-1, 2-3
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }
    else {
        // Przej�cia E/W: 4-5, 6-7
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }

    // Ustaw stan migania dla pary sygnalizator�w
    pedestrianLights[idx].blinking = true;
    pedestrianLights[pair].blinking = true;

    // Zresetuj licznik migania
    blinkCount[idx] = 0;
    blinkCount[pair] = 0;

    // Rozpocznij miganie
    blinkPedestrianLight(idx);
}

void blinkPedestrianLight(int idx) {
    // Znajd� par�
    int pair;
    if (idx < 4) {
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }
    else {
        pair = (idx % 2 == 0) ? idx + 1 : idx - 1;
    }

    // Je�li nie miga ju�, zako�cz
    if (!pedestrianLights[idx].blinking) {
        return;
    }

    // Prze��cz stan migania
    blinkState[idx] = !blinkState[idx];
    blinkState[pair] = !blinkState[pair];

    blinkCount[idx]++;
    blinkCount[pair]++;

    glutPostRedisplay();

    // Miga przez 3 sekundy (6 prze��cze� co 0.5s)
    if (blinkCount[idx] < 20) {
        glutTimerFunc(250, [](int i) { blinkPedestrianLight(i); }, idx);
    }
    else {
        // Zako�cz miganie i ustaw czerwone �wiat�o
        setPedestrianRed(idx);
    }
}
