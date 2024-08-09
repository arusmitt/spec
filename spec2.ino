// Include the Adafruit LiquidCrystal library for the I2C LCD display
#include "Adafruit_LiquidCrystal.h"

// Create an instance of the Adafruit_LiquidCrystal class to control the LCD
Adafruit_LiquidCrystal lcd(0);

// Declare and initialize variables for managing switch state
int switchState = 0;

// Declare variables for photoresistor measurements
int photoresistor = 0;
float voltages[3];       // Array to store voltage readings
float avgVoltage = 0;    // Average of the voltages measured

// Variables for the calculation of absorbance, concentration, and uncertainty
float R2blank = 2787.87; // Resistance of blank cuvette used in calculations
float abs_m = 0;         // Absorbance measure
float conc = 0;          // Concentration derived from absorbance
float m = -0.1067;       // Slope of calibration curve
float b = -0.056721;     // Intercept of calibration curve
float uncertainty = 0;   // Uncertainty in the concentration measure
int numSamp = 3;         // Number of samples to determine experimental variability
int numCal = 5;          // Number of samples in the calibration curve
float s_std = -0.21631;  // Average signal for calibration standards
float Sc_std = 0.7905694; // Standard deviation for concentration in calibration standard
float s_ca = 0;
float Sr = 0.008138;     // Standard error of the estimate
float degFree = 3.182;   // Degrees of freedom, calculated as (number of samples - 2)

void setup() {
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  
  // Configure pin modes for LEDs, switch, photoresistor, and LCD
  pinMode(8, OUTPUT); // Set pin 8 as output for LED
  pinMode(2, INPUT);  // Set pin 2 as input for switch
  pinMode(A0, INPUT); // Set pin A0 as input for photoresistor
  pinMode(A4, INPUT); // Used for LCD
  pinMode(A5, INPUT); // Used for LCD

  // Initialize the LCD and display an introductory message
  lcd.begin(16,2); // Specify the LCD's width and height
  lcd.clear();     // Clear any existing characters on the LCD
  lcd.setCursor(0,0); // Position cursor at the top-left corner
  lcd.setBacklight(HIGH); // Turn on the backlight
  lcd.print("spectrophotometer!"); // Print a message on the LCD
  for(int i = 0; i < 2; i++){
    delay(400); 
    lcd.scrollDisplayLeft(); // Scroll message to the left
  }
  delay(5000); // Pause before continuing
}

void loop() {
  digitalWrite(8, HIGH); // Turn on the LED
  switchState = digitalRead(2); // Read the state of the switch

  // If the switch is pressed, measure the voltage across the photoresistor
  if(switchState == HIGH){
    for(int i = 0; i < 3; i++){
      photoresistor = analogRead(A0);
      voltages[i] = photoresistor * (5.0 / 1023.0); // Convert the analog reading to voltage
      delay(500); 
    }
    avgVoltage = (voltages[0] + voltages[1] + voltages[2])/3; // Compute the average voltage
  }

  // Display the average voltage on the LCD
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("voltage: ");
  lcd.setCursor(0,1); 
  lcd.print(avgVoltage); 
  delay(5000);

  // Calculate absorbance from the average voltage
  abs_m = 5 - avgVoltage; 
  abs_m = avgVoltage / abs_m; 
  abs_m = 1000 * abs_m; 
  abs_m = R2blank/abs_m; 
  abs_m = log10(abs_m); 

  // Display the absorbance on the LCD
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("absorbance: ");
  lcd.setCursor(0,1); 
  lcd.print(abs_m); 
  delay(5000);

  // Calculate the concentration from the absorbance
  conc = (abs_m - b)/m;

  // Calculate the uncertainty in the concentration measurement
  s_ca = (sq(Sc_std))*(numCal - 1); 
  s_ca = s_ca * sq(m); 
  s_ca = (sq(abs_m - s_std))/s_ca; 
  s_ca = (1/numSamp) + (1/numCal) + s_ca; 
  s_ca = sqrt(s_ca);
  s_ca = (Sr/m)*s_ca; 
  uncertainty = degFree * s_ca; 
  uncertainty = abs(uncertainty); 

  // Display the concentration, uncertainty, and disease state on the LCD
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("concentration: "); 
  lcd.setCursor(0,1);
  lcd.print(conc);
  lcd.print(" +/- "); 
  lcd.print(uncertainty);
  delay(5000);  

  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("disease state: ");  
  lcd.setCursor(0,1);

  // Determine and display the disease state based on concentration and uncertainty
  if(conc < 1 && conc + uncertainty > 1){
    lcd.print("inconclusive"); 
    delay(5000); 
  }
  if(conc < 1 && conc + uncertainty < 1){
    lcd.print("absent"); 
    delay(5000); 
  }
  if(conc > 1 && conc - uncertainty < 1){
    lcd.print("inconclusive"); 
    delay(5000); 
  }
  if(conc > 1 && conc - uncertainty > 1){
    lcd.print("present"); 
    delay(5000); 
  }   
}
