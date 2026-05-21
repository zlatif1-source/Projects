"""
File:    list_reverse.py
Author:  Zain Latif
Date:    10/3/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  reverse list without numbers
"""

if __name__ == '__main__':
    user_input = input("Enter a list seperated by commas: ")
    my_list = user_input.split(',')

    cleaned_list = []
    for item in my_list:
        item = item.strip()
        has_digit = False

        for character in item:
            if character in "0123456789":
                has_digit = True
                

        if not has_digit:
            cleaned_list.append(item)

    reversed_list = []
    length = len(cleaned_list)
    for i in range(length):
        reversed_list.append(cleaned_list[length - 1 - i])


    if reversed_list:
        result = ', '.join(reversed_list)
        print(result)
    else:
        print("The new list was empty")



    

