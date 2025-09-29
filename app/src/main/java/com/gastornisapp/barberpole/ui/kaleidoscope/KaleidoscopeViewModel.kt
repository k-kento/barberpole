package com.gastornisapp.barberpole.ui.kaleidoscope

import androidx.lifecycle.ViewModel
import com.gastornisapp.myvulkan.VulkanContext
import dagger.hilt.android.lifecycle.HiltViewModel
import javax.inject.Inject

@HiltViewModel
class KaleidoscopeViewModel @Inject constructor(
    val vulkanContext: VulkanContext
) : ViewModel()