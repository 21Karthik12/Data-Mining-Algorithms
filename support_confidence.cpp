#include<bits/stdc++.h>
using namespace std;

using items = unordered_set<string>;

class Data {
private:
    vector<items> dataset;
    items unique_items;
    int size;

    items split(string line, char delimiter) {
        items res;
        int i = 0, size = line.length();
        while(i < size) {
            string word;
            while(i < size && line[i] != delimiter)
                word += line[i++];
            i++;
            res.insert(word);
            unique_items.insert(word);
        }
        return res;
    }

    items concat(items s1, items s2) {
        items res;
        for(auto i1 : s1)
            res.insert(i1);
        for(auto i2 : s2)
            res.insert(i2);
        return res;
    }

    double support(string item) {
        double res = 0.0;
        for(auto l : this->dataset) {
            if(l.find(item) != l.end())
                res++;
        }
        return res / (double)this->size;
    }

    double support(items list) {
        double res = 0.0;
        for(auto l : this->dataset) {
            bool found = true;
            for(auto item : list) {
                if(l.find(item) == l.end()) {
                    found = false;
                    break;
                }
            }
            res += found;
        }
        return res / (double)this->size;
    }

    double confidence(items X, items Y) {
        double den = support(X);
        if(den == 0)
            return -1;
        return support(concat(X, Y)) / den;
    }

public:
    void input() {
        ifstream inp("datasets/apriori_input.csv");
        string line;
        while(getline(inp, line))
            this->dataset.push_back(split(line, ','));
        this->size = this->dataset.size();
        inp.close();
    }

    void print_support() {
        cout<<"Support:\n";
        for(auto item : unique_items)
            cout<<item<<": "<<support(item)<<'\n';
        cout<<'\n';
    }

    void print_support_above_threshold(double threshold_support) {
        cout<<"Support above "<<threshold_support<<":\n";
        for(auto item : unique_items) {
            double current_support = support(item);
            if(current_support > threshold_support)
                cout<<item<<": "<<current_support<<'\n';
        }
        cout<<'\n';
    }

    void print_confidence(vector<pair<string, string>> confidence_pairs) {
        for(auto p : confidence_pairs) {
            cout<<"Confidence of "<<p.first<<" -> "<<p.second<<": ";
            double c = confidence(split(p.first, ','), split(p.second, ','));
            if(c == -1)
                cout<<"Cannot be determined\n";
            else
                cout<<c<<'\n';
        }
        cout<<'\n';
    }

    void print_confidence_above_threshold(vector<pair<string, string>> c_p, double t_c) {
        cout<<"Recommended rules:\n";
        for(auto p : c_p) {
            double c = confidence(split(p.first, ','), split(p.second, ','));
            if(c >= t_c)
                cout<<p.first<<" -> "<<p.second<<" with confidence "<<c<<'\n';
        }
    }

    void print_dataset() {
        int n = 1;
        for(auto l : dataset) {
            cout<<(n++)<<": ";
            for(auto i : l)
                cout<<i<<" ";
            cout<<'\n';
        }
        cout<<'\n';
    }
};

int main() {
    Data dataset;
    dataset.input();
    dataset.print_dataset();
    dataset.print_support();

    double threshold_support;
    cout<<"Enter threshold support: ";
    cin>>threshold_support;
    dataset.print_support_above_threshold(threshold_support);

    cout<<"Enter the number of confidence pairs: ";
    int n; cin>>n;
    vector<pair<string, string>> confidence_pairs(n);
    string X, Y;
    cout<<"Enter the confidence pairs:\n";
    for(int i = 0; i < n; i++) {
        cin>>X>>Y;
        confidence_pairs[i] = {X, Y};
    }
    dataset.print_confidence(confidence_pairs);

    double threshold_confidence;
    cout<<"Enter threshold confidence: ";
    cin>>threshold_confidence;
    dataset.print_confidence_above_threshold(confidence_pairs, threshold_confidence);
}