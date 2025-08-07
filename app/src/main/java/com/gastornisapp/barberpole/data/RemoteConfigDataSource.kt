package com.gastornisapp.barberpole.data

import com.gastornisapp.barberpole.data.db.NoticeConfig

interface RemoteConfigDataSource {
    suspend fun fetchRemoteConfig(): Result<Unit>

    suspend fun getNoticeConfig(): Result<NoticeConfig>
    fun getRequiredAppVersion(): String
    fun getLatestTermsOfServiceVersion(): Int
    fun getLatestPrivacyPolicyVersion(): Int
}