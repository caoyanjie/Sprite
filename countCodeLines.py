#_*_ coding: utf-8 _*_
import os

fileExt = ['.h', '.cpp']
lines = 0
files = [item for item in os.listdir('.') if os.path.isfile(item) and os.path.splitext(item)[1] in fileExt and item != os.path.basename(__file__)]
for file in files:
    openFile = open(file)
    for line in openFile:
        lines += 1
    openFile.close()
print('共统计 %d 个文件：\n一共 %d 行代码') % (len(files), lines)
