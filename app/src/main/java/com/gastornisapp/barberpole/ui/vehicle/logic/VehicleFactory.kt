package com.gastornisapp.barberpole.ui.vehicle.logic

object VehicleFactory {

    // 車種と出現確率を定義
    private val vehicleTypes: List<Pair<Int, (Int) -> VehicleLogicModel>> = listOf(
        6 to { id -> CarLogicModel(id) },        // 車 60%
        2 to { id -> BusLogicModel(id) },        // バス 20%
        2 to { id -> LightTruckLogicModel(id) }  // トラック 20%
    )

    fun create(): List<VehicleLogicModel> {

        var idCounter = 0
        val vehicles = mutableListOf<VehicleLogicModel>()

        // 重みの合計
        val totalWeight = vehicleTypes.sumOf { it.first }

        repeat(100) {
            vehicles.add(selectVehicle(id = idCounter++, totalWeight = totalWeight))
        }

        return vehicles
    }

    // 重みに応じて車種を選ぶ
    private fun selectVehicle(id: Int, totalWeight: Int): VehicleLogicModel {
        val randomValue = (1..totalWeight).random()
        var cumulativeWeight = 0

        for ((weight, constructor) in vehicleTypes) {
            cumulativeWeight += weight
            if (randomValue <= cumulativeWeight) {
                return constructor(id)
            }
        }

        // 万一ここまで来たら最後の車種を返す
        return vehicleTypes.last().second(id)
    }
}
