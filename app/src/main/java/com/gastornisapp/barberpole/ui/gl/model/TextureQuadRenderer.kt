package com.gastornisapp.barberpole.ui.gl.model

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.gl.shader.TexturedShaderProgram
import com.gastornisapp.barberpole.ui.utils.colorCodeToFloatArray
import java.nio.ByteBuffer
import java.nio.ByteOrder

class TexturedQuadRenderer(
    private val program: TexturedShaderProgram, // 汎用のシェーダープログラム
    private val textureId: Int
) {
    companion object {
        private val VERTICES = floatArrayOf(
            -1f, -1f, 0f, 1f, // 左下
            1f, -1f, 1f, 1f, // 右下
            -1f, 1f, 0f, 0f, // 左上
            1f, 1f, 1f, 0f  // 右上
        )

        private val INDICES = shortArrayOf(
            0, 1, 2,
            2, 1, 3
        )
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
        GLES30.glBindVertexArray(vaoId)

        // 頂点バッファ
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vbo[0])
        val vertexBuffer = ByteBuffer.allocateDirect(VERTICES.size * 4)
            .order(ByteOrder.nativeOrder()).asFloatBuffer().apply {
                put(VERTICES)
                position(0)
            }
        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, VERTICES.size * 4, vertexBuffer, GLES30.GL_STATIC_DRAW)

        // インデックスバッファ
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, ibo[0])
        val indexBuffer = ByteBuffer.allocateDirect(INDICES.size * 2)
            .order(ByteOrder.nativeOrder()).asShortBuffer().apply {
                put(INDICES)
                position(0)
            }
        GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, INDICES.size * 2, indexBuffer, GLES30.GL_STATIC_DRAW)

        // attribute: 位置(x, y)
        GLES30.glEnableVertexAttribArray(program.aPosition)
        GLES30.glVertexAttribPointer(program.aPosition, 2, GLES30.GL_FLOAT, false, 4 * 4, 0)

        // attribute: UV(u, v)
        GLES30.glEnableVertexAttribArray(program.aTexCoord)
        GLES30.glVertexAttribPointer(program.aTexCoord, 2, GLES30.GL_FLOAT, false, 4 * 4, 2 * 4)

        GLES30.glBindVertexArray(0)
    }

    fun draw(mvpMatrix: FloatArray, color: FloatArray) {
        program.useProgram()

        GLES30.glUniformMatrix4fv(program.uMvpMatrixLocation, 1, false, mvpMatrix, 0)
        GLES30.glUniform3fv(program.uColorLocation, 1, color, 0)

        GLES30.glBindVertexArray(vaoId)

        GLES30.glActiveTexture(GLES30.GL_TEXTURE0)
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId)
        GLES30.glUniform1i(program.uTextureLocation, 0)
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, INDICES.size, GLES30.GL_UNSIGNED_SHORT, 0)

        GLES30.glBindVertexArray(0)
    }
}