#include <GL/glut.h>
#include <iostream>
#include <cmath>

struct TrafficLightState {
    bool red, yellow, green;
};

struct PedestrianLightState {
    bool red, green;
};

TrafficLightState lights[4] = {
    {true, false, false},
    {false, false, true},
    {true, false, false},
    {false, false, true},
};

PedestrianLightState pedestrianLights[8] = {
    {true, false}, {true, false}, {true, false}, {true, false},
    {true, false}, {true, false}, {true, false}, {true, false}
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
    glTranslatef(x, 0.01f, z); // Lekko ponad ziemi�
    drawTexturedQuad(width, length, tex);
    glPopMatrix();
}

void drawRoadLine(float x, float z, float width, float length, bool horizontal = true) {
    glPushMatrix();
    glTranslatef(x, 0.02f, z); // Jeszcze wy�ej ni� droga
    if (horizontal) {
        drawTexturedQuad(length, width, lineTexture);
    }
    else {
        drawTexturedQuad(width, length, lineTexture);
    }
    glPopMatrix();
}

void drawCompleteRoad() {
    // G��wna droga pozioma (wsch�d-zach�d)
    drawRoadSegment(0.0f, 0.0f, 50.0f, 8.0f, asphaltTexture);

    // G��wna droga pionowa (p�noc-po�udnie)
    drawRoadSegment(0.0f, 0.0f, 8.0f, 50.0f, asphaltTexture);

    // Przej�cia dla pieszych (pasy) - na ka�dej drodze
    for (int i = 0; i < 5; i++) {
        float offset = -3.2f + i * 1.6f; // 5 pas�w na szeroko�ci 4m

        // Pasy na drodze p�nocnej (przed skrzy�owaniem)
        drawRoadLine(offset, -12.0f, 0.3f, 2.0f, false);
        // Pasy na drodze po�udniowej (przed skrzy�owaniem)  
        drawRoadLine(offset, 12.0f, 0.3f, 2.0f, false);
        // Pasy na drodze wschodniej (przed skrzy�owaniem)
        drawRoadLine(-12.0f, offset, 0.3f, 2.0f, true); // Zmieniono na horizontal = true
        // Pasy na drodze zachodniej (przed skrzy�owaniem)
        drawRoadLine(12.0f, offset, 0.3f, 2.0f, true);  // Zmieniono na horizontal = true
    }
}

void drawSingleTrafficLight(float x, float z, float rotationY, TrafficLightState state) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // S�upek
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, 0.0f);
    drawBox(0.3f, 5.0f, 0.3f);
    glPopMatrix();

    // Obudowa �wiate�
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 5.5f, 0.0f);
    drawBox(1.0f, 3.0f, 1.0f);
    glPopMatrix();

    // �wiat�a z efektem �wiecenia
    glPushMatrix();
    glTranslatef(0.0f, 6.5f, 0.6f);

    // Czerwone �wiat�o
    if (state.red)
        glColor3f(1.0f, 0.0f, 0.0f); // jasna czerwie�
    else
        glColor3f(0.3f, 0.0f, 0.0f); // ciemna czerwie�
    glutSolidSphere(0.3, 20, 20);

    glTranslatef(0.0f, -1.0f, 0.0f);

    // ��te �wiat�o
    if (state.yellow)
        glColor3f(1.0f, 1.0f, 0.0f); // jasny ��ty
    else
        glColor3f(0.3f, 0.3f, 0.0f); // ciemny ��ty
    glutSolidSphere(0.3, 20, 20);

    glTranslatef(0.0f, -1.0f, 0.0f);

    // Zielone �wiat�o
    if (state.green)
        glColor3f(0.0f, 1.0f, 0.0f); // jasna ziele�
    else
        glColor3f(0.0f, 0.3f, 0.0f); // ciemna ziele�
    glutSolidSphere(0.3, 20, 20);

    glPopMatrix();
    glPopMatrix();
}

void drawAllTrafficLights() {
    drawSingleTrafficLight(-5.0f, -5.0f, 180.0f, lights[0]);   // p�nocno-zachodni (patrzy na po�udnie)
    drawSingleTrafficLight(5.0f, -5.0f, 90.0f, lights[1]);    // p�nocno-wschodni (patrzy na zach�d)
    drawSingleTrafficLight(5.0f, 5.0f, 0.0f, lights[2]);       // po�udniowo-wschodni (patrzy na p�noc)
    drawSingleTrafficLight(-5.0f, 5.0f, 270.0f, lights[3]);     // po�udniowo-zachodni (patrzy na wsch�d)
}


