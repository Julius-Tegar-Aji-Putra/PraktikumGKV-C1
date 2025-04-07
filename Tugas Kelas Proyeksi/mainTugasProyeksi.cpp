#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>

// Window size
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// Camera variables
float angle = 0.0f;
float deltaAngle = 0.0f;
float x = 0.0f, y = 2.0f, z = 5.0f;  // Initial camera position
float lx = 0.0f, ly = 0.0f, lz = -1.0f;  // Camera direction
int deltaMove = 0;
float ratio;

// Variabel untuk zoom di tampilan ortogonal
float orthoZoom = 1.0f; // Faktor zoom (1.0 = normal)

// Mode proyeksi
enum ProjectionMode {
    FREE_CAMERA,
    PERSPECTIVE_1POINT,
    PERSPECTIVE_2POINT, 
    PERSPECTIVE_3POINT,
    ORTHO_PLAN,
    ORTHO_FRONT,
    ORTHO_SIDE,
    ORTHO_ISOMETRIC,
    ORTHO_DIMETRIC,
    ORTHO_TRIMETRIC
};
int currentProjection = FREE_CAMERA;

// Struct for vanishing point
typedef struct {
    float x, y, z;
    int active;
} VanishingPoint;

VanishingPoint vanishingPoints[3]; // 3 vanishing point

