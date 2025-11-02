package com.gastornisapp.barberpole.ui.page.drawing

import androidx.lifecycle.ViewModel
import com.gastornisapp.myvulkan.VulkanContext
import dagger.hilt.android.lifecycle.HiltViewModel
import javax.inject.Inject

@HiltViewModel
class DrawingViewModel @Inject constructor(
    val vulkanContext: VulkanContext
) : ViewModel() {


    fun onEvent(event: DrawingEvent) {

    }
}
