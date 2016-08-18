// memory standard header
#pragma once
#ifndef _MEMORY_
#define _MEMORY_
#ifndef RC_INVOKED
#include <xmemory>

 #ifndef _XSTD2
  #define _XSTD2
 #endif /* _XSTD2 */

 #include <exception>
 #include <typeinfo>
 #include <type_traits>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new

 #pragma warning(disable: 28197)

_STD_BEGIN
template<class _Ty>
	struct default_delete;

template<class _Ty,
	class _Dx = default_delete<_Ty> >
	class unique_ptr;

	// CLASS bad_weak_ptr
class bad_weak_ptr
	: public _XSTD exception
	{	// exception type for invalid use of expired weak_ptr object
public:
	bad_weak_ptr() _NOEXCEPT
		{	// default construct
		}

	virtual const char *__CLR_OR_THIS_CALL what() const throw()
		{	// return pointer to message string
		return ("bad_weak_ptr");
		}
	};

	// CLASS _Ref_count_base
class _Ref_count_base
	{	// common code for reference counting
private:
	virtual void _Destroy() _NOEXCEPT = 0;
	virtual void _Delete_this() _NOEXCEPT = 0;

private:
	_Atomic_counter_t _Uses;
	_Atomic_counter_t _Weaks;

protected:
	_Ref_count_base()
		{	// construct
		_Init_atomic_counter(_Uses, 1);
		_Init_atomic_counter(_Weaks, 1);
		}

public:
	virtual ~_Ref_count_base() _NOEXCEPT
		{	// ensure that derived classes can be destroyed properly
		}

	bool _Incref_nz()
		{	// increment use count if not zero, return true if successful
		for (; ; )
			{	// loop until state is known
 #if _USE_INTERLOCKED_REFCOUNTING
			_Atomic_integral_t _Count =
				static_cast<volatile _Atomic_counter_t&>(_Uses);

			if (_Count == 0)
				return (false);

			if (static_cast<_Atomic_integral_t>(_InterlockedCompareExchange(
					reinterpret_cast<volatile long *>(&_Uses),
					_Count + 1, _Count)) == _Count)
				return (true);

 #else /* _USE_INTERLOCKED_REFCOUNTING */
			_Atomic_integral_t _Count =
				_Load_atomic_counter(_Uses);

			if (_Count == 0)
				return (false);

			if (_Compare_increment_atomic_counter(_Uses, _Count))
				return (true);
 #endif /* _USE_INTERLOCKED_REFCOUNTING */
			}
		}

	void _Incref()
		{	// increment use count
		_MT_INCR(_Uses);
		}

	void _Incwref()
		{	// increment weak reference count
		_MT_INCR(_Weaks);
		}

	void _Decref()
		{	// decrement use count
		if (_MT_DECR(_Uses) == 0)
			{	// destroy managed resource, decrement weak reference count
			_Destroy();
			_Decwref();
			}
		}

	void _Decwref()
		{	// decrement weak reference count
		if (_MT_DECR(_Weaks) == 0)
			_Delete_this();
		}

	long _Use_count() const _NOEXCEPT
		{	// return use count
		return (_Get_atomic_count(_Uses));
		}

	bool _Expired() const _NOEXCEPT
		{	// return true if _Uses == 0
		return (_Use_count() == 0);
		}

	virtual void *_Get_deleter(const _XSTD2 type_info&) const _NOEXCEPT
		{	// return address of deleter object
		return (0);
		}
	};

	// TEMPLATE CLASS _Ref_count
template<class _Ty>
	class _Ref_count
	: public _Ref_count_base
	{	// handle reference counting for object without deleter
public:
	_Ref_count(_Ty *_Px)
		: _Ref_count_base(), _Ptr(_Px)
		{	// construct
		}

private:
	virtual void _Destroy() _NOEXCEPT
		{	// destroy managed resource
		delete _Ptr;
		}

	virtual void _Delete_this() _NOEXCEPT
		{	// destroy self
		delete this;
		}

	_Ty * _Ptr;
	};

	// TEMPLATE CLASS _Ref_count_del
template<class _Ty,
	class _Dx>
	class _Ref_count_del
	: public _Ref_count_base
	{	// handle reference counting for object with deleter
public:
	_Ref_count_del(_Ty *_Px, _Dx _Dt)
		: _Ref_count_base(), _Mypair(_One_then_variadic_args_t(), _Dt, _Px)
		{	// construct
		}

	virtual void *_Get_deleter(
		const _XSTD2 type_info& _Typeid) const _NOEXCEPT
		{	// return address of deleter object
		return ((void *)(_Typeid == typeid(_Dx)
			? _STD addressof(_Mypair._Get_first()) : 0));
		}

private:
	virtual void _Destroy() _NOEXCEPT
		{	// destroy managed resource
		_Mypair._Get_first()(_Mypair._Get_second());
		}

	virtual void _Delete_this() _NOEXCEPT
		{	// destroy self
		delete this;
		}

	_Compressed_pair<_Dx, _Ty *> _Mypair;
	};

	// TEMPLATE CLASS _Ref_count_del_alloc
template<class _Ty,
	class _Dx,
	class _Alloc>
	class _Ref_count_del_alloc
	: public _Ref_count_base
	{	// handle reference counting for object with deleter and allocator
public:
	typedef _Ref_count_del_alloc<_Ty, _Dx, _Alloc> _Myt;
	typedef _Wrap_alloc<_Alloc> _Myalty0;
	typedef typename _Myalty0::template rebind<_Myt>::other _Myalty;

	_Ref_count_del_alloc(_Ty *_Px, _Dx _Dt, const _Alloc& _Ax)
		: _Ref_count_base(), _Mypair(_One_then_variadic_args_t(), _Dt,
			_One_then_variadic_args_t(), _Ax, _Px)
		{	// construct
		}

	virtual void *_Get_deleter(
		const _XSTD2 type_info& _Typeid) const _NOEXCEPT
		{	// return address of deleter object
		return ((void *)(_Typeid == typeid(_Dx)
			? _STD addressof(_Mypair._Get_first()) : 0));
		}

private:
	virtual void _Destroy() _NOEXCEPT
		{	// destroy managed resource
		_Mypair._Get_first()(_Mypair._Get_second()._Get_second());
		}

	virtual void _Delete_this() _NOEXCEPT
		{	// destroy self
		_Myalty _Al = _Mypair._Get_second()._Get_first();
		_Al.destroy(this);
		_Al.deallocate(this, 1);
		}

	_Compressed_pair<_Dx, _Compressed_pair<_Myalty, _Ty *> > _Mypair;
	};

	// DECLARATIONS
template<class _Ty>
	class weak_ptr;
template<class _Ty>
	class shared_ptr;
template<class _Ty>
	class enable_shared_from_this;

template<class _Ty1,
	class _Ty2>
	void _Do_enable(_Ty1 *, enable_shared_from_this<_Ty2> *,
		_Ref_count_base *);

template<class _Ty>
	inline void _Enable_shared(_Ty *_Ptr, _Ref_count_base *_Refptr,
		typename _Ty::_EStype * = 0)
	{	// reset internal weak pointer
	if (_Ptr)
		_Do_enable(_Ptr,
			(enable_shared_from_this<typename _Ty::_EStype>*)_Ptr, _Refptr);
	}

inline void _Enable_shared(const volatile void *, const volatile void *)
	{	// not derived from enable_shared_from_this; do nothing
	}

	// TEMPLATE CLASS _Ptr_base
