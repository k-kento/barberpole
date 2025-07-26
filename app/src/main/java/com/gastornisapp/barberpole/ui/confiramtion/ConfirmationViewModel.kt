package com.gastornisapp.barberpole.ui.confiramtion

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class ConfirmationViewModel @Inject constructor(
    private val repository: AppSettingsRepository
) : ViewModel() {

    private val _isTermsAccepted = MutableStateFlow(false)
    val isTermsAccepted: StateFlow<Boolean> = _isTermsAccepted
    fun acceptTerms() {
        viewModelScope.launch {
            try {
                repository.setTermsOfServiceAccepted()
                repository.setPrivacyPolicyAccepted()
                _isTermsAccepted.value = repository.isTermsOfServiceAccepted() && repository.isPrivacyPolicyAccepted()
            } catch (_: Exception) {

            }
        }
    }
}