package com.gastornisapp.barberpole.ui.vehicle

import android.opengl.GLES30
import android.opengl.Matrix
import java.nio.ByteBuffer
import java.nio.ByteOrder

class VehicleRendererModel(
    // 座標: x, y, u, v
    private val program: VehicleShaderProgram,
    private val textureId: Int,
) {

    companion object {

        const val WIDTH: Float = 2f
        const val HEIGHT: Float = 2f

        // 座標: x, y, u, v
        private val vertices = floatArrayOf(
            // 左下
            -1f, -1f, 0f, 1f,
            // 右下
            1f, -1f, 1f, 1f,
            // 左上
            -1f, 1f, 0f, 0f,
            // 右上
            1f, 1f, 1f, 0f
        )

        private val indices = shortArrayOf(
            0, 1, 2,
            2, 1, 3
        )

        private val modelMatrix = FloatArray(16)
    }

    private val vaoId: Int

    init {
        val vao = IntArray(1)
        val vbo = IntArray(1)
        val ibo = IntArray(1)

        GLES30.glGenVertexArrays(1, vao, 0)
        GLES30.glGenBuffers(1, vbo, 0)
        GLES30.glGenBuffers(1, ibo, 0)

        vaoId = vao[0]

        // VAOバインド
        GLES30.glBindVertexArray(vaoId)

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vbo[0])
        val vertexBuffer = ByteBuffer.allocateDirect(vertices.size * Int.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asFloatBuffer()
            .apply {
                put(vertices)
                position(0)
            }
        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, vertices.size * Int.SIZE_BYTES, vertexBuffer, GLES30.GL_STATIC_DRAW)

        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, ibo[0])
        val indexBuffer = ByteBuffer.allocateDirect(indices.size * Short.SIZE_BYTES)
            .order(ByteOrder.nativeOrder())
            .asShortBuffer()
            .apply {
                put(indices)
                position(0)
            }
        GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, indices.size * Short.SIZE_BYTES, indexBuffer, GLES30.GL_STATIC_DRAW)

        // 属性0: 位置 (vec2)
        GLES30.glEnableVertexAttribArray(program.aPosition)
        GLES30.glVertexAttribPointer(program.aPosition, 2, GLES30.GL_FLOAT, false, 4 * Int.SIZE_BYTES, 0)

        // 属性1: UV (vec2)
        GLES30.glEnableVertexAttribArray(program.aTexCoord)
        GLES30.glVertexAttribPointer(program.aTexCoord, 2, GLES30.GL_FLOAT, false, 4 * Int.SIZE_BYTES, 2 * 4)
    }

    fun draw(color: FloatArray) {
        GLES30.glBindVertexArray(vaoId)

        // テクスチャバインド
        GLES30.glActiveTexture(GLES30.GL_TEXTURE0)
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId)
        GLES30.glUniform1i(program.uTextureLocation, 0)

        // 色を変更
        GLES30.glUniform3fv(program.uColorLocation, 1, color, 0)

        // 描画
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, indices.size, GLES30.GL_UNSIGNED_SHORT, 0)
    }

    fun updateModelMatrix(posX: Float, posY: Float, orientation: Int, scale: Float) {
        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, posX, posY, 0f)
        Matrix.scaleM(modelMatrix, 0, orientation * scale, scale, 1f)
        GLES30.glUniformMatrix4fv(program.uModelMatrixLocation, 1, false, modelMatrix, 0)
    }
}
