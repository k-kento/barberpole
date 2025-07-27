package com.gastornisapp.barberpole.ui

import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository

open class BaseFakeAppSettingsRepository : AppSettingsRepository {
    override suspend fun refreshConfig(): Boolean = false
    override fun isForceUpdateRequired(currentVersionCode: Int): Boolean = false
    override fun getForceUpdateTitle(): String = ""
    override fun getForceUpdateMessage(): String = ""
    override fun isNoticeEnabled(): Boolean = false
    override fun getNotice(): Notice? = null
    override suspend fun isTermsOfServiceAccepted(): Boolean = true
    override suspend fun setTermsOfServiceAccepted() {}
    override suspend fun isPrivacyPolicyAccepted(): Boolean = true
    override suspend fun setPrivacyPolicyAccepted() {}
    override fun getVersionName() = ""
}
