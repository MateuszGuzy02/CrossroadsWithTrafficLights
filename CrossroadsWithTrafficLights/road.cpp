#include "road.h"
#include <GL/glut.h>
#include <initializer_list>

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

void drawRoadLine(float x, float z, float width, float length, bool horizontal) {
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

void drawCar(float x, float z, float rotY, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, 0.45f, z);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    // --- Nadwozie (g³ówna bry³a) ---
    glColor3f(r, g, b); // kolor przekazany jako argument
    glPushMatrix();
    glScalef(5.4f, 0.96f, 2.4f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Kabina (góra) ---
    glColor3f(0.7f, 0.7f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, 0.69f, 0.0f);
    glScalef(3.0f, 0.84f, 2.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Zderzak przedni ---
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(2.55f, -0.18f, 0.0f);
    glScalef(0.54f, 0.54f, 2.46f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Zderzak tylny ---
    glPushMatrix();
    glTranslatef(-2.55f, -0.18f, 0.0f);
    glScalef(0.54f, 0.54f, 2.46f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Œwiat³a przednie ---
    glColor3f(1.0f, 1.0f, 0.2f);
    glPushMatrix();
    glTranslatef(2.79f, 0.15f, 0.69f);
    glScalef(0.18f, 0.18f, 0.36f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.79f, 0.15f, -0.69f);
    glScalef(0.18f, 0.18f, 0.36f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Œwiat³a tylne ---
    glColor3f(1.0f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-2.79f, 0.15f, 0.69f);
    glScalef(0.18f, 0.18f, 0.36f);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.79f, 0.15f, -0.69f);
    glScalef(0.18f, 0.18f, 0.36f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Ko³a ---
    glColor3f(0.1f, 0.1f, 0.1f);
    float wheelY = -0.45f;
    float wheelX = 1.95f;
    float wheelZ = 0.96f;
    for (float dx : {-wheelX, wheelX}) {
        for (float dz : {-wheelZ, wheelZ}) {
            glPushMatrix();
            glTranslatef(dx, wheelY, dz);
            glRotatef(90, 0, 1, 0);
            glutSolidTorus(0.18, 0.39, 10, 16);
            glPopMatrix();
        }
    }

    // --- Szyba przednia ---
    glColor3f(0.5f, 0.7f, 1.0f);
    glPushMatrix();
    glTranslatef(0.96f, 0.84f, 0.0f);
    glScalef(0.96f, 0.54f, 2.04f);
    glutSolidCube(1.0);
    glPopMatrix();

    // --- Szyba tylna ---
    glPushMatrix();
    glTranslatef(-0.96f, 0.84f, 0.0f);
    glScalef(0.96f, 0.54f, 2.04f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void drawTree(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // Pieñ - wy¿szy i grubszy
    glColor3f(0.45f, 0.25f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(0.5f, 4.0f, 0.5f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Korona - kilka kul, ró¿ne pozycje i rozmiary
    glColor3f(0.1f, 0.6f, 0.1f);
    // G³ówna kula
    glPushMatrix();
    glTranslatef(0.0f, 4.5f, 0.0f);
    glutSolidSphere(1.7, 24, 24);
    glPopMatrix();
    // Lewa kula
    glPushMatrix();
    glTranslatef(-1.0f, 4.0f, 0.7f);
    glutSolidSphere(1.1, 20, 20);
    glPopMatrix();
    // Prawa kula
    glPushMatrix();
    glTranslatef(1.0f, 4.2f, -0.7f);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
    // Tylna kula
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, -1.1f);
    glutSolidSphere(0.9, 18, 18);
    glPopMatrix();
    // Przednia kula
    glPushMatrix();
    glTranslatef(0.3f, 4.3f, 1.1f);
    glutSolidSphere(0.8, 18, 18);
    glPopMatrix();

    glPopMatrix();
}

void drawCurbs() {
    // Parametry krawê¿nika
    float curbHeight = 0.2f;
    float curbWidth = 2.5f;
    float roadLength = 60.0f;
    float roadWidth = 8.0f;
    float curbY = 0.01f + curbHeight / 2.0f;
    float intersectionSize = 8.0f;

    glColor3f(0.7f, 0.7f, 0.7f); // jasno szary

    float curbSegment = (roadLength - intersectionSize) / 2.0f;

    // Krawê¿niki poziome (E-W)
    // Górny lewy
    glPushMatrix();
    glTranslatef(-((intersectionSize / 2.0f) + curbSegment / 2.0f), curbY, -(roadWidth / 2.0f + curbWidth / 2.0f));
    drawBox(curbSegment, curbHeight, curbWidth);
    glPopMatrix();

    // Górny prawy
    glPushMatrix();
    glTranslatef(((intersectionSize / 2.0f) + curbSegment / 2.0f), curbY, -(roadWidth / 2.0f + curbWidth / 2.0f));
    drawBox(curbSegment, curbHeight, curbWidth);
    glPopMatrix();

    // Dolny lewy
    glPushMatrix();
    glTranslatef(-((intersectionSize / 2.0f) + curbSegment / 2.0f), curbY, (roadWidth / 2.0f + curbWidth / 2.0f));
    drawBox(curbSegment, curbHeight, curbWidth);
    glPopMatrix();

    // Dolny prawy
    glPushMatrix();
    glTranslatef(((intersectionSize / 2.0f) + curbSegment / 2.0f), curbY, (roadWidth / 2.0f + curbWidth / 2.0f));
    drawBox(curbSegment, curbHeight, curbWidth);
    glPopMatrix();

    // Krawê¿niki pionowe (N-S)
    // Lewy górny
    glPushMatrix();
    glTranslatef(-(roadWidth / 2.0f + curbWidth / 2.0f), curbY, -((intersectionSize / 2.0f) + curbSegment / 2.0f));
    drawBox(curbWidth, curbHeight, curbSegment);
    glPopMatrix();

    // Lewy dolny
    glPushMatrix();
    glTranslatef(-(roadWidth / 2.0f + curbWidth / 2.0f), curbY, ((intersectionSize / 2.0f) + curbSegment / 2.0f));
    drawBox(curbWidth, curbHeight, curbSegment);
    glPopMatrix();

    // Prawy górny
    glPushMatrix();
    glTranslatef((roadWidth / 2.0f + curbWidth / 2.0f), curbY, -((intersectionSize / 2.0f) + curbSegment / 2.0f));
    drawBox(curbWidth, curbHeight, curbSegment);
    glPopMatrix();

    // Prawy dolny
    glPushMatrix();
    glTranslatef((roadWidth / 2.0f + curbWidth / 2.0f), curbY, ((intersectionSize / 2.0f) + curbSegment / 2.0f));
    drawBox(curbWidth, curbHeight, curbSegment);
    glPopMatrix();
}




void drawCompleteRoad() {
    // Trawa - zielony prostok¹t jako pod³o¿e
    glColor3f(0.2f, 0.6f, 0.2f); // zielony kolor trawy
    glBegin(GL_QUADS);
    glVertex3f(-30.0f, 0.0f, -30.0f);
    glVertex3f(30.0f, 0.0f, -30.0f);
    glVertex3f(30.0f, 0.0f, 30.0f);
    glVertex3f(-30.0f, 0.0f, 30.0f);
    glEnd();


    float roadLength = 60.0f;
    float roadWidth = 8.0f;
    float intersectionSize = 8.0f; // szerokoœæ skrzy¿owania (8 + 8 na drogi + marginesy)

	drawCurbs();

    // G³ówna droga pozioma (wschód-zachód)
    drawRoadSegment(0.0f, 0.0f, roadLength, roadWidth, asphaltTexture);

    // G³ówna droga pionowa (pó³noc-po³udnie)
    drawRoadSegment(0.0f, 0.0f, roadWidth, roadLength, asphaltTexture);

    // Przejœcia dla pieszych (pasy) - na ka¿dej drodze
    for (int i = 0; i < 5; i++) {
        float offset = -3.2f + i * 1.6f; // 5 pasów na szerokoœci 4m

        // Pasy na drodze pó³nocnej (przed skrzy¿owaniem)
        drawRoadLine(offset, -12.0f, 0.3f, 2.0f, false);
        // Pasy na drodze po³udniowej (przed skrzy¿owaniem)  
        drawRoadLine(offset, 12.0f, 0.3f, 2.0f, false);
        // Pasy na drodze wschodniej (przed skrzy¿owaniem)
        drawRoadLine(-12.0f, offset, 0.3f, 2.0f, true);
        // Pasy na drodze zachodniej (przed skrzy¿owaniem)
        drawRoadLine(12.0f, offset, 0.3f, 2.0f, true);
    }
    drawCar(-20.0f, 2.0f, 0.0f, 0.15f, 0.2f, 0.8f);
    drawCar(20.0f, -2.0f, 180.0f, 0.8f, 0.1f, 0.1f);
    drawCar(2.0f, 20.0f, 90.0f, 1.0f, 1.0f, 0.0f);
    drawCar(-2.0f, -20.0f, 270.0f, 0.0f, 1.0f, 0.0f);

    float treeOffset = 8.0f;  // dalsza odleg³oœæ od krawêdzi skrzy¿owania

    drawTree(-intersectionSize / 2.0f - treeOffset, -intersectionSize / 2.0f - treeOffset); // Lewy górny róg
    drawTree(intersectionSize / 2.0f + treeOffset, -intersectionSize / 2.0f - treeOffset); // Prawy górny róg
    drawTree(-intersectionSize / 2.0f - treeOffset, intersectionSize / 2.0f + treeOffset); // Lewy dolny róg
    drawTree(intersectionSize / 2.0f + treeOffset, intersectionSize / 2.0f + treeOffset); // Prawy dolny róg
}


