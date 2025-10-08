package com.gastornisapp.barberpole.ui

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.domain.usecase.InitializeAppUseCase
import com.gastornisapp.barberpole.domain.usecase.UserAgreementUseCase
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class AppStartupViewModel @Inject constructor(
    private val userAgreementUseCase: UserAgreementUseCase,
    private val initializeAppUseCase: InitializeAppUseCase
) : ViewModel() {

    private val _uiStatus = MutableStateFlow<AppStartupUiStatus>(AppStartupUiStatus.ShowingSplash)
    val uiStatus: StateFlow<AppStartupUiStatus> = _uiStatus

    // ・スプラッシュ表示中にアプリの初期化処理を実行する。
    // ・初期化処理の実行開始から1秒以上経過した場合は、UIを進行中表示に切り替える。
    fun initialize() {
        viewModelScope.launch {
            // 1秒後に進行中表示へ切り替える遅延処理を開始
            val progressJob = launch {
                delay(1000)
                _uiStatus.value = AppStartupUiStatus.ShowingProgress
            }

            initializeAppUseCase.initializeApp()

            // 初期化が1秒未満なら進行中表示をキャンセル
            if (progressJob.isActive) progressJob.cancel()

            if (userAgreementUseCase.areUserAgreementsAccepted()) {
                _uiStatus.value = AppStartupUiStatus.Completed(PageType.Confirmation)
            } else {
                _uiStatus.value = AppStartupUiStatus.Completed(PageType.Home)
            }
        }
    }
}

sealed class AppStartupUiStatus {
    object ShowingSplash : AppStartupUiStatus()
    object ShowingProgress : AppStartupUiStatus()
    data class Completed(val startDestination: PageType) : AppStartupUiStatus()
}