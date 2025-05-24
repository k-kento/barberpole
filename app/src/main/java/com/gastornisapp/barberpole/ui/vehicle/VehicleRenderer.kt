package com.gastornisapp.barberpole.ui.vehicle

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.R
import com.gastornisapp.barberpole.ui.ScreenInfo
import com.gastornisapp.barberpole.ui.loadTexture
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class VehicleRenderer(private val context: Context) : GLSurfaceView.Renderer {

    private val vehicleManager: VehicleManager = VehicleManager()
    private lateinit var vehicleModel: VehicleModel

    private lateinit var screenInfo: ScreenInfo
    private val projectionMatrix = FloatArray(16)

    private var lastFrameTime: Long = 0L // 前回のフレーム時間
    private var carTextureId: Int = 0
    private var busTextureId: Int = 0

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

        vehicleModel = VehicleModel(program)

        lastFrameTime = System.currentTimeMillis()

        carTextureId = loadTexture(context = context, R.drawable.car)
        busTextureId = loadTexture(context = context, R.drawable.bus)
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

        vehicleManager.update(deltaTime = deltaFrameTime)

        for (vehicle in vehicleManager.iterator()) {
            vehicle.updateModelMatrix(screenInfo)
            GLES30.glUniformMatrix4fv(program.uModelMatrixLocation, 1, false, vehicle.modelMatrix, 0)
            vehicleModel.draw(color = vehicle.color, textureId = if (vehicle.vehicleType == Vehicle.VehicleType.Car) carTextureId else busTextureId)
        }

        lastFrameTime = currentTime
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)

        val aspect = width.toFloat() / height

        if (width > height) {
            // 横画面
            Matrix.orthoM(projectionMatrix, 0, -aspect, aspect, -1f, 1f, -1f, 1f)
            screenInfo = ScreenInfo(-aspect, aspect, -1.0f, 1.0f)
        } else {
            // 縦画面
            Matrix.orthoM(projectionMatrix, 0, -1f, 1f, -1f / aspect, 1f / aspect, -1f, 1f)
            screenInfo = ScreenInfo(-1.0f, 1.0f, -1f / aspect, 1f / aspect)
        }
        GLES30.glUniformMatrix4fv(program.uProjectionMatrixLocation, 1, false, projectionMatrix, 0)
    }

    fun handleTouchDown(screenX: Float, screenY: Float, screenWidth: Int, screenHeight: Int) {
        // モデル座標系（-1〜1）に変換
        val touchX = (screenX / screenWidth) * 2f - 1f
        val touchY = -((screenY / screenHeight) * 2f - 1f)

        for (vehicle in vehicleManager.iterator()) {
            // 車の位置とサイズ（中心座標と半サイズ）で矩形当たり判定
            val halfWidth = Vehicle.VEHICLE_HEIGHT / 2f
            val halfHeight = Vehicle.VEHICLE_WIDTH / 2f

            val left = vehicle.posX - halfWidth
            val right = vehicle.posX + halfWidth
            val top = vehicle.posY + halfHeight
            val bottom = vehicle.posY - halfHeight

            if (touchX in left..right && touchY in bottom..top) {
                vehicle.pressed = true
                break
            }
        }
    }

    fun handleTouchUp() {
        for (vehicle in vehicleManager.iterator()) {
            vehicle.pressed = false
        }
    }
}
