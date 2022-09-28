# Auto Watering System
Programable watering system with Arduino. 

# How it works?
This set up has been created to water plants at a specific time of the day. To do so we must define at which times the pump should turn on.
On top of that we have a soil moisture sensor, which will allow (or not) the water pumping.

It is easier explained with an example:
Imagine we have set 3 times for watering the plants each day: at 9am, 3pm and 8pm. At 9:00 the Arduino will attempt to turn on the pump, but before it will check the value of the soil moisture sensor. If we get that the soil is dryer than our threshold (defined in local variables), the pump will turn on, otherwise the action will be interrupted and no new attemps will be made until 3pm.

## Components needed
- Arduino UNO (or equivalent)
- Clock
- Soil moisture sensor
- Relay
- LEDs and a button (useful fer testing and get feedback)
- Water pump

### Tips
- Set up a button to force pumping water. It will be useful for testing.
- Water pumps are good at pumping water, but not air! If there is air in your piping between the deposit and the pump, it may not work... To solve that you can get a waterproof pump and leave it at the bottom of your water tank. There are more approaches to this issue, but I found this one the easiest to implement. Let me know if you come up with another one.
