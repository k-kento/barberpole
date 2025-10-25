package com.gastornisapp.barberpole.ui.home

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.di.MainDispatcher
import com.gastornisapp.barberpole.domain.entity.Notice
import com.gastornisapp.barberpole.domain.usecase.AppLockUseCase
import com.gastornisapp.barberpole.domain.usecase.ForceUpdateUseCase
import com.gastornisapp.barberpole.domain.usecase.NoticeUseCase
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharedFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.flow.filter
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.shareIn
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class HomeViewModel @Inject constructor(
    private val forceUpdateUseCase: ForceUpdateUseCase,
    private val noticeUseCase: NoticeUseCase,
    appLockUseCase: AppLockUseCase,
    @param:MainDispatcher
    private val dispatcher: CoroutineDispatcher
) : ViewModel() {

    private val _menuExpanded: MutableStateFlow<Boolean> = MutableStateFlow(false)
    val menuExpanded: StateFlow<Boolean> = _menuExpanded

    private val _dialogStatus = MutableStateFlow<DialogStatus>(DialogStatus.None)
    val dialogStatus: StateFlow<DialogStatus> = _dialogStatus

    private val _events = MutableSharedFlow<HomeEvent>()
    val events = _events.asSharedFlow()

    val startLockEvents: SharedFlow<Unit> = appLockUseCase.isLockEnabled()
        .filter { it }
        .map { }
        .shareIn(
            scope = viewModelScope,
            started = SharingStarted.Eagerly,
        )

    init {
        viewModelScope.launch(dispatcher) {
            updateDialogStatus()
        }
    }

    fun onNoticeDismissed(noticeId: String) {
        viewModelScope.launch(dispatcher) {
            noticeUseCase.setNoticeId(noticeId)
            updateDialogStatus()
        }
    }

    fun onIntent(intent: HomeIntent) {
        when (intent) {
            is HomeIntent.TapMenu -> _menuExpanded.value = true
            is HomeIntent.CloseMenu -> _menuExpanded.value = false
            is HomeIntent.TapInfo -> {
                _menuExpanded.value = false
                viewModelScope.launch {
                    _events.emit(HomeEvent.Info)
                }
            }

            is HomeIntent.TapSettings -> {
                _menuExpanded.value = false
                viewModelScope.launch {
                    _events.emit(HomeEvent.Settings)
                }
            }
        }
    }

    private suspend fun updateDialogStatus() {
        val isForceUpdateRequired = forceUpdateUseCase.isForceUpdateRequired()
        if (isForceUpdateRequired) {
            _dialogStatus.value = DialogStatus.ForceUpdateRequired
            return
        }

        val currentNotice = noticeUseCase.getCurrentNotice().getOrNull()
        _dialogStatus.value = if (currentNotice != null) {
            DialogStatus.ShowNotice(currentNotice)
        } else {
            DialogStatus.None
        }
    }
}

sealed class DialogStatus {
    object None : DialogStatus()
    object ForceUpdateRequired : DialogStatus()
    data class ShowNotice(val notice: Notice) : DialogStatus()
}
