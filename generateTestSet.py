import random
import os
import shutil

numberOfElements = 70

numberOfFunctions = 3

maxValue = 1000

numberOfSets = 30

nameOfDirectory = "test_set"

if os.path.isdir(nameOfDirectory):
	shutil.rmtree(nameOfDirectory)


os.mkdir(nameOfDirectory)


for i in range(1,numberOfSets+1):

	elements = []
	file = open(nameOfDirectory + "/knapsack"+ str(i) + ".txt", "w")

	for x in range(0, numberOfElements):
	    element = [random.randint(1, maxValue)]
	    for y in range(0, numberOfFunctions):
	        element.append(random.randint(0, maxValue))
	    elements.append(element)

	for ele in range(len(elements)):
	    for x in range(len(elements[ele])):
	        if x != len(elements[ele]) - 1:
	            file.write(str(elements[ele][x]) + " ")
	        else:
	            file.write(str(elements[ele][x]))
	    if ele != len(elements) - 1:
	        file.write("\n")
