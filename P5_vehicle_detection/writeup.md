**Vehicle Detection Project**

The goals / steps of this project are the following:

* Perform a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a MLPClassifier.
* Optionally, you can also apply a color transform and append binned color features, as well as histograms of color, to your HOG feature vector. 
* Note: for those first two steps don't forget to normalize your features and randomize a selection for training and testing.
* Implement a sliding-window technique and use your trained classifier to search for vehicles in images.
* Run your pipeline on a video stream (start with the test_video.mp4 and later implement on full project_video.mp4) and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.

[//]: # (Image References)
[image0]: ./output_images/car.png
[image1]: ./output_images/non_car.png
[image2]: ./output_images/car_hog.png
[image3]: ./output_images/non_car_hog.png
[image4]: ./output_images/detected.png
[image5]: ./output_images/img1.png
[image6]: ./output_images/img2.png
[image7]: ./output_images/img3.png
[image8]: ./output_images/img4.png
[image9]: ./output_images/img5.png
[image10]: ./output_images/img6.png
[image11]: ./output_images/box1.png
[image12]: ./output_images/box2.png
[image13]: ./output_images/box3.png
[image14]: ./output_images/box4.png
[image15]: ./output_images/box5.png
[image16]: ./output_images/box6.png
[image17]: ./output_images/heat1.png
[image18]: ./output_images/heat2.png
[image19]: ./output_images/heat3.png
[image20]: ./output_images/heat4.png
[image21]: ./output_images/heat5.png
[image22]: ./output_images/heat6.png
[video1]: ./project_video.mp4

## [Rubric](https://review.udacity.com/#!/rubrics/513/view) Points
###Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
###Writeup / README

####1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Vehicle-Detection/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

You're reading it!

###Histogram of Oriented Gradients (HOG)

####1. Explain how (and identify where in your code) you extracted HOG features from the training images.

The code for this step is contained in the second code cell of the IPython notebook, in function get_hog_features. 

In the 4th code section, I started by reading in all the `vehicle` and `non-vehicle` images.  Here is an example of one of each of the `vehicle` and `non-vehicle` classes:

![vehicle][image0]
![non-vehicle][image1]

I then explored different color spaces and different `skimage.hog()` parameters (`orientations`, `pixels_per_cell`, `hog_channel` and `cells_per_block`).  I grabbed random images from each of the two classes and displayed them to get a feel for what the `skimage.hog()` output looks like.

Here is an example using the `YCrCb` color space and HOG parameters of `orientations=9`, `pixels_per_cell=(8, 8)` and `cells_per_block=(2, 2)`, `hog channel 'ALL'`.:


Vehicle
![car_hog][image2]

Non-Vehicle
![non_car_hog][image3]

####2. Explain how you settled on your final choice of HOG parameters.

I tried various combinations of parameters and tested in the classifier, I found that orient == 9, pix_per_cell == 8, cell_per_block = 4 give me the best test result, 99.2%

####3. Describe how (and identify where in your code) you trained a classifier using your selected HOG features (and color features if you used them).

In 5th code section, I trained a classifier using MLPClassifier with default parameters. Color space is `YCrCb`. The feature is the hog features only.

###Sliding Window Search

####1. Describe how (and identify where in your code) you implemented a sliding window search.  How did you decide what scales to search and how much to overlap windows?

In the 6th code section, I call slide_window defined in the 2nd code section. Using the following sizes, `(64, 96, 128, 196)`, using overlap 0.8. I found these parameters by testing with several testing images.

####2. Show some examples of test images to demonstrate how your pipeline is working.  What did you do to optimize the performance of your classifier?

Ultimately I searched on four scales using YCrCb ALL-channel HOG features, which provided a nice result.  Here are some example images:

![Detected][image4]
---

### Video Implementation

####1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (somewhat wobbly or unstable bounding boxes are ok as long as you are identifying the vehicles most of the time with minimal false positives.)
Here's a [link to my video result](output_images/project_output.mp4)


####2. Describe how (and identify where in your code) you implemented some kind of filter for false positives and some method for combining overlapping bounding boxes.

I recorded the positions of positive detections in each frame of the video.  From the positive detections I created a heatmap and then thresholded that map to identify vehicle positions.  I then used `scipy.ndimage.measurements.label()` to identify individual blobs in the heatmap.  I then assumed each blob corresponded to a vehicle.  I constructed bounding boxes to cover the area of each blob detected.  

Here's an example result showing the heatmap from a series of frames of video, the result of `scipy.ndimage.measurements.label()` and the bounding boxes then overlaid on the last frame of video:


### Here are six frames and their corresponding heatmaps:

![image1][image5] ![image1][image11] 
![image2][image6] ![image2][image12]
![image3][image7] ![image3][image13] 
![image4][image8] ![image4][image14]
![image5][image9] ![image5][image15] 
![image6][image10] ![image6][image16]

### Here is the output of `scipy.ndimage.measurements.label()` on the integrated heatmap from all six frames:
![image1][image17] 
![image2][image18]
![image3][image19] 
![image4][image20]
![image5][image21] 
![image6][image22]

### Here the resulting bounding boxes are drawn onto the last frame in the series:
![image6][image16]



---

###Discussion

####1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

Here I'll talk about the approach I took, what techniques I used, what worked and why, where the pipeline might fail and how I might improve it if I were going to pursue this project further.  


I trained all the data, although the testing result is about 99.5% correct, it does not work well in the real video. I think that could because the training image on vehicles are whole vehicle, while search on the real image usually has offset, even I set the overlap to 0.8. 

I need to process the training/testing vehicle images, create it with offset, either x, or y. 
