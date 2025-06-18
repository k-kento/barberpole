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

struct GenericChunk {
    char id[4];         // "data", "LIST", "fact", etc.
    uint32_t size;      // chunk data size
    // データ本体はこの後ろに続く
};

#pragma pack(pop)