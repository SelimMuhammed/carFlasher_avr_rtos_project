# Vehicle Indicator Control System with RTOS

This project implements a vehicle indicator control system using a microcontroller and an RTOS (Real-Time Operating System) for task management.
The system controls left and right flashers, as well as hazard lights based on input from external buttons.

## Description

The code consists of several modules:

- **LIB**: Contains standard types and bit manipulation macros.
- **MCAL/DIO_V1**: Provides digital I/O functions for interfacing with buttons and indicator LEDs.
- **RTOS_Layer**: Contains the RTOS interface for task management.

### Features

- Control of left and right indicator flashers.
- Activation of hazard lights.
- Task-based architecture for efficient management of system states.

#### Prerequisites

Before running this code, ensure you have the following components:

- Microcontroller unit with digital I/O support
- External buttons for left, right, and hazard controls
- Indicator LEDs for left and right flashers
- RTOS library
- Standard C libraries
 
