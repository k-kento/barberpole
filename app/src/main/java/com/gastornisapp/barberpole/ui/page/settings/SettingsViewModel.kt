package com.gastornisapp.barberpole.ui.page.settings

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.domain.usecase.AppLockUseCase
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class SettingsViewModel @Inject constructor(
    val appLockUseCase: AppLockUseCase
) : ViewModel() {

    val isLockEnabled: StateFlow<Boolean> = appLockUseCase
        .isLockEnabled()
        .stateIn(
            scope = viewModelScope,
            started = SharingStarted.Eagerly,
            initialValue = false
        )

    private val _events = MutableSharedFlow<SettingsEvent>()
    val events = _events.asSharedFlow()

    fun onEvent(intent: SettingsIntent) {
        when (intent) {
            is SettingsIntent.ToggleLockMode -> {
                val isLockEnabled = !isLockEnabled.value
                viewModelScope.launch {
                    appLockUseCase.setLockEnabled(isLockEnabled)
                    val event = if (isLockEnabled) SettingsEvent.StartLock else SettingsEvent.StopLock
                    _events.emit(event)
                }
            }

            SettingsIntent.TapSecurityScreenButton -> {
                viewModelScope.launch {
                    _events.emit(SettingsEvent.OpenSecuritySettings)
                }
            }
        }
    }
}
