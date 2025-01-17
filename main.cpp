#include "camera.hpp"
#include "draw.hpp"
#include "gui.hpp"
#include "lighting.hpp"
#include "text.hpp"

#include <cmath>

using namespace std;

Window window;
Camera camera;
Lighting lighting;
Text text;

float dt = 1/60.;
float t = 0;
bool paused = false;
vec3 p0, p1;
int n = 50;
double d_n = (double)n;
vector<vector<vec3>> flag_points(3*n, vector<vec3>(2*n, vec3(0., 0., 0.)));

void drawStuff() {
    setColor(vec3(0.8,0.2,0.2));
    drawArrow(vec3(0,0,0), vec3(1,0,0), 0.05);
    setColor(vec3(0.2,0.6,0.2));
    drawArrow(vec3(0,0,0), vec3(0,1,0), 0.05);
    setColor(vec3(0.2,0.2,0.8));
    drawArrow(vec3(0,0,0), vec3(0,0,1), 0.05);
    setPointSize(10);
    setColor(vec3(0.2,0.2,0.2));
    // drawPoint(p0);
    // drawPoint(p1);
    for(int i = 0; i < 3*n-1; i++) {
        for(int j = 0; j < 2*n-1; j++) {
            drawQuad(flag_points[i][j], flag_points[i+1][j], flag_points[i+1][j+1], flag_points[i][j+1]);
        }
    }
}

void drawWorld() {
    camera.apply(window);
    lighting.apply();
    clear(vec3(0.9,0.9,0.9));
    setColor(vec3(0.7,0.7,0.7));
    drawQuad(vec3(-3,0,-3), vec3(-3,0,3), vec3(3,0,3), vec3(3,0,-3));
    drawStuff();
    setColor(vec3(0,0,0));
    text.draw("WASD and LShift/LCtrl to move camera", -0.9, 0.90);
    text.draw("Mouse to rotate view", -0.9, 0.85);
    text.draw("Space to play/pause animation", -0.9, 0.80);
}

void update(float dt) {
    t += dt;
    // p0 = vec3(sin(M_PI*t), 1, cos(M_PI*t));
    // p1 = vec3(-sin(M_PI*t), 1, -cos(M_PI*t));
    for(int i = 0; i < 3*n; i++) {
        for(int j = 0; j < 2*n; j++) {
            flag_points[i][j][2] = flag_points[i][j][0] * sin(2*M_PI*(flag_points[i][j][0] + flag_points[i][j][1] - t)) / 20;
        }
    }
}

void keyPressed(int key) {
    // See http://www.glfw.org/docs/latest/group__keys.html for key codes
    if (key == GLFW_KEY_SPACE)
        paused = !paused;
}

int main(int argc, char **argv) {
    window.create("Animation", 1024, 768);
    window.onKeyPress(keyPressed);
    camera.lookAt(vec3(1,1.5,5), vec3(0,0.5,0));
    lighting.createDefault();
    text.initialize();

    double x_max = 0;
    for(int i = 0; i < 3*n; i++) {
        for(int j = 0; j < 2*n; j++) {
            flag_points[i][j][0] = i / d_n;
            flag_points[i][j][1] = j / d_n;
        }
    }

    while (!window.shouldClose()) {
        camera.processInput(window);
        if (!paused)
            update(dt);
        window.prepareDisplay();
        drawWorld();
        window.updateDisplay();
        window.waitForNextFrame(dt);
    }
}
