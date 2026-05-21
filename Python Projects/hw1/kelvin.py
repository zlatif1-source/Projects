"""
File:    Kelvin.py
Author:  Zain Latif
Date:    9/10/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  takes in a number of degrees in fahrenheit and converts it to Celsius and to Kelvin.
"""
fahrenheit = float(input("Enter a temperature in Fahrenheit: "))

celsius = (fahrenheit - 32) * 5 / 9

kelvin = celsius + 273.15

print("The temperature in Celsius is", celsius)
print("The temperature in Kelvin is", kelvin) 
