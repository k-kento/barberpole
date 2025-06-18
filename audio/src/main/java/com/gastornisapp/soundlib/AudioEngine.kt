package com.gastornisapp.soundlib

import android.content.res.AssetManager

class AudioEngine {
    init {
        System.loadLibrary("audio-lib")
    }

    external fun load(assetsManager: AssetManager)
    external fun startAudio()
    external fun stopAudio()
}