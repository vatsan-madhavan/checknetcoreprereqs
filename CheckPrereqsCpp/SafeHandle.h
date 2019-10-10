#pragma once
#include <windows.h>
#include <type_traits>
#include <functional>


template<typename THANDLE, typename = std::enable_if_t<std::is_pointer<THANDLE>::value>>
class SafeHandle
{
private:
	THANDLE handle;
	std::function<BOOL(THANDLE)> release;
public:
	inline SafeHandle(THANDLE handle, std::function<BOOL(THANDLE)> release)
		: handle(handle), release(release)
	{
	}

	inline SafeHandle() : handle(nullptr), release(nullptr){}

	inline SafeHandle(const SafeHandle& other, std::function<THANDLE(THANDLE)> copy = nullptr)
	{
		handle = copy != nullptr ? copy(other.handle) : other.handle;
		release = other.release;
	}

	inline SafeHandle& operator=(const SafeHandle& rhs)
	{
		handle = rhs.handle;
		release = rhs.release;
		return *this;
	}

	inline operator THANDLE()
	{
		return handle;
	}

	inline operator const THANDLE() const
	{
		return handle;
	}

	inline operator bool() const
	{
		return handle != nullptr;
	}

	inline ~SafeHandle()
	{
		if (release != nullptr)
		{
			release(handle);
			handle = nullptr;
		}
	}
};