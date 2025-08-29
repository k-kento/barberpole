package com.gastornisapp.barberpole.ui.swarm

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.utils.ViewUtil
import ui.swarm.FishMesh
import ui.swarm.FishMeshLoader
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class SwarmRenderer(context: Context) : GLSurfaceView.Renderer {

    private val context = context.applicationContext
    private val projectionMatrix = FloatArray(16)
    private val mvpMatrix = FloatArray(16)

    private val boidTransform: BoidTransform = BoidTransform()

    private lateinit var shaderProgram: FishShaderProgram
    private lateinit var fishMesh: FishMesh

    private lateinit var boidSimulation: BoidSimulation
    private var tapPoint: Pair<Float, Float>? = null

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        GLES30.glClearColor(1.0f, 1.0f, 1.0f, 1.0f)

        shaderProgram = FishShaderProgram()
        shaderProgram.initialize(context)

        val vertexData = FishMeshLoader.load(context = context)
        fishMesh = FishMesh(program = shaderProgram, vertices = vertexData.vertices, indices = vertexData.indices)
        boidSimulation = BoidSimulation()
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)
        boidSimulation.boids.forEach { boid ->
            boidSimulation.update(self = boid, tapPoint = tapPoint)
            boidTransform.updateMatrix(boid)
            Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, boidTransform.modelMatrix, 0)
            fishMesh.draw(mvpMatrix = mvpMatrix, color = boid.color)
        }
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

        boidSimulation.viewBounds = viewBounds
    }

    fun handleTouchDownAndMove(x: Float, y: Float, width: Int, height: Int) {
        tapPoint = ViewUtil.toWorldCoordDirect(
            x = x,
            y = y,
            screenWidth = width,
            screenHeight = height,
            viewBounds = boidSimulation.viewBounds
        )
    }

    fun handleTouchUp() {
        tapPoint = null
    }

    fun release() {
        fishMesh.release()
        shaderProgram.release()
    }
}
