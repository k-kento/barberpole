package ui.swarm

import android.opengl.Matrix

class Bones {
    // 各ボーンのローカルマトリックス（1次元配列）
    private val localMatrices = FloatArray(NUM_BONES * 16)

    // 各ボーンのワールドマトリックス（1次元配列）
    private val worldMatrices = FloatArray(NUM_BONES * 16)

    // バインドポーズマトリックス（初期姿勢）
    private val bindPoseMatrices = FloatArray(NUM_BONES * 16)

    // 最終ボーンマトリックス
    val finalMatrices = FloatArray(NUM_BONES * 16)

    // ボーンの階層構造
    private val parentIndices = IntArray(NUM_BONES) { if (it == 0) -1 else it - 1 }

    fun initializeBones() {
        val tempMatrix = FloatArray(16)
        for (i in 0 until NUM_BONES) {
            val offset = i * 16

            // 単位行列に設定
            Matrix.setIdentityM(bindPoseMatrices, offset)

            // X軸に沿って配置
            // Matrix.translateM(bindPoseMatrices, offset, BONE_LENGTH, 0.0f, 0.0f)

            // 親ボーンがいる場合は乗算
            val parentIndex = parentIndices[i]
            if (parentIndex != -1) {
                val parentOffset = parentIndex * 16
                System.arraycopy(bindPoseMatrices, offset, tempMatrix, 0, 16)
                Matrix.multiplyMM(bindPoseMatrices, offset, bindPoseMatrices, parentOffset, tempMatrix, 0)
            }
        }
    }

    fun updateBoneAnimation(rootBoneAngle: Float) {
        val parentAngles = FloatArray(NUM_BONES)
        parentAngles[0] = rootBoneAngle

        val rotationMatrix = FloatArray(16)
        val inverseBindPose = FloatArray(16)

        for (i in 0 until NUM_BONES) {
            val parentIndex = parentIndices[i]

            val parentAngle = if (parentIndex != -1) parentAngles[parentIndex] else rootBoneAngle
            val angle = parentAngle * 0.7f  // 減衰係数
            parentAngles[i] = angle

            val offset = i * 16
            Matrix.setRotateM(rotationMatrix, 0, angle, 0f, 0f, 1f)

            // ローカルマトリックス = バインドポーズ * 回転
            Matrix.multiplyMM(localMatrices, offset, bindPoseMatrices, offset, rotationMatrix, 0)

            // ワールドマトリックス = 親ワールド * ローカル
            if (parentIndex != -1) {
                val parentOffset = parentIndex * 16
                Matrix.multiplyMM(worldMatrices, offset, worldMatrices, parentOffset, localMatrices, offset)
            } else {
                System.arraycopy(localMatrices, offset, worldMatrices, offset, 16)
            }

            // 最終ボーンマトリックス = ワールド * 逆バインドポーズ
            Matrix.invertM(inverseBindPose, 0, bindPoseMatrices, offset)
            Matrix.multiplyMM(finalMatrices, offset, worldMatrices, offset, inverseBindPose, 0)
        }
    }

    companion object {
        // ボーンの数
        private const val NUM_BONES = 8

        // 波の速度
        private const val WAVE_SPEED = 2.0f

        // 波の振幅
        private const val WAVE_AMPLITUDE = 20.0f

        // ボーンの長さ（魚の体長に応じて調整）
        private const val BONE_LENGTH = 0.2f
    }
}