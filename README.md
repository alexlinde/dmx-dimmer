# DMX Dimmer Controller

A custom-built DMX lighting controller for my RV/Airstream to provide centralized control and dimming functionality for 12V lighting systems. 

## Overview

This project was born out of necessity when the original 4-zone 12V touch controller in my Airstream trailer failed after 10+ years of reliable service. With no replacement available, I developed this custom solution to maintain the same functionality using modern components.

## Features

- 4-zone lighting control
- Individual dimming control for each zone
- Momentary push button interface
- DMX protocol support via RS485
- Compatible with existing 12V lighting systems
- Custom 3D-printed case that fits existing mounting location

## Hardware Components

- Seeeduino XAIO SAMD21 microcontroller
- RS485 module for DMX communication
- 4 momentary push switches
- 3D-printed enclosure
- Power supply components for 12V operation

## Technical Details

The controller uses the DMX512 protocol to communicate with slave dimmers, allowing for precise control of lighting levels across four independent zones. The Seeeduino XAIO SAMD21 reads the state of the momentary push switches and translates these inputs into DMX control signals, which are then transmitted via the RS485 interface.

## Usage

Each zone can be controlled independently:
- Quick press to toggle the zone on/off
- Press and hold a zone button to dim the lights
