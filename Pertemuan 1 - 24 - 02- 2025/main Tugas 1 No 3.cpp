#include <GL/glut.h>

void drawCube(float x, float y, float size)
{
    glBegin(GL_QUADS);
    glColor3f(0.2, 0.5, 0.8);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();

    glColor3f(0, 0, 0); 
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();
}

void drawCubes()
{
    float size = 0.2;
    float spacing = 0.02; 

    drawCube(-0.35, -0.3, size);
    drawCube(-0.1, -0.3, size);
    drawCube(0.15, -0.3, size);

    drawCube(-0.225, -0.1, size);
    drawCube(0.025, -0.1, size);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawCubes();

    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Kubus Bertingkat");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

