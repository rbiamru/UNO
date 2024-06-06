#include "HX711.h"
#include <LiquidCrystal.h>

// Initialize the LiquidCrystal library with the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

// Initialize the HX711 scale with data and clock pins
HX711 scale(A1, A0);   

void setup() {
  // Initialize the LCD display with 16 columns and 2 rows
  lcd.begin(16, 2);     
  lcd.setCursor(4, 0);  
  lcd.print("Arduino"); 
  lcd.setCursor(0, 1);   
  lcd.print("load cell sensor"); 
  delay(1000);  // Wait for a second to display the initial message

  // Clear the LCD display
  lcd.clear(); 
  
  // Start serial communication at 38400 baud rate
  Serial.begin(38400); 
  
  // Display setup message on LCD
  lcd.setCursor(2, 0);        
  lcd.print("-Setting.");  
  
  // Serial output before setting up the scale
  Serial.print("get value: \t\t");           
  Serial.println(scale.get_value(5));  // Print the average of 5 readings minus tare weight (not set yet)
  Serial.print("get units: \t\t");          
  Serial.println(scale.get_units(5), 1);  // Print the average of 5 readings minus tare weight divided by the scale factor (not set yet)

  // Set the scale factor to calibrate the scale
  scale.set_scale(2280.f);     
  
  // Reset the scale to 0
  scale.tare();               

  // Serial output after setting up the scale
  Serial.println("After setting up the scale:"); 
  Serial.print("read: \t\t");                    
  Serial.println(scale.read());  // Print a raw reading from the ADC
  Serial.print("read average: \t\t");            
  Serial.println(scale.read_average(20));  // Print the average of 20 readings from the ADC
  Serial.print("get value: \t\t");              
  Serial.println(scale.get_value(5));  // Print the average of 5 readings minus the tare weight
  Serial.print("get units: \t\t");              
  Serial.println(scale.get_units(5), 1);  // Print the average of 5 readings minus tare weight divided by the scale factor
  
  // Indicate that the readings will be printed
  Serial.println("Readings:");  
  lcd.clear();  // Clear the LCD display
}

void loop() {
  // Print one reading to the serial monitor
  Serial.print("one reading:\t");               
  Serial.print(scale.get_units()*5.64, 1);  // Print the reading converted by a factor of 5.64 to the serial monitor
  Serial.print("\t| average:\t");               
  Serial.println(scale.get_units(10)*5.64, 1);  // Print the average of 10 readings converted by a factor of 5.64 to the serial monitor
  
  // Put the scale into sleep mode to save power
  scale.power_down();  
  delay(100);  // Wait for 100 milliseconds
  scale.power_up();  // Wake up the scale
  
  // Display weight average on the LCD
  lcd.setCursor(1, 0);  // Set the cursor at column 1, row 0
  lcd.print("Weight Average");  
  lcd.setCursor(4, 1);  // Set the cursor at column 4, row 1
  
  // Calculate and display the weight on the LCD
  int x = scale.get_units()*56.4;  // Multiply the reading by 56.4 to get the weight in grams
  if(x>0){  // If the weight is positive, set it to 0
    x=0;
  }
  else{  // If the weight is negative, make it positive
    x = -x;
  }
  lcd.print(x, 1);  // Display the calculated weight on the LCD
  lcd.print(" g. ");  // Append " g." to indicate grams
  delay(200);  // Wait for 200 milliseconds before the next reading
}
