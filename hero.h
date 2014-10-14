#ifndef HERO_H
#define HERO_H

#include "Plataform.h"
#include "TextureManager.h"
// HERO STATES
#define HERO_RUNNING 1
#define HERO_JUMPING 2
#define HERO_SLIDING 3

#define HERO_SPEED (3.0)

#define GRAVITY (-0.5)      // gravity acceleration

//hijita  
GLint sprites;


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
                    cout<<"rpos: " << y << endl;
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
        
        
        //var for the sprites :3
        int timer;
        int timebase;
        int anim;
        int i;
        double xsprite;
        double ysprite;
        
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
    private:
        int nsprites;
    public:
        HeroStateRunning(Plataform* plataform): 
            HeroStateBase(plataform), nsprites(6) {
            
            //we reset each var whe a new 
            timer = timebase =  anim = i = 0;
            xsprite = 0.1678;
            ysprite = 0.25;
        }
        // override
        // draw hero sprite
        void draw_hero() {
        
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
	        glEnable(GL_BLEND);//utilizar transparencia                
            // delta timer
           	timer = glutGet(GLUT_ELAPSED_TIME); // recupera el
            int dt = timer -timebase;
            timebase = timer;
            //duracion de la animacion entre dos cambios de Sprite
            anim += dt;
            // si el tiempo de animacion dura mas 0.15s cambiamos de sprite
            if (anim / 1000.0 > 0.15)
            {
                i++;
                anim = 0.0;
            }
            if (i == nsprites) i = 0;            
            int ipos=nsprites-i;
            glBindTexture(GL_TEXTURE_2D, sprites);                         
        
        
        
            //glColor3f(1, 100/255.0, 0);
            glBegin(GL_QUADS);
                //coordenadas de textura
            	glTexCoord2f(0.0f + xsprite*ipos, ysprite);            
                glVertex3d(-50, 0, 0);
                
               	glTexCoord2f(0.0f + xsprite*ipos, ysprite+0.25);
                glVertex3d(-50, 100, 0);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite+0.25);
                glVertex3d(0, 100, 0);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite);
                glVertex3d(0, 0, 0);
            glEnd();
            glDisable(GL_BLEND);    
        }
};

class HeroStateJumping: public HeroStateBase {
    private:
        int nsprites;
    public:
        HeroStateJumping(Plataform* plataform): 
                HeroStateBase(plataform), nsprites(5) {
            next_state = HERO_JUMPING;
            
            //hijita we reset each var whe a new 
            timer = timebase =  anim = i = 0;
            xsprite = 0.1678;
            ysprite = 0.5;           
        }
        void jump() {

        }
        void slide() {

        }
        // override
        // draw hero sprite
        void draw_hero() {
            //glColor4f(0, 0, 0, 1);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
        	glEnable(GL_BLEND);//utilizar transparencia                
            // delta timer
           	timer = glutGet(GLUT_ELAPSED_TIME); // recupera el
            int dt = timer -timebase;
            timebase = timer;
            //duracion de la animacion entre dos cambios de Sprite
            anim += dt;
            // si el tiempo de animacion dura mas 0.15s cambiamos de sprite
            if (anim / 1000.0 > 0.15)
            {
                i++;
                anim = 0.0;
            }

            if (i == nsprites) i = 0;            
            int ipos=nsprites-i;
            glBindTexture(GL_TEXTURE_2D, sprites);
            //glColor3f(1, 100/255.0, 0);
            cout<<"load texture! ipos:"<<ipos<<" x:"<<xsprite*ipos<<"  Y:"<<ysprite<<endl;
            glBegin(GL_QUADS);
                //coordenadas de textura
            	glTexCoord2f(0.0f + xsprite*ipos, ysprite);            
                glVertex3d(-50, 0, 0);
                
               	glTexCoord2f(0.0f + xsprite*ipos, ysprite+0.25);
                glVertex3d(-50, 100, 0);

               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite+0.25);
                glVertex3d(0, 100, 0);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite);
                glVertex3d(0, 0, 0);
            glEnd();
            
            glDisable(GL_BLEND);
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
    private:
        int nsprites;
    public:
        HeroStateSliding(Plataform* plataform): 
                HeroStateBase(plataform), nsprites(2) {
            next_state = HERO_SLIDING;
            width = 99;
            height = 49;
            
            //hijita we reset each var whe a new 
            timer = timebase =  anim = i = 0;
            xsprite = 0.1678;
            ysprite = 0.78;           

        }
        // override
        // draw hero sprite
        void draw_hero() {
            //glColor4f(0, 0, 0, 1);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
        	glEnable(GL_BLEND);//utilizar transparencia                
            // delta timer
           	timer = glutGet(GLUT_ELAPSED_TIME); // recupera el
            int dt = timer -timebase;
            timebase = timer;
            //duracion de la animacion entre dos cambios de Sprite
            anim += dt;
            // si el tiempo de animacion dura mas 0.15s cambiamos de sprite
            if (anim / 1000.0 > 0.15)
            {
                i++;
                anim = 0.0;
            }

            if (i == nsprites) i = 0;            
            int ipos=nsprites-i+3;
            glBindTexture(GL_TEXTURE_2D, sprites);
            //glColor3f(1, 100/255.0, 0);
       
            glBegin(GL_QUADS);
            
            //coordenadas de textura
            	glTexCoord2f(0.0f + xsprite*ipos, ysprite);            
                glVertex3d(-100, 0, 0);
                
               	glTexCoord2f(0.0f + xsprite*ipos, ysprite+0.22);
                glVertex3d(-100, 50, 0);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite+0.22);
                glVertex3d(0, 50, 0);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite);
                glVertex3d(0, 0, 0);

            glEnd();
            glDisable(GL_BLEND);
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
