# Project_Talon

Break these projects into their own branches..
Creat ICD for each subsystem..

## Project: "Comm Motor Control Test with Interrupt"
**Description:** This project was created for the integration of hc-11 communication systems with the flight controller's initial stages -- The command execution for all actuators. This particular integration tests the correct parsing of command strings by blinking the PSOC's on board LED, as well as the actuation of each servo/bldc motor which are controlled with each corresponding command string.<br>

An additional feature allows the user to press the button on board the PSOC to toggle a propulsion motor test. When toggled on, the BLDC motor will sweep from 0 to full speed. This feature defaults to off. <br>

**TO DO's:**
1. Make a separate feature with this code. This is a useful test that can be used to troubleshoot. 
2. If this becomes a separate feature/tool, make the toggle work so that servos can be tested for full range of motion. Maybe use the LED flashing as feedback as to what mode it is in (Servo, ESC, or full flight troubleshooter) <br>



## Project: "Talon_01"
**Description** Integration testing for entire flight controller, starting with integration of HC12 project and Comm Motor Control Test to test wireless control. Then integration of sensorDataFlow to test all sensors are connected properly and data is received at home base properly. 

**Next Steps:** Tie up loose ends. Investage and integrate new IMU method to reduce bandwidth utilization. Once measurements can be made accurately with high frequency and no latency (enough for fast response PID controller) and with reduced overall bandwidth of CPU, then focus should shift to control and interface with control station.

1. Try to fit ADC input onto a pin for measuring battery volts.
2. Find a current measurement solution for battery output and/or esc input. *NOT important for this iteration. Only to see if possible with I/O.
3. Once this is set in stone, start designing a PCB solution for entire flight control. Decide on which sensors to include on PCB and which are external.
4. Work on making HC-12 project independent too. Before moving forward on this, look for other possibly better radio solutions. Create a project/repository to manage versions and attempt to make pushing the code to this project seemless. Since it will be UART based, the project specific code should be handled in the interrupts but HC12 libraries should be self contained.
5. Create working Air Speed handler
6. Make interrupts a separate file from .c and .h libraries. Interrupts should be handled on a per project basis. Also see about doing this with all peripherals. 
	* EX1: TopDown   BMP280_elevation -> BMP280_readSPI -> spiDataHandler (waits for BMP280's turn) -> SPI_1_write(0) + enable_BMP280 -> SPI_1_read() x n -> spiDataHandler writes BMP280_dataAvail=True -> BMP280_elevation returns. 
		- Would have to rewrite functions for request_Elevation and Elevation_Available 
		- Should only do this for things that are not time sensitive and to prevent blocking code. For timing critical measurements, need to rewrite so that bandwidth is accounted for and make measurements at evenly spaced intervals. This drives TODO#6 and #7.
	* EX2: BottomUp  uartInterupt -> uartDataHandler  -> BMP280_dataAvail==True -> BMP280_Elevation()
7. Make a branch to test bandwidth / time to execute of each substantial function call in code. Especially for sensor data and SD write.
8. Create option to change output string format and contents
9. Update SD file date and time with GPS time
10. Create a new branch to test different IMU's and calculate any transient
11. Add function to prevent motor from turning on until controller is "armed" by holding joystick down to the right (max pitch up, max roll right).
