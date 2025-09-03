package ui.swarm

import android.content.Context
import java.io.BufferedReader
import java.io.InputStreamReader

class FishMeshLoader {

    class VertexData(
        val vertices: FloatArray,
        val indices: ShortArray,
    )

    companion object {

        fun load(context: Context): VertexData {
            val vertices = loadVertexDataFromCsv(context)
            val indices = loadIndicesFromCsv(context)
            return VertexData(
                vertices = vertices,
                indices = indices,
            )
        }

        // CSV読み込み → 各配列に分けて返す
        private fun loadVertexDataFromCsv(context: Context): FloatArray {
            val positions = mutableListOf<Float>()

            context.assets.open("vertices/fish/fish_vertices.csv").use { inputStream ->
                BufferedReader(InputStreamReader(inputStream)).use { reader ->
                    reader.readLine() // ヘッダー行スキップ
                    reader.lineSequence().forEach { line ->
                        val parts = line.split(",")
                        if (parts.size >= 10) {
                            val x = parts[0].toFloatOrNull()
                            val y = parts[1].toFloatOrNull()
                            if (x != null && y != null) {
                                positions.add(x)
                                positions.add(y)
                            }
                        }
                    }
                }
            }

            return positions.toFloatArray()
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
    }
}
