# Project_Talon

Break these projects into their own branches..
Creat ICD for each subsystem..

## Project: "Comm Motor Control Test with Interrupt"
**Description:** This project was created for the integration of hc-11 communication systems with the flight controller's initial stages -- The command execution for all actuators. This particular integration tests the correct parsing of command strings by blinking the PSOC's on board LED, as well as the actuation of each servo/bldc motor which are controlled with each corresponding command string.<br>

An additional feature allows the user to press the button on board the PSOC to toggle a propulsion motor test. When toggled on, the BLDC motor will sweep from 0 to full speed. This feature defaults to off. <br>

**TO DO's:**
1. Make a separate feature with this code. This is a useful test that can be used to troubleshoot. 
2. If this becomes a separate feature/tool, make the toggle work so that servos can be tested for full range of motion. Maybe use the LED flashing as feedback as to what mode it is in (Servo, ESC, or full flight troubleshooter) <br>

## Project: "Sensor Data Flow"
**Description:** This was used to build the data aquisition functionallity for the flight controller. Here, all of the sensor libraries were constructed as well as the process to handle the information that is extracted. <br>

More about that information should go HERE. <br>

**To DO's:**
1. I think this has fallen behind the main code. Should update the main.c code. Everything else uses the same source.
2. Update the GPS code. Break GPS into useful variables and send data back with only a few important features, or send back starting characters to decipher what kind of senetence is being sent (May need both, soon debugging will need simplified and debug/status sentence to know which sensors are available and what state the plane is in will be needed)
3. Create working Air Speed handler
4. Work on making this independent. If all changes could be made here and be pushed to the main project, that would be great. Or if changes could be made at either place and the main code here still works for testing without changes, that is an good solution too. 
5. Make interrupts a separate file from .c and .h libraries. Interrupts should be handled on a per project basis. Also see about doing this with all peripherals. 
	* EX1: TopDown   BMP280_elevation -> BMP280_readSPI -> spiDataHandler (waits for BMP280's turn) -> SPI_1_write(0) + enable_BMP280 -> SPI_1_read() x n -> spiDataHandler writes BMP280_dataAvail=True -> BMP280_elevation returns. 
		- Would have to rewrite functions for request_Elevation and Elevation_Available 
		- Should only do this for things that are not time sensitive and to prevent blocking code. For timing critical measurements, need to rewrite so that bandwidth is accounted for and make measurements at evenly spaced intervals. This drives TODO#6 and #7.
	* EX2: BottomUp  uartInterupt -> uartDataHandler  -> BMP280_dataAvail==True -> BMP280_Elevation()
6. Make a branch to test bandwidth / time to execute of each substantial function call in code. Especially for sensor data.
7. Make a branch to attempt using dma for all sensor data. (Especially SPI and I2C data. UART can be interrupt based outside of DMA).

## Project: "Talon_01"
**Description** Integration testing for entire flight controller, starting with integration of HC12 project and Comm Motor Control Test to test wireless control. Then integration of sensorDataFlow to test all sensors are connected properly and data is received at home base properly. 

**Next Steps:** Project "sensorDataFlow" should be developed the rest of the way to include other  sensors. Some optimization should occur to clean code up. Make sure that all main sub-systems can be run and edited independently within its own project, and changes pushed to this project. Create a new branch to test different IMU's and calculate any transient. If new IMU is 10 DOF, that would be ideal to free up I/O.

**To Do's**
1. Try to fit ADC input onto a pin for measuring battery volts.
2. Find a current measurement solution for battery output and/or esc input. *NOT important for this iteration. Only to see if possible with I/O.
3. Once this is set in stone, start designing a PCB solution for entire flight control. Decide on which sensors to include on PCB and which are external.
4. Work on making HC-12 project independent too. Before moving forward on this, look for other possibly better radio solutions. Create a project/repository to manage versions and attempt to make pushing the code to this project seemless. Since it will be UART based, the project specific code should be handled in the interrupts but HC12 libraries should be self contained.
