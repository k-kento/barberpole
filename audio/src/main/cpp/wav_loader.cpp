#include "wav_loader.h"
#include "log_util.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstring>
#include <iostream>

#pragma pack(push, 1)
struct RiffHeader {
    char id[4];       // "RIFF"
    uint32_t size;    // ファイルサイズ - 8
    char format[4];   // "WAVE"
};

struct FmtChunk {
    char id[4];          // "fmt "
    uint32_t size;       // fmt チャンクサイズ（16 or 18 or 40）
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    // 追加データがある場合もあるが、ここでは省略（必要なら追加）
};

struct DataChunk {
    char id[4];
    uint32_t size;
};

#pragma pack(pop)

WavLoader::WavLoader(AAssetManager* assetManager) : mAssetManager(assetManager) {}

WavLoader::~WavLoader() = default;

bool WavLoader::load(const char* filename) {
    if (!mAssetManager) {
        LOGE("AssetManager is null in WavLoader");
        return false;
    }

    AAsset* asset = AAssetManager_open(mAssetManager, filename, AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to open asset: %s", filename);
        return false;
    }

    // RIFFヘッダ読み込み
    RiffHeader riff{};
    AAsset_read(asset, &riff, sizeof(riff));

    if (std::strncmp(riff.id, "RIFF", 4) != 0 || std::strncmp(riff.format, "WAVE", 4) != 0) {
        LOGE("File is not a valid WAV file: %s", filename);
        AAsset_close(asset);
        return false;
    }

    // "fmt " チャンク読み込み
    FmtChunk fmt{};
    AAsset_read(asset, &fmt, sizeof(fmt));

    if (std::strncmp(fmt.id, "fmt ", 4) != 0) {
        LOGE("Invalid fmt chunk in WAV file: %s", filename);
        AAsset_close(asset);
        return false;
    }

    // PCM のみ受け付ける
    if (fmt.audioFormat != 1) {
        LOGE("Unsupported audio format in WAV file (only PCM is supported): %s", filename);
        AAsset_close(asset);
        return false;
    }

    // 拡張フォーマットを考慮して、必要なら fmt.size - 16 分を読み飛ばす
    if (fmt.size > 16) {
        AAsset_seek(asset, fmt.size - 16, SEEK_CUR);
    }

    // "data" チャンクを探す
    DataChunk dataChunk{};
    while (AAsset_read(asset, &dataChunk, sizeof(dataChunk)) == sizeof(dataChunk)) {
        if (std::strncmp(dataChunk.id, "data", 4) == 0) {
            // 見つけた → chunk.size がPCMデータ長
            break;
        } else {
            // スキップ
            AAsset_seek(asset, dataChunk.size, SEEK_CUR);
        }
    }

    mChannels = fmt.numChannels;
    mSampleRate = fmt.sampleRate;

    size_t sampleCount = dataChunk.size / (fmt.numChannels * (fmt.bitsPerSample / 8));
    mPcmData.resize(sampleCount);

    int bytesRead = AAsset_read(asset, mPcmData.data(), dataChunk.size);
    AAsset_close(asset);

    if (bytesRead != static_cast<int>(dataChunk.size)) {
        LOGE("Failed to read all PCM data from WAV file: %s", filename);
        return false;
    }

    LOGD("Successfully parsed WAV file: %s", filename);
    return true;
}

const std::vector<int16_t>& WavLoader::getPcmData() const {
    return mPcmData;
}

uint16_t WavLoader::getChannels() const {
    return mChannels;
}

uint32_t WavLoader::getSampleRate() const {
    return mSampleRate;
}
