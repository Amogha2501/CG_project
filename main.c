#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define M_PI if it's not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- GLOBAL VARIABLES ---
float globalTime = 0.0f;
int currentScene = 1;
const float SCENE_DURATION = 60.0f; // Exactly 60 seconds per scene

// Camera variables
float camX = 0.0f, camY = 2.0f, camZ = 10.0f;
float lookX = 0.0f, lookY = 2.0f, lookZ = 0.0f;

// --- UTILITY & LIGHTING FUNCTIONS ---

void setLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat light_pos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}

// --- SUBTITLE SYSTEM ---

void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawSubtitles(const char* text) {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(width, 0);
        glVertex2f(width, 80);
        glVertex2f(0, 80);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(1.0f, 1.0f, 1.0f); 
    renderBitmapString(width / 2 - (strlen(text) * 4), 35, GLUT_BITMAP_HELVETICA_18, text);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// --- DRAWING HELPERS ---

void drawCharacter(float walkSpeed, float r, float g, float b, bool isMoving) {
    glColor3f(r, g, b);
    
    float armAngle = isMoving ? sin(globalTime * walkSpeed) * 30.0f : 0.0f;
    float legAngle = isMoving ? sin(globalTime * walkSpeed) * 30.0f : 0.0f;

    glPushMatrix();
        // Head
        glPushMatrix(); glTranslatef(0.0f, 1.5f, 0.0f); glutSolidSphere(0.3, 20, 20); glPopMatrix();
        // Torso
        glPushMatrix(); glTranslatef(0.0f, 0.7f, 0.0f); glScalef(0.6f, 1.0f, 0.4f); glutSolidCube(1.0); glPopMatrix();
        // Left Arm
        glPushMatrix(); glTranslatef(-0.4f, 1.1f, 0.0f); glRotatef(armAngle, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.2f, 0.8f, 0.2f); glutSolidCube(1.0); glPopMatrix();
        // Right Arm
        glPushMatrix(); glTranslatef(0.4f, 1.1f, 0.0f); glRotatef(-armAngle, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.2f, 0.8f, 0.2f); glutSolidCube(1.0); glPopMatrix();
        // Left Leg
        glPushMatrix(); glTranslatef(-0.2f, 0.4f, 0.0f); glRotatef(-legAngle, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.25f, 0.8f, 0.25f); glutSolidCube(1.0); glPopMatrix();
        // Right Leg
        glPushMatrix(); glTranslatef(0.2f, 0.4f, 0.0f); glRotatef(legAngle, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.25f, 0.8f, 0.25f); glutSolidCube(1.0); glPopMatrix();
    glPopMatrix();
}

void drawGalleryRoom() {
    glDisable(GL_LIGHTING); 
    
    // Base Floor Grid
    glColor3f(0.2f, 0.2f, 0.2f); 
    glBegin(GL_LINES);
    for(int i = -10; i <= 10; i++) {
        glVertex3f(i, 0, -10); glVertex3f(i, 0, 10);
        glVertex3f(-10, 0, i); glVertex3f(10, 0, i);
    }
    glEnd();

    // Red Carpet
    glColor3f(0.6f, 0.1f, 0.1f); 
    glBegin(GL_QUADS);
        glVertex3f(-2.5f, 0.01f, -10.0f); 
        glVertex3f( 2.5f, 0.01f, -10.0f); 
        glVertex3f( 2.5f, 0.01f,  10.0f); 
        glVertex3f(-2.5f, 0.01f,  10.0f); 
    glEnd();

    // Back Wall
    glColor3f(0.8f, 0.8f, 0.7f); 
    glBegin(GL_QUADS);
        glVertex3f(-10.0f, 0.0f, -5.0f);
        glVertex3f(10.0f, 0.0f, -5.0f);
        glVertex3f(10.0f, 5.0f, -5.0f);
        glVertex3f(-10.0f, 5.0f, -5.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// --- NEW: STARFIELD HELPER ---
void drawStarfield() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    srand(8675309); // Fixed seed so stars don't flicker
    for(int i = 0; i < 600; i++) {
        float sx = (rand() % 400 - 200) / 2.0f;
        float sy = (rand() % 400 - 200) / 2.0f;
        float sz = (rand() % 400 - 200) / 2.0f;
        glVertex3f(sx, sy, sz);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// --- UPGRADED SOLAR SYSTEM HELPER ---
// --- UPGRADED SOLAR SYSTEM HELPER ---
void drawSolarSystem(float sceneTime, float destabFactor) {
    glDisable(GL_LIGHTING); 
    
    // Core of the Sun
    glPushMatrix();
        glColor3f(1.0f, 0.9f, 0.0f); 
        glutSolidSphere(2.0, 30, 30);
    glPopMatrix();

    // Sun's Aura/Glow
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
    glDepthMask(GL_FALSE); 
    
    for(int glow = 1; glow <= 4; glow++) {
        glPushMatrix();
            glColor4f(1.0f, 0.6f, 0.0f, 0.25f / glow); 
            glutSolidSphere(2.0 + (glow * 0.4f), 20, 20);
        glPopMatrix();
    }
    
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING); 

    // The 9 Planets
    // FIX 1: Boosted the base speeds of the outer planets so they clearly revolve!
    float planets[9][6] = {
        { 3.5f,  0.15f, 0.6f, 0.6f, 0.6f, 4.0f },  // Mercury 
        { 5.0f,  0.25f, 0.9f, 0.6f, 0.2f, 3.0f },  // Venus 
        { 7.0f,  0.3f,  0.1f, 0.4f, 1.0f, 2.0f },  // Earth 
        { 9.0f,  0.2f,  0.8f, 0.2f, 0.1f, 1.5f },  // Mars 
        { 13.0f, 1.0f,  0.8f, 0.5f, 0.3f, 0.8f },  // Jupiter 
        { 17.0f, 0.8f,  0.9f, 0.8f, 0.5f, 0.6f },  // Saturn
        { 21.0f, 0.5f,  0.4f, 0.8f, 0.9f, 0.4f },  // Uranus 
        { 25.0f, 0.48f, 0.1f, 0.2f, 0.8f, 0.3f },  // Neptune 
        { 29.0f, 0.1f,  0.7f, 0.6f, 0.5f, 0.2f }   // Pluto 
    };

    for (int i = 0; i < 9; i++) {
        glPushMatrix();
            float speedMult = 1.0f + (destabFactor * 15.0f);
            
            // FIX 2: Multiplied the master speed by 40.0 so they all move fast enough to see
            glRotatef(sceneTime * 40.0f * planets[i][5] * speedMult, 0.0f, 1.0f, 0.0f);
            glTranslatef(planets[i][0], 0.0f, 0.0f);
            glRotatef(sceneTime * 50.0f, 0.0f, 1.0f, 0.0f);
            
            if (destabFactor > 0.0f) {
                float stretch = 1.0f + fabs(sin(sceneTime * 10.0f + i)) * destabFactor * 3.0f;
                glScalef(1.0f, stretch, 1.0f);
            }

            glColor3f(planets[i][2], planets[i][3], planets[i][4]);
            glutSolidSphere(planets[i][1], 20, 20);

            // Highlight Earth (Index 2)
            if (i == 2) {
                glDisable(GL_LIGHTING); 
                glColor3f(0.0f, 1.0f, 1.0f); 
                glPushMatrix();
                    glRotatef(sceneTime * 100.0f, 1.0f, 1.0f, 0.0f);
                    glutWireSphere(planets[i][1] + 0.15f, 12, 12);
                glPopMatrix();
                glEnable(GL_LIGHTING);
            }

            // Saturn's Rings (Index 5)
            if (i == 5) {
                glColor3f(0.7f, 0.6f, 0.4f); 
                glPushMatrix();
                    glRotatef(70.0f, 1.0f, 0.0f, 0.0f); 
                    glutSolidTorus(0.1, planets[i][1] + 0.4f, 10, 40);
                glPopMatrix();
            }
        glPopMatrix();
    }
}

// --- THE NEW "LIVING" PAINTING (NO TEXTURE NEEDED) ---
void drawPainting(float pulseIntensity) {
    glDisable(GL_LIGHTING);
    
    // Frame
    glColor3f(0.3f, 0.15f, 0.05f);
    glPushMatrix();
        glTranslatef(0.0f, 2.5f, -4.9f);
        glScalef(3.2f, 2.2f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    // Canvas Background (Deep Space Black)
    // FIX 1: We scale down the pulse so the red and blue never max out to Neon Pink!
    float safeGlow = pulseIntensity * 0.15f; 
    glColor3f(0.05f + safeGlow, 0.0f, 0.1f + (safeGlow * 1.5f)); 
    
    glPushMatrix();
        glTranslatef(0.0f, 2.5f, -4.84f);
        glBegin(GL_QUADS);
            glVertex3f(-1.5f, -1.0f, 0.0f);
            glVertex3f( 1.5f, -1.0f, 0.0f);
            glVertex3f( 1.5f,  1.0f, 0.0f);
            glVertex3f(-1.5f,  1.0f, 0.0f);
        glEnd();
    glPopMatrix();
    
    glEnable(GL_LIGHTING);

    // The Painted Solar System
    glPushMatrix();
        glTranslatef(0.0f, 2.5f, -4.78f); 
        glScalef(0.045f, 0.045f, 0.01f); 
        drawSolarSystem(0.0f, pulseIntensity); 
    glPopMatrix();
}

// --- GALLERY DECORATIONS ---

void drawSecondPainting() {
    glPushMatrix();
        glTranslatef(-4.5f, 2.5f, -4.9f); 
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix(); glScalef(2.0f, 2.0f, 0.1f); glutSolidCube(1.0); glPopMatrix();

        glColor3f(0.95f, 0.95f, 0.90f);
        glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.06f); glScalef(1.8f, 1.8f, 0.02f); glutSolidCube(1.0); glPopMatrix();

        glTranslatef(0.0f, 0.0f, 0.08f); 
        glDisable(GL_LIGHTING); 
        glColor3f(0.8f, 0.1f, 0.1f); glBegin(GL_QUADS); glVertex3f(-0.6f, -0.6f, 0.0f); glVertex3f(0.1f, -0.6f, 0.0f); glVertex3f(0.1f, 0.5f, 0.0f); glVertex3f(-0.6f, 0.5f, 0.0f); glEnd();
        glColor3f(0.1f, 0.3f, 0.8f); glBegin(GL_QUADS); glVertex3f(-0.1f, -0.8f, 0.01f); glVertex3f(0.6f, -0.8f, 0.01f); glVertex3f(0.6f, 0.2f, 0.01f); glVertex3f(-0.1f, 0.2f, 0.01f); glEnd();
        glColor3f(0.9f, 0.8f, 0.1f); glBegin(GL_QUADS); glVertex3f(-0.8f, 0.3f, 0.02f); glVertex3f(0.8f, 0.3f, 0.02f); glVertex3f(0.8f, 0.45f, 0.02f); glVertex3f(-0.8f, 0.45f, 0.02f); glEnd();
        glEnable(GL_LIGHTING); 
    glPopMatrix();
}

void drawPainter(float sceneTime) {
    glPushMatrix();
        glTranslatef(-9.5f, 0.0f, -4.0f); 
        glColor3f(0.4f, 0.2f, 0.1f);
        glPushMatrix(); glTranslatef(0.0f, 1.0f, -0.6f); glScalef(0.1f, 2.0f, 0.1f); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, 1.0f, 0.6f); glScalef(0.1f, 2.0f, 0.1f); glutSolidCube(1.0); glPopMatrix();
        glColor3f(0.9f, 0.9f, 0.9f);
        glPushMatrix(); glTranslatef(0.1f, 2.0f, 0.0f); glScalef(0.1f, 1.5f, 1.4f); glutSolidCube(1.0); glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-8.0f, 0.0f, -4.0f); 
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); 
        glColor3f(0.1f, 0.3f, 0.5f); 
        glPushMatrix(); glTranslatef(0.0f, 1.5f, 0.0f); glutSolidSphere(0.3, 20, 20); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, 0.7f, 0.0f); glScalef(0.6f, 1.0f, 0.4f); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef(-0.2f, 0.4f, 0.0f); glScalef(0.25f, 0.8f, 0.25f); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef(0.2f, 0.4f, 0.0f); glScalef(0.25f, 0.8f, 0.25f); glutSolidCube(1.0); glPopMatrix();
        
        glPushMatrix(); 
            glTranslatef(-0.4f, 1.1f, 0.0f); glRotatef(-45.0f, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.2f, 0.8f, 0.2f); glutSolidCube(1.0); 
            glColor3f(0.6f, 0.4f, 0.2f); glTranslatef(0.0f, -0.5f, 0.2f); glScalef(3.0f, 0.1f, 2.0f); glutSolidCube(1.0);
        glPopMatrix();
        
        glColor3f(0.1f, 0.3f, 0.5f); 
        float strokePhase = sceneTime * 3.0f;
        float brushAngleX = sin(strokePhase) * 30.0f - 45.0f; 
        float brushAngleZ = cos(strokePhase * 0.5f) * 15.0f;  
        glPushMatrix(); 
            glTranslatef(0.4f, 1.1f, 0.0f); glRotatef(brushAngleX, 1.0f, 0.0f, 0.0f); glRotatef(brushAngleZ, 0.0f, 0.0f, 1.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.2f, 0.8f, 0.2f); glutSolidCube(1.0); 
            glColor3f(0.8f, 0.8f, 0.6f); glTranslatef(0.0f, -0.5f, 0.0f); glScalef(0.2f, 1.0f, 0.2f); glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();
}

