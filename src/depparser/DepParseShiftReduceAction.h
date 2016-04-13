//
// Created by zhouh on 16-4-12.
//

#ifndef SNNOW_DEPPARSESHIFTREDUCEACTION_H
#define SNNOW_DEPPARSESHIFTREDUCEACTION_H

#include <vector>

#include "base/Action.h"

class DepParseAction : public Action{

public:
    static int shift_type = 0;
    static int left_type = 1;
    static int right_type = 2;

    DepParseAction(int action_type, int action_label){
        this->action_type = action_type;
        this->action_label = action_label;
        action_code = getActionCode(action_type, action_label);
    }

    /**
     * get the action code given the action type and action label
     */
    static int getActionCode(int action_type, int action_label){
        switch (action_type){
            case shift_type : return shift_type;
            case left_type : return left_type + action_label;
            case right_type : return action_label + 1 + DepParseShiftReduceActionFactory::action_la bel_num;

        }
    }

};

class DepParseShiftReduceActionFactory : public ActionFactory {

public:
    static DepParseAction shift_action;
    static std::vector<DepParseAction> left_reduce_actions;
    static std::vector<DepParseAction> right_reduce_actions;



    DepParseShiftReduceActionFactory(int action_label_num){
        this->action_type_num = 3;
        this->action_label_num = action_label_num;
        total_action_num = 2 * action_label_num + 1;
        action_table.resize(total_action_num, nullptr); // resize the action table

    }

    // return the action given action type and label
    virtual static DepParseAction* makeAction(int action_type, int action_label) {

        int action_code = DepParseAction::getActionCode(action_type, action_label);

        if( action_table[action_code] != nullptr){
            return action_table[action_code].get();
        }
        else{
            std::shared_ptr<Action> new_action_ptr = new DepParseAction(action_type, action_label);
            action_table[action_code] = new_action_ptr;

            if( action_type == DepParseAction::shift_type )
                shift_action = new_action_ptr.operator*();
            else if( action_type == DepParseAction::left_type )
                left_reduce_actions.push_back(new_action_ptr.operator*());
            else if( action_type == DepParseAction::right_type )
                right_reduce_actions.push_back(new_action_ptr.operator*());
            else
                exit(1);  //it is not a valid action type

            return new_action_ptr;
        }

    }
};


#endif //SNNOW_DEPPARSESHIFTREDUCEACTION_H