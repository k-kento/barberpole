package com.gastornisapp.barberpole.usecase

import com.gastornisapp.barberpole.domain.entity.SemVer
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.barberpole.domain.usecase.ForceUpdateUseCase
import io.mockk.every
import io.mockk.mockk
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.params.ParameterizedTest
import org.junit.jupiter.params.provider.MethodSource

class ForceUpdateUseCaseTest {

    companion object {
        val ERROR = Result.failure<SemVer>(Exception("error"))
        val SEMVER_SMALL = Result.success(SemVer(1, 0, 0))
        val SEMVER_LARGE = Result.success(SemVer(2, 0, 0))

        data class TestCase(
            val requiredVersion: Result<SemVer>,
            val currentVersion: Result<SemVer>,
            val expected: Boolean
        )

        @JvmStatic
        fun testCases() = listOf(
            TestCase(ERROR, ERROR, false),
            TestCase(SEMVER_SMALL, ERROR, false),
            TestCase(SEMVER_SMALL, SEMVER_SMALL, false),
            TestCase(SEMVER_SMALL, SEMVER_LARGE, false),
            TestCase(SEMVER_LARGE, ERROR, false),
            TestCase(SEMVER_LARGE, SEMVER_SMALL, true),
            TestCase(SEMVER_LARGE, SEMVER_LARGE, false)
        )
    }

    private val repository = mockk<AppSettingsRepository>()
    private val useCase = ForceUpdateUseCase(repository)

    @ParameterizedTest
    @MethodSource("testCases")
    fun testIsForceUpdateRequired(testCase: TestCase) {
        every { repository.getRequiredAppVersion() } returns testCase.requiredVersion
        every { repository.getCurrentAppVersion() } returns testCase.currentVersion

        val actual = useCase.isForceUpdateRequired()
        assertEquals(testCase.expected, actual)
    }
}
