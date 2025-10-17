package com.gastornisapp.barberpole.ui.kaleidoscope

import androidx.lifecycle.ViewModel
import com.gastornisapp.myvulkan.VulkanContext
import com.gastornisapp.myvulkan.kaleidoscope.RotationState
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import javax.inject.Inject

@HiltViewModel
class KaleidoscopeViewModel @Inject constructor(
    val vulkanContext: VulkanContext
) : ViewModel() {

    private val _rotationState = MutableStateFlow(RotationState.None)
    val rotationState: StateFlow<RotationState> = _rotationState

    fun onEvent(intent: KaleidoscopeEvent) {
        when (intent) {
            KaleidoscopeEvent.RotateCW -> _rotationState.value = RotationState.RotatingCW
            KaleidoscopeEvent.RotateCCW -> _rotationState.value = RotationState.RotatingCCW
            KaleidoscopeEvent.StopRotation -> _rotationState.value = RotationState.None
        }
    }
}
