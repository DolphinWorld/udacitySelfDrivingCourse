import csv
import keras
from keras.callbacks import ModelCheckpoint
from keras.callbacks import Callback
import cv2
import numpy as np
from PIL import Image

def readLines() : 
	with open("driving_log.csv") as csvfile:
		reader = csv.reader(csvfile)
		for line in reader :
			yield line;

lines = readLines()

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
                image = Image.open(path)
                img = np.asarray(image)
                images.append(img)

	measurements.append(steering_center)
	measurements.append(steering_left)
	measurements.append(steering_right)

augmented_images, augmented_measurements = [], []

for image, measurement in zip(images, measurements) :
	augmented_images.append(image)
	augmented_measurements.append(measurement)
	augmented_images.append(cv2.flip(image, 1))
	augmented_measurements.append(measurement * -1.0)

X_train = np.array(augmented_images)
y_train = np.array(augmented_measurements)

from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Cropping2D
from keras.layers import Convolution2D, Dropout
from keras.layers.pooling import MaxPooling2D


model = Sequential()
model.add(Lambda(lambda x: x/255.0 - 0.5, input_shape=(160,320,3)))
model.add(Cropping2D(cropping=((55,25), (0,0))))

model.add(Convolution2D(24,5,5, subsample=(2,2), activation="relu"))
model.add(Convolution2D(36,5,5, subsample=(2,2), activation="relu"))
model.add(Convolution2D(48,5,5, subsample=(2,2), activation="relu"))
model.add(Convolution2D(64,3,3, activation="relu"))
model.add(Convolution2D(64,3,3, activation="relu"))
model.add(Flatten())
model.add(Dense(100))
model.add(Dense(50))
model.add(Dense(10))
model.add(Dropout(0.50))
model.add(Dense(1))

model.compile(loss='mse', optimizer='adam')

#class EarlyStoppingByLossVal(Callback):
class EarlyStoppingByLossVal(Callback):
    def __init__(self, monitor='val_loss', value=0.00001, verbose=0):
        super(Callback, self).__init__()
        self.monitor = monitor
        self.value = value
        self.verbose = verbose

    def on_epoch_end(self, epoch, logs={}):
        current = logs.get(self.monitor)
        if current is None:
            warnings.warn("Early stopping requires %s available!" % self.monitor, RuntimeWarning)

        if current < self.value:
            if self.verbose > 0:
                print("Epoch %05d: early stopping THR" % epoch)
            self.model.stop_training = True

kfold_weights_path = "weights-improvement-{epoch:02d}-{val_loss:.2f}.hdf5"
lossCallback = [
    EarlyStoppingByLossVal(monitor='val_loss', value=0.00001, verbose=1),
    ModelCheckpoint(kfold_weights_path, monitor='val_loss', save_best_only=True, verbose=1),
]
model.fit(X_train, y_train, validation_split=0.3, shuffle=True, nb_epoch=10, callbacks = lossCallback)

model.save('model.h5')

