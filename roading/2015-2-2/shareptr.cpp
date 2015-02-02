#include <iostream>
#include <vector>
#include <tr1/memory>

class RefCountable
{
public:
	inline void incRef() const
	{
		++refCount_;
	}

	inline void decRef() const
	{
		int currRef = --refCount_;
		if(0 >= currRef)
			onRefOver();
	}

	virtual void onRefOver() const
	{
		delete const_cast<RefCountable*>(this);
	}

	void setRefCount(int n)
	{
		refCount_ = n;
	}

	int getRefCount() const
	{
		return refCount_;
	}

protected:
	RefCountable(): refCount_(0)
	{
	}

	virtual ~RefCountable()
	{
		std::cout << "~RefCountable call\n";
	}

	volatile mutable long refCount_;
};

template <typename T>
inline void incrementReferenceCount(const T& obj)
{
	obj.incRef();
}

template <typename T>
inline void decrementReferenceCount(const T& obj)
{
	obj.decRef();
}

template <class T>
class ConstSmartPointer
{
public:
	enum REF_TAG { STEAL_REF, NEW_REF };

	ConstSmartPointer(const T* obj = 0, REF_TAG tag = ConstSmartPointer::NEW_REF):
	obj_(obj)
	{
		if(obj)
		{
			if(tag == ConstSmartPointer::NEW_REF)
			{
				incrementReferenceCount(*obj_);
			}
		}
	}

	ConstSmartPointer(const ConstSmartPointer<T>& P)
	{
		obj_ = P.get();
		if(obj_)
			incrementReferenceCount(*obj_);
	}

	~ConstSmartPointer()
	{
		if(obj_)
			decrementReferenceCount(*obj_);
		obj_ = 0;
	}

	void clear()
	{
		if(obj_)
			decrementReferenceCount(*obj_);
		obj_ = 0;
	}

	const T* get() const
	{
		return obj_;
	}

	const T* operator()()
	{
		return obj_;
	}

	ConstSmartPointer<T>& operator=(const ConstSmartPointer<T>& X)
	{
		if(obj_ != X.get())
		{
			const T* pOldObj = obj_;
			obj_ = X.get();
			if(obj_) incrementReferenceCount(*obj_);
			if(pOldObj) decrementReferenceCount(*pOldObj);
		}
		return *this;
	}

	const T* operator->() const
	{
		return obj_;
	}

	const T& operator*() const
	{
		return *obj_;
	}

protected:
	const T* obj_;
};

template <class T>
class SmartPointer : public ConstSmartPointer<T>
{
public:
	typedef ConstSmartPointer<T> ConstProxy;

	SmartPointer(T* obj = 0, typename ConstProxy::REF_TAG tag = ConstProxy::NEW_REF):
	ConstProxy(obj, tag)
	{
	}

	SmartPointer(const SmartPointer<T>& P): ConstProxy(P) {}

	template <class DerivedType>
	SmartPointer(ConstSmartPointer<DerivedType>& dt):
	ConstProxy(dt.get())
	{
	}

	SmartPointer<T>& operator=(const SmartPointer<T>& P)
	{
		ConstProxy::operator=(P);
		return *this;
	}

	template <class DerivedType>
	SmartPointer<T>& operator=(ConstSmartPointer<DerivedType>& dt)
	{
		ConstProxy::operator=(dt.get());
		return *this;
	}

	T* get() const
	{
		return const_cast<T*>(this->obj_);
	}

	T* operator->() const
	{
		return const_cast<T*>(this->obj_);
	}

	T& operator*() const
	{
		return *const_cast<T*>(this->obj_);
	}
};

class TmpCount : public RefCountable
{
public:
	TmpCount(int n): _n(n)
	{
		std::cout << "TmpCount() called.\n";
	}

	~TmpCount()
	{
		std::cout << "~TmpCount() called.\n";
	}

	int _n;
};

typedef SmartPointer<TmpCount> TmpCountSmartPtr;

TmpCountSmartPtr createTmpCount()
{
	TmpCountSmartPtr result = TmpCountSmartPtr(new TmpCount(100));
	return result;
}

typedef std::tr1::shared_ptr<TmpCount> TmpCountSharedPtr;

TmpCountSharedPtr createTmpCount2()
{
	TmpCountSharedPtr result = TmpCountSharedPtr(new TmpCount(100));
	return result;
}

class TmpCounts
{
public:
	TmpCounts(int n): _n(n)
	{
		std::cout << "TmpCounts() called.\n";
	}

	~TmpCounts()
	{
		std::cout << "~TmpCounts() called.\n";
	}

	int _n;
};

typedef std::tr1::shared_ptr<TmpCounts> TmpCountsSharedPtr;

TmpCountsSharedPtr createTmpCounts()
{
	TmpCountsSharedPtr result = TmpCountsSharedPtr(new TmpCounts(100));
	return result;
}

int main()
{
	TmpCountSmartPtr get = createTmpCount();
	std::cout << get->_n << "\n";
	get = TmpCountSmartPtr();
	TmpCountSharedPtr get2 = createTmpCount2();
	std::cout << get2->_n << "\n";
	get2 = TmpCountSharedPtr();
	TmpCountsSharedPtr get3 = createTmpCounts();
	std::cout << get3->_n << "\n";
	get3 = TmpCountsSharedPtr();
	std::cout << "ended.\n";
	return 0;
}