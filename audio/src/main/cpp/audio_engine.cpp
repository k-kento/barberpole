//
// Created by 黒須権人 on 2025/06/11.
//

#include <oboe/Oboe.h>
#include "audio_engine.h"
#include "log_util.h"

bool AudioEngine::load(int sr, int ch, std::vector<int16_t>& pcm) {
    sampleRate = sr;
    channels = ch;
    pcmData = std::move(pcm);

    // Oboeストリーム初期化
    oboe::AudioStreamBuilder builder;
    builder.setSampleRate(sampleRate)
            ->setChannelCount(channels)
            ->setFormat(oboe::AudioFormat::I16)
            ->setCallback(this)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency);

    auto result = builder.openStream(&stream);
    if (result == oboe::Result::OK) {
        return true;
    } else {
        return false;
    }
}

void AudioEngine::start() {
    if (stream) {
        readIndex = 0;
        auto result = stream->requestStart();
        if (result != oboe::Result::OK) {
            LOGE("failed to start.");
        }
    }
}

void AudioEngine::stop() {
    if (stream) {
        readIndex = 0;
        stream->stop();
    }
}

oboe::DataCallbackResult AudioEngine::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    auto *outputBuffer = static_cast<int16_t *>(audioData);
    size_t framesAvailable = (pcmData.size() / channels) - readIndex;
    size_t framesToCopy = std::min<size_t>(numFrames, framesAvailable);

    // PCMデータをコピー
    for (size_t i = 0; i < framesToCopy * channels; i++) {
        outputBuffer[i] = pcmData[readIndex * channels + i];
    }

    // 足りない分はゼロで埋める
    for (size_t i = framesToCopy * channels; i < numFrames * channels; i++) {
        outputBuffer[i] = 0;
    }

    readIndex += framesToCopy;

    if (pcmData.size() / channels <= readIndex) {
        return oboe::DataCallbackResult::Stop;  // 再生終了
    } else {
        return oboe::DataCallbackResult::Continue;  // 継続再生
    }
}
