#**Finding Lane Lines on the Road** 

##Udacity Self Drive Course Project 1

---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report


[//]: # (Image References)

[image1]: ./examples/grayscale.jpg "Grayscale"
[blurImage]: ./middle_steps/blur.png "Gaussian Blur"
[edgeImage]: ./middle_steps/edge.png "Canny Edge"

---

### Reflection

###1. Description of this Pipeline 

My pipeline consisted of 5 steps. 
First, I converted the images to grayscale,

![gray image][image1]

Then I call use Gaussian Blur to smooth the image

![Blur Image][blurImage]

After that, I called canny to get the edge information

![Edge image][edgeImage]

In order to draw a single line on the left and right lanes, I modified the draw_lines() function by ...

If you'd like to include images to show how the pipeline works, here is how to include an image: 



###2. Identify potential shortcomings with your current pipeline


One potential shortcoming would be what would happen when ... 

Another shortcoming could be ...


###3. Suggest possible improvements to your pipeline

A possible improvement would be to ...

Another potential improvement could be to ...
