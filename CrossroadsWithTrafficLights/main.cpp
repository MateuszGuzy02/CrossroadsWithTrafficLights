#include <GL/glut.h>
#include <iostream>
#include <cmath>

struct TrafficLightState {
    bool red, yellow, green;
};

TrafficLightState lights[4] = {
    {true, false, false},
    {false, false, true},
    {true, false, false},
    {false, false, true},
};

int currentPhase = 0;
float cameraAngle = 0.0f;
float cameraHeight = 10.0f;
float cameraDistance = 20.0f;
bool autoRotate = false;
float rotationSpeed = 0.01f;

// Tekstury (symulowane przez kolory)
struct RoadTexture {
    float r, g, b;
};

RoadTexture asphaltTexture = { 0.2f, 0.2f, 0.2f };
RoadTexture lineTexture = { 1.0f, 1.0f, 0.8f };
RoadTexture sidewalkTexture = { 0.6f, 0.6f, 0.6f };

void drawBox(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawTexturedQuad(float w, float h, RoadTexture tex) {
    glColor3f(tex.r, tex.g, tex.b);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-w / 2, 0.0f, -h / 2);
    glVertex3f(w / 2, 0.0f, -h / 2);
    glVertex3f(w / 2, 0.0f, h / 2);
    glVertex3f(-w / 2, 0.0f, h / 2);
    glEnd();
}

void drawRoadSegment(float x, float z, float width, float length, RoadTexture tex) {
    glPushMatrix();
    glTranslatef(x, 0.01f, z); // Lekko ponad ziemi¹
    drawTexturedQuad(width, length, tex);
    glPopMatrix();
}

void drawRoadLine(float x, float z, float width, float length, bool horizontal = true) {
    glPushMatrix();
    glTranslatef(x, 0.02f, z); // Jeszcze wy¿ej ni¿ droga
    if (horizontal) {
        drawTexturedQuad(length, width, lineTexture);
    }
    else {
        drawTexturedQuad(width, length, lineTexture);
    }
    glPopMatrix();
}

void drawCompleteRoad() {
    // G³ówna droga pozioma (wschód-zachód)
    drawRoadSegment(0.0f, 0.0f, 20.0f, 4.0f, asphaltTexture);

    // G³ówna droga pionowa (pó³noc-po³udnie)
    drawRoadSegment(0.0f, 0.0f, 4.0f, 20.0f, asphaltTexture);

    // Przejœcia dla pieszych (pasy) - na ka¿dej drodze
    for (int i = 0; i < 5; i++) {
        float offset = -1.6f + i * 0.8f; // 5 pasów na szerokoœci 4m

        // Pasy na drodze pó³nocnej (przed skrzy¿owaniem)
        drawRoadLine(offset, -6.0f, 0.3f, 1.0f, false);
        // Pasy na drodze po³udniowej (przed skrzy¿owaniem)  
        drawRoadLine(offset, 6.0f, 0.3f, 1.0f, false);
        // Pasy na drodze wschodniej (przed skrzy¿owaniem)
        drawRoadLine(-6.0f, offset, 0.3f, 1.0f, true); // Zmieniono na horizontal = true
        // Pasy na drodze zachodniej (przed skrzy¿owaniem)
        drawRoadLine(6.0f, offset, 0.3f, 1.0f, true);  // Zmieniono na horizontal = true
    }
}

void drawSingleTrafficLight(float x, float z, float rotationY, TrafficLightState state) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // S³upek
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, 0.0f);
    drawBox(0.3f, 5.0f, 0.3f);
    glPopMatrix();

    // Obudowa œwiate³
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 5.5f, 0.0f);
    drawBox(1.0f, 3.0f, 1.0f);
    glPopMatrix();

    // Œwiat³a z efektem œwiecenia
    glPushMatrix();
    glTranslatef(0.0f, 6.5f, 0.6f);

    // Czerwone œwiat³o
    if (state.red) {
        glColor3f(1.0f, 0.2f, 0.2f);
        // Dodatkowy efekt blasku
        glPushMatrix();
        glScalef(1.2f, 1.2f, 1.2f);
        glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
        glutSolidSphere(0.35, 20, 20);
        glPopMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else {
        glColor3f(0.3f, 0.0f, 0.0f);
    }
    glutSolidSphere(0.3, 20, 20);

    glTranslatef(0.0f, -1.0f, 0.0f);

    // ¯ó³te œwiat³o
    if (state.yellow) {
        glColor3f(1.0f, 1.0f, 0.2f);
        glPushMatrix();
        glScalef(1.2f, 1.2f, 1.2f);
        glColor4f(1.0f, 1.0f, 0.0f, 0.3f);
        glutSolidSphere(0.35, 20, 20);
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 0.0f);
    }
    else {
        glColor3f(0.3f, 0.3f, 0.0f);
    }
    glutSolidSphere(0.3, 20, 20);

    glTranslatef(0.0f, -1.0f, 0.0f);

    // Zielone œwiat³o
    if (state.green) {
        glColor3f(0.2f, 1.0f, 0.2f);
        glPushMatrix();
        glScalef(1.2f, 1.2f, 1.2f);
        glColor4f(0.0f, 1.0f, 0.0f, 0.3f);
        glutSolidSphere(0.35, 20, 20);
        glPopMatrix();
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else {
        glColor3f(0.0f, 0.3f, 0.0f);
    }
    glutSolidSphere(0.3, 20, 20);

    glPopMatrix();
    glPopMatrix();
}

