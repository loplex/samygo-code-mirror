/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(XSTRINGCACHEDALLOCATOR_INCLUDE_GUARD_1357924680)
#define XSTRINGCACHEDALLOCATOR_INCLUDE_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/XStringCached.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XStringCachedAllocator
{
public:

	typedef XStringCached	string_type;

	typedef string_type::GetAndReleaseCachedString	GetAndReleaseCachedString;

	typedef ReusableArenaAllocator<string_type>		ArenaAllocatorType;
	typedef ArenaAllocatorType::size_type			size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XStringCachedAllocator(size_type	theBlockCount);

	~XStringCachedAllocator();

	/**
	 * Create an XString object from a cached string.
	 * 
	 * @param theXObject The source cached string
	 *
	 * @return a pointer to the new XStringCached instance.
	 */
	string_type*
	createString(GetAndReleaseCachedString&		theValue);

	/**
	 * Clone an XStringCached object.
	 * 
	 * @param value source XStringCached
	 *
	 * @return pointer to an XStringCached
	 */
	string_type*
	clone(const string_type&	value);

	/**
	 * Delete an XStringCached object from allocator.	 
	 */
	bool
	destroy(string_type*	theString);

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const string_type*	theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all XStringCached objects from allocator.	 
	 */
	void 
	reset();

	/**
	 * Get size of an ArenaBlock, that is, the number
	 * of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockCount() const
	{
		return m_allocator.getBlockCount();
	}

	/**
	 * Get the number of ArenaBlocks currently allocated.
	 *
	 * @return The number of blocks.
	 */
	size_type
	getBlockSize() const
	{
		return m_allocator.getBlockSize();
	}

private:

	// Not implemented...
	XStringCachedAllocator(const XStringCachedAllocator&);

	XStringCachedAllocator&
	operator=(const XStringCachedAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSTRINGCACHEDALLOCATOR_INCLUDE_GUARD_1357924680
