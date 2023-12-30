#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>

using namespace std;

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


int main(int argc, char *argv[])
{

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Read File & Parser Example


	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
	fi.open("data/0.txt", ios::in);

    // GET TITLENAME
	getline(fi, title_name);

    // GET TITLENAME WORD ARRAY
    tmp_string = split(title_name, " ");

	vector<string> title = word_parse(tmp_string);

	// for(auto &word : title){
	// 	cout << word << endl;
	// }

    // GET CONTENT LINE BY LINE
	while(getline(fi, tmp)){

        // GET CONTENT WORD VECTOR
		tmp_string = split(tmp, " ");

		// PARSE CONTENT
		vector<string> content = word_parse(tmp_string);

		for(auto &word : content){
			cout << word << endl;
		}
		//......
	}

    // CLOSE FILE
	fi.close();
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
