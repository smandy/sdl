#pragma once

class GameApp {
    
    int width, height;
public:
    GameApp(int width, int height);

    ~GameApp();
    
    void InitSDL();
    
    void InitGL(int width, int height);

    void handleResize(int width, int height);

    void draw();
};
