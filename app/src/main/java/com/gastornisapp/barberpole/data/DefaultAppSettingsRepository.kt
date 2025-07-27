package com.gastornisapp.barberpole.data

import android.content.Context
import android.content.pm.PackageManager
import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository

class DefaultAppSettingsRepository(
    private val context: Context,
    private val remoteConfigDataSource: RemoteConfigDataSource,
    private val appPreferencesDataSource: AppPreferencesDataSource
) : AppSettingsRepository {

    override suspend fun refreshConfig(): Boolean {
        TODO("Not yet implemented")
    }

    override fun isForceUpdateRequired(currentVersionCode: Int): Boolean {
        TODO("Not yet implemented")
    }

    override fun getForceUpdateTitle(): String {
        TODO("Not yet implemented")
    }

    override fun getForceUpdateMessage(): String {
        TODO("Not yet implemented")
    }

    override fun isNoticeEnabled(): Boolean {
        TODO("Not yet implemented")
    }

    override fun getNotice(): Notice? {
        TODO("Not yet implemented")
    }

    override suspend fun isTermsOfServiceAccepted(): Boolean {
        val latestVersion = remoteConfigDataSource.getLatestTermsOfServiceVersion()
        val acceptedVersion = appPreferencesDataSource.getTermsOfServiceAcceptedVersion() ?: -1
        return latestVersion <= acceptedVersion
    }

    override suspend fun setTermsOfServiceAccepted() {
        val latestVersion = remoteConfigDataSource.getLatestTermsOfServiceVersion()
        appPreferencesDataSource.setTermsOfServiceAcceptedVersion(latestVersion)
    }

    override suspend fun isPrivacyPolicyAccepted(): Boolean {
        val latestVersion = remoteConfigDataSource.getLatestPrivacyPolicyVersion()
        val acceptedVersion = appPreferencesDataSource.getPrivacyPolicyAcceptedVersion() ?: -1
        return latestVersion <= acceptedVersion
    }

    override suspend fun setPrivacyPolicyAccepted() {
        val latestVersion = remoteConfigDataSource.getLatestPrivacyPolicyVersion()
        appPreferencesDataSource.setPrivacyPolicyAcceptedVersion(latestVersion)
    }

    override fun getVersionName(): String? {
        return try {
            val packageInfo = context.packageManager.getPackageInfo(context.packageName, 0)
            packageInfo.versionName
        } catch (e: PackageManager.NameNotFoundException) {
            null
        }
    }
}


