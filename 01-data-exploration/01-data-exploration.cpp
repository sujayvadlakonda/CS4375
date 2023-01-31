#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

void printTitle(std::string title) {
  std::cout << "Statistics for " << std::endl;
}

void printNamedNumber(std::string name, double number) {
  std::cout << name << ": " << number << std::endl;
}

void printBlankLine() {
  std::cout << std::endl;
}

double sum(std::vector<double> vector) {
  double sum = 0;
  for (size_t index = 0; index < vector.size(); index++)
    sum += vector[index];
  return sum;
}

double computeMean(std::vector<double> vector) {
  return sum(vector) / vector.size();
}

double median(std::vector<double> vector) {
  sort(vector.begin(), vector.end());
  if (vector.size() % 2 == 0) {
    double left_median = vector[(vector.size() - 1) / 2];
    double right_median = vector[vector.size() / 2];
    return (left_median + right_median) / 2;
  }
  return vector[vector.size() / 2];
}

double range(std::vector<double> vector) {
  double minimum = vector[0];
  double maximum = vector[0];
  for (size_t index = 0; index < vector.size(); index++) {
    if (vector[index] < minimum)
      minimum = vector[index];
    if (vector[index] > maximum)
      maximum = vector[index];
  }

  return maximum - minimum;
}

void printVector(std::vector<double> vector, std::string vectorName) {
  printTitle(vectorName);
  printNamedNumber("Sum", sum(vector));
  printNamedNumber("Mean", computeMean(vector));
  printNamedNumber("Median", median(vector));
  printNamedNumber("Range", range(vector));
  printBlankLine();
}

double computeCovariance(std::vector<double> a, std::vector<double> b) {
  double covariance = 0;
  double a_mean = computeMean(a);
  double b_mean = computeMean(b);

  for(size_t index = 0; index < a.size(); index++) {
    double a_diff = a[index] - a_mean;
    double b_diff = b[index] - b_mean;
    covariance = covariance + (a_diff * b_diff);
  }
  return covariance / (a.size() - 1);
}

double computeCorrelation(std::vector<double> a, std::vector<double> b) {
  double covariance = computeCovariance(a, b);
  double sigma_a = sqrt(computeCovariance(a, a));
  double sigma_b = sqrt(computeCovariance(b, b));
  return covariance / (sigma_a * sigma_b);
}

int main(int argc, char **argv) {
  std::ifstream file;
  std::string line;
  std::string rm_in, medv_in;
  const int MAX_LEN = 1000;
  std::vector<double> rm(MAX_LEN);
  std::vector<double> medv(MAX_LEN);

  std::cout << "Opening file" << std::endl;

  file.open("Boston.csv");
  if (!file.is_open()) {
    std::cout << "Could not open Boston.csv" << std::endl;
    return 1;
  }

  std::cout << "Reading line 1" << std::endl;
  getline(file, line);
  std::cout << "Heading: " << line << std::endl;

  int num_observations = 0;
  while (file.good()) {
    getline(file, rm_in, ',');
    getline(file, medv_in, '\n');

    rm.at(num_observations) = stof(rm_in);
    medv.at(num_observations) = stof(medv_in);

    num_observations++;
  }

  rm.resize(num_observations);
  medv.resize(num_observations);

  std::cout << "New length: " << num_observations << std::endl;

  printVector(rm, "rm");
  printVector(medv, "medv");

  double covariance = computeCovariance(rm, medv);
  printNamedNumber("Covariance", covariance);
  printBlankLine();

  double correlation = computeCorrelation(rm, medv);
  printNamedNumber("Correlation", correlation);
  printBlankLine();

  return 0;
}

