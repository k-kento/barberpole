package com.gastornisapp.barberpole.di

import com.gastornisapp.barberpole.domain.port.SoundPlayer
import com.gastornisapp.barberpole.infrastructure.audio.SoundPoolPlayer
import dagger.Binds
import dagger.Module
import dagger.hilt.InstallIn
import dagger.hilt.android.components.ViewModelComponent
import dagger.hilt.android.scopes.ViewModelScoped

@Module
@InstallIn(ViewModelComponent::class)
abstract class InfrastructureModule {

    @Binds
    @ViewModelScoped
    abstract fun bindSoundPlayer(soundPlayer: SoundPoolPlayer): SoundPlayer
}
