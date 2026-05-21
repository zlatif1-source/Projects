"""
File:    pascal.py
Author:  Zain Latif
Date:    10/15/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  finds the next pascal triangle line 
"""
def next_level(level):
    """
    Calculate the next level of Pascal's Triangle based on the provided level.
    Each entry is the sum of the two diagonal/ entries above it.
    """
    
    new_level = [1]  
    
    
    for i in range(len(level) - 1):
        new_value = level[i] + level[i + 1]
        new_level.append(new_value)


    new_level.append(1)  
    
    return new_level 


if __name__ == "__main__":
    in_string = input('What values do you want to run next_level on? ')
    while in_string != '':
        values = []
        for x in in_string.split():
            values.append(int(x))  
        print(next_level(values))  
        in_string = input('What values do you want to run next_level on? ')