void drawFrameHanger() {
    float t = globalTime; 
    float charX = 5.0f, charZ = -4.0f;
    float rotY = 0.0f;
    bool showChar = true, isMoving = false, holdingFrame = true;
    float armRaise = 0.0f;

    if (t < 5.0f) { charX = 12.0f - (t / 5.0f) * 7.0f; rotY = -90.0f; isMoving = true; } 
    else if (t < 8.0f) { charX = 5.0f; rotY = 180.0f; armRaise = -140.0f; } 
    else if (t < 13.0f) { charX = 5.0f + ((t - 8.0f) / 5.0f) * 7.0f; rotY = 90.0f; isMoving = true; holdingFrame = false; } 
    else { showChar = false; holdingFrame = false; }

    if (holdingFrame && showChar) {
        glPushMatrix(); glTranslatef(charX, 1.8f, charZ - 0.4f); glColor3f(0.5f, 0.3f, 0.1f); glScalef(1.0f, 1.2f, 0.05f); glutSolidCube(1.0); glPopMatrix();
    } else if (!holdingFrame) {
        glPushMatrix();
            glTranslatef(5.0f, 2.5f, -4.9f);
            glColor3f(0.5f, 0.3f, 0.1f); glPushMatrix(); glScalef(1.0f, 1.2f, 0.1f); glutSolidCube(1.0); glPopMatrix();
            glColor3f(0.9f, 0.9f, 0.9f); glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.06f); glScalef(0.8f, 1.0f, 0.02f); glutSolidCube(1.0); glPopMatrix();
            glTranslatef(0.0f, 0.0f, 0.08f); 
            glColor3f(1.0f, 0.8f, 0.0f); glPushMatrix(); glTranslatef(0.25f, 0.3f, 0.0f); glutSolidSphere(0.1, 10, 10); glPopMatrix();
            glColor3f(0.4f, 0.2f, 0.0f); glPushMatrix(); glTranslatef(-0.2f, -0.3f, 0.0f); glScalef(0.05f, 0.3f, 0.01f); glutSolidCube(1.0); glPopMatrix();
            glColor3f(0.1f, 0.6f, 0.2f); glPushMatrix(); glTranslatef(-0.2f, -0.1f, 0.0f); glutSolidSphere(0.15, 10, 10); glPopMatrix();
            glColor3f(0.0f, 0.4f, 0.8f); glPushMatrix(); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.7f, 0.05f, 0.01f); glutSolidCube(1.0); glPopMatrix();
        glPopMatrix();
    }

    if (showChar) {
        float walkAnim = isMoving ? sin(t * 10.0f) * 30.0f : 0.0f;
        glPushMatrix();
            glTranslatef(charX, 0.0f, charZ); glRotatef(rotY, 0.0f, 1.0f, 0.0f);
            glColor3f(0.2f, 0.6f, 0.2f); 
            glPushMatrix(); glTranslatef(0.0f, 1.5f, 0.0f); glutSolidSphere(0.3, 20, 20); glPopMatrix();
            glPushMatrix(); glTranslatef(0.0f, 0.7f, 0.0f); glScalef(0.6f, 1.0f, 0.4f); glutSolidCube(1.0); glPopMatrix();
            glPushMatrix(); glTranslatef(-0.4f, 1.1f, 0.0f); glRotatef(isMoving?walkAnim:armRaise, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.2f, 0.8f, 0.2f); glutSolidCube(1.0); glPopMatrix();
            glPushMatrix(); glTranslatef(0.4f, 1.1f, 0.0f); glRotatef(isMoving?-walkAnim:armRaise, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.2f, 0.8f, 0.2f); glutSolidCube(1.0); glPopMatrix();
            glColor3f(0.3f, 0.3f, 0.3f); 
            glPushMatrix(); glTranslatef(-0.2f, 0.4f, 0.0f); glRotatef(isMoving?-walkAnim:0.0f, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.25f, 0.8f, 0.25f); glutSolidCube(1.0); glPopMatrix();
            glPushMatrix(); glTranslatef(0.2f, 0.4f, 0.0f); glRotatef(isMoving?walkAnim:0.0f, 1.0f, 0.0f, 0.0f); glTranslatef(0.0f, -0.4f, 0.0f); glScalef(0.25f, 0.8f, 0.25f); glutSolidCube(1.0); glPopMatrix();
        glPopMatrix();
    }
}

