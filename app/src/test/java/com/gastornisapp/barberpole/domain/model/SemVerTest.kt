package com.gastornisapp.barberpole.domain.model

import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Assertions.assertNull
import org.junit.jupiter.api.Assertions.assertThrows
import org.junit.jupiter.api.DisplayName
import org.junit.jupiter.api.Nested
import org.junit.jupiter.api.Test
import org.junit.jupiter.params.ParameterizedTest
import org.junit.jupiter.params.provider.CsvSource
import kotlin.test.assertTrue

@DisplayName("SemVer Unit Tests")
class SemVerTest {

    @Nested
    @DisplayName("Parse tests")
    inner class ParseTests {

        @Test
        fun parse_version_without_suffix() {
            val v = SemVer.parse("1.2.3")
            assertEquals(1, v.major)
            assertEquals(2, v.minor)
            assertEquals(3, v.patch)
            assertNull(v.suffix)
        }

        @Test
        fun parse_version_with_suffix() {
            val v = SemVer.parse("2.5.10-beta")
            assertEquals(2, v.major)
            assertEquals(5, v.minor)
            assertEquals(10, v.patch)
            assertEquals("-beta", v.suffix)
        }

        @Test
        fun parse_version_with_build_metadata() {
            val v = SemVer.parse("3.4.5+build.123")
            assertEquals(3, v.major)
            assertEquals(4, v.minor)
            assertEquals(5, v.patch)
            assertEquals("+build.123", v.suffix)
        }

        @Test
        fun parse_invalid_format_throws_exception() {
            assertThrows(IllegalArgumentException::class.java) {
                SemVer.parse("invalid_version")
            }
            assertThrows(IllegalArgumentException::class.java) {
                SemVer.parse("1.2")
            }
        }

        @Test
        fun parse_and_toString_should_roundtrip() {
            val versionStr = "1.2.3-alpha+build.42"
            val parsed = SemVer.parse(versionStr)
            val result = parsed.toString()
            assertEquals(versionStr, result)
        }
    }

    @Nested
    @DisplayName("Comparison tests")
    inner class CompareTests {

        @Test
        fun suffix_should_not_affect_comparison() {
            val v1 = SemVer.parse("1.2.3")
            val v2 = SemVer.parse("1.2.3-beta")
            val v3 = SemVer.parse("1.2.4")

            assertEquals(0, v1.compareTo(v2))
            assertTrue(v1 < v3)
            assertTrue(v3 > v2)
        }

        @ParameterizedTest(name = "{0} < {1}")
        @CsvSource(
            "1.0.0, 2.0.0",
            "1.2.0, 1.3.0",
            "1.2.3, 1.2.4"
        )
        fun compare_versions_ordering_is_correct(ver1: String, ver2: String) {
            val v1 = SemVer.parse(ver1)
            val v2 = SemVer.parse(ver2)
            assertTrue(v1 < v2)
            assertTrue(v2 > v1)
        }

        @Test
        fun equal_versions_should_be_equal() {
            val v1 = SemVer.parse("1.2.3")
            val v2 = SemVer.parse("1.2.3")
            assertEquals(0, v1.compareTo(v2))
        }
    }

    @Nested
    @DisplayName("ToString tests")
    inner class ToStringTests {

        @Test
        fun toString_without_suffix() {
            val v = SemVer(1, 0, 0)
            assertEquals("1.0.0", v.toString())
        }

        @Test
        fun toString_with_suffix() {
            val v = SemVer(1, 0, 0, "-alpha")
            assertEquals("1.0.0-alpha", v.toString())
        }
    }
}
