#include "TextSimilarity.hpp"

using namespace std;

const int maxAimWords_ = 20;

void selectAimWords(const TextSimilarity::wfVec& wfvec1, const TextSimilarity::wfVec& wfvec2, TextSimilarity::StringSet& aimWordsSet)
{
    assert(wfvec1.size() > maxAimWords_/2 || wfvec2.size() > maxAimWords_/2);
    int size = maxAimWords_;
    for(int i = 0; i < size/2; ++i )
    {
        aimWordsSet.insert(wfvec1[i].first);
    }

    for(int i = 0; i < size/2; ++i )
    {
        aimWordsSet.insert(wfvec2[i].first);
    }
    for(const auto & e: aimWordsSet)
    {
        cout << e << endl;
    }
}

void getOneHot(TextSimilarity::wordFreq& wf1,TextSimilarity::wordFreq& wf2, const TextSimilarity::StringSet& aimWordsSet, TextSimilarity::wordFreqCoding& wfCoding1, TextSimilarity::wordFreqCoding& wfCoding2)
{
    for(const auto& e : aimWordsSet)
    {
        if(wf1.count(e))
        {
            wfCoding1.push_back(wf1[e]);
        }
        else
        {
            wfCoding1.push_back(0);
        }
    }
    
    for(const auto & e: wfCoding1)
    {
        cout << e << " ";
    }
    cout << endl;
    for(const auto& e : aimWordsSet)
    {
        if(wf2.count(e))
        {
            wfCoding2.push_back(wf2[e]);
        }
        else
        {
            wfCoding2.push_back(0);
        }
    }
    for(const auto & e: wfCoding2)
    {
        cout << e << " ";
    }
    cout << endl;
}

double getSimilarityRatio(const vector<int>& v1, const vector<int>& v2)
{
    double ratio;
    double sum = 0, sumSquv1 = 0, sumSquv2 = 0;

    for(int i = 0; i < (int)v1.size(); ++i)
    {
        sum += v1[i] * v2[i];
    }

    for(const auto &e : v1)
    {
        sumSquv1 += e*e;
    }
    for(const auto &e : v2)
    {
        sumSquv2 += e*e;
    }

    ratio = sum/(sqrt(sumSquv1) * sqrt(sumSquv2));
    return ratio;
}

void testWordFreq(const char * file1, const char * file2)
{
    cout << file1 << endl;
	TextSimilarity ts1;
    //统计词频到map
	TextSimilarity::wordFreq wf1 = ts1.getWordFreq(file1);

    //按词频排序到vector
    TextSimilarity::wfVec wfvec1;
    ts1.sortByFreq(wf1, wfvec1);
    cout << "***************按词频排序:\n";
    for(int i = 0; i < 10; ++i)
    { 
        cout << wfvec1[i] << endl;
    }
    TextSimilarity::StringSet wfSet;

    cout << file2 << endl;
	TextSimilarity ts2;
    //统计词频到map
	TextSimilarity::wordFreq wf2 = ts1.getWordFreq(file2);

    //按词频排序到vector
    TextSimilarity::wfVec wfvec2;
    ts2.sortByFreq(wf2, wfvec2);
    cout << "***************按词频排序:\n";
    for(int i = 0; i < 10; ++i)
    { 
        cout << wfvec2[i] << endl;
    }

    TextSimilarity::wordFreqCoding wfCoding1;
    TextSimilarity::wordFreqCoding wfCoding2;

    cout << "***************用set去重:\n";
    selectAimWords(wfvec1, wfvec2, wfSet);

    cout << "编码:\n";
    getOneHot(wf1, wf2, wfSet, wfCoding1, wfCoding2);    
    cout << "文本相似性:" << getSimilarityRatio(wfCoding1, wfCoding2) << endl;
}

void test()
{
    TextSimilarity ts;
    ts.test(IDF_PATH);
}

int main(int argc, char** argv)
{
#if 1
    if(argc != 3)
    {
        cout << "user page: programname filename1 filename2" << endl;
        exit(-1);
    }

	testWordFreq(argv[1], argv[2]);
#endif
	return 0;
 }
