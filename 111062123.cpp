#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<unordered_map>
#include<algorithm>
#include<set>
#include <filesystem>
#include <unordered_set>

#include <chrono>

using namespace std;
namespace fs = std::filesystem;

class TrieNode {
public:
    TrieNode* child[26];
	TrieNode* parent[26];
    // bitset<5000005> prefix(0);
	// bitset<5000005> suffix(0);
	bool terminate, terminate_parent;
    TrieNode() {
        for (int i = 0; i < 26; i++) {
            child[i] = nullptr;
			parent[i] = nullptr;
        }
		terminate_parent = false;
        terminate = false;
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode* p = root;
		TrieNode* back = root;
		int len = word.size();
        for (auto &c : word) {
            int index = 0;
            if (c >= 'A' && c <= 'Z')
                index = c - 'A';
            else if (c >= 'a' && c <= 'z')
                index = c - 'a';

            if (p->child[index] == nullptr)
                p->child[index] = new TrieNode();
            p = p->child[index];
        }
		for(int i = len-1; i >= 0; i--) {
			int index = 0;
            if (word[i] >= 'A' && word[i] <= 'Z')
                index = word[i] - 'A';
            else if (word[i] >= 'a' && word[i] <= 'z')
                index = word[i] - 'a';

            if (back->parent[index] == nullptr)
                back->parent[index] = new TrieNode();
            back = back->parent[index];
		}
        p->terminate = true;
		back->terminate_parent = true;
    }
	bool WildCardSearch(string word, int index, TrieNode* cur) {
		int len = word.size();
		// cout << word[index] << endl;
		if (index == len-1) {
			return cur->terminate;
		}
		if (word[index] == '*') {
			while (index+1 < len && word[index+1] == '*') {
				index++; // avoid multiple '*'
			}
			if (index == len-1) {
				return true;
			}
			bool result = false;
			for (int i = 0; i < 26; i++) {
				if (cur->child[i] != nullptr) {
					if (index+1 < len && (i == (word[index+1] - 'a') || i == (word[index+1] - 'A')) && WildCardSearch(word, index + 1, cur->child[i])) {
						result = true;
					}
					if (WildCardSearch(word, index, cur->child[i])) {
						result =  true;
					}
				}
			}
			return result;
		} 
		else {
			int charIndex = 0;
			if (word[index] >= 'A' && word[index] <= 'Z') {
				charIndex = word[index] - 'A';
			} 
			else if (word[index] >= 'a' && word[index] <= 'z') {
				charIndex = word[index] - 'a';
			}
			if (cur->child[charIndex] == nullptr) {
				return false;
			}
			return WildCardSearch(word, index + 1, cur->child[charIndex]);
		}
	}
	bool SuffixSearch(string word) {
		TrieNode* p = root;
		int len = word.size();
		for(int i = len-1; i >= 0; i--) {
			if(isalpha(word[i])) {
				int index = 0;
				if(word[i] >= 'A' && word[i] <= 'Z')
					index = word[i]-'A';
				else if(word[i] >= 'a' && word[i] <= 'z')
					index = word[i]-'a';
				if(p->parent[index] == nullptr)
					return false;
				p = p->parent[index];
			}
		}
		return true;
	}
	bool search(string word, bool prefix = false) {
		TrieNode* p = root;
		for(auto c: word) {
			if(isalpha(c)) {
				int index = 0;
				if(c >= 'A' && c <= 'Z')
					index = c-'A';
				else if(c >= 'a' && c <= 'z')
					index = c-'a';
				if(p->child[index] == nullptr)
					return false;
				p = p->child[index];
			}
		}
		if(prefix == false) return p->terminate;
		return true;
    }

    bool startsWith(string prefix) {
        return search(prefix, true);
    }
};

// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str += ch;
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;

	char * strs = new char[str.length() + 1] ; 
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; 
		res.push_back(s); 
		p = strtok(nullptr, d);
	}

	return res;
}

