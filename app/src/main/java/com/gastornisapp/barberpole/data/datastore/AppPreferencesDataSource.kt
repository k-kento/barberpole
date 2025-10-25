package com.gastornisapp.barberpole.data.datastore

import kotlinx.coroutines.flow.Flow

interface AppPreferencesDataSource {

    suspend fun getTermsOfServiceAcceptedVersion(): Int?

    suspend fun setTermsOfServiceAcceptedVersion(version: Int)

    suspend fun getPrivacyPolicyAcceptedVersion(): Int?
    suspend fun setPrivacyPolicyAcceptedVersion(version: Int)

    fun isLockEnabled(): Flow<Boolean>

    suspend fun setLockEnabled(enabled: Boolean)
}
