House Price Prediction:
- Single neuron doing size to prize model
- Single computation merging inputs to a new value
- Combine neurons together to make a neural network
- Implement by giving a set of inputs and an output. rest works out by training
- Top layer takes all inputs, each node below takes all input from layer above
- Densely connected

Virtually all supervised learning

Neural network architecture may need to be different for each application
- Price and Click prediction standard NN
- Image use Computational Neural Network
- Sequence data use Recurrent NN

Traditional models (Support Vector Machine, Logistic or Linear regression) plateau at large data amounts
Popularity rising due to large amount of (labelled) data and ability to train a large scale NN
With small amounts of data much more variable which one is best
Computation increase makes iteration cycle much shorter

Activation functions:
- Sigmoid function
- ReLU - Recitifed Linear Unit
- ReLU makes gradient descent work much faster. The sigmoid derivative close to 0 at wings

## Notation
- Input is a Feature Vector (*X*) of size *n* or *n<sub>x</sub>*
- Output is a 1 or 0
- Training set has *m* examples - *{X<sub>i</sub>, y<sub>i</sub>}*
- Mathematically useful to have X training vectors as columns in a n x m - Matrix with target 1 x M matrix of Y values

## Logistic Regression in a Neural Network
- Binary classification (e.g. Cat or not Cat)
- Given X want to get Y^ = probability the y = 1 given X (between 0 and 1)
- z = w^T X + b where w is a n-vector
- Sigmoid function = 1 / (1 + e(-z))
- Goal is to find w and b
- Loss function as square error isn't good for Logistic regression as creates local minima that gradient descent might find
- Better one of logistic regression = y log(y^) + (1-y) log (1-y^)
- For full set of training sum and divide by m, called the cost function (J)

- Gradient descent w = w - alpha dJ(w) / dw
- alpha is the learning rate