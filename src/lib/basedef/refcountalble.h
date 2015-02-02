#ifndef BASEDEF_REFCOUNTABLE_H
#define BASEDEF_REFCOUNTABLE_H

#include "commondef.h"

namespace basedef {

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
		assert(currRef >= 0 && "RefCountable: currRef maybe a error!");
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
		assert(0 == refCount_ && "RefCountable: currRef maybe a error!");
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

	friend bool operator==(const ConstSmartPointer<T>& A,
		const ConstSmartPointer<T>& B)
	{
		return A.obj_ == B.obj_;
	}

	friend bool operator==(const ConstSmartPointer<T>& A,
		const T* B)
	{
		return A.obj_ == B;
	}

	friend bool operator==(const T* A,
		const ConstSmartPointer<T>& B)
	{
		return A == B.obj_;
	}

	friend bool operator!=(const ConstSmartPointer<T>& A,
		const ConstSmartPointer<T>& B)
	{
		return A.obj_ != B.obj_;
	}

	friend bool operator!=(const ConstSmartPointer<T>& A,
		const T* B)
	{
		return A.obj_ != B;
	}

	friend bool operator!=(const T* A,
		const ConstSmartPointer<T>& B)
	{
		return A != B.obj_;
	}

	friend bool operator<(const ConstSmartPointer<T>& A,
		const ConstSmartPointer<T>& B)
	{
		return A.obj_ < B.obj_;
	}

	friend bool operator<(const ConstSmartPointer<T>& A,
		const T* B)
	{
		return A.obj_ < B;
	}

	friend bool operator<(const T* A,
		const ConstSmartPointer<T>& B)
	{
		return A < B.obj_;
	}

	friend bool operator>(const ConstSmartPointer<T>& A,
		const ConstSmartPointer<T>& B)
	{
		return A.obj_ > B.obj_;
	}

	friend bool operator>(const ConstSmartPointer<T>& A,
		const T* B)
	{
		return A.obj_ > B;
	}

	friend bool operator>(const T* A,
		const ConstSmartPointer<T>& B)
	{
		return A > B.obj_;
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

}

#endif