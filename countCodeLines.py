import os
print(os.path.basename(__file__))
fileExt = ['.h', '.cpp']
countLines = 0
files = [item for item in os.listdir() if os.path.isfile(item) and os.path.splitext(item)[1] in fileExt and item != os.path.basename(__file__)]
for file in files:
    openFile = open(file)
    for line in openFile:
        countLines += 1
    openFile.close()
print('一共 %d 行代码') % countLines
