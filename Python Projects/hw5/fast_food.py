"""
File:    fast_food.py
Author:  Zain Latif
Date:    10/15/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  does fast food stuff
"""

def fast_food_receipt(order):
    burger_price = 5.00
    fries_price = 3.00
    drink_price = 2.50
    combo_price = 8.50
    other_price = 4.00

    
    num_burgers = 0
    num_fries = 0
    num_drinks = 0
    total = 0.0


    for item in order:
        if 'burger' in item or 'sandwich' in item:
            num_burgers += 1
        elif 'fries' in item:
            num_fries += 1
        elif 'coke' in item or 'sprite' in item or 'mountain dew' in item:
            num_drinks += 1
        else:
            total += other_price

    num_combos = 0
    while num_burgers > 0 and num_fries > 0 and num_drinks > 0:
        num_combos += 1
        num_burgers -= 1
        num_fries -= 1
        num_drinks -= 1

    total += num_combos * combo_price
    total += num_burgers * burger_price
    total += num_fries * fries_price
    total += num_drinks * drink_price

    return total

if __name__ == '__main__':
    order = []
    placing_order = False
    while not placing_order:
        item = input("What would you like to order? ").strip().lower()
        if item == 'place order':
            placing_order = True
        else:
            order.append(item)

    total_bill = fast_food_receipt(order)
    print(f"The total bill is {total_bill}")



        
