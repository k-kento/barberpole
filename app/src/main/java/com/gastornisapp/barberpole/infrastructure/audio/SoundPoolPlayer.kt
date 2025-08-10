package com.gastornisapp.barberpole.infrastructure.audio

import android.content.Context
import android.media.AudioAttributes
import android.media.SoundPool
import android.util.Log
import com.gastornisapp.barberpole.domain.port.SoundPlayer
import dagger.hilt.android.qualifiers.ApplicationContext
import kotlinx.coroutines.CompletableDeferred
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import javax.inject.Inject

class SoundPoolPlayer @Inject constructor(
    @param: ApplicationContext private val context: Context
) : SoundPlayer {

    private val soundPool: SoundPool
    private val soundMap = mutableMapOf<String, Int>()
    private val soundLoadResultMap = mutableMapOf<Int, CompletableDeferred<SoundLoadResult>>()

    init {
        val audioAttributes = AudioAttributes.Builder()
            .setUsage(AudioAttributes.USAGE_GAME)  // 効果音用途に最適
            .setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION) // 効果音
            .build()

        soundPool = SoundPool.Builder()
            .setMaxStreams(5)  // 同時に鳴らせる効果音の最大数
            .setAudioAttributes(audioAttributes)
            .build()

        soundPool.setOnLoadCompleteListener { _, id, status ->
            val deferred = soundLoadResultMap.remove(id)
            val result = SoundLoadResult(status = status, soundId = id)
            deferred?.complete(result)
        }
    }

    override suspend fun load(fileName: String) {
        val afd = withContext(Dispatchers.IO) {
            context.assets.openFd(fileName)
        }
        val soundId = soundPool.load(afd, 1)
        val deferred = CompletableDeferred<SoundLoadResult>()
        soundLoadResultMap[soundId] = deferred
        val result = deferred.await()
        // TODO エラーハンドリング
        if (result.status == 0) {
            soundMap[fileName] = soundId
        } else {
            Log.e("SoundPoolPlayer", "Sound load failed: $fileName")
        }
    }

    override fun play(fileName: String) {
        val soundId = soundMap[fileName]
        if (soundId != null) {
            soundPool.play(soundId, 1f, 1f, 1, 0, 1f)
        } else {
            // ロードされていない場合の対応
            // 例: Logや例外処理
            Log.e("SoundPoolPlayer", "Sound not loaded: $fileName")
        }
    }

    override fun release() {
        soundPool.setOnLoadCompleteListener(null)
        soundPool.release()
    }
}

private data class SoundLoadResult(val status: Int, val soundId: Int)