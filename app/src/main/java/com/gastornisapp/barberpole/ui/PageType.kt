package com.gastornisapp.barberpole.ui

enum class PageType(val route: String) {
    Home("home"),
    BarberPole("barber_pole"),
    Confirmation("confirmation"),
    Vehicle("vehicle"),
    Harmony("harmony"),
    Percussion("percussion"),
    Swarm("swarm"),
    WebPage("webpage?url={url}"),
    Info("info"),
    License("license");
}