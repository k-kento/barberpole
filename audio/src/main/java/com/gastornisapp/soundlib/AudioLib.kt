package com.gastornisapp.soundlib

import android.content.res.AssetManager

class AudioLib {
    init {
        System.loadLibrary("audio-lib")
    }

    external fun load(assetsManager: AssetManager, type: String)
    external fun start()
    external fun stop()
    external fun release()
}