int checkType(string s) {
	if(s[0] == '"') { 
		return 0; // exact word
	}
	else if(s[0] == '*' && s[s.size()-1] == '*') {
		return 2; // suffix word
	}
	else if(s[0] == '<' && s[s.size()-1] == '>') {
		return 3; // wildcard search
	}
	else if(s == "/" || s == "-" || s == "+") // operator
		return 4;
	else {
		return 1; // prefix
	}
}


int main(int argc, char *argv[]) {

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route
	auto start_time = std::chrono::high_resolution_clock::now();
    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	ifstream fi(query, ios::in);
	string output = string(argv[3]);

	string file, title_name, tmp, q_tmp;
	ofstream outputFile(output);
	vector<string> tmp_string;
	int cnt = 0; // count of data

	const int EXACT = 0;
	const int PREFIX = 1;
	const int SUFFIX = 2;
	const int WILDCARD = 3;

    // GET TITLENAME WORD ARRAY
	vector<string> title;
	unordered_map<string, Trie*> mp;
	/*
		create trie of every data
	*/
	vector<string> file_paths;
	int i = 0;
	while (1) {
        std::string current_file_path = data_dir + std::to_string(i) + ".txt";
        if (fs::exists(current_file_path)) {
            file_paths.emplace_back(current_file_path);
			// cout << current_file_path << endl;
        } 
		else {
            break;
        }
        i++;
    }
	for (auto file_path : file_paths) {

		ifstream inputFile(file_path);
		getline(inputFile, title_name);
		title.emplace_back(title_name);
		cnt++;
		
		string tmp;
		while (getline(inputFile, tmp)) {
			// GET CONTENT WORD VECTOR
			vector<string> tmp_string = split(tmp, " ");

			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);
			for (auto word : content) {
				if (mp[title_name] == nullptr) {
					mp[title_name] = new Trie();  
				}
				mp[title_name]->insert(word);
        	}
		}
		inputFile.close();
	}
	set<int> ans_title;
	unordered_set<int> title_tmp;
	int n = 1;
	while(getline(fi, q_tmp)) { // get seperate query
		int merge_type = -1;
		vector<string> q = split(q_tmp, " ");
		outputFile << n++ <<"\n";
		for(auto it_query : q) { // iterate all the query
			string front = "", end = "";
			int type = checkType(it_query);
			if(type == 4) {
				if(it_query == "/") {
					merge_type = 0;
				}
				else if(it_query == "-") {
					merge_type = 1;
				}
				else if(it_query == "+") {
					merge_type = 2;
				}
				continue;
			}
			for(int i = 0; i < cnt; i++) {
				auto data = mp[title[i]];
				if(type == EXACT) {
					if(data->search(it_query))
						title_tmp.insert(i);
				}
				else if(type == PREFIX) {
					if(data->startsWith(it_query))
						title_tmp.insert(i);
				}
				else if(type == SUFFIX) {
					if(data->SuffixSearch(it_query))
						title_tmp.insert(i);
				}
				else if(type == WILDCARD) {
					if(data->WildCardSearch(it_query, 1, data->root))
						title_tmp.insert(i);
				}
			}
			if(merge_type == 0) { // OR
				for(auto it : title_tmp) {
					ans_title.insert(it);
				}
			}
			else if(merge_type == 1) { // NOT
				for (auto it = ans_title.begin(); it != ans_title.end();) {
					if (title_tmp.count(*it) == 1)
						it = ans_title.erase(it); 
					else
						++it; 
				}
			}
			else if(merge_type == 2) { // AND
				for (auto it = ans_title.begin(); it != ans_title.end();) {
					if (title_tmp.count(*it) == 0)
						it = ans_title.erase(it); 
					else
						++it; 
				}
			}
			else { // -1, don't need to merge
				for(auto it: title_tmp) {
					ans_title.insert(it);
				}
			}
			title_tmp.clear();
			merge_type = -1;
		}
		if(ans_title.empty()) {
			outputFile << "Not Found!\n";
		}
		else {
			for(auto it : ans_title) {
				outputFile << title[it] << "\n";
			}
		}
		ans_title.clear();
	}
	fi.close();
	outputFile.close();
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
	std::cout << "程式執行時間: " << duration.count()/1000 << " ms" << std::endl;
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
