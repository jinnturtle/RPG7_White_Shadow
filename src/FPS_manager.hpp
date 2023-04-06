#ifndef SRC_FPS_MANAGER_HPP_
#define SRC_FPS_MANAGER_HPP_

#include <chrono>

class FPS_manager final {
 public:
    FPS_manager();

    // mark end of current frame (update fps, etc)
    void end_frame();
    // get actual fps
    unsigned get_fps();
    // what fps to aim for if frame cap is on
    void set_fps(unsigned fps);
    void toggle_cap();
    void cap();
    void uncap();

 private:
    bool cap_frames;
    std::chrono::nanoseconds tgt_dur; // target duration of one frame
    std::chrono::nanoseconds real_dur; // real duration of last frame
    std::chrono::high_resolution_clock::time_point prev_end;
    std::chrono::high_resolution_clock::time_point end;
};

#endif // SRC_FPS_MANAGER_HPP_
