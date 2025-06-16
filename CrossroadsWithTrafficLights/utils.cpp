#include "utils.h"
#include <GL/glut.h>
#include <iostream>
#include <iomanip>

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

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void printControls() {
	setlocale(LC_ALL, "Polish");

    std::cout << "\n=== STEROWANIE SYGNALIZACJ� �WIETLN� 3D ===" << std::endl;
	std::cout << "========================================\n" << std::endl;
    std::cout << "KAMERA:" << std::endl;
    std::cout << "  A/D - obracanie kamery w lewo/prawo" << std::endl;
    std::cout << "  W/S - kamera w g�r�/d�" << std::endl;
    std::cout << "  Q/E - przybli�anie/oddalanie kamery" << std::endl;
    std::cout << "  R   - automatyczne obracanie kamery ON/OFF" << std::endl;
    std::cout << "  +/- - szybko�� automatycznego obracania" << std::endl;
    std::cout << "========================================\n" << std::endl;
    std::cout << "\nFAZY �WIATE� (sterowanie klawiszami):" << std::endl;
    std::cout << "  1 - Faza 1" << std::endl;
    std::cout << "  2 - Faza 2" << std::endl;
    std::cout << "  3 - Faza 3" << std::endl;
    std::cout << "  4 - Faza 4" << std::endl;
    std::cout << "========================================\n" << std::endl;
    std::cout << "\nPRZYCISKI DLA PIESZYCH (klawiatura):" << std::endl;
    std::cout << "  5 - Przej�cie p�nocne" << std::endl;
    std::cout << "  6 - Przej�cie po�udniowe" << std::endl;
    std::cout << "  7 - Przej�cie zachodnie" << std::endl;
	std::cout << "  8 - Przej�cie wschodnie" << std::endl;
    std::cout << "\n  ESC - wyj�cie z programu" << std::endl;
    std::cout << "========================================\n" << std::endl;
}
