/*************************************************************************
	> File Name: Dictionary.h
	> Author: cheng chuan
	> Mail: cc.square0@gmail.com 
	> Created Time: Thu 24 Dec 2015 03:24:57 PM CST
 ************************************************************************/
#ifndef _CHUNKER_DICTIONARY_H_
#define _CHUNKER_DICTIONARY_H_

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <tr1/unordered_map>
#include <unordered_set>

#include "ChunkedSentence.h"

#define DEBUG

class Dictionary {
public:
    std::vector<std::string> m_lKnownElements;
    std::tr1::unordered_map<std::string, int> m_mElement2Idx;

    int nullIdx;
    int unkIdx;

    static const std::string nullstr;
    static const std::string unknownstr;

public:
    Dictionary() {}
    virtual ~Dictionary() {}

    int size() {
        return static_cast<int>(m_mElement2Idx.size());
    }

    const std::vector<std::string>& getKnownElements() const {
        return m_lKnownElements;
    }

    virtual int element2Idx(const std::string &s) {
        auto it = m_mElement2Idx.find(s);

        return (it == m_mElement2Idx.end()) ? unkIdx: it->second;
    }

    virtual void makeDictionaries(const ChunkedDataSet &goldSet) = 0;

    void printDict() {
        std::cerr << "known feature size: " << m_lKnownElements.size() << std::endl;
    }

private:
    Dictionary(const Dictionary &dManager) = delete;
    Dictionary& operator= (const Dictionary &dManager) = delete;
};

class WordDictionary : public Dictionary {
public:
    int numberIdx;

    static const std::string numberstr;

public:
    WordDictionary() {}
    ~WordDictionary() {}

    void makeDictionaries(const ChunkedDataSet &goldSet);

    int element2Idx(const std::string &s);

    static std::string processWord(const std::string &word);

    static bool isNumber(const std::string &word);
};

class POSDictionary : public Dictionary {
public:
    POSDictionary() {}
    ~POSDictionary() {}

    void makeDictionaries(const ChunkedDataSet &goldSet);
};

class LabelDictionary : public Dictionary {
public:
    LabelDictionary() { }
    ~LabelDictionary() {}

    int element2Idx(const std::string &s);

    void makeDictionaries(const ChunkedDataSet &goldSet);
};

class CapitalDictionary : public Dictionary {
public:
    static const std::string noncapitalstr;
    static const std::string allcapitalstr;
    static const std::string firstlettercapstr;
    static const std::string hadonecapstr;

public:
    CapitalDictionary() {}
    ~CapitalDictionary() {}

    int element2Idx(const std::string &s);

    void makeDictionaries(const ChunkedDataSet &goldSet);
};

#endif