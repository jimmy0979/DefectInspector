import pandas as pd
import random
from tqdm import tqdm

# 負責生成 100,000,000 筆測試資料
Range = 900

# strIdx = '0' * (4 - len(str(i))) + str(i)
filepath = f"2274_DefectData_TEST_PartALL.csv"
region = 0

with open(filepath, 'w') as fileObj:
    fileObj.write("LOT ID,Wafer ID,Region,Date,Time,Number,DefectType,TaskID,DieX,DieY,FrameX,FrameY,PosX_Image,PosY_Image,SizeX_Image,SizeY_Image,Length_Image,Width_Image,Angle_Image,Area_Image,Circulity_Image,Outer_Radius,Gray_Difference,PosX_Real,PosY_Real,SizeX_Real,SizeY_Real,Length_Real,Width_Real,Ball_Diameter,Machine PosX,Machine PosY,Review Flag,REJUDGE,DL_Judge\n")
    for row in tqdm(range(50000000)):
        if row % 390625 == 0:
            region += 1
        dieX = random.randint(0, Range-1)
        dieY = random.randint(0, Range-1)
        # randomly generate (dieX, dieY) for testing data
        fileObj.write(
            f"01,01,{region},2000/01/01,0,0,256,1,{dieX},{dieY},0,0,340,152.5,115,36,115,36,0,3087,0.23,59.57,118.968,66.407,28.262,63.25,19.8,63.25,19.8,0,27.169,-29.047,0,-999,2\n")
