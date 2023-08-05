from random import randint
import fxconv
import json
import pathlib
import csv

def convert(input, output, params, target):
	if params["custom-type"] == "map":
		convert_map(input, output, params, target)
		return 0
	else:
		return 1

#def world_conv(input, output, params, target):


def convert_map(input, output, params, target):
	data = json.load(open(input, "r"))

	#Extract from the json the width, height
	w, h = data["width"], data["height"]

	structMap = fxconv.Structure()

	structMap += fxconv.u32(w) 
	structMap += fxconv.u32(h)

	print(data["layers"]["data"])
	#generate the array of tiles from the layer
	#for i in range(0, w):
	#	for j in range(0, h):
			
			

	#generate !
	fxconv.elf(structMap, output, "_" + params["name"], **target)

