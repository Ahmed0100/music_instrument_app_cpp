/**
 * Instrument.cpp by Olivia Stone for Code Clinic: C++
 **/

#include "Instrument.hpp"

using namespace std;

Instrument::Instrument()
{
    memset(&format_,0,sizeof(format_));
    format_.bits=16;
    format_.channels=1;
    format_.rate=44100;
    format_.byte_format = AO_FMT_LITTLE;
    ao_initialize();
    int default_driver = ao_default_driver_id();
    device_ = ao_open_live(default_driver,&format_,NULL);

    if(device_==NULL)
    {
        cerr << "ERROR: Failed to open audio device"<<endl;
        exit(1);
    }

    bufferSize_ = format_.bits/8 * format_.rate * format_.channels * (float)UPDATE_PERIOD_MS/1000;
    buffer_ = (char*) calloc(bufferSize_,sizeof(char));

    instrumentActive_=true;
    instrumentPlaying_=false;
    playerThread_ = thread([this] { playerLoop();});

}

void Instrument::play(int x,int y,int winWidth,int winHeight)
{
    float frequency = ((float) (winHeight-y+1)/(winHeight+1) * (MAX_FREQ - MIN_FREQ)) + MIN_FREQ;
    float volume = (float) x/winWidth;
    int sample;
    for(int i=0;i< bufferSize_/2; i++)
    {
        sample = volume *32768.0* sin(2* M_PI * frequency * ((float)i/format_.rate));
        buffer_[2*i] = sample & 0xff;
        buffer_[2*i + 1] = (sample>>8) & 0xff;
    }
    instrumentPlaying_ = true;
}
void Instrument::pause()
{
    instrumentPlaying_ = false;
}

void Instrument::destroy()
{
    instrumentPlaying_ = false;
    instrumentActive_ = false;
    playerThread_.join();
    ao_close(device_);
    ao_shutdown();
}
void Instrument::playerLoop()
{
    while (instrumentActive_) {
        while (instrumentPlaying_)
        {
            ao_play(device_, buffer_, bufferSize_);
           this_thread::sleep_for(chrono::milliseconds(UPDATE_PERIOD_MS));
        }
        this_thread::sleep_for(chrono::milliseconds(UPDATE_PERIOD_MS));
    }
}