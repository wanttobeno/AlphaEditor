// AutoLock.cpp: implementation of the CAutoLock class.
//
//////////////////////////////////////////////////////////////////////

#include "AutoLock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace ALOCK
{
Mutex::Mutex()
{
	impl = new Mutex::Impl;
}

Mutex::~Mutex()
{
	if( InterlockedExchangeAdd(&impl->refcount, -1) == 1 )
		delete impl;
	impl = 0;
}

Mutex::Mutex(const Mutex& m)
{
	impl = m.impl;
	InterlockedExchangeAdd(&impl->refcount, 1);
}

Mutex& Mutex::operator = (const Mutex& m)
{
	InterlockedExchangeAdd(&m.impl->refcount, 1);
	if( InterlockedExchangeAdd(&impl->refcount, -1) == 1 )
		delete impl;
	impl = m.impl;
	return *this;
}

void Mutex::lock() { impl->lock(); }
void Mutex::unlock() { impl->unlock(); }
BOOL Mutex::trylock() { return impl->trylock(); }

} // namespace LOCK
