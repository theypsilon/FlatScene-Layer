#ifndef __CNODE_H__
#define __CNODE_H__

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "definitions.h"

class CNode {
private:
	CNode** links;
	int numLinks;
public:
	CNode(int numLinks=0);
	virtual ~CNode();
	virtual void inDelete();
	virtual CNode* get(int link);
	virtual CNode* set(int link, CNode* newNode);
	virtual int size();
};

#endif
