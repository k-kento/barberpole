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
    int framesCopied = 0;

    // 再生終了判定用
    bool isFinished = false;

    while (framesCopied < numFrames) {
        size_t framesAvailable = (pcmData.size() / channels) - readIndex;

        if (framesAvailable == 0) {
            // 再生終了 → 残りを無音にしてループ終了
            memset(outputBuffer + framesCopied * channels, 0, (numFrames - framesCopied) * channels * sizeof(int16_t));
            isFinished = true;
            break;
        }

        size_t framesToCopy = std::min<size_t>(numFrames - framesCopied, framesAvailable);

        memcpy(outputBuffer + framesCopied * channels,
               pcmData.data() + readIndex * channels,
               framesToCopy * channels * sizeof(int16_t));

        readIndex += framesToCopy;
        framesCopied += framesToCopy;
    }

    // 最後のデータ再生が終わったらストリームを停止
    if (isFinished) {
        return oboe::DataCallbackResult::Stop;
    } else {
        return oboe::DataCallbackResult::Continue;
    }
}
