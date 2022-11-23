#include<bits/stdc++.h>

using namespace std;
using PDI = pair<double, int>;

class TrainingDataPoint {
private:
	vector<double> data_point;
	string class_name;
	int feature_size;
	double sqr(double d) {
		return d * d;
	}
public:
	TrainingDataPoint(vector<double> data_point, string class_name) {
		this->data_point = data_point;
		this->class_name = class_name;
		this->feature_size = data_point.size() - 1;
	}
	string get_class() {
		return this->class_name;
	}
	int num_features() {
		return this->feature_size;
	}
	double distance(vector<double> feature_vector) {
		double dist = 0.0;
		for(int i = 1; i < this->feature_size; i++)
			dist += sqr(feature_vector[i - 1] - this->data_point[i]);
		return sqrt(dist);
	}
	void print() {
		for(auto i : this->data_point)
			cout<<i<<" ";
		cout<<this->class_name<<endl;
	}
};

class KNearestNeighboursClassifier {
private:
	vector<TrainingDataPoint> data;
	int size, k;
public:
	KNearestNeighboursClassifier(vector<TrainingDataPoint> data, int k) {
		this->data = data;
		this->size = (int)data.size();
		this->k = k;
	}
	string predict(vector<double> feature_vector) {
		priority_queue<PDI, vector<PDI>, greater<PDI>> sorter;
		unordered_map<string, int> counter;
		for(int i = 0; i < size; i++) {
			double dist = this->data[i].distance(feature_vector);
			sorter.push({dist, i});
		}
		for(int i = 0; i < k; i++) {
			counter[this->data[sorter.top().second].get_class()]++;
			sorter.pop();
		}
		int max_count = -1;
		string res;
		for(auto p : counter) {
			if(max_count < p.second) {
				max_count = p.second;
				res = p.first;
			}
		}
		return res;
	}
};

double atod(string num) {
	double res = 0, div = 10;
	int i = 0;
	while(i < num.length() && num[i] != '.') {
		res *= 10;
		res += (double)(num[i++] - '0');
	}
	i++;
	while(i < num.length()) {
		res += (double)(num[i++] - '0')/div;
		div /= 10;
	}
	return res;
}

int main() {
	ifstream inp("datasets/iris.csv");
	string str;
	vector<TrainingDataPoint> data;
	getline(inp, str); // Column names
	while(getline(inp, str)) {
		vector<double> tuple;
		int size = (int)str.length(), i = 0;
		while(i < size) {
			string temp;
			while(i < size && str[i] != ',')
				temp += str[i++];
			i++;
			if(temp[0] >= '0' && temp[0] <= '9')
				tuple.push_back(atod(temp));
			else
				data.push_back(TrainingDataPoint(tuple, temp));
		}
	}
	cout<<"Enter the value of k: ";
	int k; cin>>k;
	KNearestNeighboursClassifier tool(data, k);
	cout<<"Enter the number of feature vectors: ";
	int n; cin>>n;
	cout<<"Enter the feature vectors:\n";
	for(int i = 0; i < n; i++) {
		vector<double> tuple;
		for(int j = 0; j < data[0].num_features(); j++) {
			double bin; cin>>bin;
			tuple.push_back(bin);
		}
		cout<<"Predicted Class: "<<tool.predict(tuple)<<'\n';
	}
	return 0;
}
