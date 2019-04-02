#include <iostream>
#include <vector>
#include <armadillo>
#include <cmath>
#include <float.h>
#include <time.h>

#include "learningvector.h"
#include "sequenceprediction.h"

using namespace std;
using namespace arma;

#define INPUT_SIZE 16

SequencePrediction::SequencePrediction(){
    T = 0;
    initANN();
    createWeightMatrix();
    makeSequence();
}

void SequencePrediction::initANN(){
    /*cout << "Enter number of neuron for first layer(n):" << endl;
    cin >> n;
    cout << "Enter number of neuron for second layer(m):" << endl;
    cin >> m;
    cout << "Enter error degree(e):" << endl;
    cin >> e;
    cout << "Enter step(a):" << endl;
    cin >> a;
    cout << "Enter max number iteration:" << endl;
    cin >> maxIteration;
    cout << "Enter the number of predicted numbers:" << endl;
    cin >> predict;*/
	n = 2; m = 4; e = 0.0048; a = 0.000001; maxIteration = 1000000000; predict = 10;
}

void SequencePrediction::run(){
	//setup input for testing prediciton
	//use different part of the sequence for prediction

	double sequence1[4] = { 987, 1597, 2584, 4181};

	for (int index = 0; index < 4 - n; index++) {
		LearningVector vector;
		for (int offset = 0; offset < n; offset++) {
			vector.addElement(sequence1[index + offset]);
		}
		vector.createMatrixX();
		vector.setAnswer(sequence1[index + n]);
		inputVectors1.push_back(vector);
	}
	double sequence2[4] = { 6765, 10946, 17711, 28657};

	for (int index = 0; index < 4 - n; index++) {
		LearningVector vector;
		for (int offset = 0; offset < n; offset++) {
			vector.addElement(sequence2[index + offset]);
		}
		vector.createMatrixX();
		vector.setAnswer(sequence2[index + n]);
		inputVectors2.push_back(vector);
	}

	cout << "Learning..." << endl;
	clock_t tStart = clock();
    double E;
    int iteration = 0;
    Ti = mat(1, m, fill::zeros);
    do {
        if (iteration >= maxIteration){
            break;
        }
        E = 0;
        setContextNeuron(0);
        for (int index = 0; index < L; index++){
            mat X = learningVectors.at(index).getX();
            mat P = X * W - Ti;
            mat Y = P * V - T;
            double answer = learningVectors.at(index).getAnswer();
            //debug(X, P, Y, W, V, answer);
            double delta = Y(0,0) - answer;
            setContextNeuron(Y(0,0));
            W = W - (a * delta * X.t() * V.t());
            V = V - (a * delta * P.t());
            // Threshold, It works worse
            //T = T + (a * delta);
            //Ti = Ti + (a * delta * V.t());
        }
        setContextNeuron(0); // calculate error after correction
        for (int index = 0; index < L; index++){
            mat X = learningVectors.at(index).getX();
            mat P = X * W - Ti;
            mat Y = P * V - T;
            double answer = learningVectors.at(index).getAnswer();
            double delta = Y(0,0) - answer;
            setContextNeuron(Y(0,0));
            E += getErrorDegree(delta);
        }
        iteration++;
		if ((iteration % 1000000) == 0) {
			cout << "Iteration: " << iteration << " Error: " << E << endl;
		}
    } while (E > e);
	cout << "Learning finished." << endl;
	
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	cin.get();
	cout << "Learning next batch." << endl;
	tStart = clock();
	//Ti = mat(1, m, fill::zeros);
	iteration = 0;
	do {
		if (iteration >= maxIteration) {
			break;
		}
		E = 0;
		setContextNeuron(0);
		for (int index = 0; index < inputVectors1.size(); index++) {
			mat X = inputVectors1.at(index).getX();
			mat P = X * W - Ti;
			mat Y = P * V - T;
			double answer = inputVectors1.at(index).getAnswer();
			//debug(X, P, Y, W, V, answer);
			double delta = Y(0, 0) - answer;
			setContextNeuron(Y(0, 0));
			W = W - (a * delta * X.t() * V.t());
			V = V - (a * delta * P.t());
			// Threshold, It works worse
			//T = T + (a * delta);
			//Ti = Ti + (a * delta * V.t());
		}
		setContextNeuron(0); // calculate error after correction
		for (int index = 0; index < inputVectors1.size(); index++) {
			mat X = inputVectors1.at(index).getX();
			mat P = X * W - Ti;
			mat Y = P * V - T;
			double answer = inputVectors1.at(index).getAnswer();
			double delta = Y(0, 0) - answer;
			setContextNeuron(Y(0, 0));
			E += getErrorDegree(delta);
		}
		iteration++;
		if ((iteration % 1000000) == 0) {
			cout << "Iteration: " << iteration << " Error: " << E << endl;
		}
	} while (E > e);
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	cout << "Press Enter to predict. Based on 6765, 10946, 17711, 28657" << endl;
	cin.get();
	predictNo(inputVectors1);

	cout << "Learning next batch." << endl;
	tStart = clock();
	//Ti = mat(1, m, fill::zeros);
	iteration = 0;
	do {
		if (iteration >= maxIteration) {
			break;
		}
		E = 0;
		setContextNeuron(0);
		for (int index = 0; index < 4; index++) {
			mat X = inputVectors2.at(index).getX();
			mat P = X * W - Ti;
			mat Y = P * V - T;
			double answer = inputVectors2.at(index).getAnswer();
			//debug(X, P, Y, W, V, answer);
			double delta = Y(0, 0) - answer;
			setContextNeuron(Y(0, 0));
			W = W - (a * delta * X.t() * V.t());
			V = V - (a * delta * P.t());
			// Threshold, It works worse
			//T = T + (a * delta);
			//Ti = Ti + (a * delta * V.t());
		}
		setContextNeuron(0); // calculate error after correction
		for (int index = 0; index < 4; index++) {
			mat X = inputVectors2.at(index).getX();
			mat P = X * W - Ti;
			mat Y = P * V - T;
			double answer = inputVectors2.at(index).getAnswer();
			double delta = Y(0, 0) - answer;
			setContextNeuron(Y(0, 0));
			E += getErrorDegree(delta);
		}
		iteration++;
		if ((iteration % 1000000) == 0) {
			cout << "Iteration: " << iteration << " Error: " << E << endl;
		}
	} while (E > e);
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	cout << "Press Enter to predict. Based on 46368, 75025, 121393, 196418." << endl;
	cin.get();
	predictNo(inputVectors2);    
}

