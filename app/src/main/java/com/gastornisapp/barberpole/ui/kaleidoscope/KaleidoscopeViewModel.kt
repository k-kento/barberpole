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

    private val _isImagePickerShowing = MutableStateFlow(false)
    val isImagePickerShowing: StateFlow<Boolean> = _isImagePickerShowing

    private val _images = MutableStateFlow(
        listOf("bus_body.png", "bus_main.png", "car_body.png").map(::ImageItem)
    )
    val images: StateFlow<List<ImageItem>> = _images

    private val _selectedImage = MutableStateFlow<ImageItem?>(null)
    val selectedImage: StateFlow<ImageItem?> = _selectedImage

    fun onEvent(intent: KaleidoscopeEvent) {
        when (intent) {
            KaleidoscopeEvent.RotateCW -> _rotationState.value = RotationState.RotatingCW
            KaleidoscopeEvent.RotateCCW -> _rotationState.value = RotationState.RotatingCCW
            KaleidoscopeEvent.StopRotation -> _rotationState.value = RotationState.None
            KaleidoscopeEvent.SelectButtonClicked -> {
                _isImagePickerShowing.value = true
            }

            KaleidoscopeEvent.ImagePickerDismissed -> {
                _isImagePickerShowing.value = false
            }

            is KaleidoscopeEvent.ImageSelected -> {
                _selectedImage.value = intent.imageItem
            }
        }
    }
}
