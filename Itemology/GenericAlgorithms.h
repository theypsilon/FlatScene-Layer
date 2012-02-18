#ifndef _GENERIC_ALGORITHMS_JOSE_
#define _GENERIC_ALGORITHMS_JOSE_

template <typename T,typename S> void setIfTrue(T& a, T b, S foo) {
	if (foo(a,b))
		a = b;
};

template <typename T,typename S> void setIfTrue(bool a, T& b, S foo) {
	if (a) foo(b);
};

#define LambdaInc(A) LambdaIncT(A,int)
#define LambdaIncT(A,B) [] (B& a) { a += A; }

#define LambdaSet(A) LambdaSetT(A,int)
#define LambdaSetT(A,B) [] (B& a) { a = A; }

#define LambdaLess LambdaLessT(int)
#define LambdaLessT(A) [] (A a,A b) { return a < b; }

#define LambdaMore LambdaMoreT(int)
#define LambdaMoreT(A) [] (A a,A b) { return a > b; };


template <class T, class U> class Conversion
{
	typedef char Small;
	typedef int Big;

	static Small Test(U);
	static Big Test(...);

	static T MakeT();
public:
	enum { 
		exists = sizeof(Test(MakeT())) == sizeof(Small),
		sameType = false 
	};
};

template <class T> class Conversion<T, T>
{
public:
	enum { exists = 1, sameType = 1 };
};

#define SUPERSUBCLASS(T, U) \
	(Conversion<const U*, const T*>::exists && \
	!Conversion<const T*, const void*>::sameType)

#define SUPERSUBCLASS_STRICT(T, U) \
	(SUPERSUBCLASS(T, U) && \
	!Conversion<const T, const U>::sameType)

#endif



class NullType;

template <typename T>
struct Type2Type {
	typedef T OriginalType;
};

template <int v>
struct Int2Type
{
	enum { value = v };
};


template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

