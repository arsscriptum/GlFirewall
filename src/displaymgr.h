
//==============================================================================
//
//     displaymgr.h
//
//============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================



#ifndef __DISPLAYMGR_H__
#define __DISPLAYMGR_H__

#include <GL/glut.h>

class DisplayMgr {
public:
    DisplayMgr(int argc, char** argv);
    void init();
    void displayMain();
    void reshape(int width, int height);
    void mouseMotion(int x, int y);
    void mouseButton(int button, int state, int x, int y);
    void mouseWheel(int button, int dir, int x, int y);
    void KeyDown(unsigned char key, int x, int y);
    void Display(void);
    void DrawNet(GLfloat size, GLint LinesX, GLint LinesZ);
    void reshapeWindow(int x, int y);
    void reshapeRefresh(int x, int y);
    static void displayWrapper();
    static void reshapeWrapper(int width, int height);
    static void mouseMotionWrapper(int x, int y);
    static void mouseButtonWrapper(int button, int state, int x, int y);
    static void mouseWheelWrapper(int button, int dir, int x, int y);
    static void keyboardWrapper(unsigned char key, int x, int y);
private:
    float camX, camY, camZ;
    float camAngleX, camAngleY;
    int lastMouseX, lastMouseY;
    bool leftMouseButtonPressed;
    static DisplayMgr* instance;
    class CCamera *_cam;
};

#endif // __DISPLAYMGR_H__
