#ifndef HERO_H
#define HERO_H

#define HERO_KEEP_STATE 0
#define HERO_RUNNING 1
#define HERO_JUMPING 2

class HeroStateBase {
	public:
		void set_color() {
			glColor3f(1,1,1);
		}
		void pre_draw() {
			glLoadIdentity();
			set_color();
		}
		// override
		// return next state
		virtual int draw() {
		    glutSolidTeapot(4);
			return HERO_KEEP_STATE;
		}
};

class HeroStateRunning: public HeroStateBase {

};

class HeroStateJumping: public HeroStateBase {
	public:
		HeroStateJumping(): hero_pos_y(0), hero_pos_x(-sqrt(hero_pos_y_max-0.1)) {

		}

		int draw() {
			hero_pos_y = hero_pos_y_max - hero_pos_x*hero_pos_x;
			hero_pos_x += hero_pos_x_delta;
			if (hero_pos_y < 0) {
			    hero_pos_y = 0;
			    hero_pos_x = -sqrt(hero_pos_y_max-0.1);  // -0.1 to correct error
			    return HERO_RUNNING;
			}
			glTranslated(0, hero_pos_y, 0);
			glutSolidTeapot(4);
			return HERO_KEEP_STATE;
		}
	private:
		const static double hero_pos_y_delta = 1;
		static const double hero_pos_y_max = 20;
		static const double hero_pos_x_delta = 0.2;
		double hero_pos_y;
		double hero_pos_x;  // -0.1 to correct error
};


class HeroStateSliding: public HeroStateBase {

};


class HeroStateManager {
	public:
		HeroStateManager(): state_code(HERO_RUNNING), state(new HeroStateRunning) {

		}
		void set_state(int new_state_code) {
			if (new_state_code == HERO_KEEP_STATE){
				return;
			}
			state_code = new_state_code;

			delete state;
			switch(new_state_code){
				case HERO_RUNNING:
					state = new HeroStateRunning();
					break;
				case HERO_JUMPING:
					state = new HeroStateJumping();
					break;
			}
		}
		int get_state() {
			return state_code;
		}
		void draw() {
			state->pre_draw();
			int new_state_code = state->draw();
			if (new_state_code != state_code) {
				set_state(new_state_code);
			}
		}
	private:
		int state_code;
		HeroStateBase* state;		
};

#endif