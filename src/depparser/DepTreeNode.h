/*
 * Deptree.h
 *
 *  Created on: Jul 2, 2015
 *      Author: zhouh
 */

#ifndef SNNOW_DEPPARSER_DEPTREENODE_H_
#define SNNOW_DEPPARSER_DEPTREENODE_H_

#include <string>
#include <assert.h>
#include <sstream>

class DepTreeNode {
public:

	DepTreeNode() {head = -1;}

	DepTreeNode(std::string w, std::string t, int h, std::string l){
		word = w;
		tag = t;
		head = h;
        label = l;
	}

    /*
     * used for construct testing instances
     */
	DepTreeNode(std::string w, std::string t){ 
		word = w;
		tag = t;
	}

	virtual ~DepTreeNode(){}

public:
	std::string word;
	std::string tag;
	int head;
	std::string label;

};
	inline std::istream & operator >> (std::istream &is, DepTreeNode &node) {
	   std::string line;

	   //0
	   getline(is, line, '\t');
	   assert(is && !line.empty());
	   //1 word
	   getline(is, line, '\t');
	   assert(is && !line.empty());
	   node.word = line;
//	   //2
	   getline(is, line, '\t');
	   assert(is && !line.empty());
//	   //3
	   getline(is, line, '\t');
	   assert(is && !line.empty());
	   //4 POS-tag
	   getline(is, line, '\t');
	   assert(is && !line.empty());
	   node.tag = line;
//	   //5
	   getline(is, line, '\t');
	   assert(is && !line.empty());
	   //6 head
	   getline(is, line, '\t');
	   assert(is && !line.empty());
	   std::istringstream iss_id(line);
	   iss_id >> node.head;

	   //7 label
	   getline(is, line, '\t');
	   assert(is && !line.empty());
	   node.label = line;

	   return is ;
	}

	inline std::ostream & operator << (std::ostream &os, const DepTreeNode &node) {
	   os << node.word << '\t' << node.tag << '\t' << "_" << '\t' <<
			   node.head << "\t" << "_" << '\t' << "_" << '\t' << node.label<<std::endl;
	   return os ;
	}


#endif /* SNNOW_DEPPARSER_DEPTREENODE_H_ */
