#pragma once

// Author  : DustPG
// License : MIT: see more in "License.txt"

// 用途:	使用内存池的内存分配器 参(fu)考(zhi)std::allocator写的 
//			不调用构造/析构函数(用于基本类型的构造)

template<class Object>
class GameAllocator	: public std::allocator<Object>{
public:
	typedef GameAllocator<Object> other;

	typedef std::_Allocator_base<Object> _Mybase;
	typedef typename _Mybase::value_type value_type;

	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef void *void_pointer;
	typedef const void *const_void_pointer;

	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef std::false_type propagate_on_container_copy_assignment;
	typedef std::false_type propagate_on_container_move_assignment;
	typedef std::false_type propagate_on_container_swap;

	GameAllocator<Object> select_on_container_copy_construction() const{
		return (*this);
	}

	template<class _Other>
	struct rebind{
		typedef GameAllocator<_Other> other;
	};

	pointer address(reference _Val) const _NOEXCEPT
	{	// return address of mutable _Val
		return (_STD addressof(_Val));
	}

		const_pointer address(const_reference _Val) const _NOEXCEPT
	{	// return address of nonmutable _Val
		return (_STD addressof(_Val));
	}

		GameAllocator() _THROW0()
	{	// construct default GameAllocator (do nothing)
	}

	GameAllocator(const GameAllocator<Object>&) _THROW0()
	{	// construct by copying (do nothing)
	}

	template<class _Other>
	GameAllocator(const GameAllocator<_Other>&) _THROW0()
	{	// construct from a related GameAllocator (do nothing)
	}

	template<class _Other>
	GameAllocator<Object>& operator=(const GameAllocator<_Other>&)
	{	// assign from a related GameAllocator (do nothing)
		return (*this);
	}

	void deallocate(pointer _Ptr, size_type)
	{	// deallocate object at _Ptr, ignore size
		MPool.Free(_Ptr);
	}

	pointer allocate(size_type _Count)
	{	// allocate array of _Count elements
		return (pointer)MPool.Alloc(_Count * sizeof Object);
	}

	pointer allocate(size_type _Count, const void *)
	{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}

	size_t max_size() const _THROW0()
	{	// estimate maximum array size
		return (EzMemNode::MEMORY_CELL_BYTE - sizeof(Object*)) / sizeof (Object);
	}
};

typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, GameAllocator<wchar_t>> GameString;