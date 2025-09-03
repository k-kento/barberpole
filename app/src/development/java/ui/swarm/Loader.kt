package ui.swarm

import android.content.Context
import com.gastornisapp.barberpole.ui.swarm.FishShaderProgram
import java.io.BufferedReader
import java.io.InputStreamReader
import java.nio.ByteBuffer
import java.nio.ByteOrder


class MeshLoader {

    class VertexData(
        val positions: FloatArray,   // x, y
        val boneIndices: ShortArray, // 4本
        val boneWeights: FloatArray  // 4本
    )

    companion object {

        fun load(program: FishShaderProgram, context: Context): FishMesh {
            val vertexData = loadVertexDataFromCsv(context)
            val vertices = createInterleavedBuffer(vertexData)
            val indices = loadIndicesFromCsv(context)
            return FishMesh(
                program = program,
                vertices = floatArrayOf(),
                indices = indices,
            )
        }

        fun loadBone(context: Context): FloatArray {
            val inverseBindMatrices = loadInverseBindMatricesFromCsv(context)
            val boneOrigins = loadBoneOriginsFromCsv(context)
            return inverseBindMatrices
        }

        // 1. CSV読み込み → 各配列に分けて返す

        private fun loadVertexDataFromCsv(context: Context): VertexData {
            val positions = mutableListOf<Float>()
            val boneIndices = mutableListOf<Short>()
            val boneWeights = mutableListOf<Float>()

            context.assets.open("vertices/fish/fish_vertices.csv").use { inputStream ->
                BufferedReader(InputStreamReader(inputStream)).use { reader ->
                    reader.readLine() // ヘッダー行スキップ
                    reader.lineSequence().forEach { line ->
                        val parts = line.split(",")
                        if (parts.size >= 10) {
                            val x = parts[0].toFloatOrNull()
                            val y = parts[1].toFloatOrNull()
                            val bi = parts.slice(2..5).mapNotNull { it.toShortOrNull() }
                            val bw = parts.slice(6..9).mapNotNull { it.toFloatOrNull() }

                            if (x != null && y != null && bi.size == 4 && bw.size == 4) {
                                positions.add(x)
                                positions.add(y)
                                boneIndices.addAll(bi)
                                boneWeights.addAll(bw)
                            }
                        }
                    }
                }
            }

            return VertexData(
                positions = positions.toFloatArray(),
                boneIndices = boneIndices.toShortArray(),
                boneWeights = boneWeights.toFloatArray()
            )
        }

        // 2. interleaved ByteBuffer 作成
        fun createInterleavedBuffer(vertexData: VertexData): ByteBuffer {
            val positions = vertexData.positions
            val boneIndices = vertexData.boneIndices
            val boneWeights = vertexData.boneWeights

            val vertexCount = positions.size / 2
            val strideBytes = 2 * 4 + 4 * 2 + 4 * 4 // position + boneIndex + boneWeight

            val buffer = ByteBuffer.allocateDirect(vertexCount * strideBytes)
                .order(ByteOrder.nativeOrder())

            for (i in 0 until vertexCount) {
                // position (float x, y)
                buffer.putFloat(positions[i * 2 + 0])
                buffer.putFloat(positions[i * 2 + 1])

                // boneIndex (short[4])
                buffer.putShort(boneIndices[i * 4 + 0])
                buffer.putShort(boneIndices[i * 4 + 1])
                buffer.putShort(boneIndices[i * 4 + 2])
                buffer.putShort(boneIndices[i * 4 + 3])

                // boneWeight (float[4])
                buffer.putFloat(boneWeights[i * 4 + 0])
                buffer.putFloat(boneWeights[i * 4 + 1])
                buffer.putFloat(boneWeights[i * 4 + 2])
                buffer.putFloat(boneWeights[i * 4 + 3])
            }

            buffer.position(0)
            return buffer
        }

        // インデックスCSV読み込み→IntArray化
        private fun loadIndicesFromCsv(context: Context): ShortArray {
            val indices = mutableListOf<Short>()
            context.assets.open("vertices/fish/fish_indices.csv").use { inputStream ->
                BufferedReader(InputStreamReader(inputStream)).use { reader ->
                    reader.readLine() // ヘッダー行をスキップ
                    reader.lineSequence().forEach { line ->
                        val i = line.toShortOrNull()
                        if (i != null) {
                            indices.add(i)
                        }
                    }
                }
            }
            return indices.toShortArray()
        }

        private fun loadInverseBindMatricesFromCsv(context: Context): FloatArray {
            val matrices = mutableListOf<Float>()
            context.assets.open("vertices/fish/inverse_bind_matrices.csv").use { inputStream ->
                BufferedReader(InputStreamReader(inputStream)).use { reader ->
                    reader.readLine() // ヘッダー行スキップ (m0,...,m15)
                    reader.lineSequence().forEach { line ->
                        val values = line.split(",").mapNotNull { it.toFloatOrNull() }
                        if (values.size == 16) {
                            matrices.addAll(values)
                        }
                    }
                }
            }
            return matrices.toFloatArray()
        }

        /**
         * Blender から出力した boneOrigin XZ を読み込む
         * CSV: x,z の2列
         */
        private fun loadBoneOriginsFromCsv(context: Context): FloatArray {
            val origins = mutableListOf<Float>()
            context.assets.open("vertices/fish/bone_origins.csv").use { inputStream ->
                BufferedReader(InputStreamReader(inputStream)).use { reader ->
                    reader.readLine() // ヘッダー行スキップ
                    reader.lineSequence().forEach { line ->
                        val parts = line.split(",").mapNotNull { it.toFloatOrNull() }
                        if (parts.size == 2) {
                            origins.add(parts[0])
                            origins.add(parts[1])
                        }
                    }
                }
            }
            return origins.toFloatArray()
        }
    }
}
