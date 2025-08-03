package com.gastornisapp.barberpole.ui.home

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import java.util.LinkedList
import java.util.Queue
import javax.inject.Inject

@HiltViewModel
class HomeViewModel @Inject constructor(
    private val appSettingsRepository: AppSettingsRepository
) : ViewModel() {

    private val _isForceUpdateRequired = MutableStateFlow(false)
    val isForceUpdateRequired: StateFlow<Boolean> = _isForceUpdateRequired

    private val _noticeQueue: Queue<Notice> = LinkedList()
    private val _currentNotice = MutableStateFlow<Notice?>(null)
    val currentNotice: StateFlow<Notice?> = _currentNotice

    init {
        val isForceUpdateRequired = isForceUpdateRequired()
        _isForceUpdateRequired.value = isForceUpdateRequired
        if (!isForceUpdateRequired) {
            // 強制アップデートが必要な場合は、何もしない
            viewModelScope.launch {
                loadNotices()
            }
        }
    }

    fun onNoticeDismissed(noticeId: String) {
        _currentNotice.value = null
        _noticeQueue.poll()?.let {
            viewModelScope.launch {
                appSettingsRepository.setNoticeId(it.id)
            }
        }
        _currentNotice.value = _noticeQueue.peek()
    }

    private fun isForceUpdateRequired(): Boolean {
        val requiredVersion = appSettingsRepository.getRequiredAppVersion().getOrNull() ?: return false
        val currentVersion = appSettingsRepository.getCurrentAppVersion().getOrNull() ?: return false
        return currentVersion < requiredVersion
    }

    private suspend fun loadNotices() {
        appSettingsRepository.getNotice().onSuccess { notices ->
            val readStatusMap = mutableMapOf<String, Boolean>()

            for (notice in notices) {
                readStatusMap[notice.id] = appSettingsRepository.isNoticeRead(notice.id).getOrDefault(false)
            }

            val nowSec = System.currentTimeMillis() / 1000
            val filtered = notices.filter {
                it.startAt <= nowSec && nowSec <= it.endAt && readStatusMap[it.id] == false
            }

            _noticeQueue.clear()
            _noticeQueue.addAll(filtered)
            _currentNotice.value = _noticeQueue.peek()
        }
    }
}
