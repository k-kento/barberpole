package com.gastornisapp.barberpole.di

import android.content.Context
import com.gastornisapp.myvulkan.VulkanContext
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object VulkanModule {

    @Provides
    @Singleton
    fun provideVulkanContext(@ApplicationContext context: Context): VulkanContext {
        return VulkanContext(context)
    }
}
