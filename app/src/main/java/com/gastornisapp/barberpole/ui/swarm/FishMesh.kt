package ui.swarm

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.swarm.FishShaderProgram
import java.nio.ByteBuffer
import java.nio.ByteOrder

class FishMesh(
    private val program: FishShaderProgram,
    private val vertices: FloatArray,
    private val indices: ShortArray,
) {

    private var vaoId = 0
    private var vboId = 0
    private var iboId = 0

    init {
        createBuffers()
        GLES30.glBindVertexArray(vaoId)
        uploadData()
        setUpVertexAttribPointers()
        GLES30.glBindVertexArray(0)
    }

    private fun createBuffers() {
        val vaoIds = IntArray(1)
        GLES30.glGenVertexArrays(1, vaoIds, 0)
        vaoId = vaoIds[0]

        val vboIds = IntArray(1)
        GLES30.glGenBuffers(1, vboIds, 0)
        vboId = vboIds[0]

        val iboIds = IntArray(1)
        GLES30.glGenBuffers(1, iboIds, 0)
        iboId = iboIds[0]
    }

    private fun uploadData() {
        // VBO
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vboId)
        val vertexBuffer = ByteBuffer.allocateDirect(vertices.size * 4)
            .order(ByteOrder.nativeOrder())
            .asFloatBuffer()
            .apply {
                put(vertices)
                position(0)
            }
        GLES30.glBufferData(
            GLES30.GL_ARRAY_BUFFER,
            vertexBuffer.capacity(),
            vertexBuffer,
            GLES30.GL_STATIC_DRAW
        )

        // IBO
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, iboId)
        val indexBuffer = ByteBuffer.allocateDirect(indices.size * 2)
            .order(ByteOrder.nativeOrder())
            .asShortBuffer()
            .apply {
                put(indices)
                position(0)
            }
        GLES30.glBufferData(
            GLES30.GL_ELEMENT_ARRAY_BUFFER,
            indexBuffer.capacity() * 2,
            indexBuffer,
            GLES30.GL_STATIC_DRAW
        )
    }

    private fun setUpVertexAttribPointers() {
        val posSize = 2
        val stride = posSize * Float.SIZE_BYTES

        // 位置 (x,y)
        GLES30.glEnableVertexAttribArray(program.posHandle)
        GLES30.glVertexAttribPointer(
            program.posHandle,
            posSize,
            GLES30.GL_FLOAT,
            false,
            stride,
            0
        )
    }


    fun draw(mvpMatrix: FloatArray) {
        program.useProgram()

        program.setMvpMatrix(mvpMatrix)

        GLES30.glBindVertexArray(vaoId)
        GLES30.glDrawElements(
            GLES30.GL_TRIANGLES,
            indices.size,
            GLES30.GL_UNSIGNED_SHORT,
            0
        )
        GLES30.glBindVertexArray(0)
    }

    fun release() {
        if (vaoId != 0) {
            GLES30.glDeleteVertexArrays(1, intArrayOf(vaoId), 0)
            vaoId = 0
        }
        if (vboId != 0) {
            GLES30.glDeleteBuffers(1, intArrayOf(vboId), 0)
            vboId = 0
        }
        if (iboId != 0) {
            GLES30.glDeleteBuffers(1, intArrayOf(iboId), 0)
            iboId = 0
        }
    }
}
