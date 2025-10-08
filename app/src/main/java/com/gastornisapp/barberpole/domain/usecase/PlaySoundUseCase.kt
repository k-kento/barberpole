package com.gastornisapp.barberpole.domain.usecase

import com.gastornisapp.barberpole.domain.port.SoundPlayer
import javax.inject.Inject

class PlaySoundUseCase @Inject constructor(private val soundPlayer: SoundPlayer) {

    suspend fun load(fileName: String) {
        soundPlayer.load(fileName)
    }

    fun play(fileName: String) {
        soundPlayer.play(fileName)
    }

    fun release() {
        soundPlayer.release()
    }

}
