"""
File:    connect_four.py
Author:  Zain Latif
Date:    10/15/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  makes the connect four game
"""
def connect_four(the_grid, player_symbol):
    """
    Check for a Connect Four victory condition in the given grid.

    Arguements:
    the_grid (list of list of str): 2D list representing the game grid.
    player_symbol (str): The player's symbol to check for.

    Returns:
    True if there is a Connect Four for the player_symbol, False otherwise.
    """


    for row in the_grid:
        count = 0  
        for cell in row:
            if cell == player_symbol:
                count += 1
                if count == 4: 
                    return True
            else:
                count = 0  

    
    for col in range(len(the_grid[0])):
        count = 0
        for row in range(len(the_grid)):
            if the_grid[row][col] == player_symbol:
                count += 1
                if count == 4: 
                    return True
            else:
                count = 0 

    return False 


if __name__ == "__main__":
    rows = int(input('How many rows will be entered? '))
    grid = []
    for x in range(rows):
        new_row = []
        new_line = input()
        for char in new_line:
            new_row.append(char)
        grid.append(new_row)  

    print(connect_four(grid, "x"))  
