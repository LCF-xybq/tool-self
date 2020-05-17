import os

filepath = r'C:\Users\MoodyBlues\Desktop\壁纸'
pics = os.listdir(filepath)

length = len(pics)

for i in range(length):
    index = pics[i].find('.')
    suffix = pics[i][index:len(pics[i])]
    newName = str(i) + suffix
    fullPath = os.path.join(filepath, pics[i])
    newPath = os.path.join(filepath, newName)
    os.rename(fullPath, newPath)
