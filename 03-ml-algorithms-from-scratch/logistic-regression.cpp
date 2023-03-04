#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>

const std::string FILE_NAME = "titanic_project.csv";
const double LEARNING_RATE = 0.001;

void openFile(std::ifstream &file) {
  file.open(FILE_NAME, std::ifstream::in);

  if(!file.is_open()) {
    std::cout << "Could not find file named " << FILE_NAME << std::endl;
    exit(1);
  }

  std::cout << "Opened file " << FILE_NAME << std::endl;
}

void tossDataHeader(std::ifstream &file) {
  std::string line;
  getline(file, line);
}

void readFile(std::vector<double> &train_sex, std::vector<double> &train_survived, std::vector<double> &test_sex, std::vector<double> &test_survived) {
  std::ifstream file;
  openFile(file);
  tossDataHeader(file);
  std::string buffer;
  int num_observations = 0;
  while(num_observations < 800) {
    getline(file, buffer, ',');
    getline(file, buffer, ',');
    getline(file, buffer, ',');
    train_survived.at(num_observations) = stof(buffer);
    getline(file, buffer, ',');
    train_sex.at(num_observations) = stof(buffer);
    getline(file, buffer, '\n');
    num_observations = num_observations + 1;
  }
  train_sex.resize(num_observations);
  train_survived.resize(num_observations);

  num_observations = 0;
  while(file.good()) {
    getline(file, buffer, ',');
    getline(file, buffer, ',');
    getline(file, buffer, ',');
    test_survived.at(num_observations) = stof(buffer);
    getline(file, buffer, ',');
    test_sex.at(num_observations) = stof(buffer);
    getline(file, buffer, '\n');
    num_observations = num_observations + 1;
  }
  test_sex.resize(num_observations);
  test_survived.resize(num_observations);
}

double sigmoid(double value) {
  return 1 / (1 + exp(-value));
}

std::vector<double> calculateProbabilities(std::vector<double> train_sex, double slope, double intercept) {
  std::vector<double> probabilities(train_sex.size());

  for(unsigned long i = 0; i < train_sex.size(); i++) {
    double probability = sigmoid((train_sex.at(i) * slope) + intercept);
    probabilities.at(i) = probability;
  }

  return probabilities;
}

std::vector<double> calculateError(std::vector<double> probabilities, std::vector<double> labels) {
  std::vector<double> errors(probabilities.size());

  for(unsigned long i = 0; i < errors.size(); i++) {
    errors[i] = labels[i] - probabilities[i];
  }

  return errors;
}

double sum(std::vector<double> vector) {
  double sum = 0.0;

  for(unsigned long i = 0; i < vector.size(); i++) {
    sum += vector[i];
  }

  return sum;
}

std::vector<double> vector_multiply(std::vector<double> vector_a, std::vector<double> vector_b) {
  std::vector<double> product(vector_a.size());

  for(unsigned long i = 0; i < product.size(); i++) {
    product[i] = vector_a[i] * vector_b[i];
  }

  return product;
}

void logistic_regression(std::vector<double> &train_sex, std::vector<double> &train_survived, double &slope, double &intercept) {
  for(int iteration = 0; iteration < 500000; iteration++) {
    std::vector<double> probabilities = calculateProbabilities(train_sex, slope, intercept);
    std::vector<double> errors = calculateError(probabilities, train_survived);
    intercept += sum(errors) * LEARNING_RATE;
    slope += sum(vector_multiply(train_sex, errors)) * LEARNING_RATE;
    // std::cout << "Iteration " << iteration << " complete!" << std::endl;
  }
}

std::vector<double> calculatePredictions(std::vector<double> sex, double slope, double intercept) {
  std::vector<double> predictions(sex.size());

  for(unsigned long i = 0; i < predictions.size(); i++) {
    double raw = (sex[i] * slope) + intercept;
    double probability = exp(raw) / (1 + exp(raw));
    double prediction = ((probability > 0.5) ? 1 : 0);
    predictions[i] = prediction;
  }

  return predictions;
}

double calculate_accuracy(std::vector<double> predictions, std::vector<double> survived) {
  double correct_count = 0;

  for(unsigned long i = 0; i < predictions.size(); i++) {
    if(predictions[i] == survived[i]) {
      correct_count++;
    }
  }

  return correct_count / predictions.size();
}

double calculate_sensitivity(std::vector<double> predictions, std::vector<double> survived) {
  double true_positive_count = 0;
  double positive_count = 0;

  for(unsigned long i = 0; i < predictions.size(); i++) {
    if(predictions[i] == 1) {
      positive_count++;
      if(survived[i] == 1) {
        true_positive_count++;
      }
    }
  }

  return true_positive_count / positive_count;
}

double calculate_specificity(std::vector<double> predictions, std::vector<double> survived) {
  double true_negative_count = 0;
  double negative_count = 0;

  for(unsigned long i = 0; i < predictions.size(); i++) {
    if(predictions[i] == 0) {
      negative_count++;
      if(survived[i] == 0) {
        true_negative_count++;
      }
    }
  }

  return true_negative_count / negative_count;
}


int main() {
  std::vector<double> train_sex(800);
  std::vector<double> train_survived(800);
  std::vector<double> test_sex(800);
  std::vector<double> test_survived(800);

  readFile(train_sex, train_survived, test_sex, test_survived);

  double slope = 1;
  double intercept = 1;
  std::chrono::steady_clock::time_point training_start_time = std::chrono::steady_clock::now();
  logistic_regression(train_sex, train_survived, slope, intercept);
  std::chrono::steady_clock::time_point training_end_time = std::chrono::steady_clock::now();

  std::cout << "Model Training Time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(training_end_time - training_start_time).count()
            << " milliseconds" << std::endl;


  std::cout << "Slope: " << slope << std::endl;
  std::cout << "Intercept: " << intercept << std::endl;

  std::vector<double> predictions = calculatePredictions(test_sex, slope, intercept);
  double accuracy = calculate_accuracy(predictions, test_survived);
  std::cout << "Accuracy: " << accuracy << std::endl;
  double sensitivity = calculate_sensitivity(predictions, test_survived);
  std::cout << "Sensitivity: " << sensitivity << std::endl;
  double specificity = calculate_specificity(predictions, test_survived);
  std::cout << "Specificity: " << specificity << std::endl;
}
