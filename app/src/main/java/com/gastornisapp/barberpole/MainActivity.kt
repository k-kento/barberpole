package com.gastornisapp.barberpole

import BarberPolePage
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.ui.Modifier
import com.gastornisapp.barberpole.ui.theme.BarberPoleTheme

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        enableEdgeToEdge()
        setContent {
            BarberPoleTheme {
                Scaffold { innerPadding ->
                    BarberPolePage(Modifier.padding(innerPadding))
                }
            }
        }
    }
}