void SequencePrediction::predictNo(vector<LearningVector>& inputVectors){
    cout << endl;
    for (int index = 0; index < n; index++){
        cout << "Input No " << index + 1 << ": "  << inputVectors.at(0).getX()(0, index) << endl;
    }
    setContextNeuron(0);
    for (int index = 0; index < inputVectors.size(); index++){
        mat X = inputVectors.at(index).getX();
        mat P = X * W - Ti;
        mat Y = P * V - T;
        setContextNeuron(Y(0,0));
        cout << "Predict No " << index + n + 1 << ": "  << Y(0,0) << endl;
		debug(X, P, Y, W, V, Y(0, 0));
		cin.get();
    }
    for (int predictNo = 0; predictNo < predict; predictNo++){
        LearningVector vector;
        for (int index = 1; index < n + 1; index++){
            vector.addElement(inputVectors.at(inputVectors.size() - 1).getX()(0,index));
        }
        vector.createMatrixX();
        vector.setContextNeuron(inputVectors.at(inputVectors.size() - 1).getX()(0, n));
		inputVectors.push_back(vector);
        mat X = inputVectors.at(inputVectors.size() - 1).getX();
        mat P = X * W - Ti;
        mat Y = P * V - T;
		
		inputVectors.at(inputVectors.size() - 1).setContextNeuron(Y(0,0));
        cout << "Predict No " << L + predictNo + n + 1 << ": "  << Y(0,0) << endl;
		debug(X, P, Y, W, V, Y(0, 0));
		cin.get();
    }
	cin.get();
}

void SequencePrediction::createWeightMatrix(){
    srand (time(NULL));
    W = randu<mat>(n + 1, m);
    for (int i = 0; i < n + 1; i++){
        for (int j = 0; j < m; j++)
            W(i,j) = (((double)rand() / RAND_MAX) * 2 - 1 ) * 0.1;
    }
    V = randu<mat>(m, 1);
    for (int i = 0; i < n; i++){
        V(i,0) = (((double)rand() / RAND_MAX) * 2 - 1 ) * 0.1;
    }
}

double SequencePrediction::getErrorDegree(double delta){
    return 1./2 * pow(delta, 2);
}

double SequencePrediction::getContextNeuron(){
    return contextNeuron;
}

void SequencePrediction::setContextNeuron(double neuron){
    this->contextNeuron = neuron;
    for (int index = 0; index < L; index++){
        learningVectors.at(index).setContextNeuron(neuron);
    }
}

void SequencePrediction::makeSequence(){
    /*int choose;
    cout << "Choose sequence" << endl;
    cout << "1) Fibonacci number" << endl;
    cout << "2) Periodic function T = 3 (42, -17, 02, 42, ...)" << endl;
    cout << "3) 2^x " << endl;
    cout << "4) x^2" << endl;
    cout << "5) Input yourself" << endl;
    cin >> choose;
    double sequence[16];
    int size;
    switch (choose) {
    case 1:{
        double tmp[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610};
        size = 16;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case 2:{
        double tmp[] =  {42, -17, 02, 42, -17, 02, 42, -17, 02, 42, -17, 02, 42, -17, 02, 42};
        size = 16;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case 3:{
        double tmp[] =  {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
        size = 10;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case 4:{
        double tmp[] =  {1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225, 256};
        size = 16;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case 5:{
        do{
            cout << "Input size sequence (size > n) ";
            cin >> size;
        } while (size < n);
        for (int index = 0; index < size; index++){
            cout << "Input sequence[" << index + 1 << "] : ";
            cin >> sequence[index];
        }
        break;
    }
    default:
        cout << "Wrong number. Exit..." << endl;
        exit(0);
    }*/

	double sequence[16];
	int size;
	double tmp[] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610 };
	size = 16;
	memcpy(sequence, tmp, sizeof(tmp));
    for (int index = 0; index < size - n; index++){
        LearningVector vector;
        for (int offset = 0; offset < n; offset++){
            vector.addElement(sequence[index + offset]);
        }
        vector.createMatrixX();
        vector.setAnswer(sequence[index + n]);
        learningVectors.push_back(vector);
    }
    L = learningVectors.size();
}

void SequencePrediction::debug(mat X, mat P, mat Y, mat W, mat V, double answer){
    cout << "X" << endl;
    cout << X << endl;
    cout << "P" << endl;
    cout << P << endl;
    cout << "Y" << endl;
    cout << Y << endl;
    cout << "W" << endl;
    cout << W << endl;
    cout << "V" << endl;
    cout << V << endl;
    cout << "Real answer: " << answer << "; RNN answer - " << Y(0,0) << endl;
}
