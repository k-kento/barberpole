package com.gastornisapp.barberpole.ui

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.soundlib.AudioLib
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject
import kotlin.time.Duration.Companion.seconds

@HiltViewModel
class MainViewModel @Inject constructor(
    private val repository: AppSettingsRepository
) : ViewModel() {

    private val _startDestination = MutableStateFlow<PageType?>(null)
    val startDestination: StateFlow<PageType?> = _startDestination

    private val _isSplashScreenShowing = MutableStateFlow(true)
    val isSplashScreenShowing: StateFlow<Boolean> = _isSplashScreenShowing.asStateFlow()

    init {
        viewModelScope.launch {

            // 2秒後にスプラッシュ画面を強制的に消すタイマー
            launch {
                delay(duration = 2.seconds)
                _isSplashScreenShowing.value = false
            }

            AudioLib.initialize()

            if (!repository.isTermsOfServiceAccepted()) {
                _startDestination.value = PageType.Confirmation
                return@launch
            }

            _isSplashScreenShowing.value = false
            _startDestination.value = PageType.Home
        }
    }
}