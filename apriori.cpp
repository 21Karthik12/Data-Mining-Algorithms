#include<bits/stdc++.h>
using namespace std;

using items = set<string>;

class Apriori {
private:
	vector<items> dataset;
	items unique_items;
	unordered_map<string, int> freq_map;
	int threshold_support = -1;
	double threshold_confidence = -1.0;
	vector<items> max_freq_item_set;
	items split(string line) {
		items res;
		int i = 0, curr_size = (int)line.length();
		while(i < curr_size) {
			string word;
			while(i < curr_size && line[i] != ',')
				word += line[i++];
			i++;
			res.insert(word);
			this->freq_map[word]++;
			this->unique_items.insert(word);
		}
		return res;
	}
	static items concat(const items& s1, const items& s2) {
		items res;
		for(const auto& i1 : s1)
			res.insert(i1);
		for(const auto& i2 : s2)
			res.insert(i2);
		return res;
	}
	int frequency(const items& item_set) {
		int res = 0;
		for(const auto& l : this->dataset) {
			bool found = true;
			for(const auto& item : item_set) {
				if(!l.count(item)) {
					found = false;
					break;
				}
			}
			res += found;
		}
		return res;
	}
	double support(const items& list) {
		return (double)frequency(list) / (double)this->dataset.size();
	}
	double confidence(const items& X, const items& Y) {
		double den = support(X);
		if(den == 0)
			return -1;
		return support(concat(X, Y)) / den;
	}
	static bool contains(const vector<items>& prev_data, const items& item_set) {
		for(const auto& l : prev_data) {
			if(l == item_set)
				return true;
		}
		return false;
	}
	static bool has_infrequent(const items& item_set, const vector<items>& prev_data) {
		for(const auto& item : item_set) {
			items temp = item_set;
			temp.erase(item);
			if(!contains(prev_data, temp))
				return true;
		}
		return false;
	}
	static vector<items> join(vector<items> curr_data) {
		vector<items> res;
		int n = (int)curr_data.size(), k = (int)curr_data[0].size();
		for(int i = 0; i < n; i++) {
			for(int j = i + 1; j < n; j++) {
				bool equal = true;
				for(int m = 0; m < k - 1; m++) {
					auto it_p = curr_data[i].begin();
					auto it_q = curr_data[j].begin();
					if(*next(it_p, m) != *next(it_q, m)) {
						equal = false;
						break;
					}
				}
				if(!equal)
					break;
				items temp = curr_data[i];
				temp.insert(*(prev(curr_data[j].end())));
				if(!has_infrequent(temp, curr_data))
					res.push_back(temp);
			}
		}
		return res;
	}
	vector<items> prune(const vector<items>& curr_data) {
		vector<items> res;
		for(const auto& item_set : curr_data) {
			if(frequency(item_set) >= this->threshold_support)
				res.push_back(item_set);
		}
		return res;
	}
	static void print_item_set(const items& item_set) {
		cout<<"{";
		for(auto it = item_set.begin(); it != prev(item_set.end(), 1); it++)
			cout<<*it<<", ";
		cout<<*prev(item_set.end(), 1)<<"}";
	}
	static void print_item_set_list(const vector<items>& item_list) {
		for(const auto& i : item_list) {
			print_item_set(i);
			cout<<" ";
		}
		cout<<'\n';
	}
	static vector<items> combinations(const items& item_set) {
		int power_set_size = (int)pow(2, item_set.size());
		vector<items> res;
		for(int i = 1; i < power_set_size - 1; i++) {
			items temp;
			for(int j = 0; j < item_set.size(); j++) {
				if(i & (1 << j))
					temp.insert(*next(item_set.begin(), j));
			}
			res.push_back(temp);
		}
		return res;
	}
	static bool has_overlap(const items& X, const items& Y) {
		for(const auto& x : X) {
			if(Y.count(x))
				return true;
		}
		return false;
	}
public:
	void input() {
		ifstream inp("datasets/apriori_input.csv");
		string line;
		while(getline(inp, line))
			this->dataset.push_back(split(line));
		inp.close();
		cout<<"Enter threshold_support: ";
		cin>>this->threshold_support;
	}
	void generate_frequent_item_sets() {
		vector<items> one_item_set;
		for(const auto& ui : this->unique_items)
			one_item_set.push_back(items({ui}));
		int k = 1;
		cout<<k++<<" item-set:\n";
		cout<<"Before pruning:\n";
		print_item_set_list(one_item_set);
		vector<items> before_prune;
		vector<items> after_prune = prune(one_item_set);
		cout<<"After pruning:\n";
		print_item_set_list(after_prune);
		cout<<'\n';
		while(true) {
			before_prune = join(after_prune);
			after_prune = prune(before_prune);
			if(after_prune.empty()) {
				cout<<'\n';
				return;
			}
			this->max_freq_item_set = after_prune;
			cout<<k++<<" item-set:\n";
			cout<<"Before pruning:\n";
			print_item_set_list(before_prune);
			cout<<"After pruning:\n";
			print_item_set_list(after_prune);
			cout<<'\n';
		}
	}
	void generate_associations() {
		cout<<"Enter threshold confidence: ";
		cin>>this->threshold_confidence;
		cout<<"Recommended associations:\n";
		for(const auto& item_set : this->max_freq_item_set) {
			vector<items> power_set = combinations(item_set);
			for(const auto& X : power_set) {
				for(const auto& Y : power_set) {
					if(!has_overlap(X, Y)) {
						if(confidence(X, Y) >= this->threshold_confidence) {
							print_item_set(X);
							cout<<" -> ";
							print_item_set(Y);
							cout<<'\n';
						}
					}
				}
			}
		}
	}
	void print_dataset() {
		int n = 1;
		for(const auto& l : dataset) {
			cout<<(n++)<<": ";
			for(const auto& i : l) cout<<i<<" ";
			cout<<'\n';
		}
		cout<<'\n';
	}
};

int main() {
	Apriori tool;
	tool.input();
	tool.print_dataset();
	tool.generate_frequent_item_sets();
	tool.generate_associations();
	return 0;
}
