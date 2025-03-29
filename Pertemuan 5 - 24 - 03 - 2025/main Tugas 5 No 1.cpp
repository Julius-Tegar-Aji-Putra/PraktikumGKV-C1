#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

double rx = 0.0;
double ry = 0.0;
float l[] = { 0.0, 80.0, 0.0 }; 
float n[] = { 0.0, -40.0, 0.0 };
float e[] = { 0.0, -60.0, 0.0 };

void help();

// Obyek yang akan digambar
void draw(int isShadow) {
    // Set uniform shadow color if drawing shadow
    if (isShadow) {
        glColor3f(0.4, 0.4, 0.4); 
    }
    
    // Main blade 
    glBegin(GL_TRIANGLES);
    if (!isShadow) glColor3f(0.1, 0.1, 0.1); 
    // Tip of the blade
    glVertex3f(0.0, 60.0, 0.0);  
    glVertex3f(-5.0, 40.0, 1.0);  
    glVertex3f(5.0, 40.0, 1.0);   
    
    // Back of tip
    glVertex3f(0.0, 60.0, 0.0);  
    glVertex3f(5.0, 40.0, 1.0);  
    glVertex3f(5.0, 40.0, -1.0); 
    
    glVertex3f(0.0, 60.0, 0.0);   
    glVertex3f(-5.0, 40.0, -1.0); 
    glVertex3f(-5.0, 40.0, 1.0);  
    
    glVertex3f(0.0, 60.0, 0.0);   
    glVertex3f(-5.0, 40.0, -1.0); 
    glVertex3f(5.0, 40.0, -1.0);  
    glEnd();
    
    // Main body of the blade
    glBegin(GL_QUADS);
    if (!isShadow) glColor3f(0.1, 0.1, 0.1); 
    // Front face
    glVertex3f(-5.0, 40.0, 1.0);  
    glVertex3f(5.0, 40.0, 1.0);   
    glVertex3f(8.0, -40.0, 1.0);  
    glVertex3f(-8.0, -40.0, 1.0); 
    
    // Back face
    glVertex3f(-5.0, 40.0, -1.0); 
    glVertex3f(5.0, 40.0, -1.0);  
    glVertex3f(8.0, -40.0, -1.0); 
    glVertex3f(-8.0, -40.0, -1.0);
    
    // Right face
    glVertex3f(5.0, 40.0, 1.0);   
    glVertex3f(5.0, 40.0, -1.0);  
    glVertex3f(8.0, -40.0, -1.0); 
    glVertex3f(8.0, -40.0, 1.0);  
    
    // Left face
    glVertex3f(-5.0, 40.0, 1.0);  
    glVertex3f(-5.0, 40.0, -1.0); 
    glVertex3f(-8.0, -40.0, -1.0);
    glVertex3f(-8.0, -40.0, 1.0); 
    glEnd();
    
    // Edge details on the blade 
    glBegin(GL_QUADS);
    if (!isShadow) glColor3f(0.8, 0.8, 0.9); 
    // Right edge highlight
    glVertex3f(4.8, 40.0, 1.1);   
    glVertex3f(7.8, -40.0, 1.1);  
    glVertex3f(7.8, -40.0, 0.9);  
    glVertex3f(4.8, 40.0, 0.9);   
    
    // Left edge highlight
    glVertex3f(-4.8, 40.0, 1.1);  
    glVertex3f(-7.8, -40.0, 1.1); 
    glVertex3f(-7.8, -40.0, 0.9); 
    glVertex3f(-4.8, 40.0, 0.9);  
    glEnd();
    
    // Decorative diamond pattern on blade 
    glBegin(GL_QUADS);
    if (!isShadow) glColor3f(0.3, 0.3, 0.35); 
    // Diamond 1
    glVertex3f(0.0, 30.0, 1.1); 
    glVertex3f(2.0, 25.0, 1.1);  
    glVertex3f(0.0, 20.0, 1.1);  
    glVertex3f(-2.0, 25.0, 1.1); 
    
    // Diamond 2
    glVertex3f(0.0, 10.0, 1.1); 
    glVertex3f(2.0, 5.0, 1.1);  
    glVertex3f(0.0, 0.0, 1.1);   
    glVertex3f(-2.0, 5.0, 1.1);  
    
    // Diamond 3
    glVertex3f(0.0, -10.0, 1.1);  
    glVertex3f(2.0, -15.0, 1.1);  
    glVertex3f(0.0, -20.0, 1.1);  
    glVertex3f(-2.0, -15.0, 1.1); 
    
    // Diamond 4
    glVertex3f(0.0, -30.0, 1.1);  
    glVertex3f(2.5, -35.0, 1.1); 
    glVertex3f(0.0, -40.0, 1.1);  
    glVertex3f(-2.5, -35.0, 1.1); 
    glEnd();
    
    // Crossguard
    glBegin(GL_QUADS);
    if (!isShadow) glColor3f(0.15, 0.15, 0.15); 
    // Main body
    // Top face
    glVertex3f(-15.0, -40.0, 5.0);  
    glVertex3f(15.0, -40.0, 5.0);   
    glVertex3f(15.0, -40.0, -5.0);  
    glVertex3f(-15.0, -40.0, -5.0); 
    
    // Bottom face
    glVertex3f(-15.0, -45.0, 5.0);  
    glVertex3f(15.0, -45.0, 5.0);   
    glVertex3f(15.0, -45.0, -5.0);  
    glVertex3f(-15.0, -45.0, -5.0); 
    
    // Front face
    glVertex3f(-15.0, -40.0, 5.0);  
    glVertex3f(15.0, -40.0, 5.0);   
    glVertex3f(15.0, -45.0, 5.0);   
    glVertex3f(-15.0, -45.0, 5.0);  
    
    // Back face
    glVertex3f(-15.0, -40.0, -5.0); 
    glVertex3f(15.0, -40.0, -5.0);  
    glVertex3f(15.0, -45.0, -5.0);  
    glVertex3f(-15.0, -45.0, -5.0); 
    
    // Left face
    glVertex3f(-15.0, -40.0, 5.0);  
    glVertex3f(-15.0, -40.0, -5.0); 
    glVertex3f(-15.0, -45.0, -5.0); 
    glVertex3f(-15.0, -45.0, 5.0);  
    
    // Right face
    glVertex3f(15.0, -40.0, 5.0);   
    glVertex3f(15.0, -40.0, -5.0);  
    glVertex3f(15.0, -45.0, -5.0);  
    glVertex3f(15.0, -45.0, 5.0);   
    glEnd();
    
    // Decorative cross element on crossguard (silver accent)
    glBegin(GL_QUADS);
    if (!isShadow) glColor3f(0.75, 0.75, 0.8); 
    // Horizontal part of cross (front)
    glVertex3f(-10.0, -42.0, 5.1);  
    glVertex3f(10.0, -42.0, 5.1);   
    glVertex3f(10.0, -43.0, 5.1);   
    glVertex3f(-10.0, -43.0, 5.1);  
    
    // Vertical part of cross (front)
    glVertex3f(-1.5, -40.5, 5.1);  
    glVertex3f(1.5, -40.5, 5.1);   
    glVertex3f(1.5, -44.5, 5.1);    
    glVertex3f(-1.5, -44.5, 5.1);   
    
    // Horizontal part of cross (back)
    glVertex3f(-10.0, -42.0, -5.1);  
    glVertex3f(10.0, -42.0, -5.1);  
    glVertex3f(10.0, -43.0, -5.1); 
    glVertex3f(-10.0, -43.0, -5.1); 
    
    // Vertical part of cross (back)
    glVertex3f(-1.5, -40.5, -5.1); 
    glVertex3f(1.5, -40.5, -5.1);    
    glVertex3f(1.5, -44.5, -5.1);    
    glVertex3f(-1.5, -44.5, -5.1);   
    glEnd();
    
    // Handle/grip 
    glBegin(GL_QUADS);
    if (!isShadow) glColor3f(0.15, 0.15, 0.15); 
    // Front face
    glVertex3f(-3.0, -45.0, 3.0);  
    glVertex3f(3.0, -45.0, 3.0);  
    glVertex3f(3.0, -70.0, 3.0);   
    glVertex3f(-3.0, -70.0, 3.0);  
    
    // Back face
    glVertex3f(-3.0, -45.0, -3.0); 
    glVertex3f(3.0, -45.0, -3.0);  
    glVertex3f(3.0, -70.0, -3.0);  
    glVertex3f(-3.0, -70.0, -3.0); 
    
    // Left face
    glVertex3f(-3.0, -45.0, 3.0);  
    glVertex3f(-3.0, -45.0, -3.0); 
    glVertex3f(-3.0, -70.0, -3.0); 
    glVertex3f(-3.0, -70.0, 3.0);  
    
    // Right face
    glVertex3f(3.0, -45.0, 3.0);   
    glVertex3f(3.0, -45.0, -3.0);  
    glVertex3f(3.0, -70.0, -3.0);  
    glVertex3f(3.0, -70.0, 3.0);   
    glEnd();
    
    // Grip texture 
    glBegin(GL_LINES);
    if (!isShadow) glColor3f(0.3, 0.3, 0.3);
    for (int i = -45; i > -70; i -= 3) {
        glVertex3f(-3.1, i, 3.1);  
        glVertex3f(3.1, i, 3.1); 
        
        glVertex3f(-3.1, i, -3.1); 
        glVertex3f(3.1, i, -3.1); 
        
        glVertex3f(-3.1, i, 3.1);  
        glVertex3f(-3.1, i, -3.1); 
        
        glVertex3f(3.1, i, 3.1);  
        glVertex3f(3.1, i, -3.1); 
    }
    glEnd();
    
    // Pommel (end piece of the handle)
    glBegin(GL_QUADS);
    if (!isShadow) glColor3f(0.2, 0.2, 0.2); 
    // Main body 
    // Top face
    glVertex3f(-5.0, -70.0, 5.0);  
    glVertex3f(5.0, -70.0, 5.0);   
    glVertex3f(5.0, -70.0, -5.0);  
    glVertex3f(-5.0, -70.0, -5.0); 
    
    // Bottom face
    glVertex3f(-5.0, -75.0, 5.0);  
    glVertex3f(5.0, -75.0, 5.0);  
    glVertex3f(5.0, -75.0, -5.0); 
    glVertex3f(-5.0, -75.0, -5.0); 
    
    // Front face
    glVertex3f(-5.0, -70.0, 5.0);  
    glVertex3f(5.0, -70.0, 5.0);   
    glVertex3f(5.0, -75.0, 5.0);   
    glVertex3f(-5.0, -75.0, 5.0);  
    
    // Back face
    glVertex3f(-5.0, -70.0, -5.0); 
    glVertex3f(5.0, -70.0, -5.0);  
    glVertex3f(5.0, -75.0, -5.0);  
    glVertex3f(-5.0, -75.0, -5.0); 
    
    // Left face
    glVertex3f(-5.0, -70.0, 5.0);  
    glVertex3f(-5.0, -70.0, -5.0); 
    glVertex3f(-5.0, -75.0, -5.0); 
    glVertex3f(-5.0, -75.0, 5.0);  
    
    // Right face
    glVertex3f(5.0, -70.0, 5.0);   
    glVertex3f(5.0, -70.0, -5.0);  
    glVertex3f(5.0, -75.0, -5.0);  
    glVertex3f(5.0, -75.0, 5.0);   
    glEnd();
    
    // Decorative element
    if (!isShadow) {
        glPushMatrix();
        glColor3f(0.8, 0.8, 0.9); 
        glTranslatef(0.0, -72.5, 5.1); 
        glutSolidSphere(1.5, 8, 8);   
        glPopMatrix();
        
        glPushMatrix();
        glColor3f(0.8, 0.8, 0.9);
        glTranslatef(0.0, -72.5, -5.1); 
        glutSolidSphere(1.5, 8, 8);      
        glPopMatrix();
    } else {
        glPushMatrix();
        glTranslatef(0.0, -72.5, 5.1); 
        glutSolidSphere(1.5, 8, 8);    
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.0, -72.5, -5.1); 
        glutSolidSphere(1.5, 8, 8);     
        glPopMatrix();
    }
}

