import gzip
import struct
import tensorflow as tf
import numpy as np

# MNIST data is stored in binary format, 
# and we transform them into numpy ndarray objects by the following two utility functions
def read_image(file_name):
    with gzip.open(file_name, 'rb') as f:
        buf = f.read()
        index = 0
        magic, images, rows, columns = struct.unpack_from('>IIII' , buf , index)
        index += struct.calcsize('>IIII')

        image_size = '>' + str(images*rows*columns) + 'B'
        ims = struct.unpack_from(image_size, buf, index)
        
        im_array = np.array(ims).reshape(images, rows, columns)
        return im_array

def read_label(file_name):
    with gzip.open(file_name, 'rb') as f:
        buf = f.read()
        index = 0
        magic, labels = struct.unpack_from('>II', buf, index)
        index += struct.calcsize('>II')
        
        label_size = '>' + str(labels) + 'B'
        labels = struct.unpack_from(label_size, buf, index)

        label_array = np.array(labels)
        return label_array

'''print "Start processing MNIST handwritten digits data..."
train_x_data = read_image("MNIST_data/train-images-idx3-ubyte.gz")
train_x_data = train_x_data.reshape(train_x_data.shape[0], -1).astype(np.float32)
train_y_data = read_label("MNIST_data/train-labels-idx1-ubyte.gz")
test_x_data = read_image("MNIST_data/t10k-images-idx3-ubyte.gz")
test_x_data = test_x_data.reshape(test_x_data.shape[0], -1).astype(np.float32)
test_y_data = read_label("MNIST_data/t10k-labels-idx1-ubyte.gz")

train_x_minmax = train_x_data / 255.0
test_x_minmax = test_x_data / 255.0'''



def add_layer(inputs, in_size, out_size, activation_function=None):
    # add one more layer and return the output of this layer
    with tf.name_scope("Layer"):
        with tf.name_scope("Weight"):
            weights = tf.Variable(tf.random_normal([in_size, out_size]))
        with tf.name_scope("Biases"):
            biases = tf.Variable(tf.zeros([1, out_size]) + 0.1)
        with tf.name_scope("Wx_plus_b"):
            Wx_plus_b = tf.matmul(inputs, weights) + biases
            tf.summary.histogram('pre_activations', Wx_plus_b)
        if activation_function is None:
            outputs = Wx_plus_b
        else:
            with tf.name_scope(activation_function.func_name):
                outputs = activation_function(Wx_plus_b)
        tf.summary.histogram('activations', outputs)
        return outputs

# Train Data
x_data = np.linspace(-1, 1, 300)[:, np.newaxis]
noise = np.random.normal(0, 0.05, x_data.shape)
y_data = np.square(x_data) - 0.5 + noise
with tf.name_scope("Input"):
    xs = tf.placeholder(tf.float32, (None, 1))
with tf.name_scope("Output"):
    ys = tf.placeholder(tf.float32, (None, 1))

# Nerual Network
l1 = add_layer(xs, 1, 10, activation_function=tf.nn.relu)
prediction = add_layer(l1, 10, 1, activation_function=None)

# Caculate Loss
with tf.name_scope('loss'):
    loss = tf.reduce_mean(tf.reduce_sum(
        tf.square(ys - prediction), reduction_indices=[1]))
    tf.summary.scalar('loss', loss)

# Train Func
with tf.name_scope('train'):
    optimizer = tf.train.GradientDescentOptimizer(0.1)
    train = optimizer.minimize(loss)

# Init Variables
init = tf.global_variables_initializer()
# Open TensorBoard Session
sess = tf.Session()
# Merged All Summary
merged = tf.summary.merge_all()
# TensorBoard Summary Out
writer = tf.summary.FileWriter("logs/", sess.graph)
# Init
sess.run(init)

#Start Training...
for i in range(1000):
    sess.run(train, feed_dict={xs: x_data, ys: y_data})
    if i % 10 == 0:
        rs = sess.run(merged, feed_dict={xs: x_data, ys: y_data})
        writer.add_summary(rs, i)
