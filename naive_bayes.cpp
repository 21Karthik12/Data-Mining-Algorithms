#include <bits/stdc++.h>
using namespace std;

class NaiveBayesClassifier {
private:
	/* Full dataset */
	vector<vector<string>> dataset;
	
	/* Preprocessed data */
	unordered_map<string, vector<unordered_map<string, double>>> probability;
	unordered_map<string, double> total_probability;
	
	/* Returns the probabilities of the prediction class corresponding to the values in the feature */
	unordered_map<string, double> feature_probability(int feature, const string& value) {
		unordered_map<string, double> result;
		int n = (int)dataset.size(), m = (int)dataset[0].size();
		for(int i = 1; i < n; i++)
			result[dataset[i][feature]] = 0.0;
		double sum = 0.0;
		for(int i = 1; i < n; i++) {
			if(dataset[i][m - 1] == value) {
				result[dataset[i][feature]]++;
				sum++;
			}
		}
		for(auto& p : result)
			p.second /= sum;
		return result;
	}
	
	/* Returns the vector of probabilities of all the features */
	vector<unordered_map<string, double>> probability_vector(const string& value) {
		int m = (int)dataset[0].size();
		vector<unordered_map<string, double>> result;
		for(int f = 0; f < m - 1; f++)
			result.push_back(feature_probability(f, value));
		return result;
	}
	
	/* Counts the number of occurrences of a value in a feature column */
	double count_occurrences(int feature, const string& value) {
		double result = 0.0;
		int n = (int)dataset.size();
		for(int i = 0; i < n; i++) {
			if(dataset[i][feature] == value)
				result++;
		}
		return result;
	}
	
	/* Initializes the preprocess data */
	void preprocess() {
		unordered_set<string> unique_values;
		int n = (int)dataset.size(), m = (int)dataset[0].size();
		for(int i = 1; i < n; i++)
			unique_values.insert(dataset[i][m - 1]);
		for(const auto& v : unique_values) {
			probability[v] = probability_vector(v);
			total_probability[v] = count_occurrences(m - 1, v) / (n - 1);
		}
	}

public:
	/* Constructor */
	NaiveBayesClassifier(const vector<vector<string>>& data) {
		this->dataset = data;
		preprocess();
	}
	
	/* Returns the predicted output */
	string predict(const vector<string>& feature_vector) {
		double max_probability = -1.0;
		string result;
		int n = (int)feature_vector.size();
		for(const auto& p : probability) {
			double current_probability = total_probability[p.first];
			auto current_vector = p.second;
			for(int i = 0; i < n; i++)
				current_probability *= current_vector[i][feature_vector[i]];
			if(current_probability > max_probability) {
				max_probability = current_probability;
				result = p.first;
			}
		}
		return result;
	}
};

/* Driver function */
int main() {
	/* Taking input from a file */
	vector<vector<string>> data;
	ifstream inp("datasets/golf.csv");
	string temp;
	while(getline(inp, temp)) {
		vector<string> temp_vector;
		int size = (int)temp.length(), i = 0;
		while(i < size) {
			string bin;
			while(i < size && temp[i] != ',') {
				bin += temp[i];
				i++;
			}
			i++;
			temp_vector.push_back(bin);
		}
		data.push_back(temp_vector);
	}
	
	/* Fit the classifier model on the data */
	NaiveBayesClassifier tool(data);
	
	/* Taking inputs for prediction */
	cout<<"\nEnter the number of test feature vectors: ";
	int tests; cin>>tests;
	cout<<"\nEnter the values of the features:\n";
	while(tests--) {
		vector<string> feature_vector;
		string value;
		cout<<'\n';
		for(int i = 0; i < (data[0].size() - 1); i++) {
			cout<<data[0][i]<<": ";
			cin>>value;
			feature_vector.push_back(value);
		}
		cout<<"Predicted - "<<data[0][data[0].size() - 1]<<": ";
		cout<<tool.predict(feature_vector)<<'\n';
	}
	inp.close();
	return 0;
}