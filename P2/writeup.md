#**Traffic Sign Recognition** 
#V 0.1
---

**Build a Traffic Sign Recognition Project**

The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./diagram.png "histogram"
[image2]: ./colorImg.png "Color Image"
[image3]: ./grayImg.png "Grayed out image"
[image4]: ./sign1.png "Traffic Sign 1"
[image5]: ./sign2.png "Traffic Sign 2"
[image6]: ./sign3.png "Traffic Sign 3"
[image7]: ./sign4.png "Traffic Sign 4"
[image8]: ./sign5.png "Traffic Sign 5"

## Rubric Points
###Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and describe how I addressed each point in my implementation.  

---
###Writeup / README

####1. Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf. You can use this template as a guide for writing the report. The submission includes the project code.

You're reading it! and here is a link to my [project code](Traffic_Sign_Classifier.ipynb)

###Data Set Summary & Exploration

####1.  The following is the data summary. Analysis is done using python, numpy.

The code for this step is contained in the second code cell of the IPython notebook.  

I used the pandas library to calculate summary statistics of the traffic
signs data set:

* The size of training set is ? 34799
* The size of test set is ? 12630
* The shape of a traffic sign image is ? (32, 32, 3)
* The number of unique classes/labels in the data set is ? 43

####2. Include an exploratory visualization of the dataset and identify where the code is in your code file.

The code for this step is contained in the third code cell of the IPython notebook.  

Here is an exploratory visualization of the data set. It is a bar chart showing how the data is distributed across different traffic signs.

![alt text][image1]

###Design and Test a Model Architecture

####1. The pre-process of the data set is in the code block [4], I use the following code block to gray out the data set. Which is convert the image matrics from [32, 32, 3] to [32, 32, 1]. 

The code for this step is contained in the fourth code cell of the IPython notebook.

As a first step, I decided to convert the images to grayscale so that the training model can detect the same sign with different color.

Here is an example of a traffic sign image before and after grayscaling.

![alt text][image2]
![alt text][image3]

As a last step, I normalized the image data because with bright environment, the gray level of the traffic sign color tends to be bigger, and in dark environment, the gray level of traffic sign tends to be smaller.

####2. Process summary

Since data zip file provided training, validation, and testing data, I created three array to hold image data from those three sets. There are 34799 data in training set, 12630 in testing set, and 4410 data in the validation set. I am using default files for three sets, but if I have only one set of data, I could create three sets of data via randomly picking the data.


My final training set had 34799 number of images for training. My validation set and test set had 12630 and 4410 number of images.

####3.  The code for my final model is located in the fifth cell of the ipython notebook. 


My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 32x32x1 RGB image   							| 
| Convolution 5x5     	| 1x1 stride, valid padding, outputs 28x28x6	|
| Sigmoid
| Max Pooling | 2x2
| Convolution 5x5     	| 1x1 stride, valid padding, outputs 14x14x16 	|
| Sigmoid
| Max Pooling | 2x2
| Flatten
| Fully connected		| Input 400, output 120
| Sigmoid
| Fully connected		| Input 120, output 86
|						|												|
 


####4. To train the model, I used an LeNet with adjusted activate functions. 

Then I adjusted the activation method, from RELU to sigmoid, the performance has been improved significantly. It even reached 95% in the training set. After I realize that I need to use Normalized function, I first tried to divide all the pixel by 255, and result is about 88%. After consulted with my mentor, I run normalize function on each image, find the min and max gray value of the image, and use the formular of (pixelValue - min) / (max - min), if max - min less than 1.0, I default use 255.0 as divisor. Then the accuracy raise to about 90%. In further, I adjusted the sigma, tested several values from 0.25 to 0.5, and found that the 0.28 provided the best training result, which is 94.8%. I tried several values for the parameters, such as learning rate, batch size, and epoches. I found that learning rate 0.001 is the best in multiple scenarios; batch size does not make big difference; the number of epoches helps increase the accuracy until reaches peak accuracy. The testing result is also reaches 91%


