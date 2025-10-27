package com.gastornisapp.barberpole.ui.page.barberpole

import android.opengl.GLES30
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import kotlin.math.PI
import kotlin.math.sin

class BarberPoleMesh(private val positionLocation: Int, private val colorLocation: Int) {

    private val vertices: FloatArray
    private val indices: ShortArray
    private val bufferIds = IntArray(2)
    private val vaoId = IntArray(1)

    private val vertexBuffer: FloatBuffer


    init {
        vertices = initVertexData()
        indices = initIndexData()

        vertexBuffer = ByteBuffer
            .allocateDirect(vertices.size * Float.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asFloatBuffer()

        GLES30.glGenVertexArrays(1, vaoId, 0)
        GLES30.glGenBuffers(2, bufferIds, 0)
        GLES30.glBindVertexArray(vaoId[0])
        initBuffers()
        initAttributes()
        GLES30.glBindVertexArray(0)
    }

    private fun initVertexData(): FloatArray {
        val result = FloatArray(RECT_COUNT * VERTEX_COUNT * VERTEX_STRIDE)
        var y = -TOTAL_HEIGHT / 2
        val yGap = sin(PI / 4).toFloat()

        for (i in 0 until RECT_COUNT) {
            val base = i * VERTEX_COUNT * VERTEX_STRIDE
            result[base] = -1f
            result[base + 1] = y
            result[base + VERTEX_STRIDE] = -1f
            result[base + VERTEX_STRIDE + 1] = RECT_HEIGHT + y
            result[base + 2 * VERTEX_STRIDE] = 1f
            result[base + 2 * VERTEX_STRIDE + 1] = y + yGap
            result[base + 3 * VERTEX_STRIDE] = 1f
            result[base + 3 * VERTEX_STRIDE + 1] = RECT_HEIGHT + y + yGap
            y += RECT_HEIGHT
        }
        return result
    }

    private fun initIndexData(): ShortArray {
        val result = ShortArray(RECT_COUNT * INDEX_COUNT)
        var idx = 0
        for (i in 0 until RECT_COUNT) {
            val base = i * VERTEX_COUNT
            result[idx++] = base.toShort()
            result[idx++] = (base + 1).toShort()
            result[idx++] = (base + 2).toShort()
            result[idx++] = (base + 1).toShort()
            result[idx++] = (base + 2).toShort()
            result[idx++] = (base + 3).toShort()
        }
        return result
    }

    private fun initBuffers() {
        // 頂点バッファ
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, bufferIds[0])
        vertexBuffer.apply {
            clear()
            put(vertices)
            position(0)
        }
        GLES30.glBufferData(
            GLES30.GL_ARRAY_BUFFER,
            vertices.size * Float.SIZE_BYTES,
            vertexBuffer,
            GLES30.GL_STATIC_DRAW
        )

        // インデックスバッファ
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, bufferIds[1])
        val indexBuffer = ByteBuffer.allocateDirect(indices.size * Short.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asShortBuffer()
            .apply {
                put(indices)
                position(0)
            }
        GLES30.glBufferData(
            GLES30.GL_ELEMENT_ARRAY_BUFFER,
            indices.size * Short.SIZE_BYTES,
            indexBuffer,
            GLES30.GL_STATIC_DRAW
        )
    }

    private fun initAttributes() {
        val strideBytes = VERTEX_STRIDE * Float.SIZE_BYTES

        // 位置属性
        GLES30.glEnableVertexAttribArray(positionLocation)
        GLES30.glVertexAttribPointer(
            positionLocation,
            VERTEX_DIMENSION,
            GLES30.GL_FLOAT,
            false,
            strideBytes,
            0
        )

        // 色属性
        GLES30.glEnableVertexAttribArray(colorLocation)
        GLES30.glVertexAttribPointer(
            colorLocation,
            COLOR_DIMENSION,
            GLES30.GL_FLOAT,
            false,
            strideBytes,
            VERTEX_DIMENSION * Float.SIZE_BYTES
        )
    }

    fun updateColorsBuffer(firstColor: FloatArray, secondColor: FloatArray) {
        val white = floatArrayOf(1f, 1f, 1f, 0f)
        val cycle = arrayOf(firstColor, white, secondColor, white)

        var index = 0
        for (i in 0 until RECT_COUNT) {
            val color = cycle[i % cycle.size]
            repeat(VERTEX_COUNT) {
                index += VERTEX_DIMENSION
                color.copyInto(vertices, index)
                index += COLOR_DIMENSION
            }
        }

        vertexBuffer.apply {
            clear()
            put(vertices)
            position(0)
        }

        // 更新されたカラーをバッファへ送信
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, bufferIds[0])
        GLES30.glBufferSubData(
            GLES30.GL_ARRAY_BUFFER,
            0,
            vertices.size * Float.SIZE_BYTES,
            vertexBuffer,
        )
    }

    fun draw() {
        GLES30.glBindVertexArray(vaoId[0])
        GLES30.glDrawElements(
            GLES30.GL_TRIANGLES,
            indices.size,
            GLES30.GL_UNSIGNED_SHORT,
            0
        )
    }

    fun release() {
        GLES30.glDeleteVertexArrays(1, vaoId, 0)
        GLES30.glDeleteBuffers(bufferIds.size, bufferIds, 0)
    }

    companion object {
        private const val STRIPE_PER_SET = 4
        private const val SETS_PER_BAR = 4
        private const val BAR_COUNT = 2
        private const val RECT_COUNT = STRIPE_PER_SET * SETS_PER_BAR * BAR_COUNT

        /**
         * 帯一つあたりの頂点数
         */
        private const val VERTEX_COUNT = 4

        /**
         * 帯一つにあたりのインデックスの数
         */
        private const val INDEX_COUNT = 6

        private const val VERTEX_DIMENSION = 2
        private const val COLOR_DIMENSION = 4
        private const val VERTEX_STRIDE = VERTEX_DIMENSION + COLOR_DIMENSION

        /**
         * 帯一つにあたりの高さ
         */
        private const val RECT_HEIGHT = 2f / (RECT_COUNT / 4)

        private const val TOTAL_HEIGHT = RECT_COUNT * RECT_HEIGHT
    }
}