template<class _Ty>
	class _Ptr_base
	{	// base class for shared_ptr and weak_ptr
public:
	typedef _Ptr_base<_Ty> _Myt;
	typedef _Ty element_type;

	_CONST_FUN _Ptr_base() _NOEXCEPT
		: _Ptr(0), _Rep(0)
		{	// construct
		}

	_Ptr_base(_Myt&& _Right)
		: _Ptr(_Right._Ptr), _Rep(_Right._Rep)
		{	// construct _Ptr_base object that takes resource from _Right
		_Right._Ptr = 0;
		_Right._Rep = 0;
		}

	template<class _Ty2>
		_Ptr_base(_Ptr_base<_Ty2>&& _Right)
		: _Ptr(_Right._Ptr), _Rep(_Right._Rep)
		{	// construct _Ptr_base object that takes resource from _Right
		_Right._Ptr = 0;
		_Right._Rep = 0;
		}

	_Myt& operator=(_Myt&& _Right)
		{	// construct _Ptr_base object that takes resource from _Right
		_Assign_rv(_STD move(_Right));
		return (*this);
		}

	void _Assign_rv(_Myt&& _Right)
		{	// assign by moving _Right
		_Swap(_Right);
		}

	long use_count() const _NOEXCEPT
		{	// return use count
		return (_Rep ? _Rep->_Use_count() : 0);
		}

	void _Swap(_Ptr_base& _Right) _NOEXCEPT
		{	// swap pointers
		_STD swap(_Rep, _Right._Rep);
		_STD swap(_Ptr, _Right._Ptr);
		}

	template<class _Ty2>
		bool owner_before(const _Ptr_base<_Ty2>& _Right) const
		{	// compare addresses of manager objects
		return (_Rep < _Right._Rep);
		}

	void *_Get_deleter(const _XSTD2 type_info& _Typeid) const _NOEXCEPT
		{	// return pointer to deleter object if its type is _Typeid
		return (_Rep ? _Rep->_Get_deleter(_Typeid) : 0);
		}

	_Ty *_Get() const _NOEXCEPT
		{	// return pointer to resource
		return (_Ptr);
		}

	bool _Expired() const _NOEXCEPT
		{	// test if expired
		return (!_Rep || _Rep->_Expired());
		}

	void _Decref()
		{	// decrement reference count
		if (_Rep != 0)
			_Rep->_Decref();
		}

	void _Reset()
		{	// release resource
		_Reset(0, 0);
		}

	template<class _Ty2>
		void _Reset(const _Ptr_base<_Ty2>& _Other)
		{	// release resource and take ownership of _Other._Ptr
		_Reset(_Other._Ptr, _Other._Rep);
		}

	template<class _Ty2>
		void _Reset(const _Ptr_base<_Ty2>& _Other, bool _Throw)
		{	// release resource and take ownership from weak_ptr _Other._Ptr
		_Reset(_Other._Ptr, _Other._Rep, _Throw);
		}

 #if _HAS_AUTO_PTR_ETC
	template<class _Ty2>
		void _Reset(auto_ptr<_Ty2>&& _Other)
		{	// release resource and take _Other.get()
		_Ty2 *_Px = _Other.get();
		_Reset0(_Px, new _Ref_count<_Ty>(_Px));
		_Other.release();
		_Enable_shared(_Px, _Rep);
		}
 #endif /* _HAS_AUTO_PTR_ETC */

	template<class _Ty2>
		void _Reset(_Ty *_Other_ptr, const _Ptr_base<_Ty2>& _Other)
		{	// release resource and alias _Other_ptr with _Other
		_Reset(_Other_ptr, _Other._Rep);
		}

	void _Reset(_Ty *_Other_ptr, _Ref_count_base *_Other_rep)
		{	// release resource and take _Other_ptr through _Other_rep
		if (_Other_rep)
			_Other_rep->_Incref();
		_Reset0(_Other_ptr, _Other_rep);
		}

	void _Reset(_Ty *_Other_ptr, _Ref_count_base *_Other_rep, bool _Throw)
		{	// take _Other_ptr through _Other_rep from weak_ptr if not expired
			// otherwise, leave in default state if !_Throw,
			// otherwise throw exception
		if (_Other_rep && _Other_rep->_Incref_nz())
			_Reset0(_Other_ptr, _Other_rep);
		else if (_Throw)
			_THROW_NCEE(bad_weak_ptr, _EMPTY_ARGUMENT);
		}

	void _Reset0(_Ty *_Other_ptr, _Ref_count_base *_Other_rep)
		{	// release resource and take new resource
		if (_Rep != 0)
			_Rep->_Decref();
		_Rep = _Other_rep;
		_Ptr = _Other_ptr;
		}

	void _Decwref()
		{	// decrement weak reference count
		if (_Rep != 0)
			_Rep->_Decwref();
		}

	void _Resetw()
		{	// release weak reference to resource
		_Resetw((_Ty *)0, 0);
		}

	template<class _Ty2>
		void _Resetw(const _Ptr_base<_Ty2>& _Other)
		{	// release weak reference to resource and take _Other._Ptr
		_Resetw(_Other._Ptr, _Other._Rep);
		}

	template<class _Ty2>
		void _Resetw(_Ty2 *_Other_ptr, _Ref_count_base *_Other_rep)
		{	// point to _Other_ptr through _Other_rep
		if (_Other_rep)
			_Other_rep->_Incwref();
		if (_Rep != 0)
			_Rep->_Decwref();
		_Rep = _Other_rep;
		_Ptr = const_cast<remove_cv_t<_Ty2> *>(_Other_ptr);
		}

private:
	_Ty *_Ptr;
	_Ref_count_base *_Rep;
	template<class _Ty0>
		friend class _Ptr_base;
	};

	// TEMPLATE CLASS shared_ptr
