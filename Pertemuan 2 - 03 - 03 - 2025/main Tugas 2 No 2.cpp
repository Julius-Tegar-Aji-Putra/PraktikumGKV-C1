#include <GL/glut.h>
#include <math.h>

// Fungsi untuk menggambar badan mobil 
void drawCarBody() {
    // Bagian utama mobil (badan)
    glColor3f(1.0, 1.0, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.7, -0.2);
        glVertex2f(0.655, -0.2);
        glVertex2f(0.655, 0.1);
        glVertex2f(-0.7, 0.1);
    glEnd();

    // Bagian atap mobil
    glColor3f(1.0, 1.0, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.7, 0.1);
        glVertex2f(0.4, 0.1);
        glVertex2f(0.3, 0.4);
        glVertex2f(-0.7, 0.4);
    glEnd();
    
    // List Jendela (bingkai hitam)
    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.7, 0.125);
        glVertex2f(0.35, 0.125);
        glVertex2f(0.275, 0.375);
        glVertex2f(-0.7, 0.375);
    glEnd();

    // Jendela kiri
    glColor4f(0.5, 0.5, 0.5, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.7, 0.15);
        glVertex2f(-0.5, 0.15);
        glVertex2f(-0.5, 0.35);
        glVertex2f(-0.7, 0.35);
    glEnd();
    
    // Jendela tengah
    glColor4f(0.5, 0.5, 0.5, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.4, 0.15);
        glVertex2f(-0.1, 0.15);
        glVertex2f(-0.1, 0.35);
        glVertex2f(-0.4, 0.35);
    glEnd();
    
    // Jendela kanan
    glColor4f(0.5, 0.5, 0.5, 1.0);  
    glBegin(GL_POLYGON);
        glVertex2f(0.3, 0.15);
        glVertex2f(0.0, 0.15);
        glVertex2f(0.0, 0.35);
        glVertex2f(0.24, 0.35);
    glEnd();

    // Gagang pintu kiri 
    glLineWidth(10.0f); 
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(-0.3f, 0.05f);
        glVertex2f(-0.4f, 0.05f);
    glEnd();
    
    // Gagang pintu kanan
    glLineWidth(10.0f); 
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(0.0f, 0.05f);
        glVertex2f(0.1f, 0.05f);
    glEnd();

    // Outline hitam di badan mobil
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.7, -0.2);
        glVertex2f(0.655, -0.2);
        glVertex2f(0.655, 0.1);
        glVertex2f(-0.7, 0.1);
    glEnd();

    // Outline hitam di atap mobil
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.7, 0.1);
        glVertex2f(0.4, 0.1);
        glVertex2f(0.3, 0.4);
        glVertex2f(-0.7, 0.4);
    glEnd();
    
    // Lampu merah (belakang)
    glColor4f(1.0, 0.0, 0.0, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.7, 0.05);
        glVertex2f(-0.6, 0.05);
        glVertex2f(-0.6, 0.0);
        glVertex2f(-0.7, 0.0);
    glEnd();
    
    // Lampu kuning kiri 
    glColor4f(1.0, 1.0, 0.0, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.7, 0.0);
        glVertex2f(-0.6, 0.0);
        glVertex2f(-0.6, -0.05);
        glVertex2f(-0.7, -0.05);
    glEnd();
    
    // Lampu kuning kanan (depan)
    glColor4f(1.0, 1.0, 0.0, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(0.655, 0.0);
        glVertex2f(0.555, 0.0);
        glVertex2f(0.555, -0.05);
        glVertex2f(0.655, -0.05);
    glEnd();
    
    // Tambahan list body bagian bawah
    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_POLYGON);
        glVertex2f(0.66, -0.165);  
        glVertex2f(-0.7, -0.165); 
        glVertex2f(-0.7, -0.2);   
        glVertex2f(0.66, -0.2);   
    glEnd();
    
    // Gambar cekungan (trapezium) untuk roda mobil belakang
    glColor3f(0.2, 0.2, 0.2); 
    glBegin(GL_POLYGON);
        glVertex2f(-0.665, -0.2);  
        glVertex2f(-0.325, -0.2); 
        glVertex2f(-0.385, -0.1);   
        glVertex2f(-0.605, -0.1);   
    glEnd();
    
    // Gambar cekungan (trapezium) untuk roda mobil depan
    glColor3f(0.2, 0.2, 0.2); 
    glBegin(GL_POLYGON);
        glVertex2f(0.62, -0.2);  
        glVertex2f(0.28, -0.2); 
        glVertex2f(0.34, -0.1);   
        glVertex2f(0.56, -0.1);   
    glEnd();
    
    // Tambahkan garis vertikal kiri untuk pintu
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(-0.45f, -0.1f);
        glVertex2f(-0.45f, 0.3f); 
    glEnd();
    
    // Tambahkan garis vertikal kiri untuk pintu
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(-0.05f, -0.2f);
        glVertex2f(-0.05f, 0.3f); 
    glEnd();
    
    // Tambahkan garis vertikal kiri untuk pintu
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(0.345f, -0.1f);
        glVertex2f(0.345f, 0.138f); 
    glEnd();
}

