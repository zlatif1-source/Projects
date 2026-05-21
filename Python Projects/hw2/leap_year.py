"""
File:    leap_year.py
Author:  Zain Latif
Date:    9/16/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  defines if a year is a leap or not.
"""
year = int(input("Enter a year: "))

if year % 4 == 0:
    if year % 100 == 0:
        if year % 400 == 0:
            print("It is a leap year.")
        else:
            print("It is not a leap year.")
    else:
        print("It is a leap year.")
else:
    print("It is not a leap year.")