void drawPedestrianLight(float x, float z, float rotationY, PedestrianLightState state) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    // S�upek
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    drawBox(0.15f, 2.4f, 0.15f);
    glPopMatrix();

    // Obudowa �wiate�
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 2.6f, 0.0f);
    drawBox(0.4f, 0.8f, 0.4f);
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

    // Zielone �wiat�o
    glTranslatef(0.0f, -0.3f, 0.0f);
    if (state.green)
        glColor3f(0.0f, 1.0f, 0.0f);
    else
        glColor3f(0.0f, 0.3f, 0.0f);
    glutSolidSphere(0.13, 16, 16);

    glPopMatrix();

    // Kwadratowy przycisk
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.1f); // na froncie s�upka, lekko wystaje
    glColor3f(0.8f, 0.8f, 0.2f);
    drawBox(0.18f, 0.18f, 0.05f); // szeroko�� i wysoko�� wi�ksza ni� s�upek, g��boko�� niewielka
    glPopMatrix();

    glPopMatrix();
}

//void drawAllPedestrianLights() {
//    PedestrianLightState pedRed = { true, false };
//    PedestrianLightState pedGreen = { false, true };
//
//    // Przej�cie p�nocne (na ko�cu pas�w)
//    drawPedestrianLight(-6.0f, -12.8f, 90.0f, pedRed);
//    drawPedestrianLight(6.0f, -12.8f, -90.0f, pedRed);
//
//    // Przej�cie po�udniowe
//    drawPedestrianLight(-6.0f, 12.8f, 90.0f, pedRed);
//    drawPedestrianLight(6.0f, 12.8f, -90.0f, pedRed);
//
//    // Przej�cie zachodnie
//    drawPedestrianLight(-12.8f, -6.0f, 0.0f, pedRed);
//    drawPedestrianLight(-12.8f, 6.0f, 180.0f, pedRed);
//
//    // Przej�cie wschodnie
//    drawPedestrianLight(12.8f, -6.0f, 0.0f, pedRed);
//    drawPedestrianLight(12.8f, 6.0f, 180.0f, pedRed);
//}

void drawAllPedestrianLights() {
    // Przej�cie p�nocne (na ko�cu pas�w)
    drawPedestrianLight(-6.0f, -12.8f, 90.0f, pedestrianLights[0]);
    drawPedestrianLight(6.0f, -12.8f, -90.0f, pedestrianLights[1]);

    // Przej�cie po�udniowe
    drawPedestrianLight(-6.0f, 12.8f, 90.0f, pedestrianLights[2]);
    drawPedestrianLight(6.0f, 12.8f, -90.0f, pedestrianLights[3]);

    // Przej�cie zachodnie
    drawPedestrianLight(-12.8f, -6.0f, 0.0f, pedestrianLights[4]);
    drawPedestrianLight(-12.8f, 6.0f, 180.0f, pedestrianLights[5]);

    // Przej�cie wschodnie
    drawPedestrianLight(12.8f, -6.0f, 0.0f, pedestrianLights[6]);
    drawPedestrianLight(12.8f, 6.0f, 180.0f, pedestrianLights[7]);
}

void updateLights(int value) {
    currentPhase = (currentPhase + 1) % 6;

    switch (currentPhase) {
    case 0:
        // Faza A: N/S zielone, E/W czerwone
        lights[0] = lights[2] = { false, false, true };   // N/S
        lights[1] = lights[3] = { true, false, false };   // E/W
        glutTimerFunc(6000, updateLights, 0); // 2 sek
        break;
    case 1:
        // Faza B: N/S ��te
        lights[0] = lights[2] = { false, true, false };   // N/S
        glutTimerFunc(2000, updateLights, 0); // 1 sek
        break;
    case 2:
        // Faza C: N/S czerwone, E/W czerwono-��te
        lights[0] = lights[2] = { true, false, false };   // N/S
        lights[1] = lights[3] = { true, true, false };    // E/W
        glutTimerFunc(2000, updateLights, 0); // 1 sek
        break;
    case 3:
        // Faza D: E/W zielone, N/S czerwone
        lights[1] = lights[3] = { false, false, true };   // E/W
        lights[0] = lights[2] = { true, false, false };   // N/S
        glutTimerFunc(6000, updateLights, 0); // 2 sek
        break;
    case 4:
        // Faza E: E/W ��te
        lights[1] = lights[3] = { false, true, false };   // E/W
        glutTimerFunc(2000, updateLights, 0); // 1 sek
        break;
    case 5:
        // Faza F: E/W czerwone, N/S czerwono-��te
        lights[1] = lights[3] = { true, false, false };   // E/W
        lights[0] = lights[2] = { true, true, false };    // N/S
        glutTimerFunc(2000, updateLights, 0); // 1 sek
        break;
    }

    glutPostRedisplay();
}


