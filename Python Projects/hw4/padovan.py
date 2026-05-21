"""
File:    padovan.py
Author:  Zain Latif
Date:    10/3/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  does the padovan sequence
"""
if __name__ == '__main__':
    sequence = [1,1,1]
    goal=int(input("Enter the goal to reach in the Padovan sequence: "))
    if goal == 1:
        print("It takes 1 step to get there or above.")
    else:
        steps = 3
        while sequence[-1] < goal:
            next_number = sequence[-2] + sequence[-3]
            sequence.append(next_number)
            steps += 1


        print("It takes", steps, "steps to get there or above.")

