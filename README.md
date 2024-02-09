# Essay-Search Engine

## Intro
This project is a simple search engine akin to popular search engines like Google, Yahoo, and Baidu. We used the Kaggle arXiv Dataset ([link](https://www.kaggle.com/datasets/Cornell-University/arxiv?resource=download)) as the materials for searching. 
The `query.txt` file serves as the keyword input for searches, while the `query_output.txt` is the correct answer of the result, which can be used to compared with the `output.txt` to ensure the correctness.

Please note that the implementation utilizes optimized functions such as `fgets_unlocked`, hence it is recommended to execute the code on a Linux environment to avoid errors.

## Functionality
This project in C++ boasts several functions:
- **Exact Search**: Find essays that exactly match the query. The pattern will be like: `"graph"`
- **Prefix Search**: Discover essays with titles or abstracts starting with the query. The pattern will be like: `graph`.
- **Suffix Search**: Identify essays with titles or abstracts ending with the query. The pattern will be like: `*graph*`.
- **WildCard Search**: Perform wildcard searches for flexible results. The pattern will be like: `<gr*ph>`. The `*` can represent one character, several characters, or no character.

Beyond search methods, the engine supports operators for refining results:
- **AND ('+')**
- **NOT ('-')**
- **OR ('/')**

## Input and Output
**Input:**
- A collection of TXT files (essays) within the specified folder path (e.g., `0.txt`, `1.txt`, ...).
- A designated TXT file containing search queries.

**Output:**
- Generate a TXT file with the specified name.
- Given a word, the objective is to list essays with titles or abstracts containing the word.
- Only alphabetic words are considered; special symbols or digits are ignored.
- Queries are case-insensitive, treating uppercase and lowercase characters the same.

## Implementation

- **Trie**
    The project primarily employs the `Trie` data structure for implementation. The Trie node contains `child[26]` and `parent[26]` array to deal with the searching pattern. The `terminate` bool was used to determine if the character is the end of the word, which can be used in Exact Search. 
    Here's the definition of TrieNode.
    ```
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
    ```
- **WildCard Search**:
    Wildcard search functionality is implemented using Depth-First Search (DFS) to traverse the Trie, exploring all possibilities. The search stops when a valid result is found.

For detailed implementations of Exact Search, Prefix Search, Suffix Search, and Wildcard Search, please refer to the code.