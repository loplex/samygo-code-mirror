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

#if !defined(XALANSOURCETREEATTRIBUTEALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANSOURCETREEATTRIBUTEALLOCATOR_INCLUDE_GUARD_12455133



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeAttr.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeAttributeAllocator
{
public:

	typedef XalanSourceTreeAttr						ObjectType;

	typedef ObjectType::IndexType					IndexType;

#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
	typedef ArenaBlock<ObjectType>					ArenaBlockType;

	typedef ArenaAllocator<ObjectType,
						   ArenaBlockType>			ArenaAllocatorType;
#else
	typedef ArenaAllocator<ObjectType>				ArenaAllocatorType;
#endif

	typedef ArenaAllocatorType::size_type			size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XalanSourceTreeAttributeAllocator(size_type		theBlockCount);

	~XalanSourceTreeAttributeAllocator();

	/**
	 * Create an instance.
	 * 
	 * @param theName The name of the attribute
	 * @param theValue The value of the attribute
	 * @param theOwnerDocument The document that owns the instance
	 * @param theOwnerElement The element that owns the instance
	 * @param theIndex The document-order index of the node.
	 *
	 * @return pointer to the instance
	 */
	ObjectType*
	create(
			const XalanDOMString&		theName,
			const XalanDOMString&		theValue,
			XalanSourceTreeElement*		theOwnerElement = 0,
			IndexType					theIndex = 0);

	/**
	 * Delete all objects from allocator.	 
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
	XalanSourceTreeAttributeAllocator(const XalanSourceTreeAttributeAllocator&);

	XalanSourceTreeAttributeAllocator&
	operator=(const XalanSourceTreeAttributeAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANSOURCETREEATTRIBUTEALLOCATOR_INCLUDE_GUARD_12455133
