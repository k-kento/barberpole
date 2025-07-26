package com.gastornisapp.barberpole.data

interface RemoteConfigDataSource {
    suspend fun fetchRemoteConfig(): Result<Unit>

    fun getAnnouncement(): String
    fun isForceUpdateRequired(): Boolean
    fun getLatestTermsOfServiceVersion(): Int
    fun getLatestPrivacyPolicyVersion(): Int
}