#include "FPS_manager.hpp"

#include <chrono>
using std::chrono::nanoseconds;
using std::chrono::seconds;
using Clock = std::chrono::high_resolution_clock;
#include <thread> // sleep

FPS_manager::FPS_manager()
: cap_frames{true}
, tgt_dur{1}
, real_dur{1}
, prev_end{Clock::now()}
, end{Clock::now()}
{
    this->set_fps(60);
}

void FPS_manager::end_frame()
{
    this->end = Clock::now();
    this->real_dur = this->end - this->prev_end;

    if (this->cap_frames) {
        std::this_thread::sleep_for(this->tgt_dur - this->real_dur);
        this->real_dur = Clock::now() - this->prev_end;
    }

    this->prev_end = Clock::now();
}

unsigned FPS_manager::get_fps()
{
    return std::chrono::duration_cast<nanoseconds>(seconds(1)) / this->real_dur;
}

void FPS_manager::set_fps(unsigned fps)
{
    // compensating for rounding inaccuracy
    fps += 1;

    this->tgt_dur = std::chrono::duration_cast<nanoseconds>(seconds(1)) / fps;
}

void FPS_manager::toggle_cap()
{
    this->cap_frames = !this->cap_frames;
}

void FPS_manager::cap()
{
    this->cap_frames = true;
}

void FPS_manager::uncap()
{
    this->cap_frames = false;
}
