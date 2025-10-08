package com.gastornisapp.barberpole.ui.home

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.di.MainDispatcher
import com.gastornisapp.barberpole.domain.entity.Notice
import com.gastornisapp.barberpole.domain.usecase.ForceUpdateUseCase
import com.gastornisapp.barberpole.domain.usecase.NoticeUseCase
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class HomeViewModel @Inject constructor(
    private val forceUpdateUseCase: ForceUpdateUseCase,
    private val noticeUseCase: NoticeUseCase,
    @param:MainDispatcher
    private val dispatcher: CoroutineDispatcher
) : ViewModel() {

    private val _dialogStatus = MutableStateFlow<DialogStatus>(DialogStatus.None)
    val dialogStatus: StateFlow<DialogStatus> = _dialogStatus

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
