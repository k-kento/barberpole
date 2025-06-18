//
// Created by 黒須権人 on 2025/06/11.
//

#pragma once

#include <oboe/Oboe.h>
#include <vector>

class AudioEngine : public oboe::AudioStreamCallback {
public:
    bool load(int sr, int ch, std::vector<int16_t>& pcm);
    void start();
    void stop();
private:
    oboe::AudioStream* stream = nullptr;
    int sampleRate;
    int channels;
    unsigned int readIndex;
    std::vector<int16_t> pcmData;
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override;
};