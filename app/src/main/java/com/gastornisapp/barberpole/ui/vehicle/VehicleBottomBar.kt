package com.gastornisapp.barberpole.ui.vehicle

import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.size
import androidx.compose.material3.BottomAppBar
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.unit.dp
import com.gastornisapp.barberpole.ui.vehicle.model.VehicleType
import com.gastornisapp.barberpole.R

@Composable
fun BottomBar(onAddVehicle: (VehicleType) -> Unit) {
    BottomAppBar {
        Row(
            modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceEvenly
        ) {
            Image(
                painter = painterResource(R.drawable.ic_car),
                contentDescription = null,
                modifier = Modifier
                    .size(60.dp)
                    .clickable { onAddVehicle(VehicleType.Car) },
                contentScale = ContentScale.Fit
            )
            Image(
                painter = painterResource(R.drawable.ic_light_track),
                contentDescription = null,
                modifier = Modifier
                    .size(60.dp)
                    .clickable { onAddVehicle(VehicleType.LightTruck) },
                contentScale = ContentScale.Fit
            )
            Image(
                painter = painterResource(R.drawable.ic_bus),
                contentDescription = null,
                modifier = Modifier
                    .size(60.dp)
                    .clickable { onAddVehicle(VehicleType.Bus) },
                contentScale = ContentScale.Fit
            )
        }
    }
}
