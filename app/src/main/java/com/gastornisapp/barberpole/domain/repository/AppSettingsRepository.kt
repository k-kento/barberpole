package com.gastornisapp.barberpole.domain.repository

import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.model.SemVer

interface AppSettingsRepository {
    suspend fun refreshConfig(): Result<Unit>

    // region notice

    suspend fun getNotice(): Result<List<Notice>>

    suspend fun setNoticeId(noticeId: String): Result<Unit>

    suspend fun isNoticeRead(noticeId: String): Result<Boolean>

    // end region notice

    // region user agreement

    suspend fun getLatestTermsOfServiceVersion(): Int

    suspend fun getTermsOfServiceAcceptedVersion(): Int?

    suspend fun setTermsOfServiceAcceptedVersion(version: Int)

    suspend fun getLatestPrivacyPolicyVersion(): Int

    suspend fun getPrivacyPolicyAcceptedVersion(): Int?

    suspend fun setPrivacyPolicyAcceptedVersion(version: Int)

    // end region user agreement

    fun getCurrentAppVersion(): Result<SemVer> // TODO infra に移動

    fun getRequiredAppVersion(): Result<SemVer>
}