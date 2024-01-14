#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<unordered_map>
#include<bitset>
#include <filesystem>
#include<stdio.h>
#pragma GCC optimize("Ofast")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("unroll-loops")
// #pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-ftree-tail-merge")
#define MAX 5000005
#define DATA_MAX 11000

using namespace std;
namespace fs = std::filesystem;

int charToIndex(char& c) {
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	else if (c >= 'a' && c <= 'z')
		return c - 'a';
	return -1;
}

class TrieNode {
public:
    TrieNode* child[26];
	TrieNode* parent[26];
	bool terminate;
    TrieNode() {
        for (int i = 0; i < 26; ++i) {
            child[i] = nullptr;
			parent[i] = nullptr;
        }
        terminate = false;
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode();
    }

    void insert(string& word) {
        TrieNode* p = root;
		TrieNode* back = root;
		int len = word.size();
        for (auto &c : word) {
            int index = charToIndex(c);

            if (p->child[index] == nullptr)
                p->child[index] = new TrieNode();
            p = p->child[index];
        }
		for(int i = len-1; i >= 0; --i) {
			int index = charToIndex(word[i]);

            if (back->parent[index] == nullptr)
                back->parent[index] = new TrieNode();
            back = back->parent[index];
		}
        p->terminate = true;
    }
	bool WildCardSearch(string word, int index, TrieNode* cur) {
		int len = word.size();
		if (word[index] == '*') {
			while (index+1 < len && word[index+1] == '*') {
				index++; // avoid multiple '*'
			}
			if (index == len-2) {
				return true;
			}
			bool result = false;
			for (int i = 0; i < 26; ++i) {
				if (cur->child[i] != nullptr && result == false) {
					if (WildCardSearch(word, index, cur->child[i]) || WildCardSearch(word, index + 1, cur)) {
						result = true;
					}
				}
			}
			return result;
		} 
		else {
			int charIndex = charToIndex(word[index]);
			if (cur->child[charIndex] == nullptr) {
				return false;
			}
			if(index == len-2) {
				return cur->child[charIndex]->terminate;
			}
			else
				return WildCardSearch(word, index + 1, cur->child[charIndex]);
		}
	}
	bool SuffixSearch(string& word) {
		TrieNode* p = root;
		int len = word.size();
		for(int i = len-1; i >= 0; --i) {
			if(isalpha(word[i])) {
				int index = charToIndex(word[i]);
				if(p->parent[index] == nullptr)
					return false;
				p = p->parent[index];
			}
		}
		return true;
	}
	bool search(string& word, bool prefix = false) {
		TrieNode* p = root;
		for(auto &c: word) {
			if(isalpha(c)) {
				int index = charToIndex(c);
				if(p->child[index] == nullptr)
					return false;
				p = p->child[index];
			}
		}
		if(prefix == false) return p->terminate;
		return true;
    }

    bool startsWith(string& prefix) {
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

int checkType(string& s) {
	if(s == "/" || s == "-" || s == "+") // operator
		return 4;
	else if(s[0] == '"') { 
		return 0; // exact word
	}
	else if(s[0] == '*' && s[s.size()-1] == '*') {
		return 2; // suffix word
	}
	else if(s[0] == '<' && s[s.size()-1] == '>') {
		return 3; // wildcard search
	}
	else {
		return 1; // prefix
	}
}


int main(int argc, char *argv[]) {

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route
    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	ifstream fi(query, ios::in);
	string output = string(argv[3]);

	string file, title_name, tmp, q_tmp;
	FILE *outputFile = freopen(output.c_str(), "w", stdout);
	vector<string> tmp_string;
	int cnt = 0; // count of data

	const int EXACT = 0;
	const int PREFIX = 1;
	const int SUFFIX = 2;
	const int WILDCARD = 3;

    // GET TITLENAME WORD ARRAY
	string title[DATA_MAX];
	unordered_map<int, Trie*> mp;
	char str[MAX];
	// string tmp;

	while (1) {
		string current_file_path = data_dir + to_string(cnt) + ".txt";
		FILE *inputFile = freopen(current_file_path.c_str(), "r", stdin);
		
		if (inputFile == nullptr) {
			break;
		} 

		fgets(str, MAX, stdin);
		title[cnt] = str;
		vector<string> tmp_title = word_parse(split(str, " "));
		for(auto &word : tmp_title) {
			if (mp[cnt] == nullptr) {
				mp[cnt] = new Trie();  
			}
			mp[cnt]->insert(word);
		}

		while (fgets(str, MAX, stdin)) {
			// GET CONTENT WORD VECTOR
			vector<string> tmp_string = split(str, " ");
			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);
			for (auto &word : content) {
				mp[cnt]->insert(word);
			}
		}
		++cnt;
		fclose(inputFile);
	}

	bitset<5000005> ans(0);
	bitset<5000005> title_tmp(0);
	// set<int> ans_title;
	// unordered_set<int> title_tmp;
	int n = 1;
	while(getline(fi, q_tmp)) { // get seperate query
		int merge_type = -1;
		vector<string> q = split(q_tmp, " ");
		// outputFile << n++ <<"\n";
		for(auto &it_query : q) { // iterate all the query
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
			for(int i = 0; i < cnt; ++i) {
				auto data = mp[i];
				if(type == EXACT) {
					if(data->search(it_query))
						title_tmp[i] = 1;
				}
				else if(type == PREFIX) {
					if(data->startsWith(it_query))
						title_tmp[i] = 1;
				}
				else if(type == SUFFIX) {
					if(data->SuffixSearch(it_query))
						title_tmp[i] = 1;
				}
				else if(type == WILDCARD) {
					if(data->WildCardSearch(it_query, 1, data->root))
						title_tmp[i] = 1;
				}
			}
			if(merge_type == 0) { // OR
				ans |= title_tmp;
			}
			else if(merge_type == 1) { // NOT
				ans &= ~title_tmp;
			}
			else if(merge_type == 2) { // AND
				ans &= title_tmp;
			}
			else { // -1, don't need to merge
				ans = title_tmp;
			}
			title_tmp.reset();
			merge_type = -1;
		}
		bool found = false;
		for(int i = 0; i < cnt; ++i) {
			if(ans[i] == 1) {
				for(auto &c: title[i]) {
					putchar(c);
					found = true;
				}
			}
		}
		if(found == false)
			fputs("Not Found!\n", stdout);
		ans.reset();
	}
	fi.close();
	fclose(outputFile);
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////