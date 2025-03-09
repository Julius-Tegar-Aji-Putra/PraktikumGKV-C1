#include <GL/glut.h>

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);  

    glPointSize(5.0f);
    glColor3f(1.0f, 0.0f, 0.0f); 
    glBegin(GL_POINTS);
    glVertex3f(-0.25, -0.25, 0.0);
    glEnd();

   
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBegin(GL_LINES);
    glVertex3f(0.00, 0.20, 0.0);
    glVertex3f(0.00, -0.20, 0.0);
    glEnd();

    glFlush();  
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Membuat Titik & Garis");
    glutDisplayFunc(Display); 
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); 
    glutMainLoop();
    return 0;
}

