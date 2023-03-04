#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>

const std::string FILE_NAME = "titanic_project.csv";

std::vector<double> train_sex(800);
std::vector<double> train_pclass(800);
std::vector<double> train_age(800);
std::vector<double> train_survived(800);
std::vector<double> test_sex(800);
std::vector<double> test_pclass(800);
std::vector<double> test_age(800);
std::vector<double> test_survived(800);

double survived_count;
double not_survived_count;

std::vector<double> passenger_class_given_survived(4);
std::vector<double> passenger_class_given_not_survived(4);

double male_given_survived = 0;
double male_given_not_survived = 0;
double female_given_survived = 0;
double female_given_not_survived = 0;

double survived_age_mean = 0;
double not_survived_age_mean = 0;
double survived_age_variance = 0;
double not_survived_age_variance = 0;

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

void readFile() {
  std::ifstream file;
  openFile(file);
  tossDataHeader(file);
  std::string buffer;
  int num_observations = 0;
  while(num_observations < 800) {
    getline(file, buffer, ',');
    getline(file, buffer, ',');
    train_pclass[num_observations] = stof(buffer);
    getline(file, buffer, ',');
    train_survived.at(num_observations) = stof(buffer);
    getline(file, buffer, ',');
    train_sex.at(num_observations) = stof(buffer);
    getline(file, buffer, '\n');
    train_age[num_observations] = stof(buffer);
    num_observations = num_observations + 1;
  }
  train_sex.resize(num_observations);
  train_pclass.resize(num_observations);
  train_age.resize(num_observations);
  train_survived.resize(num_observations);

  num_observations = 0;
  while(file.good()) {
    getline(file, buffer, ',');
    getline(file, buffer, ',');
    test_pclass[num_observations] = stof(buffer);
    getline(file, buffer, ',');
    test_survived.at(num_observations) = stof(buffer);
    getline(file, buffer, ',');
    test_sex.at(num_observations) = stof(buffer);
    getline(file, buffer, '\n');
    test_age[num_observations] = stof(buffer);
    num_observations = num_observations + 1;
  }
  test_sex.resize(num_observations);
  test_pclass.resize(num_observations);
  test_age.resize(num_observations);
  test_survived.resize(num_observations);
}

void calculateSurvived() {
  for(unsigned long i = 0; i < train_survived.size(); i++) {
    if(train_survived[i] == 1) {
      survived_count++;
    }
    else {
      not_survived_count++;
    }
  }

  std::cout << "Survived Prior: " << survived_count / 800 << std::endl;
  std::cout << "Not Survived Prior: " << not_survived_count / 800 << std::endl;
}

void calculatePassengerClass() {
  passenger_class_given_survived[1] = 0;
  passenger_class_given_survived[2] = 0;
  passenger_class_given_survived[3] = 0;
  passenger_class_given_not_survived[1] = 0;
  passenger_class_given_not_survived[2] = 0;
  passenger_class_given_not_survived[3] = 0;

  for(unsigned long i = 0; i < train_survived.size(); i++) {
    if(train_survived[i] == 1) {
      passenger_class_given_survived[train_pclass[i]]++;
    }
    else {
      passenger_class_given_not_survived[train_pclass[i]]++;
    }
  }

  passenger_class_given_survived[1] /= survived_count;
  passenger_class_given_survived[2] /= survived_count;
  passenger_class_given_survived[3] /= survived_count;
  passenger_class_given_not_survived[1] /= not_survived_count;
  passenger_class_given_not_survived[2] /= not_survived_count;
  passenger_class_given_not_survived[3] /= not_survived_count;

  std::cout << "Passenger Class 1 Given Survived: " << passenger_class_given_survived[1] << std::endl;
  std::cout << "Passenger Class 2 Given Survived: " << passenger_class_given_survived[2] << std::endl;
  std::cout << "Passenger Class 3 Given Survived: " << passenger_class_given_survived[3] << std::endl;
  std::cout << "Passenger Class 1 Given Not Survived: " << passenger_class_given_not_survived[1] << std::endl;
  std::cout << "Passenger Class 2 Given Not Survived: " << passenger_class_given_not_survived[2] << std::endl;
  std::cout << "Passenger Class 3 Given Not Survived: " << passenger_class_given_not_survived[3] << std::endl;
}

void calculateSex() {
  for(unsigned long i = 0; i < train_survived.size(); i++) {
    if(train_survived[i] == 1 && train_sex[i] == 1) {
      male_given_survived += 1.0;
    }
    else if(train_survived[i] == 1 && train_sex[i] == 0) {
      female_given_survived += 1.0;
    }
    else if(train_survived[i] == 0 && train_sex[i] == 1) {
      male_given_not_survived += 1.0;
    }
    else {
      female_given_not_survived += 1.0;
    }
  }

  male_given_survived /= survived_count;
  female_given_survived /= survived_count;
  male_given_not_survived /= not_survived_count;
  female_given_not_survived /= not_survived_count;

  std::cout << "Male Given Survived: " << male_given_survived << std::endl;
  std::cout << "Female Given Survived: " << female_given_survived << std::endl;
  std::cout << "Male Given Not Survived: " << male_given_not_survived << std::endl;
  std::cout << "Female Given Not Survived: " << female_given_not_survived << std::endl;
}

