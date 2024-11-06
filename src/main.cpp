
// Include necessary header files for OpenGL and other functionalities
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include <GL/glut.h>
#include <vector>
#include <random>
#include <cmath>

// Global variables to manage game state, positions, and NPC behaviors
float xoffset = 0.9f; // Horizontal position offset for the player
float yoffset = 0.2f; // Vertical position offset for the player
bool npcsCanMove = false; // Flag to control NPC movement
bool spacePressed = false; // Flag to check if the spacebar has been pressed
float speedMultiplier = 1.0f; // Speed multiplier for game difficulty
double lastSpeedIncreaseTime = 0.0; // Timestamp for the last speed increase
int score = 0; // Player's score
int level = 1; // Current game level
double lastScoreUpdateTime = 0.0; // Timestamp for the last score update
bool gameOver = false; // Flag to check if the game is over
double lastGreenCircleTime = 0.0; // Timestamp for the last appearance of the green circle
bool greenCircleVisible = false; // Flag to control the visibility of the green circle
double speedChangeEndTime = 0.0; // Timestamp for when the speed change ends
bool instructionsVisible = false; // Flag to toggle game instructions visibility

struct NPC {
    float x, y;     // Position
    float size;     // Size
    float r, g, b;  // Color
    float speed;    // Speed at which the NPC moves

    // Constructor
    NPC(float x, float y, float size, float r, float g, float b, float speed) 
        : x(x), y(y), size(size), r(r), g(g), b(b), speed(speed) {}
};
NPC greenCircle(0.0f, 0.0f, 0.05f, 0.0f, 1.0f, 0.0f, 0.0f); // Initialize a green circle NPC as a special object in the game

// Function to check collision between the player and an NPC
bool checkCollision(const NPC& npc) {
    float playerSize = 0.07f;
    float playerX = xoffset;
    float playerY = -0.9f + yoffset;
    // Check collision based on positions and sizes
    return std::abs(playerX - npc.x) < playerSize + npc.size && 
           std::abs(playerY - npc.y) < playerSize + npc.size;
}

// Function to render text on the screen
void renderText(const char* text, float x, float y, float r, float g, float b, void* font) {
    glColor3f(r, g, b); // Set text color
    glRasterPos2f(x, y); // Position for text
// Render each character of the text
    while (*text) {
        glutBitmapCharacter(font, *text);
        ++text;
    }
}
// Function to check collision with the green circle
bool checkCollisionWithGreenCircle() {
    if (!greenCircleVisible) return false;
    float playerSize = 0.07f;
    float playerX = xoffset;
    float playerY = -0.9f + yoffset;
    return std::abs(playerX - greenCircle.x) < playerSize + greenCircle.size &&
           std::abs(playerY - greenCircle.y) < playerSize + greenCircle.size;
}


// Initialize various patterns of NPCs
// Patterns are defined as vectors of NPCs
    // Each pattern consists of a series of NPCs with specific positions, colors, and speeds