void drawAllTrafficLights() {
    drawSingleTrafficLight(-3.0f, -3.0f, 180.0f, lights[0]);   // pó³nocno-zachodni (patrzy na po³udnie)
    drawSingleTrafficLight(3.0f, -3.0f, 90.0f, lights[1]);    // pó³nocno-wschodni (patrzy na zachód)
    drawSingleTrafficLight(3.0f, 3.0f, 0.0f, lights[2]);       // po³udniowo-wschodni (patrzy na pó³noc)
    drawSingleTrafficLight(-3.0f, 3.0f, 270.0f, lights[3]);     // po³udniowo-zachodni (patrzy na wschód)
}

void updateLights(int value) {
    currentPhase = (currentPhase + 1) % 3;

    if (currentPhase == 0) {
        // Pó³noc-Po³udnie: czerwone, Wschód-Zachód: zielone
        lights[0] = { true, false, false };
        lights[1] = { false, false, true };
        lights[2] = { true, false, false };
        lights[3] = { false, false, true };
        glutTimerFunc(3000, updateLights, 0);
    }
    else if (currentPhase == 1) {
        // Wszystkie: ¿ó³te
        lights[0] = { false, true, false };
        lights[1] = { false, true, false };
        lights[2] = { false, true, false };
        lights[3] = { false, true, false };
        glutTimerFunc(1000, updateLights, 0);
    }
    else if (currentPhase == 2) {
        // Pó³noc-Po³udnie: zielone, Wschód-Zachód: czerwone
        lights[0] = { false, false, true };
        lights[1] = { true, false, false };
        lights[2] = { false, false, true };
        lights[3] = { true, false, false };
        glutTimerFunc(3000, updateLights, 0);
    }

    glutPostRedisplay();
}

void setupLighting() {
    glEnable(GL_LIGHTING);

    // G³ówne Ÿród³o œwiat³a (s³oñce)
    glEnable(GL_LIGHT0);
    GLfloat lightPos0[] = { 5.0f, 15.0f, 5.0f, 1.0f };
    GLfloat lightDiffuse0[] = { 0.8f, 0.8f, 0.7f, 1.0f };
    GLfloat lightAmb0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb0);

    // Drugie Ÿród³o œwiat³a (latarnia uliczna)
    glEnable(GL_LIGHT1);
    GLfloat lightPos1[] = { -8.0f, 12.0f, -8.0f, 1.0f };
    GLfloat lightDiffuse1[] = { 0.9f, 0.8f, 0.6f, 1.0f };
    GLfloat lightAmb1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb1);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (autoRotate) {
        cameraAngle += rotationSpeed;
    }

    float camX = cameraDistance * sinf(cameraAngle);
    float camZ = cameraDistance * cosf(cameraAngle);
    gluLookAt(camX, cameraHeight, camZ, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);

    drawCompleteRoad();
    drawAllTrafficLights();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
    case 27: // ESC
        exit(0);
        break;
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
    case '1':
        // Zmiana fazy œwiate³ na ¿¹danie
        currentPhase = 0;
        lights[0] = { true, false, false };
        lights[1] = { false, false, true };
        lights[2] = { true, false, false };
        lights[3] = { false, false, true };
        glutPostRedisplay();
        break;
    case '2':
        currentPhase = 1;
        lights[0] = { false, true, false };
        lights[1] = { false, true, false };
        lights[2] = { false, true, false };
        lights[3] = { false, true, false };
        glutPostRedisplay();
        break;
    case '3':
        currentPhase = 2;
        lights[0] = { false, false, true };
        lights[1] = { true, false, false };
        lights[2] = { false, false, true };
        lights[3] = { true, false, false };
        glutPostRedisplay();
        break;
    }
}

void printControls() {
    std::cout << "\n=== STEROWANIE SYGNALIZACJ¥ ŒWIETLN¥ 3D ===" << std::endl;
    std::cout << "KAMERA:" << std::endl;
    std::cout << "  A/D - obracanie kamery w lewo/prawo" << std::endl;
    std::cout << "  W/S - kamera w górê/dó³" << std::endl;
    std::cout << "  Q/E - przybli¿anie/oddalanie kamery" << std::endl;
    std::cout << "  R   - automatyczne obracanie kamery ON/OFF" << std::endl;
    std::cout << "  +/- - szybkoœæ automatycznego obracania" << std::endl;
    std::cout << "\nŒWIAT£A:" << std::endl;
    std::cout << "  1   - Faza 1 (Pó³noc-Po³udnie: STOP, Wschód-Zachód: JED)" << std::endl;
    std::cout << "  2   - Faza 2 (Wszystkie: UWAGA)" << std::endl;
    std::cout << "  3   - Faza 3 (Pó³noc-Po³udnie: JED, Wschód-Zachód: STOP)" << std::endl;
    std::cout << "\n  ESC - wyjœcie z programu" << std::endl;
    std::cout << "========================================\n" << std::endl;
}

void idle() {
    if (autoRotate) {
        glutPostRedisplay();
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // Ciemnoniebieski jak wieczorne niebo
    setupLighting();

    // W³¹czenie mieszania kolorów dla efektów transparencji
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
    glutIdleFunc(idle);
    glutTimerFunc(1000, updateLights, 0);

    glutMainLoop();
    return 0;
}