// Fungsi untuk menggambar roda mobil dengan desain pizza
void drawDetailPizza(float centerX, float centerY, float radius) {
    int numSlices = 8; 
    float anglePerSlice = 360.0f / numSlices;

    // Gambar setiap irisan pizza
    for (int i = 0; i < numSlices; i++) {
        float startAngle = i * anglePerSlice;
        float endAngle = (i + 1) * anglePerSlice;

        glColor3f(0.0, 0.0, 0.0); 

        glBegin(GL_POLYGON);
        for (int j = (int)(startAngle); j < (int)(endAngle); j++) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle)); 
        }
        glEnd();
    }

    // Desain velg (garis-garis di dalam roda)
    glColor3f(0.0, 0.0, 0.0); 
    for (int i = 0; i < 12; i++) {
        float angle = (i * 30) * 3.14159f / 180.0f;
        glBegin(GL_LINES);
            glVertex2f(centerX + (radius * 0.5f) * cos(angle), centerY + (radius * 0.5f) * sin(angle));
            glVertex2f(centerX + radius * cos(angle),         centerY + radius * sin(angle));
        glEnd();
    }

    // Lubang tengah velg
    glColor3f(0.5, 0.5, 0.5);  
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(centerX + (radius * 0.75f) * cos(angle), centerY + (radius * 0.75f) * sin(angle));
        }
    glEnd();
}

