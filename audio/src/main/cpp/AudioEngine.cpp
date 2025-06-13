//
// Created by 黒須権人 on 2025/06/11.
//


#include <oboe/Oboe.h>
#include <jni.h>

class AudioEngine : public oboe::AudioStreamCallback {
    double phase = 0.0;
    const double frequency = 440.0;
    const double sampleRate = 48000.0;
    const double twoPi = 2 * M_PI;

public:
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *stream, void *audioData, int32_t numFrames) override {
        float *output = static_cast<float *>(audioData);
        for (int i = 0; i < numFrames; ++i) {
            output[i] = static_cast<float>(sin(phase));
            phase += twoPi * frequency / sampleRate;
            if (phase >= twoPi) phase -= twoPi;
        }
        return oboe::DataCallbackResult::Continue;
    }
};

static AudioEngine sineCallback;
static std::shared_ptr<oboe::AudioStream> stream;

//class AudioEngine : public oboe::AudioStreamCallback {
//public:
//    void start() {
//        oboe::AudioStreamBuilder builder;
//        builder.setCallback(this)
//                ->setFormat(oboe::AudioFormat::Float)
//                ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
//                ->setSharingMode(oboe::SharingMode::Exclusive)
//                ->setChannelCount(oboe::ChannelCount::Stereo)
//                ->openStream(&stream);
//        stream->requestStart();
//    }
//
//    void stop() {
//        if (stream) {
//            stream->requestStop();
//            stream->close();
//            stream = nullptr;
//        }
//    }
//
//    oboe::DataCallbackResult
//    onAudioReady(oboe::AudioStream *stream, void *audioData, int32_t numFrames) override {
//        float *outputBuffer = static_cast<float *>(audioData);
//        // ここに音声処理を書く（例：和音を作るなど）
//        return oboe::DataCallbackResult::Continue;
//    }
//
//private:
//    oboe::AudioStream *stream = nullptr;
//};


static AudioEngine player;

extern "C" JNIEXPORT void JNICALL
Java_com_gastornisapp_soundlib_AudioEngine_startAudio(JNIEnv *env, jobject /* this */) {
    oboe::AudioStreamBuilder builder;
    builder.setCallback(&sineCallback)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setFormat(oboe::AudioFormat::Float)
            ->setChannelCount(oboe::ChannelCount::Mono)
            ->setSampleRate(48000)
            ->setDirection(oboe::Direction::Output);

    builder.openStream(stream);
    if (stream) stream->start();
}

extern "C" JNIEXPORT void JNICALL
Java_com_gastornisapp_soundlib_AudioEngine_stopAudio(JNIEnv *env, jobject /* this */) {

}

