#ifndef HERO_H
#define HERO_H

#include "Plataform.h"

// HERO STATES
#define HERO_RUNNING 1
#define HERO_JUMPING 2
#define HERO_SLIDING 3

#define HERO_SPEED (3.0)

#define GRAVITY (-0.5)      // gravity acceleration


/*
    We asume we have a 800x600 window
    Then we divide this window in 16x12 regions
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |  X |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |  X |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
    |----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
    Our hero is 2 squares tall. It's marked with "X"

    The points (x, y) that represent the position of our hero are located at
        the bottom-right corner:

             |----|
             |    |
             |----|
             |    |
             |----*
                (x,y)

    When hero is sliding, (x,y) represents the point in the bottom-left corner:

        |----|----|
        |    |    |
        |----|----*
                (x,y)

*/


class HeroStateBase {
    public:
        HeroStateBase(Plataform* _plataform):
                plataform(_plataform),
                next_state(HERO_RUNNING) {
            width = 49;
            height = 99;
        }
        void pre_draw() {
            glLoadIdentity();
            recalculate_position();
            glTranslated(x, y, 0);
        }
        bool over_floor() {
            // check 5 points in the base
            double step = width/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->solid_in(x-i*step, y-1)) {
                    return true;
                }
            }
            return false;
        }
        bool colliding() {
            // check 5 points in the front
            double step = height/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->solid_in(x+1, y+i*step)) {
                    return true;
                }
            }
            return false;
        }
        bool will_land() {
            // check 5 points in the base
            double step = width/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->solid_in(x-i*step, y+velocity-1)) {
                    return true;
                }
            }
            return false;
        }
        bool will_collide() {
            // check 5 points in the front
            double step = height/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->solid_in(x+HERO_SPEED, y+i*step)) {
                    return true;
                }
            }
            return false;
        }
        void recalculate_position() {
            if (!over_floor() || velocity > 0) {
                if (will_land()) {
                    y = (int)y - (int)y % 50;
                    cout << y << endl;
                    velocity = 0;
                } else {
                    y += velocity;
                    velocity += GRAVITY;
                }
            }

            if (!colliding()){
                if (will_collide()) {
                    x += 49 - ((int)x%50);
                } else {
                    x += HERO_SPEED;
                }
            }
        }
        void draw() {
            pre_draw();
            draw_hero();
        }

        
        // override
        // action when user press up arrow
        // jump by default
        virtual void jump() {
            velocity = 12;
            next_state = HERO_JUMPING;
        }
        // override
        // action when user press up arrow
        // slide by default
        virtual void slide() {
            next_state = HERO_SLIDING;
        }
        // override
        // return next state
        virtual int get_next_state() {
            return next_state;
        }
        // override
        // draw hero sprite
        virtual void draw_hero() = 0;


        static double x;           // position in X
        static double y;           // position in Y
    protected:
        Plataform* plataform;
        int next_state;
        static double velocity;    // up or down vertical velocity
        double width;
        double height;
};

double HeroStateBase::velocity = 0;
double HeroStateBase::x = 150;
double HeroStateBase::y = 100;


class HeroStateRunning: public HeroStateBase {
    public:
        HeroStateRunning(Plataform* plataform): 
            HeroStateBase(plataform) {

        }
        // override
        // draw hero sprite
        void draw_hero() {
            glColor3f(1, 100/255.0, 0);
            glBegin(GL_QUADS);
                glVertex3d(0, 0, 0);
                glVertex3d(-50, 0, 0);
                glVertex3d(-50, 100, 0);
                glVertex3d(0, 100, 0);
            glEnd();
        }
};

class HeroStateJumping: public HeroStateBase {
    public:
        HeroStateJumping(Plataform* plataform): 
                HeroStateBase(plataform) {
            next_state = HERO_JUMPING;
        }
        void jump() {

        }
        void slide() {

        }
        // override
        // draw hero sprite
        void draw_hero() {
            glColor3f(1, 100/255.0, 0);
            glBegin(GL_QUADS);
                glVertex3d(0, 0, 0);
                glVertex3d(-50, 0, 0);
                glVertex3d(-50, 100, 0);
                glVertex3d(0, 100, 0);
            glEnd();
        }
        // override
        // return next state
        int get_next_state() {
            if (over_floor()) {
                return HERO_RUNNING;
            } else {
                return next_state;
            }
        }
};

class HeroStateSliding: public HeroStateBase {
    public:
        HeroStateSliding(Plataform* plataform): 
                HeroStateBase(plataform) {
            next_state = HERO_SLIDING;
            width = 99;
            height = 49;
        }
        // override
        // draw hero sprite
        void draw_hero() {
            glColor3f(1, 100/255.0, 0);
            glBegin(GL_QUADS);
                glVertex3d(0, 0, 0);
                glVertex3d(-100, 0, 0);
                glVertex3d(-100, 50, 0);
                glVertex3d(0, 50, 0);
            glEnd();
        }
        void slide() {

        }
};

class HeroStateManager {
    public:
        HeroStateManager(Plataform* _plataform):
            plataform(_plataform),
            state_code(HERO_RUNNING),
            state(new HeroStateRunning(plataform)) {

        }
        void set_state(int new_state_code) {
            if (new_state_code == state_code){
                return;
            }
            state_code = new_state_code;

            delete state;
            switch(new_state_code){
                case HERO_RUNNING:
                    state = new HeroStateRunning(plataform);
                    break;
                case HERO_JUMPING:
                    state = new HeroStateJumping(plataform);
                    break;
                case HERO_SLIDING:
                    state = new HeroStateSliding(plataform);
                    break;
            }
        }
        HeroStateBase* get_state() {
            return state;
        }
        void draw() {
            state->draw();
            int new_state_code = state->get_next_state();
            set_state(new_state_code);
        }
    private:
        int state_code;
        Plataform* plataform;
        HeroStateBase* state;
};

#endif