// Fungsi untuk menggambar roda mobil
void drawCarWheels() {
    float radius = 0.14;
    int numSlices = 8; 
    float anglePerSlice = 360.0f / numSlices;

    // Roda kiri
    glColor3f(0.0, 0.0, 0.0);  
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(-0.5 + radius * cos(angle), -0.3 + radius * sin(angle));
        }
    glEnd();

    // Membuat irisan pizza di velg kiri dengan ketebalan lebih tebal dan lebih banyak
    for (int i = 0; i < numSlices; i++) {
        float startAngle = i * anglePerSlice;
        float endAngle = (i + 1) * anglePerSlice;

        glColor3f(0.0, 0.0, 0.0); 

        glBegin(GL_POLYGON);
        for (int j = (int)(startAngle); j < (int)(endAngle); j++) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(-0.5 + (radius * 0.85f) * cos(angle), -0.3 + (radius * 0.85f) * sin(angle));  
        }
        glEnd();
    }

    // Roda kanan
    glColor3f(0.0, 0.0, 0.0);  
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(0.45 + radius * cos(angle), -0.3 + radius * sin(angle));
        }
    glEnd();

    // Membuat irisan pizza di velg kanan dengan ketebalan lebih tebal
    for (int i = 0; i < numSlices; i++) {
        float startAngle = i * anglePerSlice;
        float endAngle = (i + 1) * anglePerSlice;

        glColor3f(0.0, 0.0, 0.0); 

        glBegin(GL_POLYGON);
        for (int j = (int)(startAngle); j < (int)(endAngle); j++) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(0.45 + (radius * 0.85f) * cos(angle), -0.3 + (radius * 0.85f) * sin(angle)); 
        }
        glEnd();
    }

    // Desain velg (garis-garis di dalam roda)
    glColor3f(0.3, 0.3, 0.3);  
    for (int i = 0; i < 12; i++) {
        float angle = (i * 30) * 3.14159f / 180.0f;
        glBegin(GL_LINES);
            glVertex2f(-0.5 + (radius * 0.5f) * cos(angle), -0.3 + (radius * 0.5f) * sin(angle));
            glVertex2f(-0.5 + radius * cos(angle),         -0.3 + radius * sin(angle));
        glEnd();
    }

    for (int i = 0; i < 12; i++) {
        float angle = (i * 30) * 3.14159f / 180.0f;
        glBegin(GL_LINES);
            glVertex2f(0.45 + (radius * 0.5f) * cos(angle), -0.3 + (radius * 0.5f) * sin(angle));
            glVertex2f(0.45 + radius * cos(angle),          -0.3 + radius * sin(angle));
        glEnd();
    }

    // Lubang tengah velg (kiri)
    glColor3f(0.5, 0.5, 0.5);  
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(-0.5 + (radius * 0.75f) * cos(angle), -0.3 + (radius * 0.75f) * sin(angle));
        }
    glEnd();

    // Lubang tengah velg (kanan)
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(0.45 + (radius * 0.75f) * cos(angle), -0.3 + (radius * 0.75f) * sin(angle));
        }
    glEnd();

    // Roda kiri - Detail pizza dengan ukuran lebih kecil
    drawDetailPizza(-0.5f, -0.3f, radius * 0.65);

    // Roda kanan - Detail pizza dengan ukuran lebih kecil
    drawDetailPizza(0.45f, -0.3f, radius * 0.65);
    
    // Menambahkan lingkaran hitam di tengah roda kiri
    glColor3f(0.0f, 0.0f, 0.0f);  
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(-0.5f + (radius * 0.475f) * cos(angle), -0.3f + (radius * 0.475f) * sin(angle));
        }
    glEnd();

    // Menambahkan lingkaran hitam di tengah roda kanan
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(0.45f + (radius * 0.475f) * cos(angle), -0.3f + (radius * 0.475f) * sin(angle));
        }
    glEnd();
}


void drawSkyAndGround() {
    // Gambar langit (background)
    glColor3f(0.5, 0.7, 1.0); 
    glBegin(GL_POLYGON);
        glVertex2f(-1.0, 0.5);
        glVertex2f(1.0, 0.5);
        glVertex2f(1.0, 1.0);
        glVertex2f(-1.0, 1.0);
    glEnd();

    // Gambar jalan (warna coklat)
    glColor3f(0.6, 0.3, 0.0); 
    glPushMatrix();
        glRotatef(15.0f, 0.0f, 0.0f, 1.0f);  
        glBegin(GL_POLYGON);
            glVertex2f(-5.0, -0.35);
            glVertex2f(5.0, -0.35);
            glVertex2f(1.0, -5.0);
            glVertex2f(-1.0, -5.0);
        glEnd();
    glPopMatrix();

    // Gambar matahari (lingkaran)
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(0.8f + 0.1f * cos(angle), 0.8f + 0.1f * sin(angle));
        }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawSkyAndGround();

    glPushMatrix();
        glTranslatef(0.0f, -0.1f, 0.0f);  
        glRotatef(15.0f, 0.0f, 0.0f, 1.0f); 
        drawCarBody(); 
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, -0.05f, 0.0f);  
        glRotatef(15.0f, 0.0f, 0.0f, 1.0f);  
        drawCarWheels();
    glPopMatrix();

    glFlush();
}

void init() {
    glClearColor(0.5, 0.7, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Mobil");

    init();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

