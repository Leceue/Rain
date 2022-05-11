# Standard library
import random
from tkinter import NW

# Third-party libraries
import numpy as np

def sigmoid (z):
        return 1.0/(1.0 + np.exp(-z))

class Network(object):

    def __init__(self, sizes):
        self.num_layers = len(sizes)
        self.sizes = sizes
        self.biases = [np.random.randn(y, 1) for y in sizes[1:]]
        self.weights = [np.random.randn(y, x) for x, y in zip(sizes[:-1],sizes[1:])]

    

    def feedforward (self, a):
        """ Return the output of the network if "a" is input. """
        for b, w in zip(self.biases,self.weights):
            a=sigmoid(np.dot(w,a)+b)
        return a

    def SGD(self, training_data, epochs, mini_batch_size, eta, test_data=None):
        """训练方法使用随机梯度下降的方法,“training_data”是一个元组包含训练输入和期望输出
        epoch 和 mini_batch_size 是所期望的训练的epoch以及随机梯度想要采用的一次训练较小批次
        的大小, eta是学习速率 η , 而test_data如果提供，将会反馈每个训练周期后的评估，输出进度"""

        if test_data: 
            n_test = len(training_data)
        n = len(training_data)
        for j in range(epochs):
            random.shuffle(training_data)
            mini_batches = [
                training_data[k:k+mini_batch_size]
                for k in range(0, n, mini_batch_size)
            ]
            for mini_batch in mini_batches:
                self.update_mini_batch(mini_batch, eta)
            if test_data:
                print("Epoch {0}: {1} / {2}".format(j, self.evaluate(test_data), n_test))
            else:
                print("Epoch {0} complete".format(j))
    def update_mini_batch(self, mini_batch, eta):
        nabla_b = [np.zeros(b.shape) for b in self.biases]
        nabla_w = [np.zeros(w.shape) for w in self.weights]
        for x,y in mini_batch:
            delta_nabla_b,delta_nabla_w=self.backprop(x,y)
            nabla_b=[nb+dnb for nb,dnb in zip(nabla_b,delta_nabla_b)]
            nabla_w=[nw+dnw for nw,dnw in zip(nabla_w,delta_nabla_w)]
        self.weights=[w-(eta/len(mini_batch))*nw 
                        for w,nw in zip(self.weights,nabla_w)]
        self.biases=[b-(eta/len(mini_batch))*nb 
                        for b,nb in zip(self.biases,nabla_b)]
        




    
    