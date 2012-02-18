#include <stdio.h>
#include "GenericAlgorithms.h"

#include "IFirstEngine.h"

#include <iostream>
using namespace std;

class Widget {
public:
  void f(int) const { cout << "Widget::f()\n"; }
  void g(int) const { cout << "Widget::g()\n"; }
  void h(int) const { cout << "Widget::h()\n"; }
  void i(int) const { cout << "Widget::i()\n"; }
};

void changeIt(int& ref) {
	static int val=0;

	ref = val++;
};

int main (int argc, char* argv[]) {

	
	MultiDim(vector,int,3) b;

//	resizeMultiDim(b,2);

	resizeMultiDimPartial(b,0,2);
	resizeMultiDimPartial(b,1,2);
	resizeMultiDimPartial(b,2,2);

	fillMultiDim(b,3);

	doInMultiDim(b,&changeIt);

	vector<int> myVect = multiToUniDim(b);

	multiToUniDim(b,myVect);
	
	for (auto i = myVect.begin(); i != myVect.end() ; ++i) {
		cout << "value " << *i << endl;
	}

	cout << GetMultiDimType<MultiDim(vector,int,5)>::dimension << GetMultiDimType<MultiDim(vector,int,4)>::dimension << GetMultiDimType<MultiDim(vector,int,1)>::dimension << endl;

	cout << sizeof(CEngine) << " " << sizeof(IFirstEngine) << " " << SUPERSUBCLASS_STRICT(CEngine,IFirstEngine) << endl;

	CLibrary::startLibrary(640,480,32,false);

	CEngine* engine = new IFirstEngine(CLibrary::getLibrary());

	CLibrary::addEngine(engine,10);

	CLibrary::processEngines();

	Widget* w = new Widget;

	  void (Widget::*pmem)(int) const = &Widget::h;
	  (w->*pmem)(2);

	return 0;

}