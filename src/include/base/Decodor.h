//
// Created by zhouh on 16-4-1.
//

#ifndef SNNOW_DECODOR_H
#define SNNOW_DECODOR_H





#include <limits.h>
#include <memory>
#include <assert.h>

#include "chunker.h"

#include "Beam.h"
#include "State.h"
#include "Instance.h"
#include "Example.h"
#include "LabeledSequence.h"
#include "ActionStandardSystem.h"
#include "FeatureManager.h"
#include "FeatureEmbedding.h"
#include "FeatureEmbeddingManager.h"

class TNNets;

class GreedyDecodor {

};

class BeamDecoder {
public:
    std::shared_ptr<ActionStandardSystem> m_transSystemPtr;
    std::shared_ptr<FeatureManager> m_featManagerPtr;
    std::shared_ptr<FeatureEmbeddingManager> m_featEmbManagerPtr;

    bool bTrain;
    bool bEarlyUpdate;
    Beam beam;
    State * lattice;
    State ** lattice_index;
    CScoredTransition goldScoredTran;

    int nGoldTransitionIndex;
    int nMaxLatticeSize;
    int nRound;
    int nMaxRound;
    int nSentLen;
    int mMiniBatchSize;

    Instance * inst;

public:
    BeamDecoder(Instance *inst,
                std::shared_ptr<ActionStandardSystem> transitionSystemPtr,
                std::shared_ptr<FeatureManager> featureMangerPtr,
                std::shared_ptr<FeatureEmbeddingManager> featureEmbManagerPtr,
                int beamSize,
                int miniBatchSize,
                bool bTrain);

    BeamDecoder(Instance *inst,
                std::shared_ptr<ActionStandardSystem> transitionSystemPtr,
                std::shared_ptr<FeatureManager> featureMangerPtr,
                std::shared_ptr<FeatureEmbeddingManager> featureEmbManagerPtr,
                int beamSize,
                int miniBatchSize,
                State *lattice,
                State **lattice_index,
                bool bTrain);

    ~BeamDecoder();

    void generateLabeledSequence(TNNets &tnnets, LabeledSequence &predictedSent);

    State* decode(TNNets &tnnet, GlobalExample *gExample = nullptr);

private:
    void generateInputBatch(State *state, Instance *inst, std::vector<FeatureVector> &featVecs) {
        for (int i = 0; i < static_cast<int>(featVecs.size()); i++) {
            m_featManagerPtr->extractFeature(*(state + i), *inst, featVecs[i]);
        }
    }
};

#endif //SNNOW_DECODOR_H
