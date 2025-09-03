package ui.swarm

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.swarm.FishShaderProgram
import java.nio.ByteBuffer
import java.nio.ByteOrder

class FishMeshWithBone(
    private val program: FishShaderProgram,
    private val vertexBuffer: ByteBuffer,
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
        val boneIndexSize = 4
        val boneWeightSize = 4

        val posBytes = posSize * Float.SIZE_BYTES
        val boneIndexBytes = boneIndexSize * Short.SIZE_BYTES
        val boneWeightBytes = boneWeightSize * Float.SIZE_BYTES

        val stride = posBytes + boneIndexBytes + boneWeightBytes

        var offset = 0

        // 位置 (x,y)
        GLES30.glEnableVertexAttribArray(program.posHandle)
        GLES30.glVertexAttribPointer(
            program.posHandle,
            posSize,
            GLES30.GL_FLOAT,
            false,
            stride,
            offset
        )

        offset += posBytes

        // boneIndex
//        GLES30.glEnableVertexAttribArray(program.boneIndexHandle)
//        GLES30.glVertexAttribIPointer(
//            program.boneIndexHandle,
//            boneIndexSize,
//            GLES30.GL_UNSIGNED_SHORT,
//            stride,
//            offset
//        )
//
//        offset += boneIndexBytes
//
//        // boneWeight
//        GLES30.glEnableVertexAttribArray(program.boneWeightHandle)
//        GLES30.glVertexAttribPointer(
//            program.boneWeightHandle,
//            boneWeightSize,
//            GLES30.GL_FLOAT,
//            false,
//            stride,
//            offset
//        )
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