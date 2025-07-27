package com.gastornisapp.barberpole.domain.repository

import com.gastornisapp.barberpole.domain.model.Notice

interface AppSettingsRepository {

    suspend fun refreshConfig(): Boolean
    fun isForceUpdateRequired(currentVersionCode: Int): Boolean
    fun getForceUpdateTitle(): String
    fun getForceUpdateMessage(): String
    fun isNoticeEnabled(): Boolean
    fun getNotice(): Notice?

    suspend fun isTermsOfServiceAccepted(): Boolean
    suspend fun setTermsOfServiceAccepted()

    suspend fun isPrivacyPolicyAccepted(): Boolean
    suspend fun setPrivacyPolicyAccepted()

    fun getVersionName(): String?
}