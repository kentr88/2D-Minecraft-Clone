class Player{
private:
    
public:
    float x, y;
    float render_x, render_y;
    float walk_speed, run_speed;
    bool running = false;
    bool falling = false;
    int jumping = 0;    //nothing, 1 rising, 2 falling (gravity)
    int jumping_frames = 0;
    bool holdLeft, holdRight = false;
    int selectedX, selectedY = 0;

    Player(float _x, float _y, float _render_x, float _render_y, float _walk_speed, float _run_speed);
    void move_x(float a);
    void move_y(float a);
        
};