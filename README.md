  The code used in this spectrophotometer works in chronological order to display, measure, and process data. To begin, the LCD is set up based on its documentation, and variables required to control the on/off switch, photoresistor measurements, and calculations are initialized. Next, in the setup loop, the serial communication is initialized at 9600 bits per second, and the pins are configured based on what they are connected to — for example, pin 8 is connected to the LED (as an output), and is hence declared that way in the code. Also in this loop, the LCD is initialized with its dimensions and backlight on, and an introductory message is displayed (with an extra loop required to display words longer than the LCD size as scrolling text). This concludes the setup portion of the code. 
	The loop function of the code contains the remainder of the functionality. The device first powers on the LED, and reads in switchState, a variable which holds the state of the push button used to begin measurement. If the button is pressed, three analog output measurements are taken from the photoresistor and converted into voltages. These voltage readings are averaged for further calculations (concentration and confidence interval) and displayed on the LCD (more for the debugging purposes, not for the users — should be commented out for general use). Next, this voltage reading is converted to an absorbance value based on the combination of equations referenced in the prelab and also displayed on the LCD (again for debugging purposes). Based on a calibration curve, this absorbance value is plugged into a linear equation and used to reverse calculate a predicted concentration value for the solution inserted into the spectrophotometer. Next, the confidence interval is calculated from the formulas provided in class. Important to note here is that variables such as degFree and n must be updated based on the number of samples used in the calibration. Other variables such as s_std, Sr, m, and Sc_std also change based on the calibration calculations (done in an excel file with regression data analysis). Here, the concentration is displayed on the LCD, along with the uncertainty. Lastly, based on the concentration and if the uncertainty placed the projected concentrations in both disease and non-disease ranges, a conclusion is displayed on whether the sample means you are sick! or nah? For example, if the concentration is below 1%, and the upper end of the confidence interval is also, then the device will display an absent disease state. 
	Overall, the code works to interact the circuitry and hardware in a manner that follows the sequential pathway of the user, taking inputs and calculating useful outputs along the way. 
