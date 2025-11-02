package com.gastornisapp.barberpole.ui

enum class PageType(val route: String) {
    Home("home"),
    BarberPole("barber_pole"),
    Confirmation("confirmation"),
    Vehicle("vehicle"),
    Percussion("percussion"),
    Swarm("swarm"),
    Drawing("drawing"),
    Kaleidoscope("kaleidoscope"),
    WebPage("webpage?url={url}"),
    Info("info"),
    Settings("settings"),
    License("license");
}
