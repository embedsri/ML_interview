#!/usr/bin/env python3
#__author__ = "Sriram Chadalavada"
#__copyright__ = "Copyright 2019, The Cogent Project"
#__credits__ = ["Sriram Chadalavada", "Steven Meyer"]
#__license__ = "Moleculight all rights reserved"
#__version__ = "0.2"
#__maintainer__ = "Steven Meyer"
#__email__ = "sriram.chadalavada@mindleap.ca"
#__status__ = "Prototype"


# import the necessary packages
import numpy as np
import argparse
import cv2 as cv
import datetime

HSV_RANGES = {
    'yellow': [
        {
            'lower': np.array([21, 39, 64]),
            'upper': np.array([40, 255, 255])
        }
    ]
 }

def create_mask(hsv_img, colors):
    """
    Creates a binary mask from HSV image using given colors.
    """

    # noinspection PyUnresolvedReferences
    mask = np.zeros((hsv_img.shape[0], hsv_img.shape[1]), dtype=np.uint8)

    for color in colors:
        for color_range in HSV_RANGES[color]:
            # noinspection PyUnresolvedReferences
            mask += cv.inRange(
                hsv_img,
                color_range['lower'],
                color_range['upper']
            )

    return mask



# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()	
# construct the argument parser and parse the arguments
ap.add_argument("-i", "--image", required = True, help = "Path to the image")
args = vars(ap.parse_args())

# load the imag
image = cv.imread(args["image"])
#Convert BGR to HSV colorspace.
image_hsv =  cv.cvtColor(image, cv.COLOR_BGR2HSV)
yellow_mask = create_mask(image_hsv, ['yellow'])

threshold_hsv = cv.bitwise_and(image_hsv, image_hsv, mask=yellow_mask)

#Display image contthaining ONLY yellow color.
cv.imshow("Thresholded image", threshold_hsv)

#
output = threshold_hsv.copy()
gray = cv.cvtColor(output, cv.COLOR_BGR2GRAY)


# detect circles in the image=
circles = cv.HoughCircles(gray, cv.HOUGH_GRADIENT,1, 20,param1=50, param2=30,minRadius=0,maxRadius=0)
print ("Detected circles in the thresholded image")
#
# # ensure at least some circles were found
if circles is not None:
# 	# convert the (x, y) coordinates and radius of the circles to integers
     circles = np.round(circles[0, :]).astype("int")
#
# 	# loop over the (x, y) coordinates and radius of the circles
     for (x, y, r) in circles:
# 		# draw the circle in the output image, then draw a rectangle
# 		# corresponding to the center of the circle
#        cv.circle	(output, (x, y), r, (0, 255, 0), -1)
        cv.circle(output,(x,y),r,(255,153,255),-1)
#
# 	# show the output image

st = str(datetime.datetime.now())
output_name = "output" + st
cv.imshow(output_name, np.hstack([image, output]))
cv.waitKey(0)
cv.destroyAllWindows()
