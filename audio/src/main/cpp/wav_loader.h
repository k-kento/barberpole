#pragma once

#include <android/asset_manager.h>
#include <vector>
#include <cstdint>

class WavLoader {
public:
    explicit WavLoader(AAssetManager *assetManager);

    ~WavLoader();

    // WAV読み込み成功したらtrueを返す
    bool load(const char *filename);

    // 読み込んだPCMデータを取得
    const std::vector<int16_t> &getPcmData() const;

    // チャンネル数を取得
    uint16_t getChannels() const;

    // サンプルレートを取得
    uint32_t getSampleRate() const;

private:
    AAssetManager *mAssetManager = nullptr;

    std::vector<int16_t> mPcmData;
    uint16_t mChannels = 0;
    uint32_t mSampleRate = 0;
};