#define STATIC_CHECK(expr, msg) \
    { CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 


template <bool flag, typename T, typename U>
struct Select {
	typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U>	{
	typedef U Result;
};


#include <vector>

template <template <class,class> class Container, class T, unsigned int i, template <class> class Allocator = std::allocator >
struct multiDim 
{
private:
	typedef typename multiDim<Container,T,i-1,Allocator>::Result temp;
public:
	typedef Container < temp , Allocator < temp> > Result;
};

template <template <class,class> class Container, class T, template <class> class Allocator>
struct multiDim < Container, T, 1 , Allocator>
{
	typedef Container<T, Allocator <T> > Result;
};

template <template <class,class> class Container, class T , template <class> class Allocator>
struct multiDim < Container, T, 0 ,Allocator>
{
private:
	class MultiDimCantBeZero;
public:
	typedef MultiDimCantBeZero Result;
};

#define MultiDim(A,B,C) multiDim<A,B,C>::Result
#define MultiDimA(A,B,C,D) multiDim<A,B,C,D>::Result

template <class STDContainer, class T> void fillMultiDim (STDContainer& o, T value) {
	for (auto it = o.begin() ; it!= o.end() ; ++it) {
		fillMultiDim(*it,value);
	}
};

template <template <class,class> class STDContainer, class T, class Allocator> void fillMultiDim(STDContainer < T , Allocator >& o, T value) {
	for (auto it = o.begin() ; it!= o.end() ; ++it) {
		*it = value;
	}
}

template <class STDContainer, class T> void resizeMultiDim (STDContainer& o, unsigned int i,Type2Type<T>) {
	o.resize(i);
	for (auto it = o.begin() ; it!= o.end() ; ++it) {
		resizeMultiDim(*it,i,Type2Type<T>());
	}
};


template <class STDContainer> void resizeMultiDim (STDContainer& o, unsigned int i) {
	resizeMultiDim(o,i,Type2Type<GetMultiDimType<STDContainer>::Result>());
};

template <template <class,class> class STDContainer, class T, template <class> class Allocator> void resizeMultiDim(STDContainer < T , Allocator <T> >& o, unsigned int i,Type2Type<T>) {
	o.resize(i);
};

template <class T> 
struct GetMultiDimType {
	typedef NullType LastContainer;
	typedef NullType NextContainer;
	typedef T Result;
	enum { dimension = 0 };
};

template <template <class,class> class C, class T,class Allocator> 
struct GetMultiDimType < C <T, Allocator> >
{
	typedef C<T,Allocator> LastContainer;
	typedef NullType NextContainer;
	typedef T Result;
	enum { dimension = 1 };
};

template <template <class,class> class C1, template <class,class> class C2,class T,class A1, class A2> 
struct GetMultiDimType < C1 <C2 <T, A2>, A1> >
{
	typedef C2<T,A2> NextContainer;
	typedef typename GetMultiDimType<NextContainer>::LastContainer LastContainer;
	typedef typename GetMultiDimType<NextContainer>::Result Result;
	enum { dimension = GetMultiDimType<NextContainer>::dimension + 1 };
};

template <template <class,class> class STDContainer, class Functor, class T, class A> 
void doInMultiDim(STDContainer < T, A >& o, Functor f,Type2Type<T>) {
	for (auto it = o.begin() ; it!= o.end() ; ++it) {
		f(*it);
	}
};


template <class STDContainer, class Functor, class T> 
void doInMultiDim(STDContainer& o, Functor f,Type2Type<T>) {
	for (auto it = o.begin() ; it!= o.end() ; ++it) {
		doInMultiDim(*it,f,Type2Type<T>());
	}
};


template <class STDContainer, class Functor> 
void doInMultiDim(STDContainer& o, Functor f) {
	typedef GetMultiDimType<STDContainer>::Result Result;
	doInMultiDim(o,f,Type2Type<Result>());
};

template <class Container>
struct MultiToUniDimClass {
	static void conversion(Container& o, Container& ret) {
		for (auto it = o.begin() ; it != o.end() ; ++it) 
			ret.push_back( *it );
	};
};

template <template <class,class> class C1, template <class,class> class C2,class T,class A1, class A2> 
struct MultiToUniDimClass < C1 <C2 <T, A2>, A1> >
{
	typedef C1< C2 <T,A2>, A1> BigContainer;
	typedef C2 <T,A2> SmallContainer;
	typedef typename GetMultiDimType<BigContainer>::LastContainer LastContainer;

	static void conversion (BigContainer& o, LastContainer& ret) {
		for (auto it = o.begin() ; it != o.end() ; ++it) 
			MultiToUniDimClass<SmallContainer>::conversion(*it,ret);
	};
};

template <class BigContainer>
typename GetMultiDimType<BigContainer>::LastContainer multiToUniDim(BigContainer& o) {

	GetMultiDimType<BigContainer>::LastContainer ret;

	MultiToUniDimClass<BigContainer>::conversion(o,ret);

	return ret;
};

template <class BigContainer,class ResultContainer>
inline void multiToUniDim(BigContainer& o, ResultContainer& ret) {
	MultiToUniDimClass<BigContainer>::conversion(o,ret);
};

template <class Container>
void resizeMultiDimPartial(Container& o,unsigned int dimension,unsigned int value) {

	if (GetMultiDimType<Container>::dimension <= dimension)
		throw std::runtime_error("Wrong dimension for this container");

	ResizeMultiDimPartialClass<Container>::Do(o,dimension,value);
};

template <class Container>
class ResizeMultiDimPartialClass {
public:
	void Do(Container& o,unsigned int dimension,unsigned int value) {
		if (dimension > 0)
			for (auto it = o.begin() ;it!= o.end() ; ++it)
				ResizeMultiDimPartialClass<typename GetMultiDimType<Container>::NextContainer>::Do(*it,dimension-1,value);
		else if (dimension == 0) 
			o.resize(value);
	};
};