void drawBookShowcase() {
    glPushMatrix();
        glTranslatef(-5.0f, 0.0f, 3.0f); 
        glColor3f(0.8f, 0.8f, 0.8f); glPushMatrix(); glTranslatef(0.0f, 0.6f, 0.0f); glScalef(1.5f, 1.2f, 1.0f); glutSolidCube(1.0); glPopMatrix();
        glColor3f(0.3f, 0.15f, 0.05f); glPushMatrix(); glTranslatef(0.0f, 1.25f, 0.0f); glScalef(1.8f, 0.1f, 1.2f); glutSolidCube(1.0); glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.4f, 1.35f, 0.1f); glRotatef(15.0f, 0.0f, 1.0f, 0.0f); 
            glColor3f(0.9f, 0.9f, 0.9f); glPushMatrix(); glScalef(0.5f, 0.08f, 0.7f); glutSolidCube(1.0); glPopMatrix();
            glColor3f(0.7f, 0.1f, 0.1f);
            glPushMatrix(); glTranslatef(0.0f, 0.05f, 0.0f); glScalef(0.52f, 0.02f, 0.72f); glutSolidCube(1.0); glPopMatrix(); 
            glPushMatrix(); glTranslatef(0.0f, -0.05f, 0.0f); glScalef(0.52f, 0.02f, 0.72f); glutSolidCube(1.0); glPopMatrix(); 
            glPushMatrix(); glTranslatef(-0.26f, 0.0f, 0.0f); glScalef(0.02f, 0.12f, 0.72f); glutSolidCube(1.0); glPopMatrix(); 
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.4f, 1.5f, -0.1f); glRotatef(-20.0f, 0.0f, 1.0f, 0.0f); glRotatef(45.0f, 1.0f, 0.0f, 0.0f); 
            glColor3f(0.1f, 0.3f, 0.6f); glPushMatrix(); glScalef(0.6f, 0.05f, 0.8f); glutSolidCube(1.0); glPopMatrix();
            glColor3f(1.0f, 1.0f, 1.0f); glPushMatrix(); glTranslatef(0.0f, 0.03f, 0.0f); glScalef(0.5f, 0.02f, 0.7f); glutSolidCube(1.0); glPopMatrix();
            glColor3f(0.2f, 0.2f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 0.045f, 0.1f); glScalef(0.3f, 0.01f, 0.4f); glutSolidCube(1.0); glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void drawFlowerPot() {
    glPushMatrix();
        glColor3f(0.6f, 0.3f, 0.1f);
        glPushMatrix(); glTranslatef(0.0f, 0.3f, 0.0f); glScalef(0.6f, 0.4f, 0.6f); glutSolidSphere(1.0, 16, 16); glPopMatrix();
        glColor3f(0.6f, 0.3f, 0.1f); glPushMatrix(); glTranslatef(0.0f, 0.7f, 0.0f); glRotatef(90.0f, 1.0f, 0.0f, 0.0f); glutSolidTorus(0.1, 0.6, 16, 16); glPopMatrix();
        glColor3f(0.2f, 0.1f, 0.0f); glPushMatrix(); glTranslatef(0.0f, 0.6f, 0.0f); glScalef(0.5f, 0.1f, 0.5f); glutSolidSphere(1.0, 8, 8); glPopMatrix();

        glColor3f(0.1f, 0.6f, 0.2f); 
        float plantWobble = sin(globalTime * 1.5f) * 5.0f;

        glPushMatrix();
            glTranslatef(0.0f, 0.7f, 0.0f); glRotatef(plantWobble, 1.0f, 0.0f, 0.1f); 
            for(int i=0; i<3; i++) { glPushMatrix(); glTranslatef(0.0f, i*0.25f, 0.0f); glScalef(0.08f, 0.3f, 0.08f); glutSolidCone(1.0, 1.0, 8, 8); glPopMatrix(); }
            for(int leaf=0; leaf<2; leaf++) { glPushMatrix(); glTranslatef(0.0f, leaf*0.4f + 0.2f, 0.0f); glRotatef(45.0f + leaf*15.0f, 0.0f, 1.0f, 0.0f); glRotatef(30.0f + leaf*10.0f, 1.0f, 0.0f, 0.0f); glScalef(0.12f, 0.25f, 0.06f); glutSolidCone(1.0, 1.0, 6, 6); glPopMatrix(); }
            glColor3f(1.0f, 0.2f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 3*0.25f, 0.0f); glutSolidSphere(0.15, 12, 12); glPopMatrix();
            glColor3f(1.0f, 0.9f, 0.2f); for(int k=0; k<3; k++) { glPushMatrix(); glTranslatef(sin(k*120.0f*M_PI/180.0f)*0.1f, 3*0.25f, cos(k*120.0f*M_PI/180.0f)*0.1f); glutSolidSphere(0.08, 8, 8); glPopMatrix(); }
        glPopMatrix();
        
        glColor3f(0.1f, 0.6f, 0.2f); 
        for(int branch=0; branch<2; branch++) {
            glPushMatrix();
                glTranslatef(0.0f, 0.8f, 0.0f); glRotatef(60.0f*(branch==0?-1:1) + plantWobble*2.0f, 0.0f, 1.0f, 0.0f); glRotatef(30.0f, 1.0f, 0.0f, 0.0f); 
                for(int i=0; i<2; i++) { glPushMatrix(); glTranslatef(0.0f, i*0.2f, 0.0f); glScalef(0.06f, 0.25f, 0.06f); glutSolidCone(1.0, 1.0, 6, 6); glPopMatrix(); }
                for(int leaf=0; leaf<3; leaf++) { glPushMatrix(); glTranslatef(0.0f, leaf*0.15f+0.1f, 0.0f); glRotatef(45.0f + leaf*15.0f, 0.0f, 1.0f, 0.0f); glRotatef(30.0f + leaf*10.0f, 1.0f, 0.0f, 0.0f); glScalef(0.1f, 0.2f, 0.05f); glutSolidCone(1.0, 1.0, 6, 6); glPopMatrix(); }
                glColor3f((branch==0?1.0f:0.5f), (branch==0?0.9f:0.1f), (branch==0?0.2f:1.0f)); 
                glPushMatrix(); glTranslatef(0.0f, 2*0.2f+0.1f, 0.0f); glutSolidSphere(0.1, 10, 10); glPopMatrix();
                 glColor3f(0.1f, 0.6f, 0.2f); 
            glPopMatrix();
        }
    glPopMatrix();
}