void calculateAge() {
  for(unsigned long i = 0; i < train_age.size(); i++) {
    if(train_survived[i] == 1) {
      survived_age_mean += train_age[i];
    }
    else {
      not_survived_age_mean += train_age[i];
    }
  }
  survived_age_mean /= survived_count;
  not_survived_age_mean /= not_survived_count;

  for(unsigned long i = 0; i < train_age.size(); i++) {
    if(train_survived[i] == 1) {
      survived_age_variance += (train_age[i] - survived_age_mean) * (train_age[i] - survived_age_mean);
    }
    else {
      not_survived_age_variance += (train_age[i] - not_survived_age_mean) * (train_age[i] - not_survived_age_mean);
    }
  }

  survived_age_variance /= (survived_count - 1);
  not_survived_age_variance /= (not_survived_count - 1);

  std::cout << "Age Mean Given Survived: " << survived_age_mean << std::endl;
  std::cout << "Age Variance Given Survived: " << survived_age_variance << std::endl;
  std::cout << "Age Mean Given Not Survived: " << not_survived_age_mean << std::endl;
  std::cout << "Age Variance Given Not Survived: " << not_survived_age_variance << std::endl;
}

double calculateQuantitative(double value, double mean, double variance) {
  double term_one = 1 / sqrt(2 * M_PI * variance);
  double term_two = exp(-((value - mean) * (value - mean)) / (2 * variance));
  return term_one * term_two;
}

void calculate() {
  calculateSurvived();
  calculatePassengerClass();
  calculateSex();
  calculateAge();
}

double numerator_survived(double passenger_class, double sex, double age) {
  double passenger_class_probability = passenger_class_given_survived[passenger_class];
  double sex_probability = (sex == 0) ? female_given_survived : male_given_survived;
  double age_probability = calculateQuantitative(age, survived_age_mean, survived_age_variance);
  return passenger_class_probability * sex_probability * age_probability * (survived_count / 800);
}

double numerator_not_survived(double passenger_class, double sex, double age) {
  double passenger_class_probability = passenger_class_given_not_survived[passenger_class];
  double sex_probability = (sex == 0) ? female_given_not_survived : male_given_not_survived;
  double age_probability = calculateQuantitative(age, not_survived_age_mean, not_survived_age_variance);
  return passenger_class_probability * sex_probability * age_probability * (not_survived_count / 800);
}

double calculateProbabilitySurvived(double passenger_class, double sex, double age) {
  double numerator = numerator_survived(passenger_class, sex, age);
  double denominator = numerator_survived(passenger_class, sex, age) + numerator_not_survived(passenger_class, sex, age);
  return numerator / denominator;
}

double calculateProbabilityNotSurvived(double passenger_class, double sex, double age) {
  double numerator = numerator_not_survived(passenger_class, sex, age);
  double denominator = numerator_survived(passenger_class, sex, age) + numerator_not_survived(passenger_class, sex, age);
  return numerator / denominator;
}

std::vector<double> getPredictions(std::vector<double> passenger_classes, std::vector<double> sexes, std::vector<double> ages) {
  std::vector<double> predictions(passenger_classes.size());

  for(unsigned long i = 0; i < predictions.size(); i++) {
    double probability_survived = calculateProbabilitySurvived(passenger_classes[i], sexes[i], ages[i]);
    double probability_not_survived = calculateProbabilityNotSurvived(passenger_classes[i], sexes[i], ages[i]);
    predictions[i] = (probability_survived > probability_not_survived) ? 1 : 0;
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
  readFile();
  std::chrono::steady_clock::time_point training_start_time = std::chrono::steady_clock::now();
  calculate();
  std::chrono::steady_clock::time_point training_end_time = std::chrono::steady_clock::now();
  std::cout << "Training Time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(training_end_time - training_start_time).count()
            << " microseconds" << std::endl;

  std::vector<double> predictions = getPredictions(test_pclass, test_sex, test_age);
  double accuracy = calculate_accuracy(predictions, test_survived);
  std::cout << "Accuracy: " << accuracy << std::endl;
  double sensitivity = calculate_sensitivity(predictions, test_survived);
  std::cout << "Sensitivity: " << sensitivity << std::endl;
  double specificity = calculate_specificity(predictions, test_survived);
  std::cout << "Specificity: " << specificity << std::endl;
}