// Membuat proyeksi bayangan
void glShadowProjection(float *l, float *e, float *n) {
    float d, c;
    float mat[16];
    
    d = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
    c = e[0] * n[0] + e[1] * n[1] + e[2] * n[2] - d;

    // Membuat matrik proyeksi bayangan
    mat[0] = l[0] * n[0] + c;
    mat[4] = n[1] * l[0];
    mat[8] = n[2] * l[0];
    mat[12] = -l[0] * c - l[0] * d;

    mat[1] = n[0] * l[1];
    mat[5] = l[1] * n[1] + c;
    mat[9] = n[2] * l[1];
    mat[13] = -l[1] * c - l[1] * d;

    mat[2] = n[0] * l[2];
    mat[6] = n[1] * l[2];
    mat[10] = l[2] * n[2] + c;
    mat[14] = -l[2] * c - l[2] * d;

    mat[3] = n[0];
    mat[7] = n[1];
    mat[11] = n[2];
    mat[15] = -d;

    glMultMatrixf(mat); // Kalikan matrik
}

void render() {
    glClearColor(0.0, 0.6, 0.9, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Posisi sumber cahaya
    glLightfv(GL_LIGHT0, GL_POSITION, l);
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    
    // Gambar titik cahaya
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(l[0], l[1], l[2]);
    glEnd();

    // Gambar lantai
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-1300.0, e[1] - 0.1, 1300.0);
    glVertex3f(1300.0, e[1] - 0.1, 1300.0);
    glVertex3f(1300.0, e[1] - 0.1, -1300.0);
    glVertex3f(-1300.0, e[1] - 0.1, -1300.0);
    glEnd();

    // Gambar objek 3D 
    glPushMatrix();
    glTranslatef(0.0, 30.0, 0.0); 
    glRotatef(ry, 0, 1, 0);
    glRotatef(rx, 1, 0, 0);
    glScalef(0.6, 0.6, 0.6);      
    glEnable(GL_LIGHTING);
    draw(0); //  not shadow
    glPopMatrix();

    // Gambar bayangan objek 
    glPushMatrix();
    glTranslatef(0.0, 30.0, 0.0); 
    glShadowProjection(l, e, n);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rx, 1, 0, 0);
    glScalef(0.6, 0.6, 0.6);     
    glDisable(GL_LIGHTING);
    draw(1); // shadow
    glPopMatrix();

    glutSwapBuffers();
}

void keypress(unsigned char c, int a, int b) {
    if (c == 27) {
        exit(0);
    } else if (c == 's') {
        l[1] -= 5.0;
    } else if (c == 'w') {
        l[1] += 5.0;
    } else if (c == 'a') {
        l[0] -= 5.0;
    } else if (c == 'd') {
        l[0] += 5.0;
    } else if (c == 'q') {
        l[2] -= 5.0;
    } else if (c == 'e') {
        l[2] += 5.0;
    } else if (c == 'h') {
        help();
    }
}

void help() {
    printf("Tugas 5 No 1\n");
}

void idle() {
    rx += 0.1;
    ry += 0.1;
    render();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(300, 30);
    glutCreateWindow("Tugas 5 No 1");
    
    glutReshapeFunc(resize);
    glutReshapeWindow(400, 400);
    glutKeyboardFunc(keypress);
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.0, 1.0, 400.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -150.0);

    glutMainLoop();
    return 0;
}

