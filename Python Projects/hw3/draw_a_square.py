"""
File:    draw_a_square.py
Author:  Zain Latif
Date:    9/24/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  drawing a square
"""
vertical = int(input("How many rows (vertical) do you want? "))
horizontal = int(input("How many rows (horizontal) do you want? "))


for y in range(vertical):
    if y == 0 or y == vertical - 1:
        print("*" * horizontal)
    else:
        print("*" + " " * (horizontal - 2) + "*")

    

