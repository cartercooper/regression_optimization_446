import numpy as np
import pandas as pd

DATA = "delhi"
#https://www.kaggle.com/datasets/mahirkukreja/delhi-weather-data

DATA = "turbine"
#https://www.kaggle.com/datasets/theforcecoder/wind-power-forecasting


raw_data = pd.read_csv("data/"+ DATA +".csv", header=None).tail(5000)

cols = len(raw_data.columns)

f = open("data/"+ DATA +".h", "w")

f.write("#define " + DATA.upper() + " " + str(cols) + "\n")

f.write("double " + DATA + "[5000][" + str(len(raw_data.columns)) + "] =\n{\n")

for index, data in raw_data.iterrows():
    
    f.write('{' + data.to_string(index=False).replace('\n', ',') + '},' + '\n')

f.write('};')

f.close()