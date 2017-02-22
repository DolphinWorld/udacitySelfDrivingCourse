#**Finding Lane Lines on the Road** 

##Udacity Self Drive Course Project 1

---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report


[//]: # (Image References)

[image1]: ./middle_steps/gray.png "Grayscale"
[blurImage]: ./middle_steps/blur.png "Gaussian Blur"
[edgeImage]: ./middle_steps/edges.png "Canny Edge"
[regionEdgesImage]: ./middle_steps/region_edges.png "Region edges"
[HoughImage]: ./middle_steps/hough.png "Houge Image"
[ResultImage]: ./middle_steps/result.png "Result Image"

---

### Reflection

###1. Description of this Pipeline 

My pipeline consisted of 5 steps. 
First, I converted the images to grayscale,

![gray image][image1]

Secondary I use Gaussian Blur method to blur and smooth the image

![Blur Image][blurImage]

After that, I called canny to get the edge information

![Edge image][edgeImage]

To filter out edges not related to the traffic line, I filter the edges image use a trapezoid.

![Region Edge image][regionEdgesImage]

With the final edges mask page, I called hough lines to calculate the traffic lines;

![Hough image][HoughImage]

At the end, use weighted_img function to combine the original image and the traffic line image, and generated the following result image. 

![Result image][ResultImage]

In order to draw a single line on the left and right lanes, I modified the draw_lines() function by ...

If you'd like to include images to show how the pipeline works, here is how to include an image: 



###2. Identify potential shortcomings with your current pipeline


One potential shortcoming would be what would happen when ... 

Another shortcoming could be ...


###3. Suggest possible improvements to your pipeline

A possible improvement would be to ...

Another potential improvement could be to ...