// Fungsi sederhana untuk menampilkan text di OpenGL
void drawText(const char* text, float x, float y, float r, float g, float b) {
    // Set warna teks
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    
    // Render teks karakter per karakter
    for (const char* c = text; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

// Fungsi untuk menampilkan info proyeksi saat ini
void displayProjectionInfo() {
    char buffer[100];
    
    switch (currentProjection) {
        case FREE_CAMERA:
            drawText("Mode: Free Camera Mode", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case PERSPECTIVE_1POINT:
            drawText("Mode: Perspektif 1 Titik Hilang", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case PERSPECTIVE_2POINT:
            drawText("Mode: Perspektif 2 Titik Hilang", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case PERSPECTIVE_3POINT:
            drawText("Mode: Perspektif 3 Titik Hilang", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case ORTHO_PLAN:
            drawText("Mode: Ortogonal - Plan View (Tampak Atas)", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case ORTHO_FRONT:
            drawText("Mode: Ortogonal - Front View (Tampak Depan)", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case ORTHO_SIDE:
            drawText("Mode: Ortogonal - Side View (Tampak Samping)", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case ORTHO_ISOMETRIC:
            drawText("Mode: Ortogonal - Isometric View (Sudut 120°)", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case ORTHO_DIMETRIC:
            drawText("Mode: Ortogonal - Dimetric View (Sudut 130°, 130°)", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
        case ORTHO_TRIMETRIC:
            drawText("Mode: Ortogonal - Trimetric View (Sudut 125°, 135°, 100°)", 10, WINDOW_HEIGHT - 30, 1.0, 1.0, 1.0);
            break;
    }
    
    drawText("Tekan '0' untuk kembali ke mode Free Camera", 10, WINDOW_HEIGHT - 50, 1.0, 1.0, 0.0);
}

// Fungsi untuk menggambar titik hilang dan garis ke objek
void drawVanishingPoints() {
    // Hanya gambar titik hilang jika mode perspektif
    if (currentProjection < PERSPECTIVE_1POINT || currentProjection > PERSPECTIVE_3POINT)
        return;
        
    glDisable(GL_LIGHTING);
    
    // Jumlah titik hilang aktif tergantung mode proyeksi
    int numActivePoints = currentProjection - PERSPECTIVE_1POINT + 1;
    
    // Titik hilang di luar display range, gambar indikator arah saja
    // Gunakan warna berbeda untuk setiap titik hilang
    glLineWidth(2.0);
    glBegin(GL_LINES);
    
    // Posisi mobil (pusat objek)
    float carPos[3] = {0.0f, 0.0f, 0.0f};
    
    for (int i = 0; i < numActivePoints; i++) {
        if (vanishingPoints[i].active) {
            // Warna berbeda untuk setiap titik hilang
            if (i == 0) glColor3f(1.0f, 0.0f, 0.0f); // Merah
            else if (i == 1) glColor3f(0.0f, 1.0f, 0.0f); // Hijau
            else glColor3f(0.0f, 0.0f, 1.0f); // Biru
            
            // Gambar garis dari pusat objek ke arah titik hilang
            glVertex3f(carPos[0], carPos[1], carPos[2]);
            glVertex3f(vanishingPoints[i].x * 0.2f, 
                       vanishingPoints[i].y * 0.2f, 
                       vanishingPoints[i].z * 0.2f);
            
            // Gambar garis dari corner mobil
            float carCorners[8][3] = {
                {-0.7f, -0.15f, 0.3f},  // Depan kiri bawah
                {0.7f, -0.15f, 0.3f},   // Depan kanan bawah
                {0.7f, 0.3f, 0.3f},     // Depan kanan atas
                {-0.7f, 0.3f, 0.3f},    // Depan kiri atas
                {-0.7f, -0.15f, -0.3f}, // Belakang kiri bawah
                {0.7f, -0.15f, -0.3f},  // Belakang kanan bawah
                {0.7f, 0.3f, -0.3f},    // Belakang kanan atas
                {-0.7f, 0.3f, -0.3f},   // Belakang kiri atas
            };
            
            for (int j = 0; j < 8; j++) {
                // Hitung arah dari sudut ke titik hilang
                float dirX = vanishingPoints[i].x - carCorners[j][0];
                float dirY = vanishingPoints[i].y - carCorners[j][1];
                float dirZ = vanishingPoints[i].z - carCorners[j][2];
                
                // Normalisasi arah
                float len = sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
                dirX /= len;
                dirY /= len;
                dirZ /= len;
                
                // Gambar garis dari sudut ke arah titik hilang
                glVertex3f(carCorners[j][0], carCorners[j][1], carCorners[j][2]);
                glVertex3f(carCorners[j][0] + dirX * 5.0f, 
                           carCorners[j][1] + dirY * 5.0f, 
                           carCorners[j][2] + dirZ * 5.0f);
            }
        }
    }
    glEnd();
    glLineWidth(1.0);
    
    // Label titik hilang
    if (currentProjection == PERSPECTIVE_1POINT) {
        drawText("1 Titik Hilang (ke depan)", WINDOW_WIDTH - 200, 30, 1.0f, 0.0f, 0.0f);
    }
    else if (currentProjection == PERSPECTIVE_2POINT) {
        drawText("2 Titik Hilang (ke depan & samping)", WINDOW_WIDTH - 250, 30, 1.0f, 0.0f, 0.0f);
    }
    else if (currentProjection == PERSPECTIVE_3POINT) {
        drawText("3 Titik Hilang (depan, samping & atas)", WINDOW_WIDTH - 280, 30, 1.0f, 0.0f, 0.0f);
    }
    
    glEnable(GL_LIGHTING);
}

// Fungsi untuk menggambar grid dan garis bantu proyeksi
void drawProjectionGrid() {
    glDisable(GL_LIGHTING);
    
        // Jika dalam mode ortogonal
    if (currentProjection >= ORTHO_PLAN && currentProjection <= ORTHO_TRIMETRIC) {
        // Ukuran grid tetap
        float gridSize = 10.0f; // Ukuran grid tetap
        float gridStep = 0.5f;
        
        if (currentProjection == ORTHO_PLAN || 
            currentProjection == ORTHO_FRONT || 
            currentProjection == ORTHO_SIDE) {
            glLineWidth(1.0f);
            glBegin(GL_LINES);
            
            // Grid untuk tampak atas (plan)
            if (currentProjection == ORTHO_PLAN) {
                glColor3f(0.7f, 0.7f, 0.7f);
                for (float i = -gridSize; i <= gridSize; i += gridStep) {
                    // Garis sepanjang sumbu x
                    glVertex3f(-gridSize, -0.49f, i);
                    glVertex3f(gridSize, -0.49f, i);
                    
                    // Garis sepanjang sumbu z
                    glVertex3f(i, -0.49f, -gridSize);
                    glVertex3f(i, -0.49f, gridSize);
                }
            }
            // Grid untuk tampak depan (front)
            else if (currentProjection == ORTHO_FRONT) {
                glColor3f(0.7f, 0.7f, 0.7f);
                for (float i = -gridSize; i <= gridSize; i += gridStep) {
                    // Garis horizontal
                    glVertex3f(-gridSize, i, 0.0f);
                    glVertex3f(gridSize, i, 0.0f);
                    
                    // Garis vertikal
                    glVertex3f(i, -gridSize, 0.0f);
                    glVertex3f(i, gridSize, 0.0f);
                }
            }
            // Grid untuk tampak samping (side)
            else if (currentProjection == ORTHO_SIDE) {
                glColor3f(0.7f, 0.7f, 0.7f);
                for (float i = -gridSize; i <= gridSize; i += gridStep) {
                    // Garis horizontal
                    glVertex3f(0.0f, i, -gridSize);
                    glVertex3f(0.0f, i, gridSize);
                    
                    // Garis vertikal
                    glVertex3f(0.0f, -gridSize, i);
                    glVertex3f(0.0f, gridSize, i);
                }
            }
            
            // Gambar sumbu koordinat dengan warna yang menyala
            glLineWidth(3.0f);
            
            // Sumbu X (merah)
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-gridSize, 0.0f, 0.0f);
            glVertex3f(gridSize, 0.0f, 0.0f);
            
            // Sumbu Y (hijau)
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, -gridSize, 0.0f);
            glVertex3f(0.0f, gridSize, 0.0f);
            
            // Sumbu Z (biru)
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, -gridSize);
            glVertex3f(0.0f, 0.0f, gridSize);
            
            glEnd();
            glLineWidth(1.0f);
        }
        
        // Grid untuk proyeksi axonometric (isometric, dimetric, trimetric)
	else if (currentProjection == ORTHO_ISOMETRIC || 
	         currentProjection == ORTHO_DIMETRIC || 
	         currentProjection == ORTHO_TRIMETRIC) {
	    glLineWidth(0.1f);
	    glBegin(GL_LINES);
	    
	    // Gambar grid dasar (bidang XZ)
	    glColor3f(0.7f, 0.7f, 0.7f);
	    for (float i = -gridSize; i <= gridSize; i += gridStep) {
	        // Garis sepanjang sumbu x
	        glVertex3f(-gridSize, -0.49f, i);
	        glVertex3f(gridSize, -0.49f, i);
	        
	        // Garis sepanjang sumbu z
	        glVertex3f(i, -0.49f, -gridSize);
	        glVertex3f(i, -0.49f, gridSize);
	    }
	    
	    // Gambar sumbu koordinat dengan warna yang terang
	    glLineWidth(3.0f);
	    glEnd();
	    
	    glLineWidth(5.0f);
	    glBegin(GL_LINES);
	    
	    // Sumbu X (merah)
	    glColor3f(1.0f, 0.0f, 0.0f);
	    glVertex3f(0.0f, 0.0f, 0.0f);
	    glVertex3f(gridSize, 0.0f, 0.0f);
	    
	    // Sumbu Y (hijau)
	    glColor3f(0.0f, 1.0f, 0.0f);
	    glVertex3f(0.0f, 0.0f, 0.0f);
	    glVertex3f(0.0f, gridSize, 0.0f);
	    
	    // Sumbu Z (biru)
	    glColor3f(0.0f, 0.0f, 1.0f);
	    glVertex3f(0.0f, 0.0f, 0.0f);
	    glVertex3f(0.0f, 0.0f, gridSize);
	    
	    glEnd();
	    glLineWidth(1.0f);
	}
        
        // Tambahkan informasi sudut pada mode isometric, dimetric, trimetric
        if (currentProjection == ORTHO_ISOMETRIC) {
            drawText("Sudut 120° antar sumbu", WINDOW_WIDTH - 200, 80, 1.0, 1.0, 1.0);
        }
        else if (currentProjection == ORTHO_DIMETRIC) {
            drawText("Sudut 130° pada sumbu X dan Z", WINDOW_WIDTH - 250, 80, 1.0, 1.0, 1.0);
        }
        else if (currentProjection == ORTHO_TRIMETRIC) {
            drawText("Sudut 125° X, 135° Y, 100° Z", WINDOW_WIDTH - 250, 80, 1.0, 1.0, 1.0);
        }
    }
    
    glEnable(GL_LIGHTING);
}

void displayKeyInfo() {
    // Tampilkan informasi tombol pada bagian bawah layar
    int yPos = 30; // Posisi Y awal (dari bawah)
    int lineHeight = 20; // Tinggi baris
    
    drawText("9: Ortografi - Trimetric", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
    
    drawText("8: Ortografi - Dimetric", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
    
    drawText("7: Ortografi - Isometric", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;

    drawText("6: Ortografi - Side view (tampak samping)", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
	
    drawText("5: Ortografi - Front view (tampak depan)", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;

    drawText("4: Ortografi - Plan view (tampak atas)", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
    
    drawText("3: Perspektif 3 titik hilang", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;

    drawText("2: Perspektif 2 titik hilang", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
    
    drawText("1: Perspektif 1 titik hilang", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
    
    drawText("0: Mode kamera bebas", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
    
    drawText("Kontrol:", 10, yPos, 1.0f, 1.0f, 1.0f);
    yPos += lineHeight;
    
    // Jika dalam mode free camera, tampilkan kontrol tambahan
    if (currentProjection == FREE_CAMERA) {
        drawText("--- Mode Free Camera ---", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT - 50, 1.0f, 1.0f, 0.0f);
        drawText("Tombol panah: Gerak kamera secara horizontal", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT - 70, 1.0f, 1.0f, 0.0f);
        drawText("W/S: Gerak kamera secara vertikal", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT - 90, 1.0f, 1.0f, 0.0f);
    }
}

void drawAxonometricAngles() {
    if (currentProjection == ORTHO_ISOMETRIC || 
        currentProjection == ORTHO_DIMETRIC || 
        currentProjection == ORTHO_TRIMETRIC) {
        
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        
        // Tampilkan informasi sudut di sisi kanan
        int xPos = WINDOW_WIDTH - 220;
        int yPos = WINDOW_HEIGHT / 2;
        
        // Kotak informasi (background)
        glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(xPos - 10, yPos - 30);
        glVertex2f(xPos + 230, yPos - 30);
        glVertex2f(xPos + 230, yPos + 200);
        glVertex2f(xPos - 10, yPos + 200);
        glEnd();
        
        // Judul panel informasi
        drawText("INFORMASI SUDUT PROYEKSI", xPos, yPos + 170, 1.0f, 1.0f, 1.0f);
        drawText("----------------------", xPos, yPos + 150, 1.0f, 1.0f, 1.0f);
        
        // Label sumbu
        drawText("Sumbu X:", xPos, yPos + 100, 1.0f, 0.0f, 0.0f);
        drawText("Sumbu Y:", xPos, yPos + 70, 0.0f, 1.0f, 0.0f);
        drawText("Sumbu Z:", xPos, yPos + 40, 0.0f, 0.0f, 1.0f);
        
        // Nilai sudut berdasarkan mode
        if (currentProjection == ORTHO_ISOMETRIC) {
            drawText("PROYEKSI ISOMETRIK", xPos, yPos + 130, 1.0f, 1.0f, 0.0f);
            drawText("Sudut X-Y: 120°", xPos + 70, yPos + 100, 1.0f, 1.0f, 1.0f);
            drawText("Sudut Y-Z: 120°", xPos + 70, yPos + 70, 1.0f, 1.0f, 1.0f);
            drawText("Sudut X-Z: 120°", xPos + 70, yPos + 40, 1.0f, 1.0f, 1.0f);
            drawText("Semua sudut antara sumbu sama (120°)", xPos, yPos + 10, 1.0f, 1.0f, 1.0f);
        } 
        else if (currentProjection == ORTHO_DIMETRIC) {
            drawText("PROYEKSI DIMETRIK", xPos, yPos + 130, 1.0f, 1.0f, 0.0f);
            drawText("Sudut X-Y: 130°", xPos + 70, yPos + 100, 1.0f, 1.0f, 1.0f);
            drawText("Sudut Y-Z: 130°", xPos + 70, yPos + 70, 1.0f, 1.0f, 1.0f);
            drawText("Sudut X-Z: 100°", xPos + 70, yPos + 40, 1.0f, 1.0f, 1.0f);
            drawText("Dua sudut sama (130°), satu berbeda", xPos, yPos + 10, 1.0f, 1.0f, 1.0f);
        }
        else if (currentProjection == ORTHO_TRIMETRIC) {
            drawText("PROYEKSI TRIMETRIK", xPos, yPos + 130, 1.0f, 1.0f, 0.0f);
            drawText("Sudut X-Y: 125°", xPos + 70, yPos + 100, 1.0f, 1.0f, 1.0f);
            drawText("Sudut Y-Z: 135°", xPos + 70, yPos + 70, 1.0f, 1.0f, 1.0f);
            drawText("Sudut X-Z: 100°", xPos + 70, yPos + 40, 1.0f, 1.0f, 1.0f);
            drawText("Semua sudut berbeda", xPos, yPos + 10, 1.0f, 1.0f, 1.0f);
        }
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }
}

// Function to draw a 3D box
void drawBox(float width, float height, float depth) {
    float w = width / 2.0f;
    float h = height / 2.0f;
    float d = depth / 2.0f;
    
    glBegin(GL_QUADS);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-w, -h, d);
    glVertex3f(w, -h, d);
    glVertex3f(w, h, d);
    glVertex3f(-w, h, d);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, -h, -d);
    
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-w, h, -d);
    glVertex3f(-w, h, d);
    glVertex3f(w, h, d);
    glVertex3f(w, h, -d);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(w, -h, -d);
    glVertex3f(w, -h, d);
    glVertex3f(-w, -h, d);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(w, -h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, h, d);
    glVertex3f(w, -h, d);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d);
    glVertex3f(-w, h, -d);
    
    glEnd();
}

// Function to draw a custom trapezoid shape (for car roof)
void drawCustomTrapezoid(
    float x1, float y1, float z1, // Front bottom left
    float x2, float y2, float z2, // Front bottom right
    float x3, float y3, float z3, // Front top right
    float x4, float y4, float z4, // Front top left
    float x5, float y5, float z5, // Back bottom left
    float x6, float y6, float z6, // Back bottom right
    float x7, float y7, float z7, // Back top right
    float x8, float y8, float z8  // Back top left
) {
    glBegin(GL_QUADS);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x5, y5, z5);
    glVertex3f(x8, y8, z8);
    glVertex3f(x7, y7, z7);
    glVertex3f(x6, y6, z6);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x2, y2, z2);
    glVertex3f(x6, y6, z6);
    glVertex3f(x7, y7, z7);
    glVertex3f(x3, y3, z3);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x1, y1, z1);
    glVertex3f(x4, y4, z4);
    glVertex3f(x8, y8, z8);
    glVertex3f(x5, y5, z5);
    
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x4, y4, z4);
    glVertex3f(x3, y3, z3);
    glVertex3f(x7, y7, z7);
    glVertex3f(x8, y8, z8);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x1, y1, z1);
    glVertex3f(x5, y5, z5);
    glVertex3f(x6, y6, z6);
    glVertex3f(x2, y2, z2);
    
    glEnd();
}

// Function to draw a cylinder (for wheels)
void drawCylinder(float radius, float height, int segments) {
    float x, y, z;
    float angle;
    
    // Top circle
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < segments; i++) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height / 2);
    }
    glEnd();
    
    // Bottom circle
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    for (int i = segments - 1; i >= 0; i--) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, -height / 2);
    }
    glEnd();
    
    // Cylinder side
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        angle = 2.0f * 3.14159f * i / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        
        glNormal3f(cos(angle), sin(angle), 0.0f);
        glVertex3f(x, y, height / 2);
        glVertex3f(x, y, -height / 2);
    }
    glEnd();
}

void drawDetailedWheel(float radius, float thickness, int segments = 36) {
    // Roda hitam luar
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCylinder(radius, thickness, segments);

    // Lubang tengah 1
    glColor4f(0.4f, 0.4f, 0.4f, 1.0);  
    glPushMatrix();
    drawCylinder(radius * 0.6f, thickness * 1.2f, segments);
    glPopMatrix();
	
    
    // Lubang hitam 
	for (int i = 0; i < 8; i++) {
	    float angle = i * 45.0f; // 8 holes, so 360/8 = 45 degrees apart
	    
	    glPushMatrix();
	    glRotatef(angle, 0.0f, 0.0f, 1.0f); // Rotate around z-axis
	    glColor3f(0.0f, 0.0f, 0.0f);  // Black
	    drawBox(0.015f, 0.13f, 0.115f); // Thin box to create the hole
	    glPopMatrix();
	}
	
	// Lubang tengah 2
	glColor4f(0.4f, 0.4f, 0.4f, 1.0);  
    glPushMatrix();
    drawCylinder(radius * 0.25f, thickness * 1.6f, segments);
    glPopMatrix();
}




// Function to draw the entire 3D car
void drawCar3D() {
    glPushMatrix();
    
    // Main car body
    glColor3f(1.0f, 1.0f, 1.0f);  // White color for car body
    drawBox(1.4f, 0.3f, 0.6f);  // Width, height, depth
    
    // Car roof (trapezoid)
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);  // White
    drawCustomTrapezoid(
        -0.7f, 0.0f, 0.3f,   // Front bottom left
        0.4f, 0.0f, 0.3f,    // Front bottom right
        0.3f, 0.3f, 0.3f,    // Front top right
        -0.7f, 0.3f,.3f,     // Front top left
        -0.7f, 0.0f, -0.3f,  // Back bottom left
        0.4f, 0.0f, -0.3f,   // Back bottom right
        0.3f, 0.3f, -0.3f,   // Back top right
        -0.7f, 0.3f, -0.3f   // Back top left
    );
    glPopMatrix();
    
    // Window frames (black outline)
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawCustomTrapezoid(
        -0.7f, 0.025f, 0.31f,   
        0.35f, 0.025f, 0.31f,    
        0.275f, 0.275f, 0.31f,    
        -0.7f, 0.275f, 0.31f,     
        -0.7f, 0.025f, -0.31f,  
        0.35f, 0.025f, -0.31f,   
        0.275f, 0.275f, -0.31f,   
        -0.7f, 0.275f, -0.31f   
    );
    glPopMatrix();
    
    // Left window (tinted)
    glPushMatrix();
    glTranslatef(-0.6f, 0.27f, 0.31f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.7f);  // Gray, semi-transparent
    drawBox(0.2f, 0.2f, 0.01f);
    glPopMatrix();
    
    // Left window (other side)
    glPushMatrix();
    glTranslatef(-0.6f, 0.27f, -0.31f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.7f);
    drawBox(0.2f, 0.2f, 0.01f);
    glPopMatrix();
    
    // Middle window (tinted)
    glPushMatrix();
    glTranslatef(-0.25f,  0.27f, 0.31f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.7f);
    drawBox(0.3f, 0.2f, 0.01f);
    glPopMatrix();
    
    // Middle window (other side)
    glPushMatrix();
    glTranslatef(-0.25f,  0.27f, -0.31f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.7f);
    drawBox(0.3f, 0.2f, 0.01f);
    glPopMatrix();

	// Jendela depan kanan (trapesium)
	glPushMatrix();
	glColor4f(0.5f, 0.5f, 0.5f, 0.7f); 
	drawCustomTrapezoid(
	    0.0f, 0.17f, 0.311f,   // Kiri bawah
	    0.275f, 0.17f, 0.311f,   // Kanan bawah
	    0.2f, 0.37f, 0.311f,   // Kanan atas
	    0.0f, 0.37f, 0.311f,   // Kiri atas
	    0.0f, 0.17f, 0.309f,   // Kiri bawah (belakang)
	    0.275f, 0.17f, 0.309f,   // Kanan bawah (belakang)
	    0.2f, 0.37f, 0.309f,   // Kanan atas (belakang)
	    0.0f, 0.37f, 0.309f    // Kiri atas (belakang)
	);
	glPopMatrix();
	
	// Jendela depan kiri (trapesium)
	glPushMatrix();
	glColor4f(0.5f, 0.5f, 0.5f, 0.7f);  
	drawCustomTrapezoid(
	    0.0f, 0.17f, -0.309f,  // Kiri bawah
	    0.275f, 0.17f, -0.309f,  // Kanan bawah
	    0.2f, 0.37f, -0.309f,  // Kanan atas
	    0.0f, 0.37f, -0.309f,  // Kiri atas
	    0.0f, 0.17f, -0.311f,  // Kiri bawah (belakang)
	    0.275f, 0.17f, -0.311f,  // Kanan bawah (belakang)
	    0.2f, 0.37f, -0.311f,  // Kanan atas (belakang)
	    0.0f, 0.37f, -0.311f   // Kiri atas (belakang)
	);
	glPopMatrix();
	
	// Panel belakang (putih/triplek)
	glPushMatrix();
	glTranslatef(-0.70f, 0.15f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);  // Putih
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	drawBox(0.6f, 0.6f, 0.01f);  
	glPopMatrix();
    
    // Door handles (left)
    glPushMatrix();
    glTranslatef(-0.35f, 0.05f, 0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.1f, 0.02f, 0.01f);
    glPopMatrix();
    
    // Door handles (right)
    glPushMatrix();
    glTranslatef(0.05f, 0.05f, 0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.1f, 0.02f, 0.01f);
    glPopMatrix();
    
    // Door handles (left - other side)
    glPushMatrix();
    glTranslatef(-0.35f, 0.05f, -0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.1f, 0.02f, 0.01f);
    glPopMatrix();
    
    // Door handles (right - other side)
    glPushMatrix();
    glTranslatef(0.05f, 0.05f, -0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.1f, 0.02f, 0.01f);
    glPopMatrix();
    
    // Black outline at bottom
    glPushMatrix();
    glTranslatef(0.0f, -0.18f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(1.4f, 0.05f, 0.61f);
    glPopMatrix();
    
    // Wheel wells (left front)
    glPushMatrix();
    glTranslatef(0.45f, -0.15f, 0.31f);
    glColor3f(0.2f, 0.2f, 0.2f);  // Dark gray
    drawCustomTrapezoid(
        -0.17f, -0.05f, 0.01f,   
        0.17f, -0.05f, 0.01f,    
        0.11f, 0.05f, 0.01f,    
        -0.11f, 0.05f, 0.01f,     
        -0.17f, -0.05f, -0.02f,  
        0.17f, -0.05f, -0.02f,   
        0.11f, 0.05f, -0.02f,   
        -0.11f, 0.05f, -0.02f   
    );
    glPopMatrix();
    
    // Wheel wells (right front)
    glPushMatrix();
    glTranslatef(0.45f, -0.15f, -0.31f);
    glColor3f(0.2f, 0.2f, 0.2f);  // Dark gray
    drawCustomTrapezoid(
        -0.17f, -0.05f, 0.02f,   
        0.17f, -0.05f, 0.02f,    
        0.11f, 0.05f, 0.02f,    
        -0.11f, 0.05f, 0.02f,     
        -0.17f, -0.05f, -0.01f,  
        0.17f, -0.05f, -0.01f,   
        0.11f, 0.05f, -0.01f,   
        -0.11f, 0.05f, -0.01f   
    );
    glPopMatrix();
    
    // Wheel wells (left rear)
    glPushMatrix();
    glTranslatef(-0.5f, -0.15f, 0.31f);
    glColor3f(0.2f, 0.2f, 0.2f);  // Dark gray
    drawCustomTrapezoid(
        -0.17f, -0.05f, 0.01f,   
        0.17f, -0.05f, 0.01f,    
        0.11f, 0.05f, 0.01f,    
        -0.11f, 0.05f, 0.01f,     
        -0.17f, -0.05f, -0.02f,  
        0.17f, -0.05f, -0.02f,   
        0.11f, 0.05f, -0.02f,   
        -0.11f, 0.05f, -0.02f   
    );
    glPopMatrix();
    
    // Wheel wells (right rear)
    glPushMatrix();
    glTranslatef(-0.5f, -0.15f, -0.31f);
    glColor3f(0.2f, 0.2f, 0.2f);  // Dark gray
    drawCustomTrapezoid(
        -0.17f, -0.05f, 0.02f,   
        0.17f, -0.05f, 0.02f,    
        0.11f, 0.05f, 0.02f,    
        -0.11f, 0.05f, 0.02f,     
        -0.17f, -0.05f, -0.01f,  
        0.17f, -0.05f, -0.01f,   
        0.11f, 0.05f, -0.01f,   
        -0.11f, 0.05f, -0.01f   
    );
    glPopMatrix();
    
    // Door lines (vertical)
    glPushMatrix();
    glTranslatef(-0.45f, 0.0f, 0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.02f, 0.4f, 0.01f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.45f, 0.0f, -0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.02f, 0.4f, 0.01f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.05f, 0.0f, 0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.02f, 0.4f, 0.01f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.05f, 0.0f, -0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.02f, 0.4f, 0.01f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.3315f, 0.0f, 0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.02f, 0.4f, 0.01f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.3315f, 0.0f, -0.31f);
    glColor3f(0.0f, 0.0f, 0.0f);  // Black
    drawBox(0.02f, 0.4f, 0.01f);
    glPopMatrix();
    
    // Rear lights (red)
    glPushMatrix();
    glTranslatef(-0.67f, 0.025f, 0.28f);
    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    drawBox(0.1f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.67f, 0.025f, -0.28f);
    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    drawBox(0.1f, 0.05f, 0.05f);
    glPopMatrix();
    
    // Rear yellow lights
    glPushMatrix();
    glTranslatef(-0.67f, -0.025f, 0.28f);
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
    drawBox(0.1f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.67f, -0.025f, -0.28f);
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
    drawBox(0.1f, 0.05f, 0.05f);
    glPopMatrix();
    
    // Front lights (yellow)
    glPushMatrix();
    glTranslatef(0.655f, -0.025f, 0.28f);
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
    drawBox(0.1f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.655f, -0.025f, -0.28f);
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
    drawBox(0.1f, 0.05f, 0.05f);
    glPopMatrix();
    
    // Wheels
	// Left front wheel
	glPushMatrix();
	glTranslatef(0.45f, -0.25f, 0.35f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Rotate wheel to face out
	drawDetailedWheel(0.14f, 0.08f);
	glPopMatrix();
    
    // Right front wheel
    glPushMatrix();
    glTranslatef(0.45f, -0.25f, -0.35f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Rotate wheel to face out
    drawDetailedWheel(0.14f, 0.08f);
    glPopMatrix();
    
    // Left rear wheel
    glPushMatrix();
    glTranslatef(-0.5f, -0.25f, 0.35f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Rotate wheel to face out
    drawDetailedWheel(0.14f, 0.08f);
    glPopMatrix();
    
    // Right rear wheel
    glPushMatrix();
    glTranslatef(-0.5f, -0.25f, -0.35f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Rotate wheel to face out
    drawDetailedWheel(0.14f, 0.08f);
    glPopMatrix();
    
    // Back window (rear)
    glPushMatrix();
    glTranslatef(-0.7f, 0.25f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 0.7f);  // Gray, semi-transparent
    drawBox(0.6f, 0.2f, 0.01f);
    glPopMatrix();
    
    // Front window (windshield)
    glPushMatrix();
    glTranslatef(0.35f, 0.3f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(0.2f, 0.2f, 0.2f, 0.7f);  // Gray, semi-transparent
    drawBox(0.55f, 0.3f, 0.01f);
    glPopMatrix();
    
    // Frame jendela belakang (hitam)
	glPushMatrix();
	glTranslatef(-0.705f, 0.3f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);  // Hitam
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	drawBox(0.55f, 0.255f, 0.01f);  // Sedikit lebih kecil dari panel utama
	glPopMatrix();
	
	// Kaca jendela belakang (abu transparan)
	glPushMatrix();
	glTranslatef(-0.7075f, 0.275f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);  
	drawBox(0.45f, 0.2f, 0.01f);  // Lebih kecil dari frame
	glPopMatrix();
	
	// Front headlights (white with black outline)
	// Left headlight
	glPushMatrix();
	glTranslatef(0.655f, 0.025f, 0.28f);
	glColor3f(0.0f, 0.0f, 0.0f);  // Black outline
	drawBox(0.12f, 0.07f, 0.07f);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.655f, 0.025f, 0.28f);
	glColor3f(1.0f, 1.0f, 1.0f);  // White light
	drawBox(0.1f, 0.05f, 0.05f);
	glPopMatrix();
	
	// Right headlight
	glPushMatrix();
	glTranslatef(0.655f, 0.025f, -0.28f);
	glColor3f(0.0f, 0.0f, 0.0f);  // Black outline
	drawBox(0.12f, 0.07f, 0.07f);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.655f, 0.025f, -0.28f);
	glColor3f(1.0f, 1.0f, 1.0f);  // White light
	drawBox(0.1f, 0.05f, 0.05f);
	glPopMatrix();
	
	// AC grill (black)
	glPushMatrix();
	glTranslatef(0.655f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);  
	drawBox(0.1f, 0.1f, 0.4f);  // Width, height, depth
	
	// Add horizontal lines for the grill
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = -2; i <= 2; i++) {
	    glPushMatrix();
	    glColor3f(0.5f, 0.5f, 0.5f);  
	    glTranslatef(0.02f, 0.02f * i, 0.0f);
	    drawBox(0.07f, 0.01f, 0.28f);
	    glPopMatrix();
	}
	glPopMatrix();
	
	// Add spare tire at the back of the car
	glPushMatrix();
	glTranslatef(-0.71f, 0.0f, 0.0f); 
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
	drawDetailedWheel(0.14f, 0.08f); 
	
	glPopMatrix();
	
    glPopMatrix();  // End car
}

// Draw floor/ground
void drawGround() {
    glColor3f(0.3f, 0.5f, 0.3f);  // Green grass color
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, -0.5f, -10.0f);
    glVertex3f(-10.0f, -0.5f, 10.0f);
    glVertex3f(10.0f, -0.5f, 10.0f);
    glVertex3f(10.0f, -0.5f, -10.0f);
    glEnd();
}

// Function to rotate camera
void orientMe(float ang) {
    lx = sin(ang);
    lz = -cos(ang);
}

// Function to move camera
void moveMeFlat(int direction) {
    x += direction * (lx * 0.1f);
    z += direction * (lz * 0.1f);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    ratio = 1.0f * w / h;
    
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    switch (currentProjection) {
        case PERSPECTIVE_1POINT:
        case PERSPECTIVE_2POINT:
        case PERSPECTIVE_3POINT:
        case FREE_CAMERA:
            gluPerspective(45.0f, ratio, 0.1f, 100.0f);
            break;
        case ORTHO_PLAN:
        case ORTHO_FRONT:
        case ORTHO_SIDE:
        case ORTHO_ISOMETRIC:
        case ORTHO_DIMETRIC:
        case ORTHO_TRIMETRIC:
            // Untuk proyeksi ortografis, buat kotak pandang cukup besar
            float orthoSize = 3.0f;
            if (w <= h)
                glOrtho(-orthoSize, orthoSize, -orthoSize / ratio, orthoSize / ratio, 0.1, 100.0);
            else
                glOrtho(-orthoSize * ratio, orthoSize * ratio, -orthoSize, orthoSize, 0.1, 100.0);
            break;
    }
    
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Atur posisi kamera dan titik hilang berdasarkan mode proyeksi
    switch (currentProjection) {
        case FREE_CAMERA:
            // Gunakan kontrol kamera original
            gluLookAt(x, y, z, x + lx, y, z + lz, 0.0f, 1.0f, 0.0f);
            break;
            
        case PERSPECTIVE_1POINT:
            // 1-point perspective (tampak depan dengan satu titik hilang)
            gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            
            // Definisikan titik hilang
            vanishingPoints[0].x = 0.0f;
            vanishingPoints[0].y = 0.0f;
            vanishingPoints[0].z = -20.0f;
            vanishingPoints[0].active = 1;
            vanishingPoints[1].active = 0;
            vanishingPoints[2].active = 0;
            break;
            
        case PERSPECTIVE_2POINT:
            // 2-point perspective (tampak sudut dengan dua titik hilang)
            gluLookAt(3.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            
            // Definisikan titik hilang
            vanishingPoints[0].x = -20.0f;
            vanishingPoints[0].y = 0.0f;
            vanishingPoints[0].z = 0.0f;
            vanishingPoints[0].active = 1;
            
            vanishingPoints[1].x = 0.0f;
            vanishingPoints[1].y = 0.0f;
            vanishingPoints[1].z = -20.0f;
            vanishingPoints[1].active = 1;
            
            vanishingPoints[2].active = 0;
            break;
            
        case PERSPECTIVE_3POINT:
            // 3-point perspective (tampak dari bawah sudut dengan tiga titik hilang)
            gluLookAt(4.0f, 2.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            
            // Definisikan titik hilang
            vanishingPoints[0].x = -20.0f;
            vanishingPoints[0].y = 0.0f;
            vanishingPoints[0].z = 0.0f;
            vanishingPoints[0].active = 1;
            
            vanishingPoints[1].x = 0.0f;
            vanishingPoints[1].y = 0.0f;
            vanishingPoints[1].z = -20.0f;
            vanishingPoints[1].active = 1;
            
            vanishingPoints[2].x = 0.0f;
            vanishingPoints[2].y = 20.0f;
            vanishingPoints[2].z = 0.0f;
            vanishingPoints[2].active = 1;
            break;
            
		case ORTHO_PLAN:
		    // Tampak atas (plan view) dengan zoom
		    gluLookAt(0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
		    break;
		    
		case ORTHO_FRONT:
		    // Tampak depan (front view) dengan zoom
		    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		    break;
		    
		case ORTHO_SIDE:
		    // Tampak samping (side view) dengan zoom
		    gluLookAt(5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		    break;
            
        case ORTHO_ISOMETRIC:
            // Isometric (sudut sama)
            gluLookAt(3.464f, 3.464f, 3.464f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
            
        case ORTHO_DIMETRIC:
            // Dimetric (dua sudut sama)
            gluLookAt(3.464f, 2.0f, 3.464f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
            
        case ORTHO_TRIMETRIC:
            // Trimetric (tiga sudut berbeda)
            gluLookAt(4.0f, 2.5f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    // Gambar grid dan garis bantu
    drawProjectionGrid();
    
    // Gambar sumbu dan sudut untuk proyeksi axonometric
    drawAxonometricAngles();
    
    // Penting: Tetapkan posisi objek dengan benar
    glPushMatrix();
    glTranslatef(0.0f, -0.2f, 0.0f); // Turunkan sedikit objek
    
    // Terapkan zoom pada objek jika dalam mode ortogonal
    if (currentProjection == ORTHO_PLAN || 
        currentProjection == ORTHO_FRONT || 
        currentProjection == ORTHO_SIDE) {
        glScalef(orthoZoom, orthoZoom, orthoZoom);
    }
    
    // Gambar ground
    drawGround();
    
    // Gambar mobil
    drawCar3D();
    
    glPopMatrix();
    
    // Gambar titik hilang dan garis jika dalam mode perspektif
    drawVanishingPoints();
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Disable yang bisa mengganggu teks
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Render semua teks
    displayProjectionInfo();
    displayKeyInfo();
    
    // Kembalikan ke matrix state sebelumnya
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    // Kembali ke modelview untuk operasi berikutnya
    glMatrixMode(GL_MODELVIEW);
    
	// Tampilkan informasi sudut axonometric dalam mode overlay
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	// Tampilkan informasi zoom jika dalam mode ortogonal
	if (currentProjection == ORTHO_PLAN || currentProjection == ORTHO_FRONT || currentProjection == ORTHO_SIDE) {
	    drawText("--- Mode View Orthogonal ---", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT - 50, 1.0f, 1.0f, 0.0f);
	    drawText("Zoom: Gunakan panah ATAS(+)/BAWAH(-)", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT - 70, 1.0f, 1.0f, 0.0f);
	    
	    char zoomInfo[50];
	    if (orthoZoom > 1.0f) {
	        drawText("Mode: Objek Diperbesar", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT - 90, 1.0f, 1.0f, 0.0f);
	    } else if (orthoZoom < 1.0f) {
	        drawText("Mode: Objek Diperkecil", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT - 90, 1.0f, 1.0f, 0.0f);
	    } else {
	        drawText("Mode: Ukuran Normal", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT - 90, 1.0f, 1.0f, 0.0f);
	    }
	}
        
	
	// Tampilkan informasi sudut untuk proyeksi axonometric
	drawAxonometricAngles();
	
	// Kembalikan ke state semula
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
    
    glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {
    if (currentProjection == FREE_CAMERA) {
        switch (key) {
            case GLUT_KEY_LEFT:
                angle -= 0.05f;
                orientMe(angle);
                break;
            case GLUT_KEY_RIGHT:
                angle += 0.05f;
                orientMe(angle);
                break;
            case GLUT_KEY_UP:
                moveMeFlat(1);
                break;
            case GLUT_KEY_DOWN:
                moveMeFlat(-1);
                break;
        }
    } 
    else if (currentProjection == ORTHO_PLAN || 
             currentProjection == ORTHO_FRONT || 
             currentProjection == ORTHO_SIDE) {
        // Kontrol zoom untuk tampilan ortogonal
        switch (key) {
            case GLUT_KEY_UP:
                orthoZoom *= 1.1f; // Zoom in (objek lebih besar)
                if (orthoZoom > 5.0f) orthoZoom = 5.0f; // Batasi zoom
                break;
            case GLUT_KEY_DOWN:
                orthoZoom *= 0.9f; // Zoom out (objek lebih kecil)
                if (orthoZoom < 0.2f) orthoZoom = 0.2f; // Batasi zoom
                break;
        }
        glutPostRedisplay(); // Refresh tampilan
    }
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    switch (key) {
        case 'w':
        case 'W':
            y += 0.1f;  // Naikkan kamera
            break;
        case 's':
        case 'S':
            y -= 0.1f;  // Turunkan kamera
            break;
		// Tombol untuk mode kamera bebas
		case '0':
		    currentProjection = FREE_CAMERA;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;    
		// Tombol untuk proyeksi perspektif
		case '1':
		    currentProjection = PERSPECTIVE_1POINT;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		case '2':
		    currentProjection = PERSPECTIVE_2POINT;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		case '3':
		    currentProjection = PERSPECTIVE_3POINT;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		// Tombol untuk proyeksi ortografi
		case '4':
		    currentProjection = ORTHO_PLAN;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		case '5':
		    currentProjection = ORTHO_FRONT;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		case '6':
		    currentProjection = ORTHO_SIDE;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		case '7':
		    currentProjection = ORTHO_ISOMETRIC;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		case '8':
		    currentProjection = ORTHO_DIMETRIC;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
		case '9':
		    currentProjection = ORTHO_TRIMETRIC;
		    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		    break;
    }
    glutPostRedisplay();  // Memastikan layar diperbarui setelah perubahan
}

void init() {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);  // Sky blue background
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set light position and properties
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Enable blending for transparent windows
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable color material mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Proyeksi Obyek 3D");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);
    
    glutMainLoop();
    
    return 0;
}

