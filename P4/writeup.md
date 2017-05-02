## Writeup Template

---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[Undistorted]: ./output_images/undistort1.png "Undistorted"
[UndistortedChessBoard]: ./output_images/undistortedChessBoard.png "Chess Board Undistorted"
[gradient]: ./output_images/gradient.png "Gradiented Image"
[wrapped]: ./output_images/perspective.png "Wrapped Transform"
[findLines]: ./output_images/findLines.png "Find Lines"
[paveLane]: ./output_images/pave.png "Pave Lane"
[curv]: ./output_images/curv.png "Curv"
[image5]: ./examples/color_fit_lines.jpg "Fit Visual"
[image6]: ./examples/example_output.jpg "Output"
[video1]: ./project_video.mp4 "Video"

## [Rubric](https://review.udacity.com/#!/rubrics/571/view) Points

### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---

### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Advanced-Lane-Lines/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

You're reading it!

### Camera Calibration

#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

The code for this step is contained in the 3rd and 4th code cells of the IPython notebook located in "./AdLan.ipynb" 

I start by preparing "object points", which will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image.  Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image.  `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection.  

I then used the output `objpoints` and `imgpoints` to compute the camera calibration and distortion coefficients using the `cv2.calibrateCamera()` function.  I applied this distortion correction to the test image using the `cv2.undistort()` function and obtained this result: 

![Undistorted Chess Board][UndistortedChessBoard]

### Pipeline (single images)

#### 1. Provide an example of a distortion-corrected image.

In the 4th and 5th code black, which is around "Undistort test images", I used the objpoints and imgpoints which were created in the above step, and call cv2.calibrateCamera, I got return values of cameraMatrix and distCoeffs which then be passed to cv2.undistort method to perform the undistortion

To demonstrate this step, I will describe how I apply the distortion correction to one of the test images like this one:
![Undistorted Image][Undistorted]

#### 2.  Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.

I used a combination of S channel in HLS color space and L channel in LUV color space to generate a binary image. In the 8th code section, method handleGradient. Here's an example of my output for this step. 

![Gradient Image][gradient]

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.

The code (the 7th code block) for my perspective transform uses cv2.warpPerspective, I calculated the perspective transform parameter using src and dst, along with given image.  I chose the hardcode the source and destination points in the following manner:

```python
w = 1280
h = 720
offsetLeft = 200
offsetRight = 300
offsetTop = 200
offsetBottom = 10

src = [(582, 460), (705, 460), (1015, 667), (292, 667)]
dst = np.float32(
np.float32([[offsetLeft, offsetTop], [w - offsetRight, offsetTop],[w - offsetRight, h - offsetBottom], [offsetLeft, h - offsetBottom]])
)
```

This resulted in the following source and destination points:

| Source        | Destination   | 
|:-------------:|:-------------:| 
| 582, 460      | 200, 200      | 
| 705, 460      | 980, 200      |
| 1015, 667     | 980, 710      |
| 292, 667      | 200, 710      |

I verified that my perspective transform was working as expected by drawing the `src` and `dst` points onto a test image and its warped counterpart to verify that the lines appear parallel in the warped image.

![Wrapped][wrapped]

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?

In the 10th code section, I have a method called `find_lines` using polynomial to find lines like this:

![FindLines][findLines]

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

I use the following code to calculate the curvature. It is in `calcCurvature` function in AdLan.ipynb 11st code section. 

```python

    arr_fit = np.polyfit(arry, arrx, 2)
    arr_fitx = arr_fit[0] * arry ** 2 + arr_fit[1] * arry + arr_fit[2]
    arrx_int = arr_fit[0] * h ** 2 + arr_fit[1] * h  + arr_fit[2]
    arrx = np.append(arrx, arrx_int)
    arry = np.append(arry, h)
    arrx = np.append(arrx, arr_fit[2])
    arry = np.append(arry, 0)
    lsort = np.argsort(arry)
    arry = arry[lsort]
    arrx = arrx[lsort]
    arr_fit = np.polyfit(arry, arrx, 2)
    arr_fitx = arr_fit[0] * arry ** 2 + arr_fit[1] * arry + arr_fit[2]
    arr_fit_cr = np.polyfit(arry * ym_per_pix, arrx * xm_per_pix , 2)
    
    arr_curverad = ((1 + (2*arr_fit_cr[0]*np.max(arry) + arr_fit_cr[1])**2)**1.5) \
                                 /np.absolute(2*arr_fit_cr[0])
```
![Curv][curv]

#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

I implemented this step in AdLan.jpynb in the function `display_lane()` in code section 12th.  Here is an example of my result on a test image:


![Pave Lane][paveLane]
---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).

Here's a [link to my video result](./output_images/project_output.mp4)

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

This algorithm does not work well when there is suddent change in the light condition, such as drive in and out shadow. 

Also, if the traffic line is not clear, there will also be some problem.

Here I'll talk about the approach I took, what techniques I used, what worked and why, where the pipeline might fail and how I might improve it if I were going to pursue this project further.  
1. Normalize the light effect, use relative color compare with the neighbor, so that light and dark area will be treated equally.
2. Try always keep traffic light on, and always use the low threshold based on traffic light. 
