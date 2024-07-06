
//==============================================================================
//
//     displaymgr.cpp
//
//============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================


#include "stdafx.h"
#include "displaymgr.h"
#include "camera.h"
#include <iostream>
#include <GL/glut.h>

DisplayMgr* DisplayMgr::instance = nullptr;

DisplayMgr::DisplayMgr(int argc, char** argv) : camX(0.0f), camY(0.0f), camZ(5.0f), camAngleX(0.0f), camAngleY(0.0f), leftMouseButtonPressed(false) {
    instance = this;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Brick Wall with Mouse Control");
    init();
    glutDisplayFunc(displayWrapper);
    glutReshapeFunc(reshapeWrapper);
    glutMotionFunc(mouseMotionWrapper);
    glutMouseFunc(mouseButtonWrapper);
    glutKeyboardFunc(keyboardWrapper);
#ifdef __FREEGLUT_EXT_H__
    glutMouseWheelFunc(mouseWheelWrapper);
#endif
	_cam = new CCamera();
    _cam->Move(F3dVector(0.0, 0.0, 3.0));
    _cam->MoveForward(1.0);
    glutMainLoop();

}

void DisplayMgr::init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void DisplayMgr::displayMain() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -camZ);
    glRotatef(camAngleX, 1.0, 0.0, 0.0);
    glRotatef(camAngleY, 0.0, 1.0, 0.0);
	
    // Draw a brick wall
    glBegin(GL_QUADS);

    // Front face of the brick wall
    glColor3f(0.6f, 0.3f, 0.1f); // Brick color
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Back face of the brick wall
    glColor3f(0.5f, 0.2f, 0.0f); // Slightly darker for the back
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Top face
    glColor3f(0.6f, 0.3f, 0.1f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Bottom face
    glColor3f(0.6f, 0.3f, 0.1f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Left face
    glColor3f(0.5f, 0.2f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face
    glColor3f(0.5f, 0.2f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glEnd();

    glutSwapBuffers();
}

void DisplayMgr::reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void DisplayMgr::mouseMotion(int x, int y) {
    if (leftMouseButtonPressed) {
        camAngleY += (x - lastMouseX);
        camAngleX += (y - lastMouseY);
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void DisplayMgr::mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            leftMouseButtonPressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else {
            leftMouseButtonPressed = false;
        }
    }
}

void DisplayMgr::mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) {
        camZ -= 0.5f;
    }
    else {
        camZ += 0.5f;
    }
    glutPostRedisplay();
}



//draws a plane "net"
void DisplayMgr::DrawNet(GLfloat size, GLint LinesX, GLint LinesZ)
{
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(-size / 2.0 + xc / (GLfloat)(LinesX - 1) * size,
			0.0,
			size / 2.0);
		glVertex3f(-size / 2.0 + xc / (GLfloat)(LinesX - 1) * size,
			0.0,
			size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(size / 2.0,
			0.0,
			-size / 2.0 + zc / (GLfloat)(LinesZ - 1) * size);
		glVertex3f(size / -2.0,
			0.0,
			-size / 2.0 + zc / (GLfloat)(LinesZ - 1) * size);
	}
	glEnd();
}

//handles the window's resizing event
void DisplayMgr::reshapeRefresh(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return

	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0
	gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);  //Use the whole window for rendering
}

//displays the scene.
void DisplayMgr::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	_cam->Render();

	//Draw the "world" (which consists of six "nets" forming a cuboid
	glTranslatef(0.0, -0.5, -6.0);

	glScalef(3.0, 1.0, 3.0);

	GLfloat size = 2.0;
	GLint LinesX = 30;
	GLint LinesZ = 30;

	GLfloat halfsize = size / 2.0;
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, -halfsize, 0.0);
	DrawNet(size, LinesX, LinesZ);
	glTranslatef(0.0, size, 0.0);
	DrawNet(size, LinesX, LinesZ);
	glPopMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(-halfsize, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, halfsize);
	DrawNet(size, LinesX, LinesZ);
	glTranslatef(0.0, -size, 0.0);
	DrawNet(size, LinesX, LinesZ);
	glPopMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -halfsize);
	glRotatef(90.0, halfsize, 0.0, 0.0);
	DrawNet(size, LinesX, LinesZ);
	glTranslatef(0.0, size, 0.0);
	DrawNet(size, LinesX, LinesZ);
	glPopMatrix();


	//finish rendering:
	glFlush();
	glutSwapBuffers();

}

//handles the key down events
void DisplayMgr::KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:		//ESC
		PostQuitMessage(0);
		break;
	case 'a':
		_cam->RotateY(5.0);
		Display();
		break;
	case 'd':
		_cam->RotateY(-5.0);
		Display();
		break;
	case 'w':
		_cam->MoveForward(-0.1);
		Display();
		break;
	case 's':
		_cam->MoveForward(0.1);
		Display();
		break;
	case 'x':
		_cam->RotateX(5.0);
		Display();
		break;
	case 'y':
		_cam->RotateX(-5.0);
		Display();
		break;
	case 'c':
		_cam->StrafeRight(-0.1);
		Display();
		break;
	case 'v':
		_cam->StrafeRight(0.1);
		Display();
		break;
	case 'f':
		_cam->MoveUpward(-0.3);
		Display();
		break;
	case 'r':
		_cam->MoveUpward(0.3);
		Display();
		break;

	case 'm':
		_cam->RotateZ(-5.0);
		Display();
		break;
	case 'n':
		_cam->RotateZ(5.0);
		Display();
		break;

	}
}

void DisplayMgr::keyboardWrapper(unsigned char key, int x, int y) {
	instance->KeyDown(key,x,y);
}

void DisplayMgr::displayWrapper() {
    instance->displayMain();
}

void DisplayMgr::reshapeWrapper(int width, int height) {
    instance->reshape(width, height);
}

void DisplayMgr::mouseMotionWrapper(int x, int y) {
    instance->mouseMotion(x, y);
}

void DisplayMgr::mouseButtonWrapper(int button, int state, int x, int y) {
    instance->mouseButton(button, state, x, y);
}

void DisplayMgr::mouseWheelWrapper(int button, int dir, int x, int y) {
    instance->mouseWheel(button, dir, x, y);
}
