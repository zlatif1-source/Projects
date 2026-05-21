"""
File:    triangular.py
Author:  Zain Latif
Date:    11/6/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  does a triangular recursive thing
"""
def triangular(n):
    if n == 0:
        return 0
    else:
        return n  +  triangular(n - 1)

    




if __name__ == "__main__":
    for i in range(20):
        print(i,triangular(i))