std::vector<std::vector<NPC>> npcPatterns = {
    {
        NPC(-0.6f, 1.0f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.4f, 0.9f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.2f, 0.8f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.8f, 1.3f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.1f, 0.7f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC( 0.1f, 0.4f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC( 0.5f, 0.3f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC( 0.7f, 0.6f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.6f, 1.0f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.4f, 0.9f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.2f, 0.8f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.8f, 1.f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC(-0.2f, 0.3f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC( 0.3f, 0.9f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC( 0.5f, 0.3f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        NPC( 0.7f, 0.2f, 0.05f, 0.8f, 0.2f, 0.2f, 0.01f),
        
    },
    {
        NPC(-0.5f, 1.2f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC(-0.3f, 1.0f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC( 0.0f, 0.8f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC( 0.4f, 1.1f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC( 0.7f, 0.9f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC(-0.6f, 0.7f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC( 0.2f, 0.5f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC( 0.5f, 0.3f, 0.05f, 0.6f, 0.4f, 0.4f, 0.016f),
        NPC(-0.5f, 1.1f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        NPC(-0.2f, 0.9f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        NPC( 0.1f, 0.7f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        NPC( 0.3f, 1.0f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        NPC( 0.6f, 0.8f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        NPC( 0.8f, 1.2f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        NPC(-0.7f, 0.6f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        NPC( 0.4f, 0.4f, 0.05f, 0.4f, 0.6f, 0.2f, 0.011f),
        
    },
    {
        NPC(-0.8f, 1.1f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC(-0.6f, 0.9f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC(-0.4f, 0.7f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC(-0.1f, 1.0f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC( 0.3f, 0.8f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC( 0.6f, 1.2f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC( 0.8f, 0.6f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC( 0.5f, 0.4f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC(-0.7f, 1.2f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        NPC(-0.4f, 1.0f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        NPC(-0.1f, 0.8f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        NPC( 0.2f, 1.1f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        NPC( 0.5f, 0.9f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        NPC( 0.7f, 1.3f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        NPC(-0.6f, 0.5f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        NPC( 0.3f, 0.3f, 0.05f, 0.5f, 0.5f, 0.3f, 0.010f),
        
     },
     {
        NPC(-0.8f, 1.1f, 0.05f, 0.9f, 0.1f, 0.1f, 0.010f), // Red, slow
        NPC(-0.6f, 0.9f, 0.05f, 0.1f, 0.9f, 0.1f, 0.015f), // Green, medium
        NPC(-0.4f, 0.7f, 0.05f, 0.1f, 0.1f, 0.9f, 0.020f), // Blue, fast
        NPC(-0.2f, 1.0f, 0.05f, 0.9f, 0.5f, 0.1f, 0.012f), // Orange, medium-slow
        NPC( 0.0f, 0.8f, 0.05f, 0.1f, 0.5f, 0.9f, 0.018f), // Cyan, medium-fast
        NPC( 0.2f, 1.2f, 0.05f, 0.5f, 0.1f, 0.9f, 0.014f), // Purple, medium
        NPC( 0.4f, 0.6f, 0.05f, 0.9f, 0.9f, 0.1f, 0.011f), // Yellow, slow
        NPC( 0.6f, 0.4f, 0.05f, 0.7f, 0.1f, 0.7f, 0.016f), // Magenta, medium-fast
        NPC(-0.8f, 0.3f, 0.05f, 0.1f, 0.7f, 0.7f, 0.013f), // Teal, medium-slow
        NPC(-0.5f, 0.5f, 0.05f, 0.3f, 0.3f, 0.9f, 0.017f), // Slate, fast
        NPC( 0.1f, 0.2f, 0.05f, 0.9f, 0.3f, 0.3f, 0.019f), // Pink, very fast
        NPC( 0.3f, 0.0f, 0.05f, 0.3f, 0.9f, 0.3f, 0.008f)
     },
     {
        NPC(-0.7f, 1.0f, 0.05f, 0.5f, 0.2f, 0.5f, 0.012f),
        NPC(-0.5f, 0.8f, 0.05f, 0.2f, 0.5f, 0.5f, 0.018f),
        NPC(-0.3f, 0.6f, 0.05f, 0.5f, 0.5f, 0.2f, 0.021f),
        NPC(-0.1f, 0.9f, 0.05f, 0.5f, 0.2f, 0.2f, 0.014f),
        NPC( 0.1f, 0.7f, 0.05f, 0.2f, 0.5f, 0.2f, 0.016f),
        NPC( 0.3f, 1.1f, 0.05f, 0.2f, 0.2f, 0.5f, 0.010f),
        NPC( 0.5f, 0.5f, 0.05f, 0.7f, 0.7f, 0.2f, 0.013f),
        NPC( 0.7f, 0.3f, 0.05f, 0.2f, 0.7f, 0.7f, 0.015f),
        NPC(-0.7f, 0.2f, 0.05f, 0.7f, 0.2f, 0.7f, 0.017f),
        NPC(-0.4f, 0.4f, 0.05f, 0.7f, 0.5f, 0.2f, 0.019f),
        NPC( 0.2f, 0.1f, 0.05f, 0.5f, 0.7f, 0.5f, 0.011f),
        NPC( 0.6f, -0.1f, 0.05f, 0.3f, 0.2f, 0.7f, 0.020f)
     },
     {
        NPC(-0.6f, 1.2f, 0.05f, 0.3f, 0.3f, 0.7f, 0.013f),
        NPC(-0.4f, 1.0f, 0.05f, 0.7f, 0.3f, 0.3f, 0.015f),
        NPC(-0.2f, 0.8f, 0.05f, 0.3f, 0.7f, 0.3f, 0.017f),
        NPC( 0.0f, 1.1f, 0.05f, 0.3f, 0.3f, 0.7f, 0.019f),
        NPC( 0.2f, 0.9f, 0.05f, 0.7f, 0.3f, 0.7f, 0.011f),
        NPC( 0.4f, 1.3f, 0.05f, 0.3f, 0.7f, 0.7f, 0.014f),
        NPC( 0.6f, 0.7f, 0.05f, 0.7f, 0.7f, 0.3f, 0.016f),
        NPC( 0.8f, 0.5f, 0.05f, 0.7f, 0.3f, 0.3f, 0.018f),
        NPC(-0.6f, 0.4f, 0.05f, 0.3f, 0.7f, 0.3f, 0.012f),
        NPC(-0.3f, 0.2f, 0.05f, 0.7f, 0.7f, 0.7f, 0.020f),
        NPC( 0.1f, 0.0f, 0.05f, 0.3f, 0.3f, 0.3f, 0.022f),
        NPC( 0.5f, -0.2f, 0.05f, 0.3f, 0.7f, 0.7f, 0.010f)
     },
     {
        NPC(0.68f, 1.24f, 0.05f, 0.72f, 0.44f, 0.33f, 0.017f),
        NPC(-0.86f, 0.83f, 0.05f, 0.94f, 0.92f, 0.18f, 0.016f),
        NPC(-0.19f, 0.96f, 0.05f, 0.47f, 0.52f, 0.48f, 0.016f),
        NPC(0.98f, 1.37f, 0.05f, 0.65f, 0.46f, 0.97f, 0.014f),
        NPC(-0.71f, 1.25f, 0.05f, 0.8f, 0.41f, 0.44f, 0.018f),
        NPC(-0.8f, 1.19f, 0.05f, 0.4f, 0.81f, 0.63f, 0.02f),
        NPC(-0.22f, 1.08f, 0.05f, 0.42f, 0.14f, 0.48f, 0.019f),
        NPC(0.46f, 1.2f, 0.05f, 0.01f, 0.44f, 0.71f, 0.013f),
        NPC(0.47f, 0.68f, 0.05f, 0.63f, 0.71f, 0.51f, 0.019f),
        NPC(0.3f, 1.37f, 0.05f, 0.62f, 0.02f, 0.97f, 0.02f),
        NPC(0.17f, 1.42f, 0.05f, 0.68f, 0.2f, 0.16f, 0.017f),
        NPC(-0.89f, 0.63f, 0.05f, 0.24f, 0.63f, 0.72f, 0.018f)
     },
     {
        NPC(0.09f, 1.07f, 0.05f, 0.05f, 0.64f, 0.26f, 0.01f),
NPC(0.94f, 1.08f, 0.05f, 0.9f, 0.48f, 0.94f, 0.011f),
NPC(-0.62f, 1.44f, 0.05f, 0.21f, 0.47f, 0.42f, 0.015f),
NPC(-0.25f, 0.96f, 0.05f, 0.18f, 0.08f, 0.01f, 0.013f),
NPC(-0.8f, 0.52f, 0.05f, 0.49f, 0.44f, 0.01f, 0.014f),
NPC(0.19f, 0.72f, 0.05f, 0.94f, 0.87f, 0.83f, 0.015f),
NPC(-0.59f, 1.25f, 0.05f, 0.44f, 0.04f, 0.03f, 0.016f),
NPC(-0.13f, 0.97f, 0.05f, 0.85f, 0.96f, 0.87f, 0.016f),
NPC(-0.98f, 1.2f, 0.05f, 0.45f, 0.42f, 0.68f, 0.015f),
NPC(0.85f, 0.9f, 0.05f, 0.46f, 0.44f, 0.33f, 0.013f),
NPC(-0.82f, 0.71f, 0.05f, 0.06f, 0.98f, 0.57f, 0.018f),
NPC(0.65f, 1.19f, 0.05f, 0.35f, 0.66f, 0.74f, 0.014f)
     },
     {
        NPC(-0.37f, 0.76f, 0.05f, 0.44f, 0.39f, 0.12f, 0.017f),
NPC(-0.12f, 0.85f, 0.05f, 0.58f, 0.74f, 0.34f, 0.013f),
NPC(-0.25f, 0.5f, 0.05f, 0.01f, 0.75f, 0.54f, 0.018f),
NPC(-0.15f, 0.54f, 0.05f, 0.39f, 0.42f, 0.98f, 0.015f),
NPC(0.26f, 0.69f, 0.05f, 0.42f, 0.09f, 0.82f, 0.012f),
NPC(0.28f, 1.35f, 0.05f, 0.89f, 0.71f, 0.44f, 0.011f),
NPC(-0.99f, 1.39f, 0.05f, 0.22f, 0.56f, 0.63f, 0.013f),
NPC(-0.33f, 0.53f, 0.05f, 0.66f, 0.33f, 0.13f, 0.017f),
NPC(-0.18f, 1.06f, 0.05f, 0.96f, 0.25f, 0.59f, 0.013f),
NPC(0.98f, 1.47f, 0.05f, 0.48f, 0.34f, 0.71f, 0.019f),
NPC(-0.04f, 1.23f, 0.05f, 0.46f, 0.09f, 0.27f, 0.017f),
NPC(-0.8f, 0.97f, 0.05f, 0.14f, 0.55f, 0.61f, 0.011f)
     },
     {
       NPC(-0.2f, 1.38f, 0.05f, 0.92f, 0.83f, 0.33f, 0.011f),
NPC(-0.19f, 0.54f, 0.05f, 0.77f, 0.33f, 0.06f, 0.01f),
NPC(0.24f, 0.77f, 0.05f, 0.54f, 0.6f, 0.03f, 0.01f),
NPC(0.53f, 1.19f, 0.05f, 0.06f, 0.4f, 0.48f, 0.014f),
NPC(-0.51f, 0.72f, 0.05f, 0.81f, 0.81f, 0.29f, 0.018f),
NPC(0.98f, 0.59f, 0.05f, 0.74f, 0.72f, 0.53f, 0.011f),
NPC(-0.53f, 1.43f, 0.05f, 0.52f, 0.07f, 0.13f, 0.018f),
NPC(0.73f, 0.54f, 0.05f, 0.43f, 0.03f, 0.47f, 0.011f),
NPC(0.29f, 0.73f, 0.05f, 0.52f, 0.68f, 0.39f, 0.01f),
NPC(-0.66f, 0.92f, 0.05f, 0.24f, 0.97f, 0.67f, 0.014f),
NPC(0.38f, 0.55f, 0.05f, 0.08f, 0.54f, 0.41f, 0.019f),
NPC(0.34f, 1.21f, 0.05f, 0.18f, 0.99f, 0.14f, 0.012f)
     },
     {
        NPC(0.73f, 1.16f, 0.05f, 0.61f, 0.11f, 0.2f, 0.017f),
NPC(-0.55f, 1.15f, 0.05f, 0.84f, 0.31f, 0.96f, 0.015f),
NPC(-0.98f, 1.24f, 0.05f, 0.33f, 0.64f, 0.36f, 0.011f),
NPC(0.29f, 0.92f, 0.05f, 0.77f, 0.0f, 0.16f, 0.012f),
NPC(0.68f, 0.76f, 0.05f, 0.69f, 0.74f, 0.16f, 0.016f),
NPC(-0.36f, 1.28f, 0.05f, 0.23f, 0.16f, 0.98f, 0.015f),
NPC(0.74f, 0.84f, 0.05f, 0.52f, 0.68f, 0.11f, 0.014f),
NPC(0.13f, 1.37f, 0.05f, 0.82f, 0.41f, 0.15f, 0.018f),
NPC(0.46f, 0.84f, 0.05f, 0.99f, 0.49f, 0.6f, 0.01f),
NPC(0.16f, 0.94f, 0.05f, 0.35f, 0.86f, 0.7f, 0.014f),
NPC(0.72f, 1.27f, 0.05f, 0.95f, 0.44f, 0.05f, 0.012f),
NPC(-0.12f, 1.1f, 0.05f, 0.63f, 0.39f, 0.35f, 0.013f)
     },
     {
        NPC(0.23f, 0.71f, 0.05f, 0.86f, 0.65f, 0.17f, 0.014f),
NPC(-0.1f, 1.37f, 0.05f, 0.93f, 0.05f, 0.56f, 0.018f),
NPC(-0.76f, 1.42f, 0.05f, 0.64f, 0.27f, 0.96f, 0.014f),
NPC(-0.73f, 0.91f, 0.05f, 0.7f, 0.49f, 0.13f, 0.015f),
NPC(-0.14f, 1.12f, 0.05f, 0.93f, 0.98f, 0.56f, 0.018f),
NPC(-0.25f, 0.59f, 0.05f, 0.21f, 0.81f, 0.82f, 0.019f),
NPC(0.42f, 1.28f, 0.05f, 0.67f, 0.05f, 0.01f, 0.018f),
NPC(0.69f, 1.42f, 0.05f, 0.22f, 0.16f, 0.14f, 0.014f),
NPC(-0.62f, 1.28f, 0.05f, 0.24f, 0.16f, 0.06f, 0.018f),
NPC(0.72f, 1.23f, 0.05f, 0.87f, 0.64f, 0.69f, 0.012f),
NPC(-0.81f, 1.19f, 0.05f, 0.53f, 0.39f, 0.54f, 0.017f),
NPC(-0.47f, 1.0f, 0.05f, 0.79f, 0.56f, 0.33f, 0.013f)
     },
     {
        NPC(-0.25f, 1.02f, 0.05f, 0.15f, 0.12f, 0.18f, 0.011f),
NPC(-0.52f, 0.69f, 0.05f, 0.63f, 0.09f, 0.87f, 0.017f),
NPC(0.92f, 0.57f, 0.05f, 0.81f, 0.92f, 0.47f, 0.011f),
NPC(-0.77f, 1.4f, 0.05f, 0.01f, 0.91f, 0.53f, 0.019f),
NPC(0.25f, 0.52f, 0.05f, 0.12f, 0.09f, 0.02f, 0.013f),
NPC(-0.64f, 1.39f, 0.05f, 0.2f, 0.83f, 0.59f, 0.02f),
NPC(0.21f, 1.47f, 0.05f, 0.62f, 0.26f, 0.46f, 0.02f),
NPC(0.7f, 0.76f, 0.05f, 0.81f, 0.4f, 0.11f, 0.012f),
NPC(-0.18f, 1.48f, 0.05f, 0.47f, 0.45f, 0.77f, 0.018f),
NPC(-0.49f, 0.99f, 0.05f, 0.42f, 0.75f, 0.81f, 0.017f),
NPC(0.2f, 1.12f, 0.05f, 0.48f, 0.65f, 0.59f, 0.017f),
NPC(-0.88f, 1.45f, 0.05f, 0.72f, 0.46f, 0.55f, 0.016f)
     },
     {
        NPC(0.05f, 0.69f, 0.05f, 0.01f, 0.69f, 0.89f, 0.012f),
NPC(-0.49f, 0.66f, 0.05f, 0.41f, 0.78f, 0.23f, 0.019f),
NPC(0.32f, 1.1f, 0.05f, 1.0f, 0.02f, 0.59f, 0.019f),
NPC(-0.09f, 1.48f, 0.05f, 0.48f, 0.99f, 0.4f, 0.011f),
NPC(0.01f, 0.7f, 0.05f, 0.33f, 0.91f, 0.83f, 0.013f),
NPC(-0.55f, 1.28f, 0.05f, 0.76f, 0.06f, 0.89f, 0.01f),
NPC(0.44f, 1.16f, 0.05f, 0.94f, 0.32f, 0.67f, 0.015f),
NPC(-0.6f, 0.72f, 0.05f, 0.94f, 0.94f, 0.12f, 0.015f),
NPC(0.54f, 1.48f, 0.05f, 0.9f, 0.8f, 0.07f, 0.013f),
NPC(0.37f, 1.33f, 0.05f, 0.34f, 0.81f, 0.19f, 0.015f),
NPC(-0.79f, 0.75f, 0.05f, 0.87f, 0.45f, 0.66f, 0.011f),
NPC(-0.61f, 0.7f, 0.05f, 0.19f, 0.6f, 0.72f, 0.018f)
     },
     {
        NPC(0.48f, 1.05f, 0.05f, 0.89f, 0.4f, 0.36f, 0.015f),
NPC(-0.71f, 0.99f, 0.05f, 0.82f, 0.17f, 0.02f, 0.015f),
NPC(-0.51f, 1.26f, 0.05f, 0.49f, 0.18f, 0.54f, 0.014f),
NPC(-0.79f, 1.14f, 0.05f, 0.41f, 0.94f, 0.8f, 0.016f),
NPC(0.55f, 1.41f, 0.05f, 0.8f, 0.03f, 0.79f, 0.016f),
NPC(0.88f, 0.62f, 0.05f, 0.37f, 0.35f, 0.1f, 0.011f),
NPC(0.83f, 1.19f, 0.05f, 0.27f, 0.98f, 0.87f, 0.013f),
NPC(0.21f, 1.3f, 0.05f, 0.3f, 0.61f, 0.71f, 0.019f),
NPC(-0.86f, 1.1f, 0.05f, 0.38f, 0.3f, 0.72f, 0.014f),
NPC(-0.54f, 1.27f, 0.05f, 0.61f, 0.56f, 0.22f, 0.014f),
NPC(0.91f, 1.02f, 0.05f, 0.8f, 0.14f, 0.7f, 0.015f),
NPC(0.7f, 1.16f, 0.05f, 0.72f, 0.21f, 0.86f, 0.01f)
     },
     {
        NPC(-0.53f, 0.89f, 0.05f, 0.27f, 0.65f, 0.25f, 0.013f),
NPC(0.61f, 1.26f, 0.05f, 0.89f, 0.39f, 0.21f, 0.018f),
NPC(0.57f, 1.42f, 0.05f, 0.34f, 0.08f, 0.67f, 0.013f),
NPC(-0.21f, 1.0f, 0.05f, 0.94f, 0.11f, 0.22f, 0.014f),
NPC(0.41f, 0.69f, 0.05f, 0.39f, 0.51f, 0.49f, 0.012f),
NPC(-0.71f, 0.9f, 0.05f, 0.73f, 0.45f, 0.52f, 0.019f),
NPC(-0.18f, 0.68f, 0.05f, 0.6f, 0.7f, 0.36f, 0.011f),
NPC(-0.61f, 0.64f, 0.05f, 0.37f, 0.92f, 0.57f, 0.012f),
NPC(-0.89f, 0.91f, 0.05f, 0.73f, 0.74f, 0.75f, 0.015f),
NPC(0.16f, 1.07f, 0.05f, 0.18f, 0.94f, 0.71f, 0.017f),
NPC(0.44f, 1.15f, 0.05f, 0.9f, 0.46f, 0.64f, 0.012f),
NPC(-0.82f, 0.97f, 0.05f, 0.95f, 0.47f, 0.22f, 0.011f)
     },
     {
        NPC(0.96f, 0.89f, 0.05f, 0.75f, 0.14f, 0.2f, 0.01f),
NPC(0.48f, 1.22f, 0.05f, 0.63f, 0.73f, 0.68f, 0.018f),
NPC(-0.33f, 1.02f, 0.05f, 0.66f, 0.52f, 0.89f, 0.016f),
NPC(-0.47f, 0.99f, 0.05f, 0.58f, 0.57f, 0.91f, 0.019f),
NPC(-0.95f, 1.49f, 0.05f, 0.04f, 0.5f, 0.69f, 0.011f),
NPC(-0.65f, 1.42f, 0.05f, 0.81f, 0.86f, 0.1f, 0.017f),
NPC(-0.09f, 0.73f, 0.05f, 0.44f, 0.88f, 0.4f, 0.011f),
NPC(0.86f, 0.98f, 0.05f, 0.57f, 0.4f, 0.7f, 0.016f),
NPC(-0.88f, 1.11f, 0.05f, 0.16f, 0.87f, 0.29f, 0.014f),
NPC(0.71f, 1.06f, 0.05f, 0.77f, 0.46f, 0.54f, 0.019f),
NPC(-0.5f, 1.31f, 0.05f, 0.52f, 0.53f, 0.86f, 0.014f),
NPC(0.41f, 1.28f, 0.05f, 0.21f, 0.49f, 0.91f, 0.012f)
     },
     {
        NPC(-0.52f, 0.93f, 0.05f, 0.03f, 0.77f, 0.77f, 0.019f),
NPC(0.66f, 0.65f, 0.05f, 0.04f, 0.09f, 0.4f, 0.011f),
NPC(-0.66f, 1.11f, 0.05f, 0.64f, 0.58f, 0.75f, 0.017f),
NPC(-0.34f, 1.06f, 0.05f, 0.55f, 0.39f, 0.05f, 0.015f),
NPC(0.34f, 0.61f, 0.05f, 0.07f, 0.23f, 0.33f, 0.013f),
NPC(0.56f, 0.5f, 0.05f, 0.59f, 0.28f, 0.65f, 0.013f),
NPC(-0.43f, 1.21f, 0.05f, 0.17f, 0.37f, 0.15f, 0.012f),
NPC(0.45f, 1.34f, 0.05f, 0.45f, 0.32f, 0.35f, 0.012f),
NPC(-0.19f, 0.59f, 0.05f, 0.65f, 0.31f, 0.66f, 0.016f),
NPC(0.13f, 0.93f, 0.05f, 0.87f, 0.19f, 0.94f, 0.012f),
NPC(-0.76f, 1.02f, 0.05f, 0.02f, 0.89f, 0.58f, 0.011f),
NPC(0.96f, 0.88f, 0.05f, 0.35f, 0.43f, 0.0f, 0.012f)
     },
     {
        NPC(-0.33f, 1.14f, 0.05f, 0.88f, 0.6f, 0.49f, 0.02f),
NPC(0.93f, 1.38f, 0.05f, 0.53f, 0.8f, 0.29f, 0.017f),
NPC(0.45f, 0.93f, 0.05f, 0.62f, 0.25f, 0.32f, 0.011f),
NPC(-0.73f, 1.46f, 0.05f, 0.24f, 0.87f, 0.57f, 0.012f),
NPC(0.19f, 1.09f, 0.05f, 0.95f, 0.0f, 0.62f, 0.01f),
NPC(-0.91f, 0.84f, 0.05f, 0.14f, 0.91f, 0.77f, 0.019f),
NPC(0.61f, 0.67f, 0.05f, 0.74f, 0.88f, 0.84f, 0.017f),
NPC(-0.61f, 1.14f, 0.05f, 0.94f, 0.57f, 0.31f, 0.011f),
NPC(0.01f, 0.64f, 0.05f, 0.69f, 0.63f, 0.41f, 0.015f),
NPC(0.21f, 1.3f, 0.05f, 0.05f, 0.59f, 0.9f, 0.018f),
NPC(0.02f, 1.11f, 0.05f, 0.06f, 0.03f, 0.35f, 0.012f),
NPC(-0.98f, 1.02f, 0.05f, 0.57f, 0.65f, 0.58f, 0.012f),
     },
     {
        NPC(-0.88f, 1.05f, 0.05f, 0.01f, 0.86f, 0.97f, 0.011f),
NPC(0.62f, 0.67f, 0.05f, 0.39f, 0.73f, 0.52f, 0.019f),
NPC(-0.9f, 1.45f, 0.05f, 0.49f, 0.63f, 0.71f, 0.019f),
NPC(-0.27f, 0.52f, 0.05f, 0.19f, 0.33f, 0.81f, 0.013f),
NPC(0.95f, 1.07f, 0.05f, 0.48f, 0.14f, 0.22f, 0.017f),
NPC(-0.15f, 1.11f, 0.05f, 0.48f, 0.29f, 0.39f, 0.013f),
NPC(-0.19f, 0.6f, 0.05f, 0.31f, 0.12f, 0.03f, 0.015f),
NPC(-0.05f, 0.8f, 0.05f, 0.71f, 0.12f, 0.55f, 0.017f),
NPC(-0.48f, 0.64f, 0.05f, 0.67f, 0.98f, 0.52f, 0.014f),
NPC(0.43f, 1.05f, 0.05f, 0.54f, 0.34f, 0.95f, 0.016f),
NPC(-0.96f, 1.45f, 0.05f, 0.17f, 0.98f, 0.43f, 0.018f),
NPC(-0.7f, 0.56f, 0.05f, 0.47f, 0.89f, 0.14f, 0.01f)
     }
    // Add more patterns if needed
};

// Pointer to the currently active NPC pattern
std::vector<NPC>* activePattern = &npcPatterns[0];

NPC npc1(-0.5f, 0.0f, 0.05f, 1.0f, 0.0f, 0.0f, 0.01f);
NPC npc2( 0.0f, 0.2f, 0.05f, 0.0f, 1.0f, 0.0f, 0.01f);
NPC npc3( 0.5f,-0.1f, 0.05f, 0.0f, 0.0f, 1.0f, 0.01f);
NPC npc4(-0.7f, 0.0f, 0.05f, 1.0f, 0.0f, 0.0f, 0.01f);
NPC npc5( 1.0f, 0.1f, 0.05f, 0.0f, 1.0f, 0.0f, 0.01f);
NPC npc6( 0.3f,-0.2f, 0.05f, 0.0f, 0.0f, 1.0f, 0.01f);

// Function to update the position of a single NPC
void updateNPC(NPC &npc)
{
   npc.y -= npc.speed* speedMultiplier;;
}

// Function to update the positions of all NPCs in a pattern
void updateNPCs(std::vector<NPC>& npcs)
 {
    for (auto& npc : npcs) {
        updateNPC(npc);
    }
}

// Function to render a single NPC
void renderNPC(const NPC &npc) {
    glColor3f(npc.r, npc.g, npc.b); // Set NPC color

    glBegin(GL_QUADS);// Begin drawing a quadrilateral
        // Define the four corners of the quadrilateral based on NPC's position and size
        glVertex2f(npc.x - npc.size, npc.y + npc.size);
        glVertex2f(npc.x - npc.size, npc.y - npc.size);
        glVertex2f(npc.x + npc.size, npc.y - npc.size);
        glVertex2f(npc.x + npc.size, npc.y + npc.size);
    glEnd();
}

// Function to render all NPCs in a pattern
void renderNPCs(const std::vector<NPC>& npcs) {
    for (const auto& npc : npcs) {
        renderNPC(npc); // Render each NPC in the pattern
    }
}

// Function to reset NPC positions to their initial state
void resetNPCPositions(std::vector<NPC>& npcs) {
    for (auto& npc : npcs) {
        npc.y = 1.0f; // Reset to starting position at the top
    }
}

// Function to switch to a random pattern of NPCs
void switchToRandomPattern() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, npcPatterns.size() - 1);

    int patternIndex = distrib(gen);// Randomly select a pattern
    activePattern = &npcPatterns[patternIndex];// Update the active pattern
     resetNPCPositions(*activePattern);// Reset positions of NPCs in the new pattern
    level++;// Increment the level
}

void update(GLFWwindow* window) {
    // Update game logic here
    float speed = 0.01f;
    float maxOffsetX = 0.9f; // Maximum X offset, adjust as needed
    float minOffsetX = -0.9f; // Minimum X offset, adjust as needed
    float maxOffsetY = 1.8f; // Maximum Y offset, adjust as needed
    float minOffsetY = 0.1f; // Minimum Y offset, adjust as needed
    double currentTime = glfwGetTime();
   
    if (currentTime - lastScoreUpdateTime >= 0.2) {
        score++;
        lastScoreUpdateTime = currentTime;
    }
    if (currentTime - lastSpeedIncreaseTime >= 4.0) { // Every 10 seconds
        speedMultiplier *= 1.15f; // Increase the speed by 1.5 times
        lastSpeedIncreaseTime = currentTime; // Reset the timer
    }

    // Moving left
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
    instructionsVisible = true;
} else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
    instructionsVisible = false;
}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (xoffset > minOffsetX) {
            xoffset -= speed;
        }
    }
    // Moving right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (xoffset < maxOffsetX) {
            xoffset += speed;
        }
    }
    // Moving up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (yoffset < maxOffsetY) {
            yoffset += speed;
        }
    }
    // Moving down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (yoffset > minOffsetY) {
            yoffset -= speed;
        }
    }

    if (npcsCanMove) { // Only update NPCs if they are allowed to move
        updateNPC(npc1);
        updateNPC(npc2);
        updateNPC(npc3);
        updateNPC(npc4);
        updateNPC(npc5);
        updateNPC(npc6);
    }
    bool patternComplete = true;
    for (const auto& npc : *activePattern) {
        if (npc.y > -1.0f) {
            patternComplete = false;
            break;
        }
    }

    if (patternComplete) {
        switchToRandomPattern();
    }

    if (npcsCanMove) {
        updateNPCs(*activePattern);
    }
    if (!gameOver) {
        if (checkCollision(npc1) || checkCollision(npc2) || 
            checkCollision(npc3) || checkCollision(npc4) || 
            checkCollision(npc5) || checkCollision(npc6)) {
            gameOver = true;
        }

        for (const auto& npc : *activePattern) {
            if (checkCollision(npc)) {
                gameOver = true;
                break;
            }
        }
    }
    if (currentTime - lastGreenCircleTime > 30.0) {
        greenCircleVisible = true;
        lastGreenCircleTime = currentTime;
    }


    if (checkCollisionWithGreenCircle()) {
        score += 100;
        speedMultiplier = 0.5f; // Slow down the game
        speedChangeEndTime = currentTime;
        greenCircleVisible = false; // Hide the green circle after collision
    }
}


void render() {
    // Render (draw) your game here
    glClear(GL_COLOR_BUFFER_BIT);
    if (instructionsVisible) {
        renderText("Welcome to Pixel Peril! You are a white block who has to avoid all other blocks.", -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24);
        renderText("Press A to move Left and D to move right. The game speeds up after every level you pass!", -0.5f, -0.1f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24);
        renderText("Have fun:))))", -0.1f, -0.2f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24);
        return; // Skip rendering the rest of the game when instructions are visible
    }
    if (gameOver) {
        // Render the Game Over text
        renderText("GAME OVER", -0.1f, 0.0f, 1.0f, 0.0f, 0.0f,GLUT_BITMAP_TIMES_ROMAN_24); // Red color

        // Render the final score
        char finalScore[50];
        sprintf(finalScore, "Final Score: %d", score);
        renderText(finalScore, -0.15f, -0.1f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24); // White color

        // Render the final level
        char finalLevel[50];
        sprintf(finalLevel, "Final Level: %d", level);
        renderText(finalLevel, -0.15f, -0.2f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24); // White color
    }
    if (!spacePressed) {
        renderText("Pixel Peril!", -0.1f, 0.1f, 1.0f, 0.0f, 0.0f, GLUT_BITMAP_HELVETICA_18); // Adjust position and color as needed

        renderText("Press Spacebar to Start game", -0.2f, 0.0f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24);
        renderText("Hold down I for Instructions", -0.2f, -0.1f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24);
        return;
    }

    char scoreText[50];
    sprintf(scoreText, "Score: %d", score);
    renderText(scoreText, -0.15f, -0.9f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24); // Adjust position as needed

    char levelText[50];
    sprintf(levelText, "Level: %d", level);
    renderText(levelText, 0.05f, -0.9f, 1.0f, 1.0f, 1.0f,GLUT_BITMAP_TIMES_ROMAN_24); // Adjust position as needed

    // Set the color of the square (RGB)
    glColor3f(1.0f, 1.0f, 1.0f); // White color

    // Draw a square
    float size = 0.07f;
    float startY = -0.9f; // Start Y position at the bottom of the window

    glBegin(GL_QUADS); // Start drawing a quad
        
        glBegin(GL_QUADS); // Start drawing a square
        glVertex2f(-size + xoffset, startY + size + yoffset); // Top left
        glVertex2f(-size + xoffset, startY - size + yoffset); // Bottom left
        glVertex2f( size + xoffset, startY - size + yoffset); // Bottom right
        glVertex2f( size + xoffset, startY + size + yoffset); // Top right
    glEnd();
    renderNPC(npc1);
    renderNPC(npc2);
    renderNPC(npc3);
    renderNPC(npc4);
    renderNPC(npc5);
    renderNPC(npc6);
    renderNPCs(*activePattern);
    
}
void resetNPCPositions() {
    // Loop through each NPC in the active pattern and reset its position
    for (auto& npc : *activePattern) {
        npc.y = 1.0f; // Set the Y position back to the top (or whatever your starting position is)
        // You might also want to reset other properties of the NPCs as needed
    }
}
void resetGame() {
    // Reset all game variables to their initial states
    score = 0;
    level = 1;
    gameOver = false;
    xoffset = 0.9f;
    yoffset = 0.2f;
    npcsCanMove = false;
    spacePressed = false;
    speedMultiplier = 1.0f;
    lastSpeedIncreaseTime = 0.0;
    lastScoreUpdateTime = 0.0;
    // Reset NPCs
    resetNPCPositions();
}

int main(int argc, char** argv) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glutInit(&argc, argv);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1700, 950, "Pixel Peril!", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Main loop */
    bool running = true;
    while (running && !glfwWindowShouldClose(window)&& !gameOver) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep to cap frame rate

        update(window);   // Update game logic
        render();   // Render the scene

        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents();        // Poll for and process events
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            running = false;
    
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (!spacePressed) {
        spacePressed = true;
        npcsCanMove = true; // Start NPCs moving only when the game starts
        gameOver = false;   // Reset gameOver to start the game
    }
}
        if (gameOver) {
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                resetGame(); // Reset the game to its initial state
            }
            std::this_thread::sleep_for(std::chrono::seconds(15)); // Wait for 30 seconds
            break; // Exit the loop
        }

        // Check if 'running' should be updated based on game logic or input events
    }

    /* Cleanup */
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
