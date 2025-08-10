package com.gastornisapp.barberpole.domain.model

data class SemVer(
    val major: Int,
    val minor: Int,
    val patch: Int,
    val suffix: String? = null // 例: "-beta", "+build.123"
) : Comparable<SemVer> {

    override fun compareTo(other: SemVer): Int {
        return compareValuesBy(this, other, { it.major }, { it.minor }, { it.patch })
        // suffix は比較に含めない
    }

    override fun toString(): String {
        return "$major.$minor.$patch" + (suffix ?: "")
    }

    companion object {
        private val regex = Regex("""^(\d+)\.(\d+)\.(\d+)(.*)?$""")

        fun parse(version: String): Result<SemVer> {
            val match = regex.matchEntire(version)
                ?: return Result.failure(IllegalArgumentException("Invalid version format: $version"))
            val (major, minor, patch, suffix) = match.destructured
            val ver = SemVer(
                major.toInt(),
                minor.toInt(),
                patch.toInt(),
                suffix.ifBlank { null }
            )
            return Result.success(ver)
        }
    }
}