// --- NEW: BLACK HOLE HELPER ---
void drawBlackHole(float sceneTime, float sizeMult) {
    glDisable(GL_LIGHTING);
    
    // (The orange accretion disk code has been completely removed)

    // 1. Event Horizon (Pure Black Void)
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f); // Pitch black
        glutSolidSphere(1.5 * sizeMult, 30, 30);
    glPopMatrix();

    // 2. Photon ring (Thin bright wireframe ring bending around the void)
    // We keep this so you can actually see the edges of the black hole against the dark sky!
    glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glutWireSphere(1.55 * sizeMult, 25, 25);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

// --- STORY SCENE RENDERERS (60 Seconds Each) ---

void renderScene1(float sceneTime) {
    drawGalleryRoom();
    drawPainting(0.0f); 

    const char* dialog = "";
    if(sceneTime < 15.0f) dialog = "Just a quiet day. The artist is painting, the displays are set...";
    else if(sceneTime < 30.0f) dialog = "But there's something strange about that piece at the end of the red carpet.";
    else if(sceneTime < 45.0f) dialog = "It doesn't look like paint. It looks... alive.";
    else dialog = "I feel like I could just step right into it...";
    drawSubtitles(dialog);

    float charZ = 8.0f;
    bool isMoving = false;
    if (sceneTime < 30.0f) {
        charZ = 8.0f - (sceneTime * 0.36f);
        isMoving = true;
    } else {
        charZ = -3.0f;
    }
    
    glPushMatrix(); glTranslatef(0.0f, 0.0f, charZ); drawCharacter(5.0f, 0.3f, 0.3f, 0.3f, isMoving); glPopMatrix();

    drawPainter(sceneTime);       
    drawFrameHanger();            
    drawBookShowcase();           
    drawSecondPainting();         
    glPushMatrix(); glTranslatef(3.5f, 0.0f, -2.0f); drawFlowerPot(); glPopMatrix();
    
    camZ = 10.0f - (sceneTime * 0.05f);
}

