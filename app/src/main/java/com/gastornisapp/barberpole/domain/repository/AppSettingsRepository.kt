package com.gastornisapp.barberpole.domain.repository

import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.model.SemVer

interface AppSettingsRepository {
    suspend fun refreshConfig(): Result<Unit>
    fun isForceUpdateRequired(currentVersionCode: Int): Boolean
    fun getForceUpdateTitle(): String
    fun getForceUpdateMessage(): String
    suspend fun getNotice(): Result<List<Notice>>

    suspend fun setNoticeId(noticeId: String): Result<Unit>

    suspend fun isNoticeRead(noticeId: String): Result<Boolean>

    suspend fun isTermsOfServiceAccepted(): Boolean
    suspend fun setTermsOfServiceAccepted()
    suspend fun isPrivacyPolicyAccepted(): Boolean
    suspend fun setPrivacyPolicyAccepted()
    fun getCurrentAppVersion(): Result<SemVer>

    fun getRequiredAppVersion(): Result<SemVer>
}