The code for training the model is located in the fifth and sixth cell of the ipython notebook. 


My final model results were:
* training set accuracy of 0.998 
* validation set accuracy of ? 0.939
* test set accuracy of ? 0.914

If an iterative approach was chosen:
* What was the first architecture that was tried and why was it chosen?
At beginning, I used the raw value of gray pixel in LeNet, reduce the learning rate to 0.001, the accuracy of the first 10 epochs were keeping at 5%.

* What were some problems with the initial architecture?
1. Seems the activation function of RELU does not work well, I guess maybe because my input data range is 0-255, while the output of RELU just return the original value when the value is larger than 1. After I changed the activation method to sigmoid, the accuracy increased to about 90%
2. Then after I read the purpose of normalization, I think I should have normalized the input data. 

* How was the architecture adjusted and why was it adjusted? Typical adjustments could include choosing a different model architecture, adding or taking away layers (pooling, dropout, convolution, etc), using an activation function or changing the activation function. One common justification for adjusting an architecture would be due to over fitting or under fitting. A high accuracy on the training set but low accuracy on the validation set indicates over fitting; a low accuracy on both sets indicates under fitting.

1. Seems drop-out does not give me better solution, so I removed drop-out
2. I also tried max pooling with 4X4, 3X3, all seems not work well.
3. I am using two layer of fully connected, seems works better.
4. Since there are 43 different types of traffic sign, I changed the output size to 43 for the logits result.


* Which parameters were tuned? How were they adjusted and why?
1. At beginning, I use pixel as input, then after switched to normalized input, I found that the sigma should be bigger. After tested several values from 0.25 to 0.5, I found 0.28 provided the best training result.
2. I also try to adjust the number of epoch, seems after certain stage, more epoch won't provide better result, it may also cause over-hitting, which reduced accuracy of testing.
3. I tried to adjust mu, would like it to be 0.5, but does not get good result.
4. Original rate was 0.01, I reduced it along with increased epoch. I have feeling that rate x epoch would be consistent, lower rate will prevent missing global optimized value.

* What are some of the important design choices and why were they chosen? For example, why might a convolution layer work well with this problem? How might a dropout layer help with creating a successful model?
I didn't use dropout layer. I guess my data set may not bigger enough, after drop some data, the model become less accurate. And seems even without dropout, the accuracy rate between training set and testing set is acceptable, on the other words, I am not seeing big over-fit problem without dropout.


If a well known architecture was chosen:
* What architecture was chosen?
Current LeNet with two convoluted layer along with two fully-connected layer. Use sigmoid as activation method, and use max pooling. 

* Why did you believe it would be relevant to the traffic sign application?
I think traffic sign is shape significant, such as circle, triangle, and so on. So two levels of convoluted is good enough. 

* How does the final model's accuracy on the training, validation and test set provide evidence that the model is working well?
training accuracy is 99.8%, validation is 93.9%, and testing is 91.4% 

###Test a Model on New Images

####1. Choose five German traffic signs found on the web and provide them in the report. For each image, discuss what quality or qualities might be difficult to classify.

Here are five German traffic signs that I found on the web:

![alt text][image4] ![alt text][image5] ![alt text][image6] 
![alt text][image7] ![alt text][image8]

####2.  The code for making predictions on my final model is located in last code cells (12, 13, 14, 15) of the Ipython notebook.

Here are the results of the prediction:

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 70      		|  70
| Bicycle Laws      		|  Circle drive
| General caution  | General caution 
| Dangerous curve to the left		| Go straight or left
| Prohibition of motor vehicles with a gross vehicle weight of over 3.5 metric tons including their trailers and tractors, other than passenger cars, buses and coaches			| Prohibition of motor vehicles with a gross vehicle weight of over 3.5 metric tons including their trailers and tractors, other than passenger cars, buses and coaches      							|


The model was able to correctly guess 3 of the 5 traffic signs, which gives an accuracy of 60%. This compares favorably to the accuracy on the test set of 91%.