void renderScene2(float sceneTime) {
    drawGalleryRoom();
    
    const char* dialog = "";
    if(sceneTime < 20.0f) dialog = "Whoa, the colors are shifting. Is the canvas breathing?!";
    else if(sceneTime < 40.0f) dialog = "My feet are stuck! What's happening?!";
    else dialog = "The gravity is reversing! It's pulling me in! AAAAAAAHHHH!";
    drawSubtitles(dialog);

    float pulse = (sin(sceneTime * 5.0f) + 1.0f) * (sceneTime / 60.0f);
    drawPainting(pulse);

    // FIX 2: Do NOT suck him in until after 40 seconds so his feet actually stay stuck on the floor!
    float suckFactor = 0.0f;
    if (sceneTime > 40.0f) {
        suckFactor = (sceneTime - 40.0f) / 20.0f; 
    }

    float charZ = -3.0f - (suckFactor * 1.5f);
    float charScale = 1.0f - suckFactor;
    float charRot = suckFactor * 720.0f;

    glPushMatrix();
        // FIX 3: Lift him safely into the air BEFORE rotating, and pivot from his chest!
        glTranslatef(0.0f, suckFactor * 3.0f, charZ); 
        
        // Move pivot point to his chest, tumble wildly, then move back
        glTranslatef(0.0f, 1.0f, 0.0f); 
        glRotatef(charRot, 1.0f, 1.0f, 1.0f); 
        glTranslatef(0.0f, -1.0f, 0.0f);
        
        glScalef(charScale, charScale, charScale);
        
        // Make his arms flail wildly if he is being sucked in
        bool isPanicking = (suckFactor > 0.0f);
        drawCharacter(isPanicking ? 30.0f : 0.0f, 0.3f, 0.3f, 0.3f, isPanicking);
    glPopMatrix();

    camZ = 10.0f - (sceneTime * 0.05f) - (suckFactor * 2.0f);
}

