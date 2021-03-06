//
// Created by zhouh on 16-4-1.
//

#include "DepArcStandardSystem.h"

std::string DepArcStandardSystem::c_root_str = "_root_str_";

// the move action is a simple call to do action according to the action code
void DepArcStandardSystem::Move(State & state, Action& action) {

    DepParseState& parse_state = static_cast<DepParseState&>(state);
    DepParseAction &parse_action = static_cast<DepParseAction &>(action);


    const int action_type = parse_action.getActionType();
    switch (action_type){
        case (DepParseAction::shift_type) : {
            Shift(parse_state);
            break;
        }
        case(DepParseAction::left_type) : {
            ArcLeft(parse_state, parse_action);
            break;
        }
        case(DepParseAction::right_type) : {
            ArcRight(parse_state, parse_action);
            break;
        }
    }

}

/**
 * fill valid labels to retval
 */
void DepArcStandardSystem::getValidActs(State & state, std::vector<int> & retval) {

    DepParseState & parse_state = static_cast<DepParseState&>(state);

    // mark all action valid
    retval.resize(action_factory_ptr->total_action_num, 0);
    // except reduce left, because root will never be dependency word.
    retval[action_factory_ptr->makeAction(DepParseAction::left_type, rootLabelIndex)->getActionCode()] = -1; //left-root is unvalid

    int stack_size = parse_state.m_Stack.size();
    int queue_size = parse_state.len_ - parse_state.m_nNextWord;

    //shift
    if (queue_size <= 0)
        retval[action_factory_ptr->shift_action.getActionCode()] = -1;

    int stack_left = parse_state.stack2top();

    /*
     * reduce
     */
    // if stack size > 2 and queue is not null
    // all reduce is valid except reduce root
    if (stack_size >= 2 && parse_state.stack2top()!= 0) {
        retval[action_factory_ptr->makeAction(DepParseAction::right_type, rootLabelIndex)->getActionCode()] = -1;
//        retval[action_factory_ptr->makeAction(DepParseAction::left_type, rootLabelIndex)->getActionCode()] = -1;
        return;
    }
    else {  // when stack size < 2 or queue == null
        // mark all action invalid
        retval.resize(action_factory_ptr->total_action_num, -1);

        /*
         * if queue is not null,
         * only shift is valid
         */
        if (queue_size > 0)
            retval[action_factory_ptr->shift_action.getActionCode()] = 0;

        /*
         * if stack only contains root and one word, and queue is null
         * then only reduce root is valid.
         *
         * Note, ``parse_state.stack2top() == 0 "" means left word in the stack is root, because
         * root id is 0.
         */
        if( parse_state.stack2top() == 0 && queue_size == 0) //except right reduce root
            retval[action_factory_ptr->makeAction(DepParseAction::right_type, rootLabelIndex)->getActionCode()] = 0;
        return;
    }

}

Action* DepArcStandardSystem::StandardMove(State & state, Output& tree) {

    DepParseState& parse_state = static_cast<DepParseState&>(state);
    DepParseTree& parse_tree = static_cast<DepParseTree&>(tree);

    if (parse_state.complete()) {
        std::cerr << "The parsing state is completed!" << std::endl;
        exit(1);
    }

    int w2 = parse_state.stacktop();
    int w1 = parse_state.stack2top();
    int stackSize = parse_state.stacksize();

    if( stackSize >= 2 && parse_tree.nodes[w1].head == w2)
        return action_factory_ptr->makeAction(
                DepParseAction::left_type,
                dep_label_map_ptr[parse_tree.nodes[w1].label]
        );
    if( stackSize >= 2 && parse_tree.nodes[w2].head == w1 && !parse_state.hasChildOnQueue(w2, parse_tree) )
        return action_factory_ptr->makeAction(
                DepParseAction::right_type,
                dep_label_map_ptr[parse_tree.nodes[w2].label]
        );
    return static_cast<Action*>(& (action_factory_ptr->shift_action) );
}

void DepArcStandardSystem::StandardMoveStep(State & state, Output& tree) {
    auto action = StandardMove(state, tree);
    Move(state, *action);
}

// we want to pop the root item after the whole tree done
// on the one hand this seems more natural
// on the other it is easier to score
void DepArcStandardSystem::GenerateOutput(State& state, Input& input, Output& output) {

    DepParseState& parse_state = static_cast<DepParseState&>(state);
    DepParseTree& parse_output = static_cast<DepParseTree&>(output);

    std::cout<<"generate tree"<<std::endl;
    for (int i = 1; i < parse_state.len_; ++i) {
        parse_output.setHead(i, parse_state.m_lHeads[i]);
        parse_output.setLabel(i, known_labels[parse_state.m_lLabels[i]]);
    }

    std::cout<<"generate tree end"<<std::endl;
}

/*
 * Perform Arc-Left operation in the arc-standard algorithm
 */
void DepArcStandardSystem::ArcLeft(DepParseState & state, DepParseAction& action) {
    // At least, there must be two elements in the stack.
    assert(state.stacksize() > 1);

    int stack_size = state.stacksize();
    int top0 = state.stacktop(); // here top0 are the index of sentence cache
    int top1 = state.stack2top();

    state.popStack();
    state.setStackTop(top0);

    state.m_lHeads[top1] = top0;
    state.m_lLabels[top1] = action.getActionLabel();

    if (state.m_lDepsL[top0] == EMPTY_ARC) {
        state.m_lDepsL[top0] = top1;
    } else if (top1 < state.m_lDepsL[top0]) {
        state.m_lDepsL2[top0] = state.m_lDepsL[top0];
        state.m_lDepsL[top0] = top1;
    } else if (top1 < state.m_lDepsL2[top0]) {
        state.m_lDepsL2[top0] = top1;
    }

    state.last_action = action;
}

/*
 * Perform the arc-right operation in arc-standard
 */
void DepArcStandardSystem::ArcRight(DepParseState & state, DepParseAction& action) {

    assert(state.stacksize() > 1);


    int stack_size = state.stacksize();
    int top0 = state.stacktop();  // here top0 are the index of sentence cache
    int top1 = state.stack2top();

    state.popStack();
    state.m_lHeads[top0] = top1;
    state.m_lLabels[top0] = action.getActionLabel();

    if (state.m_lDepsR[top1] == EMPTY_ARC) {
        state.m_lDepsR[top1] = top0;
    } else if (state.m_lDepsR[top1] < top0) {
        state.m_lDepsR2[top1] = state.m_lDepsR[top1];
        state.m_lDepsR[top1] = top0;
    } else if (state.m_lDepsR2[top1] < top0) {
        state.m_lDepsR2[top1] = top0;
    }

    state.last_action = action;
}

/* 
 * the shift action does pushing
 *
 */
void DepArcStandardSystem::Shift(DepParseState& state) {
    state.m_Stack.push_back(state.m_nNextWord);
    state.m_nNextWord++;
    //state.ClearNext();
    state.last_action = action_factory_ptr->shift_action;
}