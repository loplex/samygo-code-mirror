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

#if !defined(XALANQNAMEBYVALUEALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANQNAMEBYVALUEALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>



#include <xalanc/PlatformSupport/ArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XalanQNameByValueAllocator
{
public:

	typedef XalanQNameByValue					data_type;
	typedef data_type::NamespacesStackType		NamespacesStackType;
	typedef data_type::LocatorType				LocatorType;

#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
	typedef ArenaBlock<data_type>				ArenaBlockType;
	typedef ArenaAllocator<data_type,
						   ArenaBlockType>		ArenaAllocatorType;
#else
	typedef ArenaAllocator<data_type>			ArenaAllocatorType;
#endif

	typedef ArenaAllocatorType::size_type		size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XalanQNameByValueAllocator(size_type		theBlockCount);

	~XalanQNameByValueAllocator();
	
	/**
	 * Create an instance.
	 *
	 * @param theSource The source of the copy.
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(const XalanQNameByValue&	theSource);

	/**
	 * Create an instance.
	 *
	 * @param theNamespace namespace string
	 * @param theLocalPart local part string
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			const XalanDOMString&	theNamespaceURI,
			const XalanDOMString&	theLocalPart);

	/**
	 * Create an instance.
	 *
	 * @param qname       QName string
	 * @param namespaces  namespace vector stack to use
	 * @param locator     The Locator instance for error reporting, if any
	 * @param fUseDefault If true, then elements with no prefix will have the default namespace URI, if there is one.
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false);

	/**
	 * Create an instance.
	 *
	 * @param qname       QName string
	 * @param namespaces  namespace vector stack to use
	 * @param locator     The Locator instance for error reporting, if any
	 * @param fUseDefault If true, then elements with no prefix will have the default namespace URI, if there is one.
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false);

	/**
	 * Create an instance.
	 *
	 * @param qname            QName string
	 * @param namespaceContext context object for namespace resolution
	 * @param envSupport       XPathEnvSupport class instance
	 * @param domSupport       DOMSupport class instance
	 * @param locator          The Locator instance for error reporting, if any
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			const XalanDOMString&	qname,
			const XalanElement*		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const DOMSupport& 		domSupport,
			const LocatorType*		locator = 0);

	/**
	 * Create an instance.
	 *
	 * @param qname       QName string
	 * @param theResolver prefix resolver to use
	 * @param locator     The Locator instance for error reporting, if any
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			const XalanDOMString&	qname,
			const PrefixResolver*	theResolver = 0,
			const LocatorType*		locator = 0);

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const data_type*		theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all objects from the allocator.	 
	 */	
	void
	reset()
	{
		m_allocator.reset();
	}

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
	XalanQNameByValueAllocator(const XalanQNameByValueAllocator&);

	XalanQNameByValueAllocator&
	operator=(const XalanQNameByValueAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANQNAMEBYVALUEALLOCATOR_INCLUDE_GUARD_12455133
