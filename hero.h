#ifndef HERO_H
#define HERO_H

#include "Plataform.h"
#include "TextureManager.h"



// HERO STATES
#define HERO_RUNNING 1
#define HERO_JUMPING 2
#define HERO_SLIDING 3
#define HERO_HITTING 4

#define HERO_SPEED (3.0)

#define GRAVITY (-0.5)      // gravity acceleration
int POWER = 0;



//hijita  
GLint hero_sprites;


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
                string object = plataform->object_in(x-i*step, y-1);
                if (plataform->is_solid(object)) {
                    return true;
                }
            }
            return false;
        }
        bool colliding() {
            // check 5 points in the front
            double step = height/4;
            for (int i = 0; i < 5; ++i) {
                string object = plataform->object_in(x+1, y+i*step);
                if (plataform->is_solid(object)) {
                    return true;
                }
            }
            return false;
        }
        bool hit_front_object() {
            // hit 5 points in the front
            double step = height/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->hit_object_in(x+1, y+i*step)) {
                    return true;
                }                
            }
            return false;
        }
        bool hit_base_object() {
            // check 5 points in the base
            double step = width/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->hit_object_in(x-i*step, y-1)) {
                    return true;
                }
            }
            return false;
        }
        bool collect_front_object() {
            // hit 5 points in the front
            double step = height/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->collect_object_in(x+1, y+i*step)) {
                	cout<<"moneda1!"<<endl;
                    play_hero_sound(HERO_COIN_SOUND);                    
                    POWER=POWER + 10;
                    return true;
                }                
            }
            return false;
        }
        bool collect_base_object() {
            // check 5 points in the base
            double step = width/4;
            for (int i = 0; i < 5; ++i) {
                if (plataform->collect_object_in(x-i*step, y-1)) {
                    cout<<"moneda2!"<<endl;
                    play_hero_sound(HERO_COIN_SOUND);                    
                    POWER=POWER +10;
                    return true;
                }
            }
            return false;   
        }
        bool collect() {
            return collect_base_object() || collect_front_object();
        }
        bool will_land() {
            // check 5 points in the base
            double step = width/4;
            for (int i = 0; i < 5; ++i) {
                string object = plataform->object_in(x-i*step, y+velocity-1);
                if (plataform->is_solid(object)) {
                    return true;
                }
            }
            return false;
        }
        bool colliding_with_roof() {
            // check 5 points in the roof
            double step = width/4;
            for (int i = 0; i < 5; ++i) {
                string object = plataform->object_in(x-i*step, y+height+1);
                if (plataform->is_solid(object)) {
                    return true;
                }
            }
            return false;
        }
        bool will_collide() {
            // check 5 points in the front
            double step = height/4;
            for (int i = 0; i < 5; ++i) {
                string object = plataform->object_in(x+HERO_SPEED, y+i*step);
                if (plataform->is_solid(object)) {
                    return true;
                }
            }
            return false;
        }
        virtual void recalculate_y() {
            if (!over_floor() || velocity > 0) {
                if (will_land()) {
                    y = (int)y - (int)y % 50;
                    velocity = 0;
                } else if (colliding_with_roof() && velocity > 0) {
                    velocity = 0;
                } else {
                    y += velocity;
                    velocity += GRAVITY;
                }
            }
        }
        virtual void recalculate_x() {
            if (!colliding()){
                if (will_collide()) {
                    x += 49 - ((int)x%50);
                } else {
                    x += HERO_SPEED;
                }
            }
        }
        void recalculate_position() {
            collect();
            recalculate_x();
            recalculate_y();
        }
        void draw() {
            char* bonus = int_to_char(POWER);
            print(x+500, 570,0, bonus);
            pre_draw();
            draw_hero();
        }

        
        // override
        // action when user press up arrow
        // jump by default
        virtual void jump() {
            velocity = 13;
            next_state = HERO_JUMPING;
        }
        // override
        // action when user press up arrow
        // slide by default
        virtual void slide() {
            next_state = HERO_SLIDING;
        }
        // override
        // action when user press space
        // hit by default
        virtual void hit() {
            next_state = HERO_HITTING;
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
        
        
        //var for the hero_sprites :3
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


class HeroStateHitting: public HeroStateBase {
    private:
        int nhero_sprites;
    public:
        HeroStateHitting(Plataform* plataform): 
            HeroStateBase(plataform), nhero_sprites(5) {
            next_state = HERO_HITTING;
            //we reset each var whe a new 
            timer = timebase =  anim = i = 0;
            xsprite = 0.1678;
            ysprite = 0;
            //function in gamesound.h
			play_hero_sound(HERO_HIT_SOUND) ;
        }
        // override
        void recalculate_x() {
            hit_front_object();
            HeroStateBase::recalculate_x();
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
            if (i == nhero_sprites) {
                i = 0;
                next_state = HERO_RUNNING;            
            }
            int ipos=nhero_sprites-i;
            glBindTexture(GL_TEXTURE_2D, hero_sprites);                         
        
        
        
            //glColor3f(1, 100/255.0, 0);
            glBegin(GL_QUADS);
                //coordenadas de textura
                glTexCoord2f(0.0f + xsprite*ipos, ysprite);            
                glVertex3d(-50, 0, 10);
                
                glTexCoord2f(0.0f + xsprite*ipos, ysprite+0.25);
                glVertex3d(-50, 100, 10);
                
                glTexCoord2f(xsprite*(ipos + 1.0), ysprite+0.25);
                glVertex3d(0, 100, 10);
                
                glTexCoord2f(xsprite*(ipos + 1.0), ysprite);
                glVertex3d(0, 0, 10);
            glEnd();
            glDisable(GL_BLEND);    
        }
};

class HeroStateRunning: public HeroStateBase {
    private:
        int nhero_sprites;
    public:
        HeroStateRunning(Plataform* plataform): 
            HeroStateBase(plataform), nhero_sprites(6) {
            
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
            if (i == nhero_sprites) i = 0;            
            int ipos=nhero_sprites-i;
            glBindTexture(GL_TEXTURE_2D, hero_sprites);                         
        
        
        
            //glColor3f(1, 100/255.0, 0);
            glBegin(GL_QUADS);
                //coordenadas de textura
            	glTexCoord2f(0.0f + xsprite*ipos, ysprite);            
                glVertex3d(-50, 0, 10);
                
               	glTexCoord2f(0.0f + xsprite*ipos, ysprite+0.25);
                glVertex3d(-50, 100, 10);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite+0.25);
                glVertex3d(0, 100, 10);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite);
                glVertex3d(0, 0, 10);
            glEnd();
            glDisable(GL_BLEND);    
        }
};

