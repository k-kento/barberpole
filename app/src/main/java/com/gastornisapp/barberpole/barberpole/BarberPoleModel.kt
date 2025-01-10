package com.gastornisapp.barberpole.barberpole

import android.opengl.GLES30
import com.gastornisapp.barberpole.ShaderProgram
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.util.ArrayDeque
import java.util.Queue
import kotlin.math.PI
import kotlin.math.sin

class BarberPoleModel(private val shaderProgram: ShaderProgram) {

    private val vertices: FloatArray = FloatArray(RECT_COUNT * VERTEX_COUNT * VERTEX_STRIDE)
    private val indices: ShortArray = ShortArray(RECT_COUNT * INDEX_COUNT)
    private val bufferIds = IntArray(2)

    init {
        val halfWidth = 1f

        var index = 0

        var y = -TOTAL_HEIGHT / 2
        val yGap = sin(PI / 4).toFloat()

        for (i in 0 until RECT_COUNT) {
            val baseIndex = i * VERTEX_COUNT * VERTEX_STRIDE
            // 頂点座標の設定
            // 左上
            vertices[baseIndex] = -halfWidth
            vertices[baseIndex + 1] = y
            // 右上
            vertices[baseIndex + VERTEX_STRIDE] = -halfWidth
            vertices[baseIndex + VERTEX_STRIDE + 1] = RECT_HEIGHT + y
            // 左下
            vertices[baseIndex + 2 * VERTEX_STRIDE] = halfWidth
            vertices[baseIndex + 2 * VERTEX_STRIDE + 1] = y + yGap
            // 右下
            vertices[baseIndex + 3 * VERTEX_STRIDE] = halfWidth
            vertices[baseIndex + 3 * VERTEX_STRIDE + 1] = RECT_HEIGHT + y + yGap

            // インデックスの設定
            val indexValue = i * VERTEX_COUNT
            indices[index++] = (indexValue).toShort()
            indices[index++] = (indexValue + 1).toShort()
            indices[index++] = (indexValue + 2).toShort()
            indices[index++] = (indexValue + 1).toShort()
            indices[index++] = (indexValue + 2).toShort()
            indices[index++] = (indexValue + 3).toShort()

            y += RECT_HEIGHT
        }

        GLES30.glGenBuffers(2, bufferIds, 0)
    }

    fun update(firstColor: FloatArray, secondColor: FloatArray) {

        val whiteColor = floatArrayOf(1.0f, 1.0f, 1.0f, 0.0f)

        val colors: Queue<FloatArray> = ArrayDeque()
        colors.add(firstColor)
        colors.add(whiteColor)
        colors.add(secondColor)
        colors.add(whiteColor)

        var index = 0
        for (i in 0 until RECT_COUNT) {
            val color = colors.poll()!!
            colors.add(color)
            repeat(4) {
                // 頂点座標部分をスキップ
                index += 2
                color.copyInto(vertices, index)
                index += 4
            }
        }

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, bufferIds[0])
        val vertexBuffer = ByteBuffer.allocateDirect(vertices.size * Float.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asFloatBuffer()
            .apply {
                put(vertices)
                position(0)
            }
        GLES30.glBufferData(
            GLES30.GL_ARRAY_BUFFER,
            vertices.size * 4,
            vertexBuffer,
            GLES30.GL_STATIC_DRAW
        )
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0)

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
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, 0)

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, bufferIds[0])
        val positionHandle = GLES30.glGetAttribLocation(shaderProgram.program!!, "vPosition")
        GLES30.glVertexAttribPointer(
            positionHandle,
            Short.SIZE_BYTES,
            GLES30.GL_FLOAT,
            false,
            VERTEX_STRIDE * Float.SIZE_BYTES,
            0,
        )
        GLES30.glEnableVertexAttribArray(positionHandle)

        val colorHandle = GLES30.glGetAttribLocation(shaderProgram.program!!, "aColor")
        GLES30.glVertexAttribPointer(
            colorHandle,
            Float.SIZE_BYTES,
            GLES30.GL_FLOAT,
            false,
            VERTEX_STRIDE * Float.SIZE_BYTES,
            VERTEX_DIMENSION * Float.SIZE_BYTES,
        )
        GLES30.glEnableVertexAttribArray(colorHandle)
    }

    fun draw() {
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, bufferIds[1])
        GLES30.glDrawElements(
            GLES30.GL_TRIANGLES,
            indices.size,
            GLES30.GL_UNSIGNED_SHORT,
            0
        )
    }

    companion object {
        /**
         * 帯の数
         */
        private const val RECT_COUNT = 4 * 4 * 2

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
