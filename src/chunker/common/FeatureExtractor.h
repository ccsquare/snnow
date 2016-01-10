/*************************************************************************
	> File Name: Feature.h
	> Author: cheng chuan
	> Mail: cc.square0@gmail.com 
	> Created Time: Sat 26 Dec 2015 03:48:13 PM CST
 ************************************************************************/
#ifndef _CHUNKER_COMMON_FEATUREEXTRACTOR_H_
#define _CHUNKER_COMMON_FEATUREEXTRACTOR_H_

#include <string>
#include <memory>

#include "Dictionary.h"
#include "State.h"
#include "Instance.h"
#include "FeatureType.h"

class FeatureExtractor {
protected:
    FeatureType featType;
    std::shared_ptr<Dictionary> dictPtr;

public:
    FeatureExtractor(const FeatureType &fType, const std::shared_ptr<Dictionary> &dictPtr) : featType(fType), dictPtr(dictPtr)
    {
    }

    const FeatureType& getFeatureType() const {
        return featType;
    }

    const std::shared_ptr<Dictionary>& getDictPtr() const {
        return dictPtr;
    }

    virtual ~FeatureExtractor() {}

    virtual std::vector<int> extract(const State &state, const Instance &inst) = 0;

private:
    FeatureExtractor(const FeatureExtractor &fe) = delete;
    FeatureExtractor& operator= (const FeatureExtractor &fe) = delete;
};

class WordFeatureExtractor : public FeatureExtractor {
public:
    WordFeatureExtractor(const FeatureType &fType, const std::shared_ptr<Dictionary> &dictPtr) : 
    FeatureExtractor(fType, dictPtr)
    {

    }
    ~WordFeatureExtractor() {}
    
    std::vector<int> extract(const State &state, const Instance &inst) {
        std::vector<int> features;

        auto getWordIndex = [&state, &inst, this](int index) -> int {
            if (index < 0 || index >= state.m_nLen) {
                return this->dictPtr->nullIdx;
            }

            return inst.wordCache[index];
        };
        
        int currentIndex = state.m_nIndex + 1;
        int IDIdx = 0;

        features.resize(featType.featSize);

        int neg2UniWord   = getWordIndex(currentIndex - 2);
        int neg1UniWord   = getWordIndex(currentIndex - 1);
        int pos0UniWord   = getWordIndex(currentIndex);
        int pos1UniWord   = getWordIndex(currentIndex + 1);
        int pos2UniWord   = getWordIndex(currentIndex + 2);
        features[IDIdx++] = neg2UniWord;
        features[IDIdx++] = neg1UniWord;
        features[IDIdx++] = pos0UniWord;
        features[IDIdx++] = pos1UniWord;
        features[IDIdx++] = pos2UniWord;

        return features;
    }
};

class POSFeatureExtractor : public FeatureExtractor {
public:
    POSFeatureExtractor(const FeatureType &fType, const std::shared_ptr<Dictionary> &dictPtr) :
        FeatureExtractor(fType, dictPtr)
    {
    }

    ~POSFeatureExtractor() {}

    std::vector<int> extract(const State &state, const Instance &inst) {
        std::vector<int> features;

        auto getPOSIndex = [&state, &inst, this](int index) -> int {
            if (index < 0 || index >= state.m_nLen) {
                return this->dictPtr->nullIdx;
            }

            return inst.tagCache[index];
        };
        
        int currentIndex = state.m_nIndex + 1;
        int IDIdx = 0;

        features.resize(featType.featSize);

        int neg2UniWord   = getPOSIndex(currentIndex - 2);
        int neg1UniWord   = getPOSIndex(currentIndex - 1);
        int pos0UniWord   = getPOSIndex(currentIndex);
        int pos1UniWord   = getPOSIndex(currentIndex + 1);
        int pos2UniWord   = getPOSIndex(currentIndex + 2);
        features[IDIdx++] = neg2UniWord;
        features[IDIdx++] = neg1UniWord;
        features[IDIdx++] = pos0UniWord;
        features[IDIdx++] = pos1UniWord;
        features[IDIdx++] = pos2UniWord;

        return features;
    }
};

class CapitalFeatureExtractor : public FeatureExtractor {
public:
    CapitalFeatureExtractor(const FeatureType &fType, const std::shared_ptr<Dictionary> &dictPtr) : 
        FeatureExtractor(fType, dictPtr)
    {
    }
    ~CapitalFeatureExtractor() {}
    
    std::vector<int> extract(const State &state, const Instance &inst) {
        std::vector<int> features;

        auto getCapfeatIndex = [&state, &inst, this](int index) -> int {
            if (index < 0 || index >= state.m_nLen) {
                return this->dictPtr->nullIdx;
            }

            return inst.capfeatCache[index];
        };

        int currentIndex = state.m_nIndex + 1;
        int IDIdx = 0;

        features.resize(featType.featSize);

        int pos0UniCap    = getCapfeatIndex(currentIndex);
        //int neg2UniCap    = getCapfeatIndex(currentIndex - 2);
        //int neg1UniCap    = getCapfeatIndex(currentIndex - 1);
        //int pos1UniCap    = getCapfeatIndex(currentIndex + 1);
        //int pos2UniCap    = getCapfeatIndex(currentIndex + 2);
        features[IDIdx++] = pos0UniCap;
        // features[IDIdx++] = neg2UniCap;
        // features[IDIdx++] = neg1UniCap;
        // features[IDIdx++] = pos1UniCap;
        // features[IDIdx++] = pos2UniCap;

        return features;
    }
};

class LabelFeatureExtractor : public FeatureExtractor {
public:
    LabelFeatureExtractor(const FeatureType &fType, const std::shared_ptr<Dictionary> &dictPtr) :
        FeatureExtractor(fType, dictPtr) 
    {
    }
    ~LabelFeatureExtractor() { }

    std::vector<int> extract(const State &state, const Instance &inst) {
        std::vector<int> features;

        auto getLabelIndex = [&state, &inst,  this](int index) -> int {
            if (index < 0) {
                return this->dictPtr->nullIdx;
            }

            return state.frontLabels[index];
        };

        int currentIndex = state.m_nIndex + 1;
        int IDIdx = 0;

        features.resize(featType.featSize);

        int neg2UniLabel  = getLabelIndex(currentIndex - 2);
        int neg1UniLabel  = getLabelIndex(currentIndex - 1);

        features[IDIdx++] = neg2UniLabel;
        features[IDIdx++] = neg1UniLabel;

        return features;
    }
};

#endif
