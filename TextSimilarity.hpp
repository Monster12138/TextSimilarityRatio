#ifndef _TEXTSIMILARITY_HPP_
#define _TEXTSIMILARITY_HPP_

#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
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
    typedef wordFreq idfTableMap;
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
        getIdfTable(IDF_PATH);
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

    void sortByFreq(wordFreq& wf, wfVec& wfvec)
    {
        for(const auto& e : wf)
        {
            std::pair<std::string, int> word_TfIdf_pair = e;
            word_TfIdf_pair.second = wf[e.first]*idfTable_[e.first];

            wfvec.push_back(word_TfIdf_pair);
        }
        sort(wfvec.begin(), wfvec.end(), cmp());
    }

    void test(const char * str)
    {
        getIdfTable(str);
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

    void getIdfTable(const char * idfFileName)
    {
        std::ifstream fin(idfFileName);
        if(!fin.is_open())
        {
			std::cout << "open file: " << idfFileName<< " failed!\n";
			return;
        }

        std::string line;
        while(!fin.eof())
        {
            getline(fin, line);
            if(line.size() <= 1)break;
            std::pair<std::string, double> word_idf_pair;
            int space_index = 0;
            while(line[space_index] != ' ')
            {
                ++space_index;
            }

            word_idf_pair.first = line.substr(0, space_index);
            std::string idf_string = line.substr(space_index + 1);

            std::istringstream is(idf_string);            
            is >> word_idf_pair.second;

            idfTable_.insert(word_idf_pair);
        }

        fin.close();
    }

	cppjieba::Jieba jieba_;
	stopWordSet stopWords_;
    idfTableMap idfTable_;
};


 #endif // !_TEXTSIMILARITY_HPP_
