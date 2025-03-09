#include <GL/glut.h>

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // GL_LINE_STRIP
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.8, 0.5);
    glVertex2f(-0.4, 0.5);
    glVertex2f(0.0, 0.2);
    glVertex2f(0.4, 0.5);
    glVertex2f(0.8, 0.5);
    glEnd();

    // GL_LINE_LOOP
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.8, 0.2);
    glVertex2f(-0.4, 0.2);
    glVertex2f(0.0, -0.1);
    glVertex2f(0.4, 0.2);
    glVertex2f(0.8, 0.2);
    glEnd();

    // GL_TRIANGLE_FAN
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(1.0, 0.0);
    glVertex2f(0.6, -0.4);
    glVertex2f(1.4, -0.4);
    glVertex2f(1.4, 0.4);
    glVertex2f(0.6, 0.4);
    glEnd();

    // GL_TRIANGLE_STRIP
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-0.6, -0.6);
    glVertex2f(0.0, -1.0);
    glVertex2f(0.6, -0.6);
    glVertex2f(0.0, -0.2);
    glEnd();

    // GL_QUADS
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -0.6);
    glVertex2f(-0.4, -0.6);
    glVertex2f(-0.4, -0.2);
    glVertex2f(-1.0, -0.2);
    glEnd();

    // GL_QUAD_STRIP
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUAD_STRIP);
    glVertex2f(0.0, -0.8);
    glVertex2f(0.4, -0.8);
    glVertex2f(0.0, -1.2);
    glVertex2f(0.4, -1.2);
    glVertex2f(0.0, -1.6);
    glVertex2f(0.4, -1.6);
    glEnd();

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Primitif OpenGL");
    glutDisplayFunc(Display);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Background putih
    glutMainLoop();
    return 0;
}

