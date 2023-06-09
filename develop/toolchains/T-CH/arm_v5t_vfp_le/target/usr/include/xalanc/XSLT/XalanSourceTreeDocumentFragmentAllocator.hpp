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

#if !defined(XALANSOURCETREEDOCUMENTFRAGMENTALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANSOURCETREEDOCUMENTFRAGMENTALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDocumentFragment.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XalanSourceTreeDocumentFragmentAllocator
{
public:

	typedef XalanSourceTreeDocumentFragment		data_type;

	typedef ReusableArenaAllocator<data_type>	ArenaAllocatorType;
	typedef ArenaAllocatorType::size_type		size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XalanSourceTreeDocumentFragmentAllocator(size_type	theBlockCount);

	~XalanSourceTreeDocumentFragmentAllocator();
	
	/**
	 * Create a XalanSourceTreeDocumentFragment object.
	 * 
	 * @param theValue The constructor parameter
	 *
	 * @return pointer to a node
	 */
	data_type*
	create(XalanSourceTreeDocument&		theValue);

	/**
	 * Create an XalanSourceTreeDocumentFragment object.
	 * 
	 * @param theSource	source instance
	 *
	 * @return pointer to a node
	 */
	data_type*
	create(const data_type&		theSource);

	/**
	 * Delete an XalanSourceTreeDocumentFragment object from allocator.	 
	 */
	bool
	destroy(data_type*	theObject);

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const data_type*		theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all XalanSourceTreeDocumentFragment objects from allocator.	 
	 */	
	void
	reset();

	/**
	 * Get the number of ArenaBlocks currently allocated.
	 *
	 * @return The number of blocks.
	 */
	size_type
	getBlockCount() const
	{
		return m_allocator.getBlockCount();
	}

	/**
	 * Get size of an ArenaBlock, that is, the number
	 * of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockSize() const
	{
		return m_allocator.getBlockSize();
	}

private:

	// Not implemented...
	XalanSourceTreeDocumentFragmentAllocator(const XalanSourceTreeDocumentFragmentAllocator&);

	XalanSourceTreeDocumentFragmentAllocator&
	operator=(const XalanSourceTreeDocumentFragmentAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANSOURCETREEDOCUMENTFRAGMENTALLOCATOR_INCLUDE_GUARD_12455133
