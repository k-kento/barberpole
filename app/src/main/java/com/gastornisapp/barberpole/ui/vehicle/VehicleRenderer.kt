package com.gastornisapp.barberpole.ui.vehicle

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.GLUtils
import android.opengl.Matrix
import com.gastornisapp.barberpole.R
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.math.abs

class VehicleRenderer(private val context: Context) : GLSurfaceView.Renderer {

    private val vehicleManager: VehicleManager = VehicleManager()

    private val scaleMatrix = FloatArray(16)
    private val modelMatrix = FloatArray(16)

    private var lastFrameTime : Long = 0L // 前回のフレーム時間
    private var textureId: Int = 0

    private lateinit var program: VehicleShaderProgram

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

        GLES30.glClearColor(1f, 1f, 1f, 1.0f)

        // アルファブレンディングを有効にする
        GLES30.glEnable(GLES30.GL_BLEND)
        // 出力色 = ソース色 × srcFactor + 背景色 × dstFactor
        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA)

        // シェーダーをコンパイルし、使用する
        program = VehicleShaderProgram()
        program.initialize()

        vehicleManager.initialize(program)

        lastFrameTime = System.currentTimeMillis()

        textureId = loadTexture(context = context, R.drawable.ic_android_black_24dp)

        val scale = 0.1f
        Matrix.setIdentityM(scaleMatrix, 0)
        Matrix.scaleM(scaleMatrix, 0, scale, scale, 1f)
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        val currentTime = System.currentTimeMillis()

        // 初回 onDrawFrame はスキップ
        if (lastFrameTime == 0L) {
            lastFrameTime = currentTime
            return
        }

        val deltaFrameTime = currentTime - lastFrameTime
        lastFrameTime = currentTime

        for (vehicle in vehicleManager.iterator()) {
            vehicle.posX += vehicle.velocity * deltaFrameTime
            if (1.0f < abs(vehicle.posX)) {
                vehicle.posX = -1f
            }

            Matrix.setIdentityM(modelMatrix, 0)
            Matrix.translateM(modelMatrix, 0, modelMatrix, 0, vehicle.posX, 0f, 0f)
            Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0)
            GLES30.glUniformMatrix4fv(program.uModelLocation, 1, false, modelMatrix, 0)
            vehicle.model.draw(textureId)
        }

        lastFrameTime = currentTime
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)
    }

    private fun loadTexture(context: Context, resourceId: Int): Int {
        // 1. テクスチャIDを生成
        val textureIds = IntArray(1)
        GLES30.glGenTextures(1, textureIds, 0)

        if (textureIds[0] == 0) {
            throw RuntimeException("Failed to generate texture ID")
        }

        // 2. Bitmapを読み込む
        val options = BitmapFactory.Options().apply {
            inPreferredConfig = Bitmap.Config.ARGB_8888
        }
        val bitmap = BitmapFactory.decodeResource(context.resources, R.drawable.bus, options)

        // 3. テクスチャをバインド
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureIds[0])

        // 4. テクスチャのパラメータ設定
        // 線形補間で画像を拡大縮小する
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR)
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR)
        // 境界外の UV に対して端の色を使う
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE)
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE)

        // 5. BitmapをOpenGLのテクスチャにアップロード
        GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0)

        // 6. Bitmapを解放
        bitmap?.recycle()

        // 7. バインド解除
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0)

        return textureIds[0]
    }
}
