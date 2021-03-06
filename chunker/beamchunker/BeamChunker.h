/*************************************************************************
	> File Name: BeamChunker.h
	> Author: cheng chuan
	> Mail: cc.square0@gmail.com 
	> Created Time: Thu 19 Nov 2015 02:39:07 PM CST
 ************************************************************************/
#ifndef _CHUNKER_BEAMCHUNKER_BEAMCHUNKER_H_
#define _CHUNKER_BEAMCHUNKER_BEAMCHUNKER_H_ 
#include <iostream>
#include <memory>

#include "chunker.h"
#include "Config.h"

#include "Beam.h"
#include "BeamDecoder.h"

#include "Model.h"
#include "Instance.h"
#include "FeatureManager.h"
#include "FeatureEmbeddingManager.h"
#include "LabeledSequence.h"
#include "ActionStandardSystem.h"

class BeamChunkerThread;

class BeamChunker{
public:
    typedef std::tuple<double, double, double> ChunkedResultType;
private:
    LabelManager labelManager;
    std::shared_ptr<ActionStandardSystem> m_transSystemPtr;
    std::shared_ptr<DictManager> m_dictManagerPtr;
    std::shared_ptr<FeatureEmbeddingManager> m_featEmbManagerPtr;
    std::shared_ptr<FeatureManager> m_featManagerPtr;
    std::shared_ptr<Model<cpu>> m_modelPtr;
    std::vector<std::shared_ptr<BeamChunkerThread>> m_chunkerThreadPtrs;

    int m_nBeamSize;
    bool m_bTrain;
    int num_in, num_hidden, num_out;

    GlobalExamples gExamples;
public:
    BeamChunker();
    BeamChunker(bool isTrain);

    ~BeamChunker();

    void train(ChunkedDataSet &trainGoldSet, InstanceSet &trainSet, ChunkedDataSet &devGoldSet,  InstanceSet &devSet);
    
private:
    std::pair<ChunkedResultType, ChunkedResultType> chunk(InstanceSet &devInstances, ChunkedDataSet &goldDevSet, Model<cpu> &modelParas);

    void generateMultiThreadsMiniBatchData(std::vector<std::vector<GlobalExample *>> &multiThread_miniBatch_data);

    void initTrain(ChunkedDataSet &goldSet, InstanceSet &trainSet);

    void initDev(InstanceSet &devSet);

    void initBeamChunkerThread(InstanceSet &devSet);

    void saveChunker(int round = -1);

    BeamChunker(const BeamChunker &chuker) = delete;
    BeamChunker& operator= (const BeamChunker &chunker) = delete;
};

#endif
