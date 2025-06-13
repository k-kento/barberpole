package com.gastornisapp.soundlib

class AudioEngine {
    init {
        System.loadLibrary("native-lib")
    }

    external fun startAudio()
    external fun stopAudio()
}