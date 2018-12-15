import cv2 as cv
import sys
y = int(sys.argv[1])
x = int(sys.argv[2])
z = sys.argv[3]
# print(z)
# def getDistance(y,x,z):
img = cv.imread(z,-1) #read the image as it is.
nums = img[y,x] 
numd = img[y,x-1]
numf = img[y,x+1]
numg = img[y+1,x]
numh = img[y+1,x-1]
numj = img[y+1,x+1]
numk = img[y-1,x]
numl = img[y-1,x+1]
numa = img[y-1,x-1]
avg = (nums+numd+numf+numg+numh+numj+numk+numl+numa)/9
print("average pixel value of 8 blocks arround point(" + str(x) +','+ str(y) + ')is ' +str(avg))
#  getDistance(y,x,z)
