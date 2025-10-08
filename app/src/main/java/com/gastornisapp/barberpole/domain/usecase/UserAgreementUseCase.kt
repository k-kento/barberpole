package com.gastornisapp.barberpole.domain.usecase

import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import kotlinx.coroutines.async
import kotlinx.coroutines.coroutineScope
import javax.inject.Inject

class UserAgreementUseCase @Inject constructor(
    private val repository: AppSettingsRepository
) {
    suspend fun areUserAgreementsAccepted(): Boolean = coroutineScope {
        val termsAccepted = async { isTermsOfServiceAccepted() }
        val privacyAccepted = async { isPrivacyPolicyAccepted() }
        termsAccepted.await() && privacyAccepted.await()
    }

    suspend fun acceptUserAgreements() = coroutineScope {
        val termsVersionDeferred = async { repository.getLatestTermsOfServiceVersion() }
        val privacyVersionDeferred = async { repository.getLatestPrivacyPolicyVersion() }

        val termsVersion = termsVersionDeferred.await()
        val privacyVersion = privacyVersionDeferred.await()

        repository.setTermsOfServiceAcceptedVersion(termsVersion)
        repository.setPrivacyPolicyAcceptedVersion(privacyVersion)
    }

    suspend fun isTermsOfServiceAccepted(): Boolean {
        val latestVersion = repository.getLatestTermsOfServiceVersion()
        val acceptedVersion = repository.getTermsOfServiceAcceptedVersion() ?: -1
        return latestVersion <= acceptedVersion
    }

    suspend fun isPrivacyPolicyAccepted(): Boolean {
        val latestVersion = repository.getLatestPrivacyPolicyVersion()
        val acceptedVersion = repository.getPrivacyPolicyAcceptedVersion() ?: -1
        return latestVersion <= acceptedVersion
    }
}