void renderScene3(float sceneTime) {
    glClearColor(0.05f, 0.0f, 0.15f, 1.0f);
    
    const char* dialog = "";
    if(sceneTime < 20.0f) dialog = "Where am I? Did I just fall through the canvas?";
    else if(sceneTime < 40.0f) dialog = "It's a whole solar system... Nine planets orbiting a glowing sun.";
    else dialog = "Earth is glowing right in front of me... It's so peaceful here.";
    drawSubtitles(dialog);

    drawStarfield();

    glPushMatrix();
        glTranslatef(0.0f, 8.0f, -30.0f); 
        glRotatef(15.0f, 1.0f, 0.0f, 0.0f); 
        drawSolarSystem(sceneTime, 0.0f); 
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 2.0f + sin(sceneTime)*0.5f, 0.0f);
        glRotatef(sceneTime * 5.0f, 0.0f, 1.0f, 0.0f);
        drawCharacter(0.0f, 1.0f, 0.8f, 0.2f, false);
    glPopMatrix();

    camZ = 8.0f;
}

void renderScene4(float sceneTime) {
    float rBg = (rand() % 100) / 1000.0f; 
    glClearColor(rBg, 0.0f, 0.0f, 1.0f);

    const char* dialog = "";
    if(sceneTime < 15.0f) dialog = "Wait... the sky is warping. The universe is starting to drip!";
    else if(sceneTime < 30.0f) dialog = "The planets are spinning out of control! The world is destabilizing!";
    else dialog = "WHAT IS THAT?! A Black Hole is tearing the system apart!!";
    drawSubtitles(dialog);

    // Chaos Camera
    camX = (sin(sceneTime * 50.0f) * 0.1f);
    camY = 2.0f + (cos(sceneTime * 45.0f) * 0.1f);

    drawStarfield();

    // --- NEW LOGIC: Swap the Solar System for the Black Hole ---
    if (sceneTime <= 30.0f) {
        // 0 to 30 Seconds: Draw the Destabilizing Solar System
        glPushMatrix();
            glTranslatef(0.0f, 8.0f, -30.0f); 
            glRotatef(15.0f + sin(sceneTime * 15.0f) * 10.0f, 1.0f, 0.0f, 1.0f); 
            drawSolarSystem(sceneTime, 1.0f); 
        glPopMatrix();
    } else {
        // 30 to 60 Seconds: Solar system is GONE, draw ONLY the Black Hole
        float bhGrow = (sceneTime - 30.0f) / 30.0f; // Grows from 0.0 to 1.0
        glPushMatrix();
            // Positioned exactly where the sun used to be
            glTranslatef(0.0f, 8.0f, -30.0f); 
            drawBlackHole(sceneTime, bhGrow * 8.0f); 
        glPopMatrix();
    }

    // Draw the running character in the foreground
    glPushMatrix();
        glTranslatef(0.0f, 2.0f, 0.0f);
        glRotatef(sin(sceneTime * 10.0f) * 20.0f, 0.0f, 1.0f, 0.0f);
        drawCharacter(30.0f, 1.0f, 0.2f, 0.2f, true);
    glPopMatrix();
}

