package com.gastornisapp.barberpole.ui.page.kaleidoscope

import android.content.Context
import androidx.lifecycle.ViewModel
import com.gastornisapp.myvulkan.VulkanContext
import com.gastornisapp.myvulkan.kaleidoscope.KaleidoscopeImage
import com.gastornisapp.myvulkan.kaleidoscope.RotationState
import dagger.hilt.android.lifecycle.HiltViewModel
import dagger.hilt.android.qualifiers.ApplicationContext
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import javax.inject.Inject

@HiltViewModel
class KaleidoscopeViewModel @Inject constructor(
    @param:ApplicationContext private val context: Context,
    val vulkanContext: VulkanContext
) : ViewModel() {

    private val _rotationState = MutableStateFlow(RotationState.None)
    val rotationState: StateFlow<RotationState> = _rotationState

    private val _isImagePickerShowing = MutableStateFlow(false)
    val isImagePickerShowing: StateFlow<Boolean> = _isImagePickerShowing

    private val _images = MutableStateFlow(
        KaleidoscopeImage.getImages(context).map { ImageItem(it) }
    )
    val images: StateFlow<List<ImageItem>> = _images

    private val _selectedImage = MutableStateFlow<ImageItem>(_images.value[0])
    val selectedImage: StateFlow<ImageItem> = _selectedImage

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
