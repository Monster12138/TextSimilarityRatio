#ifndef _TEXTSIMILARITY_HPP_
#define _TEXTSIMILARITY_HPP_

#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "cppjieba/Jieba.hpp"

const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";


class TextSimilarity
{
public:
	typedef std::unordered_map<std::string, int> wordFreq;
	typedef std::unordered_set<std::string> StringSet;
    typedef StringSet stopWordSet;
    typedef std::vector<std::pair<std::string, int>> wfVec;
    typedef std::pair<std::string, int> wordFreqPair;
    typedef std::vector<int> wordFreqCoding;

    struct cmp
    {
        bool operator()(const wordFreqPair& lp, const wordFreqPair& rp)
        {
            return lp.second > rp.second;
        }
    };

	TextSimilarity()
	    :jieba_(DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH)
	{
		getStopWordTable(STOP_WORD_PATH);
	}

	wordFreq getWordFreq(const char *filename)
	{
		std::ifstream fin(filename);
		if (!fin.is_open())
		{
			std::cout << "open file: " << filename << "failed!\n请检查文件名、后缀名及路径!\n";
            exit(-2);
		}

		wordFreq wf_;
		std::string line;
		while (!fin.eof())
		{
			getline(fin, line);
			//std::cout << line << std::endl;
			
			std::vector<std::string> words;
			jieba_.Cut(line, words, true);

			for (const auto& e : words)
			{
				if (stopWords_.count(e) == 0)
				{
					if (wf_.count(e) > 0)
					{
						++wf_[e];
                    }
                    else
                    {
                        wf_.insert(std::make_pair(e, 1));
                        //std::cout << "insert word:" << e << " " << cont << std::endl;
                    }
                }
                else
                {
                    //std::cout << "stop word:" << e << std::endl;
                }
			}
		}

		return wf_;
	}

    void sortByFreq(const wordFreq& wf, wfVec& wfvec)
    {
        for(const auto& e : wf)
        {
            wfvec.push_back(e);
        }
        sort(wfvec.begin(), wfvec.end(), cmp());
    }

    
private:
	void getStopWordTable(const char * stopWordFileName)
	{
		std::ifstream fin(stopWordFileName);
		if (!fin.is_open())
		{
			std::cout << "open file: " << stopWordFileName << " failed!\n";
			return;
		}

		std::string line;
		while (!fin.eof())
		{
			getline(fin, line);
			stopWords_.insert(line);
		}
		fin.close();
	}

	cppjieba::Jieba jieba_;
	stopWordSet stopWords_;
};


 #endif // !_TEXTSIMILARITY_HPP_
