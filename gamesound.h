#ifndef GAMESOUND_H
#define GAMESOUND_H

///functions for sounds!!!
#include <string>
#include <thread>
#include <SFML/Audio.hpp>

using namespace std;

class MyStream : public sf::SoundStream
{
public:

    void load(const sf::SoundBuffer& buffer)
    {
        // extract the audio samples from the sound buffer to our own container
        m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

        // reset the current playing position 
        m_currentSample = 0;

        // initialize the base class
        initialize(buffer.getChannelCount(), buffer.getSampleRate());
    }

private:

    virtual bool onGetData(Chunk& data)
    {
        // number of samples to stream every time the function is called;
        // in a more robust implementation, it would rather be a fixed
        // amount of time rather than an arbitrary number of samples
        const int samplesToStream = 50000;

        // set the pointer to the next audio samples to be played
        data.samples = &m_samples[m_currentSample];

        // have we reached the end of the sound?
        if (m_currentSample + samplesToStream <= m_samples.size())
        {
            // end not reached: stream the samples and continue
            data.sampleCount = samplesToStream;
            m_currentSample += samplesToStream;
            return true;
        }
        else
        {
            // end of stream reached: stream the remaining samples and stop playback
            data.sampleCount = m_samples.size() - m_currentSample;
            m_currentSample = m_samples.size();
            return false;
        }
    }

    virtual void onSeek(sf::Time timeOffset)
    {
        // compute the corresponding sample index according to the sample rate and channel count
        m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
    }

    std::vector<sf::Int16> m_samples;
    std::size_t m_currentSample;
};

//



const int sound_size=8;

#define HERO_COIN_SOUND 3
#define HERO_HIT_SOUND 5
#define HERO_DIE_SOUND 4
#define HERO_SLICE_SOUND 6
#define HERO_JUMP_SOUND 7

string soundlist[sound_size]={"Contra.wav","Sunsetriders.wav","Top Gear.wav","moneda.wav","morir.wav","golpe.wav", "deslizar.wav", "salto.wav"};
sf::SoundBuffer buffer[sound_size];
MyStream stream [sound_size];

int selected_track=0;
int hero_sound;
void play_hero_thread(){
	//when the hero makes an action
	stream[hero_sound].play();
	//play until the sound ends :3
	sf::sleep(sf::seconds(1.0f));
}

void play_hero_sound(int hero_sound_code){
	hero_sound=hero_sound_code;
	thread *t = new thread(play_hero_thread);
}
void play_track_thread(){
	//play the tracks until the end of game
	int current_track=selected_track;	
	while(1){
		stream[current_track].play();
		// let it play until it is finished
		while (stream[current_track].getStatus() == MyStream::Playing &&
				current_track==selected_track )
		    sf::sleep(sf::seconds(0.1f));
		//we sleep while to mix the current and next tracks
		stream[current_track].stop();
		current_track=selected_track;//update the new track and loop to play	
	}

}
void play_track(){
	thread *t = new thread(play_track_thread);
}
void set_track(int track){
	selected_track=track;
}
void init_sound(){
	for(int i=0; i< sound_size; ++i)
	    buffer[i].loadFromFile(soundlist[i]);
    // initialize and play our custom stream
	for(int i=0; i< sound_size; ++i)
    	stream[i].load(buffer[i]);
}

///end of functions for sounds :3



#endif
