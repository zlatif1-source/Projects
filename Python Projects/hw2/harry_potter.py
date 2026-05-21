"""
File:    harry_potter.py
Author:  Zain Latif
Date:    9/16/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  what harry potter character you are
"""
Hogwarts = input("Are you a student at Hogwarts? ")
if Hogwarts == "yes":
    scar=input("Do you have a scar? ")
    if scar == "yes":
        print("You are Harry potter")
    else:
        red_hair=input("Do you have red hair? ")
        if red_hair == "yes":
            print("Red hair, vacant expression... You must be a Weasley.")
else:
    teacher=input("Are you or have you ever been a teacher at Hogwarts? ")
    if teacher == "yes":
        beard=input("Do you have a beard? ")
        if beard == "yes":
            giant=input("Are you part giant? ")
            if giant == "yes":
                 print("You are Rubeus Hagrid.")
            else:
                print("You are Albus Dumbledore.")
        else:
            evil_vibes=input("Do you give off evil vibes, but have a good heart in the end? ")
            if evil_vibes == "yes":
                print("You are Severus Snape.")
    else:
        print("you are Sirius Black.")
            

    