void setupLighting() {
    glEnable(GL_LIGHTING);

    // G��wne �r�d�o �wiat�a (s�o�ce)
    glEnable(GL_LIGHT0);
    GLfloat lightPos0[] = { 5.0f, 15.0f, 5.0f, 1.0f };
    GLfloat lightDiffuse0[] = { 0.8f, 0.8f, 0.7f, 1.0f };
    GLfloat lightAmb0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb0);

    // Drugie �r�d�o �wiat�a (latarnia uliczna)
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
    drawAllPedestrianLights();

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

    }
}

void setPedestrianRed(int idx) {
    int pair = idx ^ 1; 
    pedestrianLights[idx] = { true, false };
    pedestrianLights[pair] = { true, false };
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    // Pobierz macierze i viewport
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Przelicz wsp�rz�dne ekranu na �wiat na wysoko�ci przycisku (y = 1.0f)
    float winX = (float)x;
    float winY = (float)viewport[3] - (float)y;

    // Rzutuj dwa punkty: na bliskiej i dalekiej p�aszczy�nie
    GLdouble pos1[3], pos2[3];
    gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &pos1[0], &pos1[1], &pos1[2]);
    gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &pos2[0], &pos2[1], &pos2[2]);

    // Wyznacz parametry promienia
    double t = (1.0 - pos1[1]) / (pos2[1] - pos1[1]); // y=1.0f
    double clickX = pos1[0] + t * (pos2[0] - pos1[0]);
    double clickZ = pos1[2] + t * (pos2[2] - pos1[2]);

    // Pozycje przycisk�w (prawy bok s�upka)
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

    for (int i = 0; i < 8; ++i) {
        float bx = buttons[i].x;
        float bz = buttons[i].z;
        if (std::abs(clickX - bx) < 0.12f && std::abs(clickZ - bz) < 0.12f) {
            int pair = i ^ 1; 
            pedestrianLights[i] = { false, true };
            pedestrianLights[pair] = { false, true };
            glutTimerFunc(3000, [](int idx) { setPedestrianRed(idx); }, i);
            glutPostRedisplay();
            break;
        }
    }
}


void printControls() {
    std::cout << "\n=== STEROWANIE SYGNALIZACJ� �WIETLN� 3D ===" << std::endl;
    std::cout << "KAMERA:" << std::endl;
    std::cout << "  A/D - obracanie kamery w lewo/prawo" << std::endl;
    std::cout << "  W/S - kamera w g�r�/d�" << std::endl;
    std::cout << "  Q/E - przybli�anie/oddalanie kamery" << std::endl;
    std::cout << "  R   - automatyczne obracanie kamery ON/OFF" << std::endl;
    std::cout << "  +/- - szybko�� automatycznego obracania" << std::endl;
    std::cout << "\n�WIAT�A:" << std::endl;
    std::cout << "  1   - Faza 1 (P�noc-Po�udnie: STOP, Wsch�d-Zach�d: JED�)" << std::endl;
    std::cout << "  2   - Faza 2 (Wszystkie: UWAGA)" << std::endl;
    std::cout << "  3   - Faza 3 (P�noc-Po�udnie: JED�, Wsch�d-Zach�d: STOP)" << std::endl;
    std::cout << "\n  ESC - wyj�cie z programu" << std::endl;
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

    // W��czenie mieszania kolor�w dla efekt�w transparencji
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
    glutIdleFunc(idle);
    glutTimerFunc(0, updateLights, 0);

    glutMainLoop();
    return 0;
}