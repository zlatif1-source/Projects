"""
File:    worldle_checker.py
Author:  Zain Latif
Date:    9/24/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  finds the word
"""
solution = input("Enter the solution word: ").strip().lower()
guess = input("Enter the guess word: ").strip().lower()

result = ""

for i in range(len(guess)):
    if guess[i] == solution[i]:
        result += "g "
    elif guess[i] in solution:
        result += "y "
    else:
        result += "_ "

print(result.strip())
