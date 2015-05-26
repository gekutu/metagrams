#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#define PATH_LENGTH 256
#define MAX_WORD_SIZE 256

class Metagrams {
private:
	vector<wstring> m_words;
	vector<int> m_result;
	vector<int> m_currentChain;
public :
	Metagrams(const vector<wstring> & words) {
		m_words.reserve(words.size());
		copy(words.begin(), words.end(), back_inserter(m_words));
	};

	bool process() {
		m_currentChain.push_back(0);
		findChain(0);
		return m_result.size() != 0;
	}

	vector<int> getResult() {
		return m_result;
	}

private:
	bool isMetagram(int i, int j) {
		int k = 0;
		for (unsigned int l = 0; l < m_words[i].length(); ++l) {
			if (m_words[i].at(l) != m_words[j].at(l)){
				++k;
				if (k > 1)
					return false;
			}
		}
		return k == 1;
	}
	
	void findChain(int i) {
		for (unsigned int j = i+1; j < m_words.size(); ++j){
			if (isMetagram(i, j)) {
				m_currentChain.push_back(j);
				if (j == m_words.size() - 1) {
					//skip chains which don't include at least one word from the dictionary
					if (m_currentChain.size() > 2 && (m_result.size() == 0 || m_currentChain.size() < m_result.size())) {
						m_result.clear();
						m_result.reserve(m_currentChain.size());
						copy(m_currentChain.begin(), m_currentChain.end(), back_inserter(m_result));
					}
				}
				else {
					findChain(j);
				}
				m_currentChain.pop_back();
			}
		}
	}
};

void prompt() {
	cout << "Press any key + Enter to close the program\n";
	char c;
	cin >> c;
}

int main(int argc, char *argv[])
{
	locale loc("rus_rus.866");

	wofstream w("c:/temp/words.txt");
	w.imbue(loc);
	w << L"ÊÎÒ" << endl;
	w << L"ÒÎÍ" << endl;
	w.close();

	wofstream d("c:/temp/dictionary.txt");
	d.imbue(loc);
	d << L"ÊÎÒ" << endl;
	d << L"ÒÎÍ" << endl;
	d << L"ÍÎÒÀ" << endl;
	d << L"ÊÎÒÛ" << endl;
	d << L"ÐÎÒ" << endl;
	d << L"ÐÎÒÀ" << endl;
	d << L"ÒÎÒ" << endl;
	d.close();

	char wordsFile[PATH_LENGTH], dictionaryFile[PATH_LENGTH]; 
	cout << "Enter full path to the file with the first and the last words\n";
	cin >> wordsFile;
	wifstream words(wordsFile);
	words.imbue(loc);
	if (!words.is_open()) {
		cout << "Invalid path to the file with the first and the last words: " << wordsFile << "\n";
		prompt();
		return 1;
	}
	if (words.eof()) {
		cout << "The file with the first and the last words doesn't contain the first word\n";
		prompt();
		return 1;
	}
	wchar_t temp[MAX_WORD_SIZE];
	wstring firstWord, lastWord;
	words.getline(temp, MAX_WORD_SIZE);
	firstWord = temp;
	if (firstWord.empty()){
		cout << "The file with the first and the last words doesn't contain the first word\n";
		prompt();
		return 1;
	}

	if (words.eof()) {
		
		cout << "The file with the first and the last words doesn't contain the last word\n";
		prompt();
		return 1;
	}
	words.getline(temp, MAX_WORD_SIZE);
	lastWord = temp;
	if (lastWord.empty()){
		cout << "The file with the first and the last words doesn't contain the last word\n";
		prompt();
		return 1;
	}
	words.close();

	if(firstWord.length() != lastWord.length()) {
		cout << "The first word length is not equal to the last word length\n";
		prompt();
		return 1;
	}

	cout << "Enter full path to the file with the dictionary\n";
	cin >> dictionaryFile;
	wifstream dictionary(dictionaryFile);
	dictionary.imbue(loc);
	if (!dictionary.is_open()) {
		cout << "Invalid path to the file with the dictionary: " << dictionaryFile << "\n";
		prompt();
		return 1;
	}

	if (dictionary.eof()) {
		cout << "The file with the dictionary is empty\n";
		prompt();
		return 1;
	}

	dictionary.getline(temp, MAX_WORD_SIZE);
	if (wcslen(temp) == 0) {
		cout << "The file with the dictionary is empty\n";
		prompt();
		return 1;
	}

	vector<wstring> allWords;
	//add the first word
	allWords.push_back(firstWord);

	//add the word from the first line of the dictionary
	if (firstWord.length() == wstring(temp).length())
		allWords.push_back(temp);
	//add the other words from the dictionary
	while (!dictionary.eof()) {
		dictionary.getline(temp, MAX_WORD_SIZE);
		if (firstWord.length() == wstring(temp).length())
			allWords.push_back(temp);
	}
	dictionary.close();
	//add the last word
	allWords.push_back(lastWord);
	Metagrams metagrams(allWords);
	if (metagrams.process()) {
		vector<int> result = metagrams.getResult();
		wcout.imbue(loc);
		for (unsigned int i = 0; i < result.size(); ++i) {
			wcout << allWords[result[i]] << "\n";
		}
	}
	else {
		cout << "Words chain is not found\n";
	}
	prompt();
	return 0;
}