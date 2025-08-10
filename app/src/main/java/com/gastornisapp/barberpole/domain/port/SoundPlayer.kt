package com.gastornisapp.barberpole.domain.port

interface SoundPlayer {

    suspend fun load(fileName: String)

    fun play(fileName: String)

    fun release()
}
