#**Behavioral Cloning** 

---

**Behavioral Cloning Project**

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report


[//]: # (Image References)

center.png            flip2.png            recover_from_left.png
flip1.png            recover_from_drive_to_right.png    recover_from_right.png

[image1]: ./images/center.png "Driving in the center"
[image2]: ./images/right_2017_04_12_21_26_44_192.jpg "right camera"
[image3]: ./images/center_2017_04_12_21_26_44_192.jpg "center camera"
[image4]: ./images/left_2017_04_12_21_26_44_192.jpg "left camera"
[image5]: ./images/recover_from_left.png "recover from left"
[image6]: ./images/recover_from_right.png "recover from right"

## Rubric Points
###Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
###Files Submitted & Code Quality

####1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* writeup_report.md summarizing the results

####2. Submission includes functional code
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

####3. Submission code is usable and readable

The model.py file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.

###Model Architecture and Training Strategy

####1. An appropriate model architecture has been employed

My model consists of a convolution neural network with 5x5 and 3x3 filter sizes and depths between 24 and 64 (model.py lines 50-54) 

The model includes RELU layers to introduce nonlinearity (code line 50-54), and the data is normalized in the model using a Keras lambda layer (code line 48). 

####2. Attempts to reduce overfitting in the model

The model contains dropout layers in order to reduce overfitting (model.py lines 59). 

The model was trained and validated on different data sets to ensure that the model was not overfitting (code line 63). The model was tested by running it through the simulator and ensuring that the vehicle could stay on the track.

####3. Model parameter tuning

The model used an adam optimizer, so the learning rate was not tuned manually (model.py line 62).

####4. Appropriate training data

Training data was chosen to keep the vehicle driving on the road. I used a combination of center lane driving, recovering from the left and right sides of the road ... 

For details about how I created the training data, see the next section. 

###Model Architecture and Training Strategy

####1. Solution Design Approach

The overall strategy for deriving a model architecture was to ...

My first step was to use a convolution neural network model similar to the NVIDIA Architecture. I thought this model might be appropriate because it drove the best in the video.

In order to gauge how well the model was working, I split my image and steering angle data into a training and validation set. I found that my first model had a low mean squared error on the training set but a high mean squared error on the validation set. This implied that the model was overfitting. 

To combat the overfitting, I augment the image, flip it horizontally to balance the number of left and right turn.

Then I drove a little bit longer in the training model so that there are more data. 

The final step was to run the simulator to see how well the car was driving around track one. There were a few spots where the vehicle fell off the track,  to improve the driving behavior in these cases, I drove multiple times on those spots in the training mode.

At the end of the process, the vehicle is able to drive autonomously around the track without leaving the road.

####2. Final Model Architecture

The final model architecture (model.py lines 42-63) consisted of a convolution neural network with the following layers and layer sizes: 
 
##### 1. Normalize Layer, get in input size (160 * 320 with color depth 3)
##### 2. Image cropping layer, which only take the middel 20% to 55%, to make the data more irrevalent to the road environment.
##### 3. Convolution layer, 5 * 5 with feature depth 24, then using relu to normalize the middle result
##### 4. Convolution layer, 5 * 5 with feature depth 36, then using relu to normalize the middle result
##### 5. Convolution layer, 5 * 5 with feature depth 48, then using relu to normalize the middle result
##### 6. Convolution layer, 5 * 5 with feature depth 64, then using relu to normalize the middle result
##### 7. Convolution layer, 5 * 5 with feature depth 64, then using relu to normalize the middle result
##### 8. Flatten to one dimension
##### 9. Dense result to 100 output
##### 10. Dense result to 50 output
##### 11. Dense result to 10 output
##### 12. Dropout 20% of the data to make it more general, and void overfitting.
##### 13. Dense result to 1 output


####3. Creation of the Training Set & Training Process

To capture good driving behavior, I first recorded two laps on track one using center lane driving. Here is an example image of center lane driving:

![alt text][image1]

I then recorded the vehicle recovering from the left side and right sides of the road back to center so that the vehicle would learn to recover from road side to the center of the road. These images show what a recovery looks like starting from road side.

![alt text][image5]
![alt text][image6]

Then I repeated this process on track two in order to get more data points.

To augment the data sat, I also use pictures from left camera, center camera, and right camera along with sheer offset. For example, here is an image that has then been flipped:

![alt text][image2]
![alt text][image3]
![alt text][image4]

After the collection process, I had 11716 number of data points. I then preprocessed this data by horizontal flip the data to get more data points, and balance the right/left turns.

I finally randomly shuffled the data set and put 30% of the data into a validation set. 

I used this training data for training the model. The validation set helped determine if the model was over or under fitting. The ideal number of epochs was 10 as evidenced by validation loss start flatten out. I used an adam optimizer so that manually training the learning rate wasn't necessary.
