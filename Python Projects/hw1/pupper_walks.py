"""
File:    pupper_walks.py
Author:  Zain Latif
Date:    9/10/2024
Section: 44
E-mail:  zlatif1@umbc.edu
Description:
  calculate how long you walk Pupper each year
"""
name = input("What is pupper's real name? ")
times = int(input("How many times per week do you walk pupper? "))
long_ = int(input("How long is the walk in miles? "))
minutes = int(input("How many minutes does it take for you to walk a mile? "))

weeks_per_year = 52

distance_per_week = times * long_
total_distance = distance_per_week * weeks_per_year

time_per_week = times * long_ * minutes
total_time = time_per_week * weeks_per_year
total_time_hours = total_time / 60

print("Your dog's name is", name + ", and you have walked", total_distance, "miles this year, in", total_time_hours, "hours.")
