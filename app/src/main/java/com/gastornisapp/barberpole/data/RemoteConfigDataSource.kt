package com.gastornisapp.barberpole.data

import com.gastornisapp.barberpole.data.db.NoticeConfig
import com.gastornisapp.barberpole.domain.model.SemVer

interface RemoteConfigDataSource {
    suspend fun fetchRemoteConfig(): Result<Unit>

    suspend fun getNoticeConfig(): Result<NoticeConfig>
    fun getRequiredAppVersion(): Result<SemVer>
    fun getLatestTermsOfServiceVersion(): Int
    fun getLatestPrivacyPolicyVersion(): Int
}