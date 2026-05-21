"""
File:    which_month.py
Author:  Zain Latif
Date:    9/16/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  predicts which month in the future it is 
"""
months = [ "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]

start=int(input("What month are we starting in (enter a number from 1 to 12)? "))

if start < 1 or start > 12:
    print("That is not a month between 1 and 12")
else:
    future=int(input("How many months in the future should we go? "))

    starting_zero = start - 1

    result = (starting_zero + future) % 12

    result_month = months[result]

    print("The month will be", result_month)
    
           
