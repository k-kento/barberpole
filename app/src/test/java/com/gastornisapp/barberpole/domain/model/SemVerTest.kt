package com.gastornisapp.barberpole.domain.model

import junit.framework.TestCase.assertEquals
import junit.framework.TestCase.assertTrue
import org.junit.Assert.assertNull
import org.junit.Test


class SemVerTest {

    @Test
    fun `parse valid version without suffix`() {
        val v = SemVer.parse("1.2.3")
        assertEquals(1, v.major)
        assertEquals(2, v.minor)
        assertEquals(3, v.patch)
        assertNull(v.suffix)
    }

    @Test
    fun `parse valid version with suffix`() {
        val v = SemVer.parse("2.5.10-beta")
        assertEquals(2, v.major)
        assertEquals(5, v.minor)
        assertEquals(10, v.patch)
        assertEquals("-beta", v.suffix)
    }

    @Test
    fun `parse valid version with build metadata`() {
        val v = SemVer.parse("3.4.5+build.123")
        assertEquals(3, v.major)
        assertEquals(4, v.minor)
        assertEquals(5, v.patch)
        assertEquals("+build.123", v.suffix)
    }

    @Test(expected = IllegalArgumentException::class)
    fun `parse invalid version throws exception`() {
        SemVer.parse("invalid_version")
    }

    @Test(expected = IllegalArgumentException::class)
    fun `parse invalid version throws exception2`() {
        SemVer.parse("1.2")
    }

    @Test
    fun `compare ignores suffix`() {
        val v1 = SemVer.parse("1.2.3")
        val v2 = SemVer.parse("1.2.3-beta")
        val v3 = SemVer.parse("1.2.4")

        assertEquals(0, v1.compareTo(v2))
        assertTrue(v1 < v3)
        assertTrue(v3 > v2)
    }

    @Test
    fun `toString includes suffix`() {
        val v1 = SemVer(1, 0, 0)
        val v2 = SemVer(1, 0, 0, "-alpha")

        assertEquals("1.0.0", v1.toString())
        assertEquals("1.0.0-alpha", v2.toString())
    }

    @Test
    fun `compare major version difference`() {
        val v1 = SemVer.parse("1.0.0")
        val v2 = SemVer.parse("2.0.0")
        assertTrue(v1 < v2)
        assertTrue(v2 > v1)
    }

    @Test
    fun `compare minor version difference`() {
        val v1 = SemVer.parse("1.2.0")
        val v2 = SemVer.parse("1.3.0")
        assertTrue(v1 < v2)
        assertTrue(v2 > v1)
    }

    @Test
    fun `compare patch version difference`() {
        val v1 = SemVer.parse("1.2.3")
        val v2 = SemVer.parse("1.2.4")
        assertTrue(v1 < v2)
        assertTrue(v2 > v1)
    }

    @Test
    fun `compare equal versions`() {
        val v1 = SemVer.parse("1.2.3")
        val v2 = SemVer.parse("1.2.3")
        assertEquals(0, v1.compareTo(v2))
    }
}