class HeroStateJumping: public HeroStateBase {
    private:
        int nhero_sprites;
    public:
        HeroStateJumping(Plataform* plataform): 
                HeroStateBase(plataform), nhero_sprites(5) {
            next_state = HERO_JUMPING;
            
            //hijita we reset each var whe a new 
            timer = timebase =  anim = i = 0;
            xsprite = 0.1678;
            ysprite = 0.5; 
            
   			play_hero_sound(HERO_JUMP_SOUND);                  
        }
        void jump() {

        }
        void hit() {
            
        }
        void slide() {

        }
        void recalculate_y() {
            HeroStateBase::recalculate_y();
            if (hit_base_object()) {
                velocity = 5;
            }
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

            if (i == nhero_sprites) i = 0;            
            int ipos=nhero_sprites-i;
            glBindTexture(GL_TEXTURE_2D, hero_sprites);
            
            glBegin(GL_QUADS);
                //coordenadas de textura
            	glTexCoord2f(0.0f + xsprite*ipos, ysprite);            
                glVertex3d(-50, 0, 10);
                
               	glTexCoord2f(0.0f + xsprite*ipos, ysprite+0.25);
                glVertex3d(-50, 100, 10);

               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite+0.25);
                glVertex3d(0, 100, 10);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite);
                glVertex3d(0, 0, 10);
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
        int nhero_sprites;
    public:
        HeroStateSliding(Plataform* plataform): 
                HeroStateBase(plataform), nhero_sprites(2) {
            next_state = HERO_SLIDING;
            width = 99;
            height = 49;
            
            //hijita we reset each var whe a new 
            timer = timebase =  anim = i = 0;
            xsprite = 0.1678;
            ysprite = 0.78;   
            
  			play_hero_sound(HERO_SLICE_SOUND);        

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

            if (i == nhero_sprites) i = 0;            
            int ipos=nhero_sprites-i+3;
            glBindTexture(GL_TEXTURE_2D, hero_sprites);
            //glColor3f(1, 100/255.0, 0);
       
            glBegin(GL_QUADS);
            
            //coordenadas de textura
            	glTexCoord2f(0.0f + xsprite*ipos, ysprite);            
                glVertex3d(-100, 0, 10);
                
               	glTexCoord2f(0.0f + xsprite*ipos, ysprite+0.22);
                glVertex3d(-100, 50, 10);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite+0.22);
                glVertex3d(0, 50, 10);
                
               	glTexCoord2f(xsprite*(ipos + 1.0), ysprite);
                glVertex3d(0, 0, 10);

            glEnd();
            glDisable(GL_BLEND);
        }
        void slide() {

        }
        void hit() {

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
                case HERO_HITTING:
                    state = new HeroStateHitting(plataform);
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
