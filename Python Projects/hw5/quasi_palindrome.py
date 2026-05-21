"""
File:    quasi_palindrome.py
Author:  Zain Latif
Date:    10/15/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  finds palindromes
"""
def quasi_palindrome(word, errors):
    word = word.lower()
    error_count = 0
    length = len(word)
    
    for i in range(length // 2):
        if word[i] != word[length - 1 - i]:
            error_count += 1
            if error_count > errors:
                return False
            
    return True

if __name__ == "__main__":
    while True:
        word = input("What word do you want to check? ")
        if word.lower() == "quit":
            break
        
        errors = int(input("How many errors do you want to allow? "))
        if quasi_palindrome(word, errors):
            print(f"It was a {errors}-quasi-palindrome!")
        else:
            print(f"It was not a {errors}-quasi-palindrome!")

