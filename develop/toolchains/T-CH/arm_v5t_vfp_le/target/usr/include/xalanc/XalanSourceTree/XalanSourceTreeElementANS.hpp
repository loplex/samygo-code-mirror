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
#if !defined(XALANSOURCETREEELEMENTANS_HEADER_GUARD_1357924680)
#define XALANSOURCETREEELEMENTANS_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElementA.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementANS : public XalanSourceTreeElementA
{
public:

	/**
	 * Constructor.
	 *
	 * @param theTagName The tag name of the element
	 * @param theLocalName The local name of the attribute
	 * @param theNamespaceURI The namespace URI of the attribute
	 * @param thePrefix The namespace prefix of the attribute
	 * @param theOwnerDocument The document that owns the instance
	 * @param theAttributes An array of pointers to the attribute instances for the element
	 * @param theAttributeCount The number of attributes.
	 * @param theParentNode The parent node, if any.
	 * @param thePreviousSibling The previous sibling, if any.
	 * @param theNextSibling The next sibling, if any.
	 * @param theIndex The document-order index of the node.
	 */
	XalanSourceTreeElementANS(
			const XalanDOMString&		theTagName,
			const XalanDOMString&		theLocalName,
			const XalanDOMString&		theNamespaceURI,
			const XalanDOMString&		thePrefix,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanSourceTreeAttr**		theAttributes,
			AttributesCountType			theAttributeCount,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0,
			IndexType					theIndex = 0);

	virtual
	~XalanSourceTreeElementANS();


#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XalanSourceTreeElementANS*
#endif
	cloneNode(bool deep) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

protected:

	XalanSourceTreeElementANS(
			const XalanSourceTreeElementANS&	theSource,
			bool								deep);

	XalanSourceTreeElementANS(
			const XalanSourceTreeElementA&	theSource,
			bool							deep);

private:

	// Not implemented...
	XalanSourceTreeElementANS&
	operator=(const XalanSourceTreeElementANS&	theSource);

	bool
	operator==(const XalanSourceTreeElementANS&	theRHS) const;


	// Data members...
	const XalanDOMString&	m_localName;

	const XalanDOMString&	m_prefix;

	const XalanDOMString&	m_namespaceURI;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREEELEMENTANS_HEADER_GUARD_1357924680)
