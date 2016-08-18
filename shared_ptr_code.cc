
/**
 * CLASS _Ref_count_base
 * 引用计数和weaks计数器是原子类型
 */
class _Ref_count_base
{   // common code for reference counting
private:
    virtual void _Destroy() noexcept = 0;
    virtual void _Delete_this() noexcept = 0;

private:
    _Atomic_counter_t _Uses;
    _Atomic_counter_t _Weaks;

protected:
    _Ref_count_base()
    {   // construct
        _Init_atomic_counter(_Uses, 1);
        _Init_atomic_counter(_Weaks, 1);
    }
    void _Incref()
    {   // increment use count
        _MT_INCR(_Uses);    // _MT_INCR是原子操作
    }
    /* 引用计数减1，如果为零则释放源对象内存*/
    void _Decref()
    {   // decrement use count
        if (_MT_DECR(_Uses) == 0)
        {   // destroy managed resource, decrement weak reference count
            _Destroy();
            _Decwref();
        }
    }
    /* 当_Weaks为零时表示没有有效的shared_ptr指针，此时释放计数器内存 */
    void _Decwref()
    {   // decrement weak reference count
        if (_MT_DECR(_Weaks) == 0)
            _Delete_this();
    }
    long _Use_count() const noexcept
    {   // return use count
        return (_Get_atomic_count(_Uses));
    }
}；

// TEMPLATE CLASS _Ref_count
template<class _Ty>
class _Ref_count : public _Ref_count_base
{   // handle reference counting for object without deleter
public:
    _Ref_count(_Ty *_Px)
        : _Ref_count_base(), _Ptr(_Px)
    {   // construct
    }

private:
    virtual void _Destroy() noexcept
    {   // destroy managed resource
        delete _Ptr;
    }

    virtual void _Delete_this() noexcept
    {   // destroy self
        delete this;
    }

    _Ty * _Ptr; //源对象指针
};

// base class for shared_ptr and weak_ptr
template<class _Ty>
class _Ptr_base
{   
public:
    typedef _Ptr_base<_Ty> _Myt;
    typedef _Ty element_type;
  
    void _Decref()
    {   // decrement reference count
        if (_Rep != 0)
            _Rep->_Decref();
    }
    
    void _Swap(_Ptr_base& _Right) noexcept
    {   // swap pointers
        std::swap(_Rep, _Right._Rep);
        std::swap(_Ptr, _Right._Ptr);
    }
    long use_count() const noexcept
    {   // return use count
        return (_Rep ? _Rep->_Use_count() : 0);
    }
private:
    _Ty *_Ptr;  // 源对象指针
    _Ref_count_base *_Rep;  // 引用计数基类指针
    template<class _Ty0>
    friend class _Ptr_base;
}

// class for reference counted resource management
template<class _Ty>
class shared_ptr : public _Ptr_base<_Ty>
{   
public:
    typedef shared_ptr<_Ty> _Myt;
    typedef _Ptr_base<_Ty> _Mybase;

    constexpr shared_ptr() noexcept {}
    // construct shared_ptr object that owns _Px
    template<class _Ux>
    explicit shared_ptr(_Ux *_Px)
    {   
        /* 需要考虑内存分配失败的情况，try catch 捕获异常并delete _Ptr */
        _Rep = new _Ref_count<_Ux>(_Px);
        _Ptr = _Px;
    }
    /* 可以自定义删除器 */
    template<class _Ux, class _Dx>
    shared_ptr(_Ux *_Px, _Dx _Dt)
    {   // construct with _Px, deleter
        _Resetp(_Px, _Dt);  //略
    }

    /**
     * shared_ptr<int> sp1(new int(10))
     * shared_ptr<int> sp2(sp1)
     * sp2直接复制了sp1的引用计数指针和源对象指针（浅复制）
     * sp1和sp2的内容完全相同，所以如果sp1析构了，引用计数减一
     * 相当于sp2的引用计数也减一。
     */
    shared_ptr(const _Myt& _Other) noexcept
    {   
        /*  */
        if (_Other._Rep != nullptr)
            _Other._Rep->_Incref();
        _Rep = _Other._Rep;
        _Ptr = _Other._Ptr;
    }

    /**
     * 1. 以右值拷贝构造了一个临时变量，临时变量和右值的引用计数都增1，
     * 2. 将本身和一个临时变量交换，本对象浅复制了临时变量的内容，
     * 3. 临时变量在退出赋值函数之后会被析构，引用计数减1
     */
    _Myt& operator=(_Myt&& _Right) noexcept
    {   // take resource from _Right
        shared_ptr(std::move(_Right)).swap(*this);
        return (*this);
    }
    _Myt& operator=(const _Myt& _Right) noexcept
    {   // assign shared ownership of resource owned by _Right
        shared_ptr(_Right).swap(*this);
        return (*this);
    }

    /*析构函数实际上调用的是成员_Rep的函数_Decref()*/
    ~shared_ptr() noexcept
    {   // release resource
        this->_Decref();
    }

    void reset() noexcept
    {   // release resource and convert to empty shared_ptr object
        /* 本对象和临时对象交换，临时对象在函数退出后会被析构，引用计数减1 */
        shared_ptr().swap(*this);
    }
    void swap(_Myt& _Other) _NOEXCEPT
    {   // swap pointers
        this->_Swap(_Other);
    }
};