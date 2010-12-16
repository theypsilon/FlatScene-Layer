#include "Node.h"
#include "Library.h"

CNode::CNode(int numLinks) {
	this->numLinks=numLinks;
	links =  alloc(CNode*,numLinks);
	for (int i=0;i<numLinks;i++)
		links[i]=NULL;
}

CNode::~CNode() {
	inDelete();
}

CNode* CNode::get(int link) {
	return ((link < numLinks) && (link >= 0)) ? links[link] : NULL;
}

CNode* CNode::set(int link,CNode* newNode) {
	CNode* ret=NULL;
	if ((link < numLinks) && (link >= 0)) {
		ret=links[link];
		links[link]=newNode;
	}
	return ret;
}

int CNode::size() {
	return numLinks;
}

void CNode::inDelete() {
	if (numLinks > 0) {
		freemem(links);
		numLinks=0;
	}
}