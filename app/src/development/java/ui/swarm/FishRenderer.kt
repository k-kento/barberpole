package ui.swarm

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.swarm.FishShaderProgram
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.math.abs

class FishRenderer(context: Context) : GLSurfaceView.Renderer {

    private val context = context.applicationContext
    private val projectionMatrix = FloatArray(16)
    private lateinit var shaderProgram: FishShaderProgram

    private lateinit var fishMesh: FishMesh
    private lateinit var bones: Bones

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        GLES30.glClearColor(1.0f, 1.0f, 1.0f, 1.0f)

        shaderProgram = FishShaderProgram().apply {
            initialize(context)
        }

        fishMesh = MeshLoader.load(program = shaderProgram, context = context)
        val inverseBindMatrices = MeshLoader.loadBone(context = context)

        bones = Bones().apply {
            initializeBones()
        }
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)
        val angle = getAngle()
        bones.updateBoneAnimation(angle)
        // shaderProgram.updateBoneMatrix(bones.finalMatrices)
        //fishMesh.draw(projectionMatrix)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)

        val aspect = width.toFloat() / height

        val viewBounds = if (height < width) {
            ViewBounds(-aspect, aspect, -1f, 1f)
        } else {
            ViewBounds(-1f, 1f, -1f / aspect, 1f / aspect)
        }

        Matrix.orthoM(
            projectionMatrix,
            0,
            viewBounds.left,
            viewBounds.right,
            viewBounds.bottom,
            viewBounds.top,
            -1f,
            1f
        )
    }

    private var startTime = 0L

    private fun getAngle(): Float {
        val currentTime = System.currentTimeMillis()
        val elapsed = currentTime - startTime

        val maxAngle = 90f
        val period = 4000L  // 1往復に2秒

        val t = elapsed % period
        val half = period / 2f

        // -1 ~ 1 の範囲で三角波を作る
        val triangle = ((half - abs(t - half)) / half) * 2f - 1f

        // -90 ~ 90 にスケーリング
        val localAngle = maxAngle * triangle
        return localAngle
    }

    fun release() {

    }
}
