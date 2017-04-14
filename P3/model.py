import csv
import cv2
import numpy as np
lines = []
with open("driving_log.csv") as csvfile:
	reader = csv.reader(csvfile)
	for line in reader :
		lines.append(line)

images = []
measurements = []
for line in lines:
	steering_center = float(line[3])
	correction = 0.5
	steering_left = steering_center + correction
	steering_right = steering_center - correction

	for i in range(3) :
		source_path = line[i]
		filename = source_path.split('/')[-1]
		path = 'IMG/' + filename
		img = cv2.imread(path)
		images.append(img)

	measurements.append(steering_center)
	measurements.append(steering_left)
	measurements.append(steering_right)

argumented_images, argumented_measurements = [], []

for image, measurement in zip(images, measurements) :
	argumented_images.append(image)
	argumented_measurements.append(measurement)
	argumented_images.append(cv2.flip(image, 1))
	argumented_measurements.append(measurement * -1.0)

X_train = np.array(argumented_images)
y_train = np.array(argumented_measurements)

from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Cropping2D
from keras.layers import Convolution2D, Dropout
from keras.layers.pooling import MaxPooling2D


model = Sequential()
model.add(Lambda(lambda x: x/255.0 - 0.5, input_shape=(160,320,3)))
model.add(Cropping2D(cropping=((55,25), (0,0))))

#model.add(Dropout(0.20))
model.add(Convolution2D(24,5,5, subsample=(2,2), activation="relu"))
model.add(Convolution2D(36,5,5, subsample=(2,2), activation="relu"))
model.add(Convolution2D(48,5,5, subsample=(2,2), activation="relu"))
model.add(Convolution2D(64,3,3, activation="relu"))
model.add(Convolution2D(64,3,3, activation="relu"))
model.add(Flatten())
model.add(Dense(100))
model.add(Dense(50))
model.add(Dense(10))
model.add(Dense(1))

model.compile(loss='mse', optimizer='adam')
model.fit(X_train, y_train, validation_split=0.3, shuffle=True, nb_epoch=5)

model.save('model.h5')

