package com.gastornisapp.barberpole.data

import android.content.Context
import com.gastornisapp.barberpole.data.db.ReadNotice
import com.gastornisapp.barberpole.data.db.ReadNoticeDao
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.model.SemVer

class DefaultAppSettingsRepository(
    private val context: Context,
    private val remoteConfigDataSource: RemoteConfigDataSource,
    private val appPreferencesDataSource: AppPreferencesDataSource,
    private val readNoticeDao: ReadNoticeDao,
) : AppSettingsRepository {

    override suspend fun refreshConfig(): Result<Unit> {
        return remoteConfigDataSource.fetchRemoteConfig()
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

    override suspend fun getNotice(): Result<List<Notice>> {
        return remoteConfigDataSource.getNoticeConfig().mapCatching { config ->
            config.notices.map {
                Notice(
                    id = it.id,
                    title = it.title,
                    message = it.message,
                    url = it.url,
                    startAt = it.startAt,
                    endAt = it.endAt
                )
            }
        }
    }

    override suspend fun setNoticeId(noticeId: String): Result<Unit> {
        return runCatching {
            readNoticeDao.insert(ReadNotice(noticeId))
        }
    }

    override suspend fun isNoticeRead(noticeId: String): Result<Boolean> {
        return runCatching {
            readNoticeDao.exists(noticeId)
        }
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

    override fun getCurrentAppVersion(): Result<SemVer> = runCatching {
        val packageInfo = context.packageManager.getPackageInfo(context.packageName, 0)
        val version = packageInfo.versionName ?: throw IllegalStateException("versionName is null")
        SemVer.parse(version)
    }

    override fun getRequiredAppVersion(): Result<SemVer> = runCatching {
        val version = remoteConfigDataSource.getRequiredAppVersion()
        SemVer.parse(version)
    }
}


