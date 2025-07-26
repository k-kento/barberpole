package com.gastornisapp.barberpole.data

import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.intPreferencesKey
import kotlinx.coroutines.flow.first

class DefaultAppPreferencesDataSource(private val dataStore: DataStore<Preferences>) : AppPreferencesDataSource {

    override suspend fun getTermsOfServiceAcceptedVersion(): Int? {
        return dataStore.data.first()[KEY_TOS_VERSION]
    }

    override suspend fun setTermsOfServiceAcceptedVersion(version: Int) {
        dataStore.edit { it[KEY_TOS_VERSION] = version }
    }

    override suspend fun getPrivacyPolicyAcceptedVersion(): Int? {
        return dataStore.data.first()[KEY_PRIVACY_POLICY_VERSION]
    }

    override suspend fun setPrivacyPolicyAcceptedVersion(version: Int) {
        dataStore.edit { it[KEY_PRIVACY_POLICY_VERSION] = version }
    }

    companion object {
        private val KEY_TOS_VERSION = intPreferencesKey("app_settings.terms_of_service_version")
        private val KEY_PRIVACY_POLICY_VERSION = intPreferencesKey("app_settings.privacy_policy_version")
    }
}