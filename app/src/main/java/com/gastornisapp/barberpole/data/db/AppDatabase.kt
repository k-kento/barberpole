package com.gastornisapp.barberpole.data.db

import androidx.room.Database
import androidx.room.RoomDatabase

@Database(entities = [ReadNotice::class], version = 1, exportSchema = false)
abstract class AppDatabase : RoomDatabase() {
    abstract fun readNoticeDao(): ReadNoticeDao
}