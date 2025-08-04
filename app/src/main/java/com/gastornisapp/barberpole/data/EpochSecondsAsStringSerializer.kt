package com.gastornisapp.barberpole.data

import kotlinx.serialization.KSerializer
import kotlinx.serialization.descriptors.PrimitiveKind
import kotlinx.serialization.descriptors.PrimitiveSerialDescriptor
import kotlinx.serialization.descriptors.SerialDescriptor
import kotlinx.serialization.encoding.Decoder
import kotlinx.serialization.encoding.Encoder
import java.time.Instant

object EpochSecondsAsStringSerializer : KSerializer<Long> {
    override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("EpochMillisAsString", PrimitiveKind.STRING)

    override fun serialize(encoder: Encoder, value: Long) {
        // Long（秒）→ ISO8601文字列に変換してシリアライズ
        val isoString = Instant.ofEpochSecond(value).toString()
        encoder.encodeString(isoString)
    }

    override fun deserialize(decoder: Decoder): Long {
        // ISO8601文字列 → Long（秒）に変換してデシリアライズ
        val string = decoder.decodeString()
        return Instant.parse(string).epochSecond
    }
}