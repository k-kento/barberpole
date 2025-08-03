package com.gastornisapp.barberpole.ui.info

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import javax.inject.Inject

@HiltViewModel
open class InfoViewModel @Inject constructor(
    appSettingsRepository: AppSettingsRepository
) : ViewModel() {

    private val _versionName = mutableStateOf("")
    val versionName: State<String> = _versionName

    init {
        appSettingsRepository.getCurrentAppVersion().onSuccess {
            _versionName.value = it.toString()
        }
    }

}