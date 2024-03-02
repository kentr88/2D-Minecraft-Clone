#include "player.hpp"


Player::Player(float _x, float _y, float _render_x, float _render_y, float _walk_speed, float _run_speed){
    x = _x;
    y = _y;
    render_x = _render_x;
    render_y = _render_y;
    walk_speed = _walk_speed;
    run_speed = _run_speed;
}

void Player::move_x(float a){
    if(a > 0){
        if((x + a) < 512) x += a;
    } else {
        if((x + a) >= 0) x += a;
    }
    return;
}

void Player::move_y(float a){
    if(a > 0){
        if((y + a) < 128) y += a;
    } else {
        if((y + a) >= 0) y += a;
    }
    return;
}