#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<map>
#include<iostream>
#include <filesystem>
#include <unordered_set>

using namespace std;
namespace fs = std::filesystem;

class TrieNode {
	public:
		TrieNode* child[26];
		bool terminate;
		TrieNode() {
			for(int i = 0; i < 26; i++) {
				child[i] = NULL;
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
    void insert(string word) {
        TrieNode* p = root;
        for(auto c: word) {
            int index = 0;
			if(c >= 'A' && c <= 'Z')
				index = c-'A';
			else if(c >= 'a' && c <= 'z')
				index = c-'a';
            if(p->child[index] == NULL)
                p->child[index] = new TrieNode();
            p = p->child[index];
        }
        p->terminate = true;
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
				if(p->child[index] == NULL)
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

/*
	Exact word : "..."
	Prefix word : ...
	Suffix word : *...*
	Wildcard search : <..*.>
*/


// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
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
		p = strtok(NULL, d);
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
	ofstream outputFile(output);
	vector<string> tmp_string;

	const int EXACT = 0;
	const int PREFIX = 1;
	const int SUFFIX = 2;
	const int WILDCARD = 3;

    // GET TITLENAME WORD ARRAY
    tmp_string = split(title_name, " ");

	vector<string> title = word_parse(tmp_string);
	unordered_map<string, Trie*> mp;
	/*
		create trie of every data
	*/
	for (const auto& entry : fs::directory_iterator(data_dir)) {
		// entry.path() get the file path
		string file_path = entry.path().string();

		// open the file
		ifstream inputFile(file_path);
		getline(inputFile, title_name);
		
		string tmp;
		while (getline(inputFile, tmp)) {
			// GET CONTENT WORD VECTOR
			vector<string> tmp_string = split(tmp, " ");

			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);
			for (auto word : content) {
				if (mp.count(title_name) == 0) {
					mp[title_name] = new Trie();  
				}
				mp[title_name]->insert(word);
        	}
		}
		inputFile.close();
	}
	unordered_set<string> ans_title;
	int n = 0;
	while(getline(fi, q_tmp)) { // get seperate query
		vector<string> q = split(q_tmp, " ");
		for(auto it_query : q) { // iterate all the query
			int type = checkType(it_query);
			for(auto data: mp) {
				if(type == EXACT) {
					if(data.second->search(it_query))
						ans_title.insert(data.first);
				}
				else if(type == PREFIX) {
					if(data.second->startsWith(it_query))
						ans_title.insert(data.first);
				}
				// else if(type == SUFFIX) {

				// }
				// else if(type == WILDCARD) {

				// }
			}
		}
		for(auto it : ans_title) {
			outputFile << it << "\n";	
		}
		// for debug
		outputFile << n++ <<"\n";
	}
	fi.close();
	outputFile.close();
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
