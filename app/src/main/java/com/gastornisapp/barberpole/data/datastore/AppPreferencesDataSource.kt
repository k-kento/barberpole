package com.gastornisapp.barberpole.data.datastore

interface AppPreferencesDataSource {

    suspend fun getTermsOfServiceAcceptedVersion(): Int?

    suspend fun setTermsOfServiceAcceptedVersion(version: Int)

    suspend fun getPrivacyPolicyAcceptedVersion(): Int?
    suspend fun setPrivacyPolicyAcceptedVersion(version: Int)
}