package com.gastornisapp.barberpole.ui.percussion

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.domain.usecase.PlaySoundUseCase
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.async
import kotlinx.coroutines.awaitAll
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class PercussionViewModel @Inject constructor(
    private val playSoundUseCase: PlaySoundUseCase
) : ViewModel() {

    private val _inProgress = MutableStateFlow(true)
    val inProgress: StateFlow<Boolean> = _inProgress

    init {
        viewModelScope.launch {
            PercussionType.entries.map { type ->
                async { playSoundUseCase.load(type.filePath()) }
            }.awaitAll()
            _inProgress.value = false
        }
    }

    fun play(percussionType: PercussionType) {
        playSoundUseCase.play(percussionType.filePath())
    }

    override fun onCleared() {
        super.onCleared()
        playSoundUseCase.release()
    }
}