template<class _Ty>
	class shared_ptr
		: public _Ptr_base<_Ty>
	{	// class for reference counted resource management
public:
	typedef shared_ptr<_Ty> _Myt;
	typedef _Ptr_base<_Ty> _Mybase;

	_CONST_FUN shared_ptr() _NOEXCEPT
		{	// construct empty shared_ptr
		}

	template<class _Ux>
		explicit shared_ptr(_Ux *_Px)
		{	// construct shared_ptr object that owns _Px
		_Resetp(_Px);
		}

	template<class _Ux,
		class _Dx>
		shared_ptr(_Ux *_Px, _Dx _Dt)
		{	// construct with _Px, deleter
		_Resetp(_Px, _Dt);
		}

	_CONST_FUN shared_ptr(nullptr_t) _NOEXCEPT
		{	// construct empty shared_ptr
		}

	template<class _Dx>
		shared_ptr(nullptr_t, _Dx _Dt)
		{	// construct with nullptr, deleter
		_Resetp((_Ty *)0, _Dt);
		}

	template<class _Dx,
		class _Alloc>
		shared_ptr(nullptr_t, _Dx _Dt, _Alloc _Ax)
		{	// construct with nullptr, deleter, allocator
		_Resetp((_Ty *)0, _Dt, _Ax);
		}

	template<class _Ux,
		class _Dx,
		class _Alloc>
		shared_ptr(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
		{	// construct with _Px, deleter, allocator
		_Resetp(_Px, _Dt, _Ax);
		}

	template<class _Ty2>
		shared_ptr(const shared_ptr<_Ty2>& _Right, _Ty *_Px) _NOEXCEPT
		{	// construct shared_ptr object that aliases _Right
		this->_Reset(_Px, _Right);
		}

	shared_ptr(const _Myt& _Other) _NOEXCEPT
		{	// construct shared_ptr object that owns same resource as _Other
		this->_Reset(_Other);
		}

	template<class _Ty2,
		class = typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
			void>::type>
		shared_ptr(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
		{	// construct shared_ptr object that owns same resource as _Other
		this->_Reset(_Other);
		}

	template<class _Ty2>
		explicit shared_ptr(const weak_ptr<_Ty2>& _Other,
			bool _Throw = true)
		{	// construct shared_ptr object that owns resource *_Other
		this->_Reset(_Other, _Throw);
		}

 #if _HAS_AUTO_PTR_ETC
	template<class _Ty2>
		shared_ptr(auto_ptr<_Ty2>&& _Other)
		{	// construct shared_ptr object that owns *_Other.get()
		this->_Reset(_STD move(_Other));
		}
 #endif /* _HAS_AUTO_PTR_ETC */

	shared_ptr(_Myt&& _Right) _NOEXCEPT
		: _Mybase(_STD move(_Right))
		{	// construct shared_ptr object that takes resource from _Right
		}

	template<class _Ty2,
		class = typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
			void>::type>
		shared_ptr(shared_ptr<_Ty2>&& _Right) _NOEXCEPT
		: _Mybase(_STD move(_Right))
		{	// construct shared_ptr object that takes resource from _Right
		}


	template<class _Ux,
		class _Dx,
		class = typename enable_if<is_convertible<
			typename unique_ptr<_Ux, _Dx>::pointer, _Ty *>::value,
			void>::type>
		shared_ptr(unique_ptr<_Ux, _Dx>&& _Right)
		{	// construct from unique_ptr
		_Resetp(_Right.release(), _Right.get_deleter());
		}

	template<class _Ux,
		class _Dx>
		_Myt& operator=(unique_ptr<_Ux, _Dx>&& _Right)
		{	// move from unique_ptr
		shared_ptr(_STD move(_Right)).swap(*this);
		return (*this);
		}

	_Myt& operator=(_Myt&& _Right) _NOEXCEPT
		{	// take resource from _Right
		shared_ptr(_STD move(_Right)).swap(*this);
		return (*this);
		}

	template<class _Ty2>
		_Myt& operator=(shared_ptr<_Ty2>&& _Right) _NOEXCEPT
		{	// take resource from _Right
		shared_ptr(_STD move(_Right)).swap(*this);
		return (*this);
		}

	~shared_ptr() _NOEXCEPT
		{	// release resource
		this->_Decref();
		}

	_Myt& operator=(const _Myt& _Right) _NOEXCEPT
		{	// assign shared ownership of resource owned by _Right
		shared_ptr(_Right).swap(*this);
		return (*this);
		}

	template<class _Ty2>
		_Myt& operator=(const shared_ptr<_Ty2>& _Right) _NOEXCEPT
		{	// assign shared ownership of resource owned by _Right
		shared_ptr(_Right).swap(*this);
		return (*this);
		}

 #if _HAS_AUTO_PTR_ETC
	template<class _Ty2>
		_Myt& operator=(auto_ptr<_Ty2>&& _Right)
		{	// assign ownership of resource pointed to by _Right
		shared_ptr(_STD move(_Right)).swap(*this);
		return (*this);
		}
 #endif /* _HAS_AUTO_PTR_ETC */

	void reset() _NOEXCEPT
		{	// release resource and convert to empty shared_ptr object
		shared_ptr().swap(*this);
		}

	template<class _Ux>
		void reset(_Ux *_Px)
		{	// release, take ownership of _Px
		shared_ptr(_Px).swap(*this);
		}

	template<class _Ux,
		class _Dx>
		void reset(_Ux *_Px, _Dx _Dt)
		{	// release, take ownership of _Px, with deleter _Dt
		shared_ptr(_Px, _Dt).swap(*this);
		}

	template<class _Ux,
		class _Dx,
		class _Alloc>
		void reset(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
		{	// release, take ownership of _Px, with deleter _Dt, allocator _Ax
		shared_ptr(_Px, _Dt, _Ax).swap(*this);
		}

	void swap(_Myt& _Other) _NOEXCEPT
		{	// swap pointers
		this->_Swap(_Other);
		}

	_Ty *get() const _NOEXCEPT
		{	// return pointer to resource
		return (this->_Get());
		}

	typename add_lvalue_reference<_Ty>::type operator*() const _NOEXCEPT
		{	// return reference to resource
		return (*this->_Get());
		}

	_Ty *operator->() const _NOEXCEPT
		{	// return pointer to resource
		return (this->_Get());
		}

	bool unique() const _NOEXCEPT
		{	// return true if no other shared_ptr object owns this resource
		return (this->use_count() == 1);
		}

	explicit operator bool() const _NOEXCEPT
		{	// test if shared_ptr object owns no resource
		return (this->_Get() != 0);
		}

private:
	template<class _Ux>
		void _Resetp(_Ux *_Px)
		{	// release, take ownership of _Px
		_TRY_BEGIN	// allocate control block and reset
		_Resetp0(_Px, new _Ref_count<_Ux>(_Px));
		_CATCH_ALL	// allocation failed, delete resource
		delete _Px;
		_RERAISE;
		_CATCH_END
		}

	template<class _Ux,
		class _Dx>
		void _Resetp(_Ux *_Px, _Dx _Dt)
		{	// release, take ownership of _Px, deleter _Dt
		_TRY_BEGIN	// allocate control block and reset
		_Resetp0(_Px, new _Ref_count_del<_Ux, _Dx>(_Px, _Dt));
		_CATCH_ALL	// allocation failed, delete resource
		_Dt(_Px);
		_RERAISE;
		_CATCH_END
		}

	template<class _Ux,
		class _Dx,
		class _Alloc>
		void _Resetp(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
		{	// release, take ownership of _Px, deleter _Dt, allocator _Ax
		typedef _Ref_count_del_alloc<_Ux, _Dx, _Alloc> _Refd;
		typedef _Wrap_alloc<_Alloc> _Alref0;
		typename _Alref0::template rebind<_Refd>::other _Alref(_Ax);

		_TRY_BEGIN	// allocate control block and reset
		_Refd *_Pref = _Alref.allocate(1);
		_Alref.construct(_Pref, _Px, _Dt, _Ax);
		_Resetp0(_Px, _Pref);
		_CATCH_ALL	// allocation failed, delete resource
		_Dt(_Px);
		_RERAISE;
		_CATCH_END
		}

public:
	template<class _Ux>
		void _Resetp0(_Ux *_Px, _Ref_count_base *_Rx)
		{	// release resource and take ownership of _Px
		this->_Reset0(_Px, _Rx);
		_Enable_shared(_Px, _Rx);
		}
	};

template<class _Ty1,
	class _Ty2>
	bool operator==(const shared_ptr<_Ty1>& _Left,
		const shared_ptr<_Ty2>& _Right) _NOEXCEPT
	{	// test if shared_ptr == shared_ptr
	return (_Left.get() == _Right.get());
	}

template<class _Ty1,
	class _Ty2>
	bool operator!=(const shared_ptr<_Ty1>& _Left,
		const shared_ptr<_Ty2>& _Right) _NOEXCEPT
	{	// test if shared_ptr != shared_ptr
	return (!(_Left == _Right));
	}

template<class _Ty1,
	class _Ty2>
	bool operator<(const shared_ptr<_Ty1>& _Left,
		const shared_ptr<_Ty2>& _Right) _NOEXCEPT
	{	// test if shared_ptr < shared_ptr
	return (less<decltype(_Always_false<_Ty1>::value
		? _Left.get() : _Right.get())>()(
			_Left.get(), _Right.get()));
	}

template<class _Ty1,
	class _Ty2>
	bool operator>=(const shared_ptr<_Ty1>& _Left,
		const shared_ptr<_Ty2>& _Right) _NOEXCEPT
	{	// shared_ptr >= shared_ptr
	return (!(_Left < _Right));
	}

template<class _Ty1,
	class _Ty2>
	bool operator>(const shared_ptr<_Ty1>& _Left,
		const shared_ptr<_Ty2>& _Right) _NOEXCEPT
	{	// test if shared_ptr > shared_ptr
	return (_Right < _Left);
	}

template<class _Ty1,
	class _Ty2>
	bool operator<=(const shared_ptr<_Ty1>& _Left,
		const shared_ptr<_Ty2>& _Right) _NOEXCEPT
	{	// test if shared_ptr <= shared_ptr
	return (!(_Right < _Left));
	}

template<class _Ty>
	bool operator==(const shared_ptr<_Ty>& _Left,
		nullptr_t) _NOEXCEPT
	{	// test if shared_ptr == nullptr
	return (_Left.get() == (_Ty *)0);
	}

template<class _Ty>
	bool operator==(nullptr_t,
		const shared_ptr<_Ty>& _Right) _NOEXCEPT
	{	// test if nullptr == shared_ptr
	return ((_Ty *)0 == _Right.get());
	}

template<class _Ty>
	bool operator!=(const shared_ptr<_Ty>& _Left,
		nullptr_t _Right) _NOEXCEPT
	{	// test if shared_ptr != nullptr
	return (!(_Left == _Right));
	}

template<class _Ty>
	bool operator!=(nullptr_t _Left,
		const shared_ptr<_Ty>& _Right) _NOEXCEPT
	{	// test if nullptr != shared_ptr
	return (!(_Left == _Right));
	}

template<class _Ty>
	bool operator<(const shared_ptr<_Ty>& _Left,
		nullptr_t _Right) _NOEXCEPT
	{	// test if shared_ptr < nullptr
	return (less<_Ty *>()(_Left.get(), (_Ty *)0));
	}

template<class _Ty>
	bool operator<(nullptr_t _Left,
		const shared_ptr<_Ty>& _Right) _NOEXCEPT
	{	// test if nullptr < shared_ptr
	return (less<_Ty *>()((_Ty *)0, _Right.get()));
	}

template<class _Ty>
	bool operator>=(const shared_ptr<_Ty>& _Left,
		nullptr_t _Right) _NOEXCEPT
	{	// test if shared_ptr >= nullptr
	return (!(_Left < _Right));
	}

template<class _Ty>
	bool operator>=(nullptr_t _Left,
		const shared_ptr<_Ty>& _Right) _NOEXCEPT
	{	// test if nullptr >= shared_ptr
	return (!(_Left < _Right));
	}

template<class _Ty>
	bool operator>(const shared_ptr<_Ty>& _Left,
		nullptr_t _Right) _NOEXCEPT
	{	// test if shared_ptr > nullptr
	return (_Right < _Left);
	}

template<class _Ty>
	bool operator>(nullptr_t _Left,
		const shared_ptr<_Ty>& _Right) _NOEXCEPT
	{	// test if nullptr > shared_ptr
	return (_Right < _Left);
	}

template<class _Ty>
	bool operator<=(const shared_ptr<_Ty>& _Left,
		nullptr_t _Right) _NOEXCEPT
	{	// test if shared_ptr <= nullptr
	return (!(_Right < _Left));
	}

template<class _Ty>
	bool operator<=(nullptr_t _Left,
		const shared_ptr<_Ty>& _Right) _NOEXCEPT
	{	// test if nullptr <= shared_ptr
	return (!(_Right < _Left));
	}

template<class _Elem,
	class _Traits,
	class _Ty>
	basic_ostream<_Elem, _Traits>&
	operator<<(basic_ostream<_Elem, _Traits>& _Out,
		const shared_ptr<_Ty>& _Px)
	{	// write contained pointer to stream
	return (_Out << _Px.get());
	}

template<class _Ty>
	void swap(shared_ptr<_Ty>& _Left,
		shared_ptr<_Ty>& _Right) _NOEXCEPT
	{	// swap _Left and _Right shared_ptrs
	_Left.swap(_Right);
	}

template<class _Ty1,
	class _Ty2>
	shared_ptr<_Ty1>
		static_pointer_cast(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
	{	// return shared_ptr object holding static_cast<_Ty1 *>(_Other.get())
	typedef typename shared_ptr<_Ty1>::element_type _Elem1;
	_Elem1 *_Ptr = static_cast<_Elem1 *>(_Other.get());
	return (shared_ptr<_Ty1>(_Other, _Ptr));
	}

template<class _Ty1,
	class _Ty2>
	shared_ptr<_Ty1>
		const_pointer_cast(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
	{	// return shared_ptr object holding const_cast<_Ty1 *>(_Other.get())
	typedef typename shared_ptr<_Ty1>::element_type _Elem1;
	_Elem1 *_Ptr = const_cast<_Elem1 *>(_Other.get());
	return (shared_ptr<_Ty1>(_Other, _Ptr));
	}

template<class _Ty1,
	class _Ty2>
	shared_ptr<_Ty1>
		dynamic_pointer_cast(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
	{	// return shared_ptr object holding dynamic_cast<_Ty1 *>(_Other.get())
	typedef typename shared_ptr<_Ty1>::element_type _Elem1;
	_Elem1 *_Ptr = dynamic_cast<_Elem1 *>(_Other.get());
	if (_Ptr)
		return (shared_ptr<_Ty1>(_Other, _Ptr));
	else
		return (shared_ptr<_Ty1>());
	}

template<class _Dx,
	class _Ty>
	_Dx *get_deleter(const shared_ptr<_Ty>& _Sx) _NOEXCEPT
	{	// return pointer to shared_ptr's deleter object if its type is _Ty
	return ((_Dx *)_Sx._Get_deleter(typeid(_Dx)));
	}

	// TEMPLATE CLASS _Ref_count_obj
template<class _Ty>
	class _Ref_count_obj
	: public _Ref_count_base
	{	// handle reference counting for object in control block, no allocator
public:
	template<class... _Types>
		_Ref_count_obj(_Types&&... _Args)
		: _Ref_count_base()
		{	// construct from argument list
		::new ((void *)&_Storage) _Ty(_STD forward<_Types>(_Args)...);
		}


	_Ty *_Getptr() const
		{	// get pointer
		return ((_Ty *)&_Storage);
		}

private:
	virtual void _Destroy() _NOEXCEPT
		{	// destroy managed resource
		_Getptr()->~_Ty();
		}

	virtual void _Delete_this() _NOEXCEPT
		{	// destroy self
		delete this;
		}

	typename aligned_union<1, _Ty>::type _Storage;
	};

	// TEMPLATE CLASS _Ref_count_obj_alloc
template<class _Ty,
	class _Alloc>
	class _Ref_count_obj_alloc
	: public _Ref_count_base
	{	// handle reference counting for object in control block, allocator
public:
	typedef _Ref_count_obj_alloc<_Ty, _Alloc> _Myt;
	typedef _Wrap_alloc<_Alloc> _Myalty0;
	typedef typename _Myalty0::template rebind<_Myt>::other _Myalty;
	typedef typename aligned_union<1, _Ty>::type _Mystoragety;

	template<class... _Types>
		_Ref_count_obj_alloc(const _Alloc& _Al_arg, _Types&&... _Args)
		: _Ref_count_base(), _Mypair(_One_then_variadic_args_t(), _Al_arg)
		{	// construct from argument list, allocator
		::new ((void *)_Getptr()) _Ty(_STD forward<_Types>(_Args)...);
		}


	_Ty *_Getptr() const
		{	// get pointer
		return ((_Ty *)&_Mypair._Get_second());
		}

private:
	virtual void _Destroy() _NOEXCEPT
		{	// destroy managed resource
		_Getptr()->~_Ty();
		}

	virtual void _Delete_this() _NOEXCEPT
		{	// destroy self
		_Myalty _Al = _Mypair._Get_first();
		_Al.destroy(this);
		_Al.deallocate(this, 1);
		}

	_Compressed_pair<_Myalty, _Mystoragety> _Mypair;
	};

	// TEMPLATE FUNCTION make_shared
template<class _Ty,
	class... _Types> inline
		shared_ptr<_Ty> make_shared(_Types&&... _Args)
	{	// make a shared_ptr
	_Ref_count_obj<_Ty> *_Rx =
		new _Ref_count_obj<_Ty>(_STD forward<_Types>(_Args)...);

	shared_ptr<_Ty> _Ret;
	_Ret._Resetp0(_Rx->_Getptr(), _Rx);
	return (_Ret);
	}

	// TEMPLATE FUNCTION allocate_shared
template<class _Ty,
	class _Alloc,
	class... _Types> inline
	shared_ptr<_Ty> allocate_shared(
		const _Alloc& _Al_arg, _Types&&... _Args)
	{	// make a shared_ptr
	typedef _Ref_count_obj_alloc<_Ty, _Alloc> _Refoa;
	typedef _Wrap_alloc<_Alloc> _Alref0;
	typename _Alref0::template rebind<_Refoa>::other _Alref(_Al_arg);

	_Refoa *_Rx = _Alref.allocate(1);

	_TRY_BEGIN
		_Alref.construct(_Rx, _Al_arg, _STD forward<_Types>(_Args)...);
	_CATCH_ALL
		_Alref.deallocate(_Rx, 1);
	_RERAISE;
	_CATCH_END

	shared_ptr<_Ty> _Ret;
	_Ret._Resetp0(_Rx->_Getptr(), _Rx);
	return (_Ret);
	}


	// TEMPLATE CLASS weak_ptr
template<class _Ty>
	class weak_ptr
		: public _Ptr_base<_Ty>
	{	// class for pointer to reference counted resource
public:
	typedef weak_ptr<_Ty> _Myt;
	typedef _Ptr_base<_Ty> _Mybase;

	_CONST_FUN weak_ptr() _NOEXCEPT
		{	// construct empty weak_ptr object
		}

	weak_ptr(const weak_ptr& _Other) _NOEXCEPT
		{	// construct weak_ptr object for resource pointed to by _Other
		this->_Resetw(_Other);
		}

	template<class _Ty2,
		class = typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
			void>::type>
		weak_ptr(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
		{	// construct weak_ptr object for resource owned by _Other
		this->_Resetw(_Other);
		}

	template<class _Ty2,
		class = typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
			void>::type>
		weak_ptr(const weak_ptr<_Ty2>& _Other) _NOEXCEPT
		{	// construct weak_ptr object for resource pointed to by _Other
		this->_Resetw(_Other.lock());
		}

	weak_ptr(_Myt&& _Other) _NOEXCEPT
		: _Mybase(_STD move(_Other))
		{	// move construct from _Other
		}

	template<class _Ty2,
		class = typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
			void>::type>
		weak_ptr(weak_ptr<_Ty2>&& _Other) _NOEXCEPT
		{	// move construct from _Other
		this->_Resetw(_Other.lock());
		_Other.reset();
		}

	~weak_ptr() _NOEXCEPT
		{	// release resource
		this->_Decwref();
		}

	weak_ptr& operator=(const weak_ptr& _Right) _NOEXCEPT
		{	// assign from _Right
		this->_Resetw(_Right);
		return (*this);
		}

	template<class _Ty2>
		weak_ptr& operator=(const weak_ptr<_Ty2>& _Right) _NOEXCEPT
		{	// assign from _Right
		this->_Resetw(_Right.lock());
		return (*this);
		}

	_Myt& operator=(_Myt&& _Right) _NOEXCEPT
		{	// move assign from _Right
		weak_ptr(_STD move(_Right)).swap(*this);
		return (*this);
		}

	template<class _Ty2>
		_Myt& operator=(weak_ptr<_Ty2>&& _Right) _NOEXCEPT
		{	// move assign from _Right
		weak_ptr(_STD move(_Right)).swap(*this);
		return (*this);
		}

	template<class _Ty2>
		weak_ptr& operator=(const shared_ptr<_Ty2>& _Right) _NOEXCEPT
		{	// assign from _Right
		this->_Resetw(_Right);
		return (*this);
		}

	void reset() _NOEXCEPT
		{	// release resource, convert to null weak_ptr object
		this->_Resetw();
		}

	void swap(weak_ptr& _Other) _NOEXCEPT
		{	// swap pointers
		this->_Swap(_Other);
		}

	bool expired() const _NOEXCEPT
		{	// return true if resource no longer exists
		return (this->_Expired());
		}

	shared_ptr<_Ty> lock() const _NOEXCEPT
		{	// convert to shared_ptr
		return (shared_ptr<_Ty>(*this, false));
		}
	};

template<class _Ty>
	void swap(weak_ptr<_Ty>& _W1, weak_ptr<_Ty>& _W2) _NOEXCEPT
	{	// swap contents of _W1 and _W2
	_W1.swap(_W2);
	}

	// TEMPLATE CLASS enable_shared_from_this
template<class _Ty>
	class enable_shared_from_this
	{	// provide member functions that create shared_ptr to this
public:
	typedef _Ty _EStype;

	shared_ptr<_Ty> shared_from_this()
		{	// return shared_ptr
		return (shared_ptr<_Ty>(_Wptr));
		}

	shared_ptr<const _Ty> shared_from_this() const
		{	// return shared_ptr
		return (shared_ptr<const _Ty>(_Wptr));
		}

protected:
	_CONST_FUN enable_shared_from_this() _NOEXCEPT
		{	// construct (do nothing)
		}

	enable_shared_from_this(const enable_shared_from_this&) _NOEXCEPT
		{	// construct (do nothing)
		}

	enable_shared_from_this&
		operator=(const enable_shared_from_this&) _NOEXCEPT
		{	// assign (do nothing)
		return (*this);
		}

	~enable_shared_from_this() _NOEXCEPT
		{	// destroy (do nothing)
		}

private:
	template<class _Ty1,
		class _Ty2>
		friend void _Do_enable(
			_Ty1 *,
			enable_shared_from_this<_Ty2>*,
			_Ref_count_base *);

	weak_ptr<_Ty> _Wptr;
	};

template<class _Ty1,
	class _Ty2>
	inline void _Do_enable(
		_Ty1 *_Ptr,
		enable_shared_from_this<_Ty2> *_Es,
		_Ref_count_base *_Refptr)
	{	// reset internal weak pointer
	_Es->_Wptr._Resetw(_Ptr, _Refptr);
	}
_STD_END

_STD_BEGIN
	// TEMPLATE CLASS unique_ptr AND HELPERS

	// TEMPLATE CLASS default_delete
template<class _Ty>
	struct default_delete
	{	// default deleter for unique_ptr
	_CONST_FUN default_delete() _NOEXCEPT = default;

	template<class _Ty2,
		class = typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
			void>::type>
		default_delete(const default_delete<_Ty2>&) _NOEXCEPT
		{	// construct from another default_delete
		}

	void operator()(_Ty *_Ptr) const _NOEXCEPT
		{	// delete a pointer
		static_assert(0 < sizeof (_Ty),
			"can't delete an incomplete type");
		delete _Ptr;
		}
	};

template<class _Ty>
	struct default_delete<_Ty[]>
	{	// default deleter for unique_ptr to array of unknown size
	_CONST_FUN default_delete() _NOEXCEPT = default;

	template<class _Uty,
		class = typename enable_if<is_convertible<_Uty(*)[], _Ty(*)[]>::value,
			void>::type>
		default_delete(const default_delete<_Uty[]>&) _NOEXCEPT
		{	// construct from another default_delete
		}

	template<class _Uty,
		class = typename enable_if<is_convertible<_Uty(*)[], _Ty(*)[]>::value,
			void>::type>
		void operator()(_Uty *_Ptr) const _NOEXCEPT
		{	// delete a pointer
		static_assert(0 < sizeof (_Uty),
			"can't delete an incomplete type");
		delete[] _Ptr;
		}
	};

		// TEMPLATE STRUCT _Get_deleter_pointer_type
template<class _Val,
	class _Ty>
	struct _Get_deleter_pointer_type
	_GET_TYPE_OR_DEFAULT(pointer,
		_Val *);

	// TEMPLATE CLASS _Unique_ptr_base
template<class _Ty,
	class _Dx>
	class _Unique_ptr_base
	{	// stores pointer and deleter
public:
	typedef typename remove_reference<_Dx>::type _Dx_noref;
	typedef typename _Get_deleter_pointer_type<_Ty, _Dx_noref>::type pointer;

	template<class _Ptr2,
		class _Dx2>
		_Unique_ptr_base(_Ptr2 _Ptr, _Dx2&& _Dt)
		: _Mypair(_One_then_variadic_args_t(), _STD forward<_Dx2>(_Dt), _Ptr)
		{	// construct with compatible pointer and deleter
		}

	template<class _Ptr2>
		_CONST_FUN _Unique_ptr_base(_Ptr2 _Ptr)
		: _Mypair(_Zero_then_variadic_args_t(), _Ptr)
		{	// construct with compatible pointer
		}

	_Dx& get_deleter() _NOEXCEPT
		{	// return reference to deleter
		return (_Mypair._Get_first());
		}

	const _Dx& get_deleter() const _NOEXCEPT
		{	// return const reference to deleter
		return (_Mypair._Get_first());
		}

	pointer& _Myptr() _NOEXCEPT
		{	// return reference to pointer
		return (_Mypair._Get_second());
		}

	const pointer& _Myptr() const _NOEXCEPT
		{	// return const reference to pointer
		return (_Mypair._Get_second());
		}

	_Compressed_pair<_Dx, pointer> _Mypair;
	};

	// TEMPLATE CLASS unique_ptr SCALAR
template<class _Ty,
	class _Dx>	// = default_delete<_Ty>
	class unique_ptr
		: public _Unique_ptr_base<_Ty, _Dx>
	{	// non-copyable pointer to an object
public:
	typedef unique_ptr<_Ty, _Dx> _Myt;
	typedef _Unique_ptr_base<_Ty, _Dx> _Mybase;
	typedef typename _Mybase::pointer pointer;
	typedef _Ty element_type;
	typedef _Dx deleter_type;

	using _Mybase::get_deleter;

	_CONST_FUN unique_ptr() _NOEXCEPT
		: _Mybase(pointer())
		{	// default construct
		static_assert(!is_pointer<_Dx>::value,
			"unique_ptr constructed with null deleter pointer");
		}

	_CONST_FUN unique_ptr(nullptr_t) _NOEXCEPT
		: _Mybase(pointer())
		{	// null pointer construct
		static_assert(!is_pointer<_Dx>::value,
			"unique_ptr constructed with null deleter pointer");
		}

	_Myt& operator=(nullptr_t) _NOEXCEPT
		{	// assign a null pointer
		reset();
		return (*this);
		}

	explicit unique_ptr(pointer _Ptr) _NOEXCEPT
		: _Mybase(_Ptr)
		{	// construct with pointer
		static_assert(!is_pointer<_Dx>::value,
			"unique_ptr constructed with null deleter pointer");
		}

	unique_ptr(pointer _Ptr,
		typename _If<is_reference<_Dx>::value, _Dx,
			const typename remove_reference<_Dx>::type&>::type _Dt) _NOEXCEPT
		: _Mybase(_Ptr, _Dt)
		{	// construct with pointer and (maybe const) deleter&
		}

	unique_ptr(pointer _Ptr,
		typename remove_reference<_Dx>::type&& _Dt) _NOEXCEPT
		: _Mybase(_Ptr, _STD move(_Dt))
		{	// construct by moving deleter
		static_assert(!is_reference<_Dx>::value,
			"unique_ptr constructed with reference to rvalue deleter");
		}

	unique_ptr(unique_ptr&& _Right) _NOEXCEPT
		: _Mybase(_Right.release(),
			_STD forward<_Dx>(_Right.get_deleter()))
		{	// construct by moving _Right
		}

	template<class _Ty2,
		class _Dx2,
		class = typename enable_if<!is_array<_Ty2>::value
			&& is_convertible<typename unique_ptr<_Ty2, _Dx2>::pointer,
				pointer>::value
			&& (is_reference<_Dx>::value
				? is_same<_Dx2, _Dx>::value
				: is_convertible<_Dx2, _Dx>::value),
			void>::type>
		unique_ptr(unique_ptr<_Ty2, _Dx2>&& _Right) _NOEXCEPT
			: _Mybase(_Right.release(),
				_STD forward<_Dx2>(_Right.get_deleter()))
		{	// construct by moving _Right
		}

 #if _HAS_AUTO_PTR_ETC
	template<class _Ty2,
		class = typename enable_if<is_convertible<_Ty2 *, _Ty *>::value
			&& is_same<_Dx, default_delete<_Ty> >::value,
			void>::type>
		unique_ptr(auto_ptr<_Ty2>&& _Right) _NOEXCEPT
			: _Mybase(_Right.release())
		{	// construct by moving _Right
		}
 #endif /* _HAS_AUTO_PTR_ETC */

	template<class _Ty2,
		class _Dx2>
		typename enable_if<!is_array<_Ty2>::value
			&& is_assignable<_Dx&, _Dx2&&>::value
			&& is_convertible<typename unique_ptr<_Ty2, _Dx2>::pointer,
				pointer>::value,
			_Myt&>::type
		operator=(unique_ptr<_Ty2, _Dx2>&& _Right) _NOEXCEPT
		{	// assign by moving _Right
		reset(_Right.release());
		this->get_deleter() = _STD forward<_Dx2>(_Right.get_deleter());
		return (*this);
		}

	_Myt& operator=(_Myt&& _Right) _NOEXCEPT
		{	// assign by moving _Right
		if (this != &_Right)
			{	// different, do the move
			reset(_Right.release());
			this->get_deleter() = _STD forward<_Dx>(_Right.get_deleter());
			}
		return (*this);
		}

	void swap(_Myt& _Right) _NOEXCEPT
		{	// swap elements
		_Swap_adl(this->_Myptr(), _Right._Myptr());
		_Swap_adl(this->get_deleter(),
			_Right.get_deleter());
		}

	~unique_ptr() _NOEXCEPT
		{	// destroy the object
		if (get() != pointer())
			this->get_deleter()(get());
		}

	typename add_lvalue_reference<_Ty>::type operator*() const
		{	// return reference to object
		return (*get());
		}

	pointer operator->() const _NOEXCEPT
		{	// return pointer to class object
		return (_STD pointer_traits<pointer>::pointer_to(**this));
		}

	pointer get() const _NOEXCEPT
		{	// return pointer to object
		return (this->_Myptr());
		}

	explicit operator bool() const _NOEXCEPT
		{	// test for non-null pointer
		return (get() != pointer());
		}

	pointer release() _NOEXCEPT
		{	// yield ownership of pointer
		pointer _Ans = get();
		this->_Myptr() = pointer();
		return (_Ans);
		}

	void reset(pointer _Ptr = pointer()) _NOEXCEPT
		{	// establish new pointer
		pointer _Old = get();
		this->_Myptr() = _Ptr;
		if (_Old != pointer())
			this->get_deleter()(_Old);
		}

	unique_ptr(const _Myt&) = delete;
	_Myt& operator=(const _Myt&) = delete;
	};

	// TEMPLATE CLASS unique_ptr ARRAY
template<class _Ty,
	class _Dx>
	class unique_ptr<_Ty[], _Dx>
		: public _Unique_ptr_base<_Ty, _Dx>
	{	// non-copyable pointer to an array object
public:
	typedef unique_ptr<_Ty[], _Dx> _Myt;
	typedef _Unique_ptr_base<_Ty, _Dx> _Mybase;
	typedef typename _Mybase::pointer pointer;
	typedef _Ty element_type;
	typedef _Dx deleter_type;

	using _Mybase::get_deleter;

	_CONST_FUN unique_ptr() _NOEXCEPT
		: _Mybase(pointer())
		{	// default construct
		static_assert(!is_pointer<_Dx>::value,
			"unique_ptr constructed with null deleter pointer");
		}

	template<class _Uty>
		using _Enable_ctor_reset = enable_if_t<
			is_same<_Uty, pointer>::value
			|| (is_same<pointer, element_type *>::value
			&& is_pointer<_Uty>::value
			&& is_convertible<
				remove_pointer_t<_Uty>(*)[],
				element_type(*)[]
			>::value)>;

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	explicit unique_ptr(_Uty _Ptr) _NOEXCEPT
		: _Mybase(_Ptr)
		{	// construct with pointer
		static_assert(!is_pointer<_Dx>::value,
			"unique_ptr constructed with null deleter pointer");
		}

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	unique_ptr(_Uty _Ptr,
		typename _If<is_reference<_Dx>::value, _Dx,
			const typename remove_reference<_Dx>::type&>::type _Dt) _NOEXCEPT
		: _Mybase(_Ptr, _Dt)
		{	// construct with pointer and (maybe const) deleter&
		}

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	unique_ptr(_Uty _Ptr,
		typename remove_reference<_Dx>::type&& _Dt) _NOEXCEPT
		: _Mybase(_Ptr, _STD move(_Dt))
		{	// construct by moving deleter
		static_assert(!is_reference<_Dx>::value,
			"unique_ptr constructed with reference to rvalue deleter");
		}

	unique_ptr(unique_ptr&& _Right) _NOEXCEPT
		: _Mybase(_Right.release(),
			_STD forward<_Dx>(_Right.get_deleter()))
		{	// construct by moving _Right
		}

	_Myt& operator=(_Myt&& _Right) _NOEXCEPT
		{	// assign by moving _Right
		if (this != &_Right)
			{	// different, do the swap
			reset(_Right.release());
			this->get_deleter() = _STD move(_Right.get_deleter());
			}
		return (*this);
		}

	template<class _Uty,
		class _Ex,
		bool _More,
		class _UP_pointer = typename unique_ptr<_Uty, _Ex>::pointer,
		class _UP_element_type = typename unique_ptr<_Uty, _Ex>::element_type>
		using _Enable_conversion = enable_if_t<
			is_array<_Uty>::value
			&& is_same<pointer, element_type *>::value
			&& is_same<_UP_pointer, _UP_element_type *>::value
			&& is_convertible<_UP_element_type(*)[], element_type(*)[]>::value
			&& _More>;

	template<class _Uty,
		class _Ex,
		class = _Enable_conversion<_Uty, _Ex,
			is_reference<_Dx>::value
			? is_same<_Ex, _Dx>::value
			: is_convertible<_Ex, _Dx>::value> >
		unique_ptr(unique_ptr<_Uty, _Ex>&& _Right) _NOEXCEPT
			: _Mybase(_Right.release(),
				_STD forward<_Ex>(_Right.get_deleter()))
		{	// construct by moving _Right
		}

	template<class _Uty,
		class _Ex,
		class = _Enable_conversion<_Uty, _Ex,
			is_assignable<_Dx&, _Ex&&>::value> >
		_Myt& operator=(unique_ptr<_Uty, _Ex>&& _Right) _NOEXCEPT
		{	// assign by moving _Right
		reset(_Right.release());
		this->get_deleter() = _STD forward<_Ex>(_Right.get_deleter());
		return (*this);
		}

	_CONST_FUN unique_ptr(nullptr_t) _NOEXCEPT
		: _Mybase(pointer())
		{	// null pointer construct
		static_assert(!is_pointer<_Dx>::value,
			"unique_ptr constructed with null deleter pointer");
		}

	_Myt& operator=(nullptr_t) _NOEXCEPT
		{	// assign a null pointer
		reset();
		return (*this);
		}

	void reset(nullptr_t = nullptr_t()) _NOEXCEPT
		{	// establish new null pointer
		reset(pointer());
		}

	void swap(_Myt& _Right) _NOEXCEPT
		{	// swap elements
		_Swap_adl(this->_Myptr(), _Right._Myptr());
		_Swap_adl(this->get_deleter(), _Right.get_deleter());
		}

	~unique_ptr() _NOEXCEPT
		{	// destroy the object
		_Delete();
		}

	_Ty& operator[](size_t _Idx) const
		{	// return reference to object
		return (get()[_Idx]);
		}

	pointer get() const _NOEXCEPT
		{	// return pointer to object
		return (this->_Myptr());
		}

	explicit operator bool() const _NOEXCEPT
		{	// test for non-null pointer
		return (get() != pointer());
		}

	pointer release() _NOEXCEPT
		{	// yield ownership of pointer
		pointer _Ans = get();
		this->_Myptr() = pointer();
		return (_Ans);
		}

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	void reset(_Uty _Ptr) _NOEXCEPT
		{	// establish new pointer
		pointer _Old = get();
		this->_Myptr() = _Ptr;
		if (_Old != pointer())
			this->get_deleter()(_Old);
		}

	unique_ptr(const _Myt&) = delete;

	_Myt& operator=(const _Myt&) = delete;

private:
	void _Delete()
		{	// delete the pointer
		if (get() != pointer())
			this->get_deleter()(get());
		}
	};


	// TEMPLATE FUNCTION make_unique
template<class _Ty,
	class... _Types> inline
	typename enable_if<!is_array<_Ty>::value,
		unique_ptr<_Ty> >::type make_unique(_Types&&... _Args)
	{	// make a unique_ptr
	return (unique_ptr<_Ty>(new _Ty(_STD forward<_Types>(_Args)...)));
	}

template<class _Ty> inline
	typename enable_if<is_array<_Ty>::value && extent<_Ty>::value == 0,
		unique_ptr<_Ty> >::type make_unique(size_t _Size)
	{	// make a unique_ptr
	typedef typename remove_extent<_Ty>::type _Elem;
	return (unique_ptr<_Ty>(new _Elem[_Size]()));
	}

template<class _Ty,
	class... _Types>
	typename enable_if<extent<_Ty>::value != 0,
		void>::type make_unique(_Types&&...) = delete;


template<class _Ty,
	class _Dx>
	void swap(unique_ptr<_Ty, _Dx>& _Left,
		unique_ptr<_Ty, _Dx>& _Right) _NOEXCEPT
	{	// swap _Left with _Right
	_Left.swap(_Right);
	}

template<class _Ty1,
	class _Dx1,
	class _Ty2,
	class _Dx2>
	bool operator==(const unique_ptr<_Ty1, _Dx1>& _Left,
		const unique_ptr<_Ty2, _Dx2>& _Right)
	{	// test if unique_ptr _Left equals _Right
	return (_Left.get() == _Right.get());
	}

template<class _Ty1,
	class _Dx1,
	class _Ty2,
	class _Dx2>
	bool operator!=(const unique_ptr<_Ty1, _Dx1>& _Left,
		const unique_ptr<_Ty2, _Dx2>& _Right)
	{	// test if unique_ptr _Left doesn't equal _Right
	return (!(_Left == _Right));
	}

template<class _Ty1,
	class _Dx1,
	class _Ty2,
	class _Dx2>
	bool operator<(const unique_ptr<_Ty1, _Dx1>& _Left,
		const unique_ptr<_Ty2, _Dx2>& _Right)
	{	// test if unique_ptr _Left precedes _Right
	typedef typename unique_ptr<_Ty1, _Dx1>::pointer _Ptr1;
	typedef typename unique_ptr<_Ty2, _Dx2>::pointer _Ptr2;
	typedef typename common_type<_Ptr1, _Ptr2>::type _Common;
	return (less<_Common>()(_Left.get(), _Right.get()));

	}

template<class _Ty1,
	class _Dx1,
	class _Ty2,
	class _Dx2>
	bool operator>=(const unique_ptr<_Ty1, _Dx1>& _Left,
		const unique_ptr<_Ty2, _Dx2>& _Right)
	{	// test if unique_ptr _Left doesn't precede _Right
	return (!(_Left < _Right));
	}

template<class _Ty1,
	class _Dx1,
	class _Ty2,
	class _Dx2>
	bool operator>(const unique_ptr<_Ty1, _Dx1>& _Left,
		const unique_ptr<_Ty2, _Dx2>& _Right)
	{	// test if unique_ptr _Right precedes _Left
	return (_Right < _Left);
	}

template<class _Ty1,
	class _Dx1,
	class _Ty2,
	class _Dx2>
	bool operator<=(const unique_ptr<_Ty1, _Dx1>& _Left,
		const unique_ptr<_Ty2, _Dx2>& _Right)
	{	// test if unique_ptr _Right doesn't precede _Left
	return (!(_Right < _Left));
	}

template<class _Ty,
	class _Dx>
	bool operator==(const unique_ptr<_Ty, _Dx>& _Left,
		nullptr_t) _NOEXCEPT
	{	// test if unique_ptr == nullptr
	return (!_Left);
	}

template<class _Ty,
	class _Dx>
	bool operator==(nullptr_t,
		const unique_ptr<_Ty, _Dx>& _Right) _NOEXCEPT
	{	// test if nullptr == unique_ptr
	return (!_Right);
	}

template<class _Ty,
	class _Dx>
	bool operator!=(const unique_ptr<_Ty, _Dx>& _Left,
		nullptr_t _Right) _NOEXCEPT
	{	// test if unique_ptr != nullptr
	return (!(_Left == _Right));
	}

template<class _Ty,
	class _Dx>
	bool operator!=(nullptr_t _Left,
		const unique_ptr<_Ty, _Dx>& _Right) _NOEXCEPT
	{	// test if nullptr != unique_ptr
	return (!(_Left == _Right));
	}

template<class _Ty,
	class _Dx>
	bool operator<(const unique_ptr<_Ty, _Dx>& _Left,
		nullptr_t _Right)
	{	// test if unique_ptr < nullptr
	typedef typename unique_ptr<_Ty, _Dx>::pointer _Ptr;
	return (less<_Ptr>()(_Left.get(), _Right));
	}

template<class _Ty,
	class _Dx>
	bool operator<(nullptr_t _Left,
		const unique_ptr<_Ty, _Dx>& _Right)
	{	// test if nullptr < unique_ptr
	typedef typename unique_ptr<_Ty, _Dx>::pointer _Ptr;
	return (less<_Ptr>()(_Left, _Right.get()));
	}

template<class _Ty,
	class _Dx>
	bool operator>=(const unique_ptr<_Ty, _Dx>& _Left,
		nullptr_t _Right)
	{	// test if unique_ptr >= nullptr
	return (!(_Left < _Right));
	}

template<class _Ty,
	class _Dx>
	bool operator>=(nullptr_t _Left,
		const unique_ptr<_Ty, _Dx>& _Right)
	{	// test if nullptr >= unique_ptr
	return (!(_Left < _Right));
	}

template<class _Ty,
	class _Dx>
	bool operator>(const unique_ptr<_Ty, _Dx>& _Left,
		nullptr_t _Right)
	{	// test if unique_ptr > nullptr
	return (_Right < _Left);
	}

template<class _Ty,
	class _Dx>
	bool operator>(nullptr_t _Left,
		const unique_ptr<_Ty, _Dx>& _Right)
	{	// test if nullptr > unique_ptr
	return (_Right < _Left);
	}

template<class _Ty,
	class _Dx>
	bool operator<=(const unique_ptr<_Ty, _Dx>& _Left,
		nullptr_t _Right)
	{	// test if unique_ptr <= nullptr
	return (!(_Right < _Left));
	}

template<class _Ty,
	class _Dx>
	bool operator<=(nullptr_t _Left,
		const unique_ptr<_Ty, _Dx>& _Right)
	{	// test if nullptr <= unique_ptr
	return (!(_Right < _Left));
	}

		// GARBAGE COLLECTION

enum class pointer_safety {	// return codes for get_pointer_safety
	relaxed,
	preferred,
	strict
	};

typedef pointer_safety _Pointer_safety;

inline void declare_reachable(void *)
	{	// increment pointer reachable count
	}

template<class _Ty> inline
	_Ty *undeclare_reachable(_Ty *_Ptr)
	{	// decrement pointer reachable count
	return (_Ptr);
	}

inline void declare_no_pointers(char *, size_t)
	{	// declare region to be pointer free
	}

inline void undeclare_no_pointers(char *, size_t)
	{	// undeclare region to be pointer free
	}

inline _Pointer_safety get_pointer_safety() _NOEXCEPT
	{	// get pointer safety status
	return (pointer_safety::relaxed);
	}

namespace tr1 {	// TR1 additions
using _STD allocate_shared;
using _STD bad_weak_ptr;
using _STD const_pointer_cast;
using _STD dynamic_pointer_cast;
using _STD enable_shared_from_this;
using _STD get_deleter;
using _STD make_shared;
using _STD shared_ptr;
using _STD static_pointer_cast;
using _STD swap;
using _STD weak_ptr;
}	// namespace tr1

		// TEMPLATE STRUCT owner_less
template<class _Ty = void>
	struct owner_less;	// not defined

template<class _Ty>
	struct owner_less<shared_ptr<_Ty> >
	{	// functor for owner_before
	typedef shared_ptr<_Ty> first_argument_type;
	typedef shared_ptr<_Ty> second_argument_type;
	typedef bool result_type;

	bool operator()(const shared_ptr<_Ty>& _Left,
		const shared_ptr<_Ty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}

	bool operator()(const shared_ptr<_Ty>& _Left,
		const weak_ptr<_Ty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}

	bool operator()(const weak_ptr<_Ty>& _Left,
		const shared_ptr<_Ty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}
	};

template<class _Ty>
	struct owner_less<weak_ptr<_Ty> >
	{	// functor for owner_before
	typedef weak_ptr<_Ty> first_argument_type;
	typedef weak_ptr<_Ty> second_argument_type;
	typedef bool result_type;

	bool operator()(const weak_ptr<_Ty>& _Left,
		const weak_ptr<_Ty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}

	bool operator()(const weak_ptr<_Ty>& _Left,
		const shared_ptr<_Ty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}

	bool operator()(const shared_ptr<_Ty>& _Left,
		const weak_ptr<_Ty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}
	};

template<>
	struct owner_less<void>
	{	// functor for owner_before
	typedef int is_transparent;

	template<class _Ty,
		class _Uty>
		bool operator()(const shared_ptr<_Ty>& _Left, const shared_ptr<_Uty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}

	template<class _Ty,
		class _Uty>
		bool operator()(const shared_ptr<_Ty>& _Left, const weak_ptr<_Uty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}

	template<class _Ty,
		class _Uty>
		bool operator()(const weak_ptr<_Ty>& _Left, const shared_ptr<_Uty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}

	template<class _Ty,
		class _Uty>
		bool operator()(const weak_ptr<_Ty>& _Left, const weak_ptr<_Uty>& _Right) const
		{	// apply owner_before to operands
		return (_Left.owner_before(_Right));
		}
	};

	// TEMPLATE STRUCT SPECIALIZATION hash
template<class _Ty,
	class _Dx>
	struct hash<unique_ptr<_Ty, _Dx> >
	{	// hash functor
	typedef unique_ptr<_Ty, _Dx> argument_type;
	typedef size_t result_type;

	size_t operator()(const argument_type& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
		typedef typename argument_type::pointer _Ptrtype;
		return (hash<_Ptrtype>()(_Keyval.get()));
		}
	};

template<class _Ty>
	struct hash<shared_ptr<_Ty> >
	{	// hash functor
	typedef shared_ptr<_Ty> argument_type;
	typedef size_t result_type;

	size_t operator()(const argument_type& _Keyval) const
		{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (hash<_Ty *>()(_Keyval.get()));
		}
	};

		// FUNCTION align
inline void *align(size_t _Bound, size_t _Size,
	void *& _Ptr, size_t& _Space) _NOEXCEPT
	{	// try to carve out _Size bytes on boundary _Bound
	size_t _Off = (size_t)((uintptr_t)_Ptr & (_Bound - 1));
	if (0 < _Off)
		_Off = _Bound - _Off;	// number of bytes to skip
	if (_Space < _Off || _Space - _Off < _Size)
		return (0);
	else
		{	// enough room, update
		_Ptr = (char *)_Ptr + _Off;
		_Space -= _Off;
		return (_Ptr);
		}
	}

		// WRAP SPIN-LOCK
struct _Shared_ptr_spin_lock
	{	// class to manage a spin lock for shared_ptr atomic operations
	_Shared_ptr_spin_lock()
		{	// lock the spin lock
		_Lock_shared_ptr_spin_lock();
		}

	~_Shared_ptr_spin_lock() _NOEXCEPT
		{	// unlock the spin lock
		_Unlock_shared_ptr_spin_lock();
		}
	};

template<class _Ty> inline
	bool atomic_is_lock_free(const shared_ptr<_Ty> *)
	{	// return true if atomic operations on shared_ptr<_Ty> are lock-free
	return (false);
	}

template<class _Ty> inline
	shared_ptr<_Ty> atomic_load_explicit(const shared_ptr<_Ty> *_Ptr,
		memory_order)
	{	// load *_Ptr atomically
	_Shared_ptr_spin_lock _Lock;
	shared_ptr<_Ty> _Result = *_Ptr;
	return (_Result);
	}

template<class _Ty> inline
	shared_ptr<_Ty> atomic_load(const shared_ptr<_Ty> *_Ptr)
	{	// load *_Ptr atomically
	return (_STD atomic_load_explicit(_Ptr, memory_order_seq_cst));
	}

template<class _Ty> inline
	void atomic_store_explicit(shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other,
		memory_order)
	{	// store _Other to *_Ptr atomically
	_Shared_ptr_spin_lock _Lock;
	_Ptr->swap(_Other);
	}

template<class _Ty> inline
	void atomic_store(shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other)
	{	// store _Other to *_Ptr atomically
	_STD atomic_store_explicit(_Ptr, _STD move(_Other), memory_order_seq_cst);
	}

template<class _Ty> inline
	shared_ptr<_Ty> atomic_exchange_explicit(
		shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other,
		memory_order)
	{	// copy _Other to *_Ptr and return previous value of *_Ptr atomically
	_Shared_ptr_spin_lock _Lock;
	_Ptr->swap(_Other);
	return (_Other);
	}

template<class _Ty> inline
	shared_ptr<_Ty> atomic_exchange(
		shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other)
	{	// copy _Other to *_Ptr and return previous value of *_Ptr atomically
	return (_STD atomic_exchange_explicit(_Ptr, _STD move(_Other), memory_order_seq_cst));
	}

template<class _Ty> inline
	bool atomic_compare_exchange_weak_explicit(
		shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp, shared_ptr<_Ty> _Value,
		memory_order, memory_order)
	{	// atomically compare and exchange
	shared_ptr<_Ty> _Old_exp;	// destroyed outside spin lock
	_Shared_ptr_spin_lock _Lock;
	bool _Success = _Ptr->get() == _Exp->get()
		&& !_Ptr->owner_before(*_Exp)
		&& !_Exp->owner_before(*_Ptr);
	if (_Success)
		_Ptr->swap(_Value);
	else
		{	// match failed
		_Exp->swap(_Old_exp);
		*_Exp = *_Ptr;
		}
	return (_Success);
	}

template<class _Ty> inline
	bool atomic_compare_exchange_weak(
		shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp,
			shared_ptr<_Ty> _Value)
	{	// atomically compare and exchange
	return (_STD atomic_compare_exchange_weak_explicit(_Ptr, _Exp, _STD move(_Value),
		memory_order_seq_cst, memory_order_seq_cst));
	}

template<class _Ty> inline
	bool atomic_compare_exchange_strong_explicit(
		shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp, shared_ptr<_Ty> _Value,
		memory_order, memory_order)
	{	// atomically compare and exchange
	return (_STD atomic_compare_exchange_weak_explicit(_Ptr, _Exp, _STD move(_Value),
		memory_order_seq_cst, memory_order_seq_cst));
	}

template<class _Ty> inline
	bool atomic_compare_exchange_strong(
	shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp,
		shared_ptr<_Ty> _Value)
	{	// atomically compare and exchange
	return (_STD atomic_compare_exchange_strong_explicit(_Ptr, _Exp, _STD move(_Value),
		memory_order_seq_cst, memory_order_seq_cst));
	}
_STD_END
 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _MEMORY_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
