#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<bitset>
#include <filesystem>
#include<stdio.h>
#pragma GCC optimize("Ofast")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-ftree-tail-merge")

#define MAX 500005
#define DATA_MAX 10005

using namespace std;

namespace fs = std::filesystem;


int num = 0;

inline int charToIndex(char& c) {
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	else if (c >= 'a' && c <= 'z')
		return c - 'a';
	return -1;
}



// class Trie {

// public:

    int trie[MAX][26];
	bitset<DATA_MAX> ed[MAX], suffix[MAX], prefix[MAX];
	bitset<DATA_MAX> NOT_FOUND;
    // Trie() {}
    inline void insert(string& word, int &index) {
        int root = 0;
		int len = word.size();
		for(char &c : word){
			int p = charToIndex(c);
			if(trie[root][p] == 0) 
				trie[root][p]= ++num;
			root = trie[root][p];
			prefix[root][index]=1;
		}
		ed[root][index] = 1;
		root = 0;
		for(int i = len-1; ~i; --i) {
			int p = charToIndex(word[i]);
			if(trie[root][p] == 0) 
				trie[root][p] = ++num;
			root = trie[root][p];
			suffix[root][index] = 1;
		}
    }

	bitset<DATA_MAX> WildCardSearch(string &word, int index, int root) {
		int len = word.size();
		if(len == 3 && word[1] == '*') {
			bitset<DATA_MAX> result;
			result.set();
			return result;
		}
		if (word[index] == '*') {
			while (index+1 < len && word[index+1] == '*') {
				++index; // avoid multiple '*'
			}
			if (index == len-2) {
				return prefix[root];
			}
			bitset<DATA_MAX> result;

			for (int i = 0; i < 26; ++i) {
				if (trie[root][i] != 0) {
					result |= ((WildCardSearch(word, index, trie[root][i]) | WildCardSearch(word, index + 1, root)));
				}
			}
			return result;
		} 
		else {
			int charIndex = charToIndex(word[index]);
			if (trie[root][charIndex] == 0) {
				return NOT_FOUND;
			}
			if(index == len-2) {
				return ed[trie[root][charIndex]];
			}
			return WildCardSearch(word, index + 1, trie[root][charIndex]);
		}
	}
	bitset<DATA_MAX> SuffixSearch(string& word) {
		int root = 0;
		int len = word.size();
		for(int i = len-1; ~i; --i) {
			if(isalpha(word[i])) {
				int index = charToIndex(word[i]);
				if(trie[root][index] == 0)
					return NOT_FOUND;
				root = trie[root][index];
			}
		}
		return suffix[root];
	}

	bitset<DATA_MAX> search(string& word, bool pre = false) {
		int root = 0;
		for(auto &c: word) {
			if(isalpha(c)) {
				int index = charToIndex(c);
				if(trie[root][index] == 0)
					return NOT_FOUND;
				root = trie[root][index];
			}
		}
		if(! pre) return ed[root];
		return prefix[root];
	}


    bitset<DATA_MAX> startsWith(string& pre) {
        return search(pre, true);
    }
// };



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


inline int checkType(string& s) {
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

	vector<string> title;

	char str[MAX];

	// Trie* tr = new Trie();



	while (1) {
		string current_file_path = data_dir + to_string(cnt) + ".txt";

		if(!freopen(current_file_path.c_str(), "r", stdin)) 
			break;

		fgets_unlocked(str, MAX, stdin);
		title.emplace_back(str);
		vector<string> tmp_title = word_parse(split(str, " "));
		for(auto &word : tmp_title) {
			insert(word, cnt);
		}
		while (fgets_unlocked(str, MAX, stdin)) {
			// GET CONTENT WORD VECTOR
			vector<string> tmp_string = split(str, " ");
			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);
			for (auto &word : content) {
				insert(word, cnt);
			}
		}
		++cnt;
		//fclose(inputFile);
	}

	bitset<DATA_MAX> ans(0);
	bitset<DATA_MAX> title_tmp(0);
	// int n = 1;
	while(getline(fi, q_tmp)) { // get seperate query

		int merge_type = -1;

		vector<string> q = split(q_tmp, " ");

		// cout << n++<< endl; 

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
			switch (type) {
				case EXACT:
					title_tmp = search(it_query);
					break;
				case PREFIX:
					title_tmp = startsWith(it_query);
					break;
				case SUFFIX:
					title_tmp = SuffixSearch(it_query);
					break;
				case WILDCARD:
					title_tmp = WildCardSearch(it_query, 1, 0);
					break;
			}

			// if(type == EXACT) {

			// 	title_tmp = search(it_query);

			// }

			// else if(type == PREFIX) {

			// 	title_tmp = startsWith(it_query);

			// }

			// else if(type == SUFFIX) {

			// 	title_tmp = SuffixSearch(it_query);

			// }

			// else if(type == WILDCARD) {

			// 	title_tmp = WildCardSearch(it_query, 1, 0);

			// }

			switch (merge_type) {
				case 0:
					ans |= title_tmp;
					break;
				case 1:
					ans &= ~title_tmp;
					break;
				case 2:
					ans &= title_tmp;
					break;
				case -1:
					ans = title_tmp;
					break;
			}

			// if(merge_type == 0) { // OR

			// 	ans |= title_tmp;

			// }

			// else if(merge_type == 1) { // NOT

			// 	ans &= ~title_tmp;

			// }

			// else if(merge_type == 2) { // AND

			// 	ans &= title_tmp;

			// }

			// else { // -1, don't need to merge

			// 	ans = title_tmp;

			// }

			title_tmp.reset();

			merge_type = -1;

		}

		bool found = false;

		for(int i=ans._Find_first();i< ans.size();i = ans._Find_next(i)) {
			for(auto &c: title[i]) {
				putchar_unlocked(c);
				found = true;
			}	
		}
		if(found == false)
			fputs_unlocked("Not Found!\n", stdout);
		ans.reset();
	}
	fclose(outputFile);
}





// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.

// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.

//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].

// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE

//    EG : A + B / C == (A + B) / C



//



//////////////////////////////////////////////////////////