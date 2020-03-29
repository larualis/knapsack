import random

numberOfElements = 60
numberOfFunctions = 3
maxValue = 1000

elements = []
file = open("knapsack.txt", "w")

for x in range(0, numberOfElements):
    element = []
    for y in range(0, numberOfFunctions + 1):
        element.append(random.randint(0, maxValue))
    elements.append(element)

for element in elements:
    for x in element:
        file.write(str(x) + " ")
    file.write("\n")