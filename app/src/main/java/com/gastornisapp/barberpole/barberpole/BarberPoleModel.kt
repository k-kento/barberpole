package com.gastornisapp.barberpole.barberpole

import android.opengl.GLES20
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

        var y = -2.6f
        val yGap = sin(PI / 4).toFloat()

        for (i in 0 until RECT_COUNT) {
            val baseIndex = i * VERTEX_COUNT * VERTEX_STRIDE
            // 左上
            vertices[baseIndex] = -halfWidth
            vertices[baseIndex + 1] = y
            // 右上
            vertices[baseIndex + VERTEX_STRIDE] = -halfWidth
            vertices[baseIndex + VERTEX_STRIDE + 1] = HEIGHT + y
            // 左下
            vertices[baseIndex + 2 * VERTEX_STRIDE] = halfWidth
            vertices[baseIndex + 2 * VERTEX_STRIDE + 1] = y + yGap
            // 右下
            vertices[baseIndex + 3 * VERTEX_STRIDE] = halfWidth
            vertices[baseIndex + 3 * VERTEX_STRIDE + 1] = HEIGHT + y + yGap

            val indexValue = i * VERTEX_COUNT
            indices[index++] = (indexValue).toShort()
            indices[index++] = (indexValue + 1).toShort()
            indices[index++] = (indexValue + 2).toShort()
            indices[index++] = (indexValue + 1).toShort()
            indices[index++] = (indexValue + 2).toShort()
            indices[index++] = (indexValue + 3).toShort()

            y += HEIGHT
        }

        GLES20.glGenBuffers(2, bufferIds, 0)

        update(floatArrayOf(1.0f, 1.0f, 0.0f, 0.0f), floatArrayOf(1.0f, 0.0f, 1.0f, 0.0f))
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

        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, bufferIds[0])
        val vertexBuffer = ByteBuffer.allocateDirect(vertices.size * Float.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asFloatBuffer()
            .apply {
                put(vertices)
                position(0)
            }
        GLES20.glBufferData(
            GLES20.GL_ARRAY_BUFFER,
            vertices.size * 4,
            vertexBuffer,
            GLES20.GL_STATIC_DRAW
        )
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0)

        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, bufferIds[1])
        val indexBuffer = ByteBuffer.allocateDirect(indices.size * Short.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asShortBuffer()
            .apply {
                put(indices)
                position(0)
            }
        GLES20.glBufferData(
            GLES20.GL_ELEMENT_ARRAY_BUFFER,
            indices.size * Short.SIZE_BYTES,
            indexBuffer,
            GLES20.GL_STATIC_DRAW
        )
        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0)

        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, bufferIds[0])
        val positionHandle = GLES20.glGetAttribLocation(shaderProgram.program!!, "vPosition")
        GLES20.glVertexAttribPointer(
            positionHandle,
            Short.SIZE_BYTES,
            GLES20.GL_FLOAT,
            false,
            VERTEX_STRIDE * Float.SIZE_BYTES,
            0,
        )
        GLES20.glEnableVertexAttribArray(positionHandle)

        val colorHandle = GLES20.glGetAttribLocation(shaderProgram.program!!, "aColor")
        GLES20.glVertexAttribPointer(
            colorHandle,
            Float.SIZE_BYTES,
            GLES20.GL_FLOAT,
            false,
            VERTEX_STRIDE * Float.SIZE_BYTES,
            VERTEX_DIMENSION * Float.SIZE_BYTES,
        )
        GLES20.glEnableVertexAttribArray(colorHandle)
    }

    fun draw() {
        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, bufferIds[1])
        GLES20.glDrawElements(
            GLES20.GL_TRIANGLES,
            indices.size,
            GLES20.GL_UNSIGNED_SHORT,
            0
        )
    }

    companion object {
        /**
         * 帯の数
         */
        private const val RECT_COUNT = 6

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
        private const val HEIGHT = 2f / RECT_COUNT
    }
}
