"""
File:    list_merge.py
Author:  Zain Latif
Date:    9/24/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  merges list
"""
num_elements = int(input("How many elements do you want in each list? "))

first_list = []
second_list = []

for i in range(num_elements):
    element = input(f"What do you want to put in the first list? ")
    first_list.append(element)

for i in range (num_elements):
    element = input(f"What do you want to put in the second list? ")
    second_list.append(element)


print("The first list is: ", first_list)
print("The second list is: ", second_list)


merged_list = []
for i in range(num_elements):
    merged_list.append(first_list[i])
    merged_list.append(second_list[i])

print("The merged list is: ", merged_list)