void renderScene5(float sceneTime) {
    if (sceneTime < 20.0f) {
        // ==========================================
        // PHASE 1: FALLING INTO THE BLACK HOLE
        // ==========================================
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        
        const char* dialog = "";
        if(sceneTime < 8.0f) dialog = "I can't escape! The gravity is immense!";
        else dialog = "Crossing the Event Horizon... Everything is stretching... AAAHHHH!!";
        drawSubtitles(dialog);

        camX = ((rand() % 100) / 5000.0f) - 0.01f; 
        camY = 2.0f + ((rand() % 100) / 5000.0f) - 0.01f;
        camZ = 10.0f;

        float bhSize = 5.0f + (sceneTime * 0.8f); 
        float bhZ = -50.0f + (sceneTime * 1.0f); 
        glPushMatrix();
            glTranslatef(0.0f, 2.0f, bhZ); 
            drawBlackHole(sceneTime, bhSize);
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 0.8f, 0.5f); 
        glBegin(GL_LINES);
        for(int i = 0; i < 150; i++) {
            float sx = (rand() % 200 - 100) / 4.0f; 
            float sy = (rand() % 200 - 100) / 4.0f; 
            float sz = -100.0f + fmod((i * 2.0f) + (sceneTime * 200.0f), 120.0f); 
            glVertex3f(sx, sy, sz); glVertex3f(sx, sy, sz - 10.0f); 
        }
        glEnd();
        glEnable(GL_LIGHTING);

        glPushMatrix();
            glTranslatef(0.0f, 2.0f, 2.0f - (sceneTime * 0.3f)); 
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
            glRotatef(sceneTime * 300.0f, 0.0f, 0.0f, 1.0f); 
            
            float stretch = 1.0f + (sceneTime * 0.3f); 
            float squish = 1.0f / (stretch * 0.6f); 
            glScalef(squish, stretch, squish);
            
            drawCharacter(50.0f, 0.3f, 0.3f, 0.3f, true); 
        glPopMatrix();

    } else if (sceneTime < 24.0f) {
        // ==========================================
        // PHASE 2: IMPACT FLASH (The Singularity)
        // ==========================================
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        camX = 0.0f; camY = 2.0f; camZ = 10.0f; 
    } else {
        // ==========================================
        // PHASE 3: WAKEUP & THE JUMP SCARE
        // ==========================================
        float galleryTime = sceneTime - 24.0f;

        // --- NEW: PHASE 4 - CUT TO BLACK ---
        // At 25 seconds, he has run past the camera. Cut to a black screen!
        if (galleryTime > 25.0f) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            
            // Setup 2D projection to draw text perfectly in the middle
            int width = glutGet(GLUT_WINDOW_WIDTH);
            int height = glutGet(GLUT_WINDOW_HEIGHT);
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(0, width, 0, height);
            
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);

            // Draw "THANK YOU" in white text
            glColor3f(1.0f, 1.0f, 1.0f); 
            const char* endMsg = "THANK YOU";
            renderBitmapString(width / 2 - 45, height / 2, GLUT_BITMAP_HELVETICA_18, endMsg);

            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
            
            // Return immediately so we DON'T draw the gallery room anymore
            return; 
        }

        // --- NORMAL GALLERY RENDERING (If we haven't cut to black yet) ---
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        drawGalleryRoom();
        
        const char* dialogWake = "";
        if (galleryTime < 6.0f) dialogWake = "*GASP* ... *Cough* ... I... I'm back?!";
        else if (galleryTime < 14.0f) dialogWake = "The museum... everything is exactly as I left it.";
        else if (galleryTime < 18.0f) dialogWake = "Was it all just a hallucination? Let me look closer...";
        else if (galleryTime < 22.0f) dialogWake = "... WHAT?! NO NO NO! IT'S WAKING UP AGAIN!";
        else dialogWake = "I AM NEVER COMING BACK HERE!! AAAAAHHHH!";
        drawSubtitles(dialogWake);

        camX = 0.0f; camY = 2.0f; camZ = 10.0f;

        glPushMatrix(); 
            if (galleryTime > 18.0f && galleryTime < 22.0f) {
                float shakeX = ((rand() % 100) / 1000.0f) - 0.05f;
                float shakeY = ((rand() % 100) / 1000.0f) - 0.05f;
                glTranslatef(shakeX, shakeY, 0.0f);
                drawPainting(sin(sceneTime * 20.0f) * 0.5f); 
            } else {
                drawPainting(0.0f); 
            }
        glPopMatrix();

        drawPainter(sceneTime);       
        drawFrameHanger();            
        drawBookShowcase();           
        drawSecondPainting();         
        glPushMatrix(); glTranslatef(3.5f, 0.0f, -2.0f); drawFlowerPot(); glPopMatrix();

        float charZ = -2.0f;
        float charY = 0.0f;
        float charRotX = 0.0f;
        float charRotY = 0.0f;
        bool isMoving = false;
        float speed = 5.0f;

        if (galleryTime < 3.0f) {
            float t = galleryTime / 3.0f; 
            charZ = -4.8f + (t * 2.8f); 
            charY = sin(t * M_PI) * 2.0f; 
            charRotX = (1.0f - t) * 720.0f; 
        } 
        else if (galleryTime < 14.0f) {
            charY = sin(galleryTime * 5.0f) * 0.05f; 
        } 
        else if (galleryTime < 18.0f) {
            charRotY = 180.0f; 
        } 
        else if (galleryTime < 22.0f) {
            charRotY = 180.0f;
            float shiver = ((rand() % 100) / 2000.0f) - 0.025f;
            glTranslatef(shiver, 0.0f, 0.0f); 
        } 
        else {
            float runT = galleryTime - 22.0f;
            charZ = -2.0f + (runT * 12.0f); 
            isMoving = true;
            speed = 40.0f; 
        }

        glPushMatrix();
            glTranslatef(0.0f, charY, charZ);
            glTranslatef(0.0f, 0.75f, 0.0f);
            glRotatef(charRotX, 1.0f, 0.0f, 0.0f);
            glRotatef(charRotY, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, -0.75f, 0.0f);
            drawCharacter(speed, 0.3f, 0.3f, 0.3f, isMoving);
        glPopMatrix();
    }
}

// --- CORE OPENGL CALLBACKS ---

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camX, camY, camZ,  lookX, lookY, lookZ,  0.0f, 1.0f, 0.0f);
    setLighting();

    float sceneTime = globalTime - ((currentScene - 1) * SCENE_DURATION);

    switch (currentScene) {
        case 1: renderScene1(sceneTime); break;
        case 2: renderScene2(sceneTime); break;
        case 3: renderScene3(sceneTime); break;
        case 4: renderScene4(sceneTime); break;
        case 5: renderScene5(sceneTime); break;
        default: 
            renderScene5(SCENE_DURATION);
            drawSubtitles("THE END");
            break;
    }

    glutSwapBuffers();
}

void timer(int value) {
    globalTime += 0.016f; 
    
    currentScene = (int)(globalTime / SCENE_DURATION) + 1;
    if (currentScene > 5) currentScene = 6; 

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h); 
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768); 
    glutCreateWindow("Stuck Inside a Painting - Full 5 Min Feature");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0); 

    glutMainLoop();
    return 0;
}