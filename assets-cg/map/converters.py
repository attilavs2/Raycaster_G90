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
		
def convert_map(input, output, params, target):
	print("test python")
	print(params["name"])
	
	data = json.load(open(input, "r"))

	#Extract from the json the width, height
	#w, h = data["width"], data["height"]

	structMap = fxconv.Structure()

	#structMap += fxconv.u32(w) 
	#structMap += fxconv.u32(h)
	structMap += fxconv.u32(4*2**16) #startpos_x
	structMap += fxconv.u32(4*2**16) #startpos_y
	structMap += fxconv.u32(1*2**16) #startdir_x
	structMap += fxconv.u32(0)       #startdir_y
	structMap += fxconv.u32(0)		 #startplane_x
	structMap += fxconv.u32(int(0.66*2**16)) #startplane_y

	data_f = data["layers"][0]["data"] #Floor data
	data_w = data["layers"][1]["data"] #Wall data
	data_s = data["layers"][2]["data"] #Sprite data
	data_m = data["layers"][3]["data"] #Mob data (aka spawn data)

	for i in range(0, 128**2):
		structMap += fxconv.u8(data_f[i])
	for i in range(0, 128**2):
		structMap += fxconv.u8(data_w[i])			
	for i in range(0, 128**2):
		structMap += fxconv.u8(data_s[i])
	for i in range(0, 128**2):
		structMap += fxconv.u8(data_m[i])
	
	#generate !
	fxconv.elf(structMap, output, "_" + params["name"], **target)
