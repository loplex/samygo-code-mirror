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
#if !defined(XALANSOURCETREEDOCUMENTFRAGMENT_HEADER_GUARD_1357924680)
#define XALANSOURCETREEDOCUMENTFRAGMENT_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



#if !defined(XALAN_NO_COVARIANT_RETURN_TYPE)
#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>
#endif



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeComment;
class XalanSourceTreeDocument;
class XalanSourceTreeElement;
class XalanSourceTreeProcessingInstruction;
class XalanSourceTreeText;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeDocumentFragment : public XalanDocumentFragment
{
public:

	XalanSourceTreeDocumentFragment(XalanSourceTreeDocument&	theOwnerDocument);

	XalanSourceTreeDocumentFragment(
			const XalanSourceTreeDocumentFragment&	theSource,
			bool									deep = false);

	virtual
	~XalanSourceTreeDocumentFragment();

	// These interfaces are inherited from XalanNode...

	virtual const XalanDOMString&
	getNodeName() const;

	virtual const XalanDOMString&
	getNodeValue() const;

	virtual NodeType
	getNodeType() const;

	virtual XalanNode*
	getParentNode() const;

	virtual const XalanNodeList*
	getChildNodes() const;

	virtual XalanNode*
	getFirstChild() const;

	virtual XalanNode*
	getLastChild() const;

	virtual XalanNode*
	getPreviousSibling() const;

	virtual XalanNode*
	getNextSibling() const;

	virtual const XalanNamedNodeMap*
	getAttributes() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanDocument*
#else
	virtual XalanSourceTreeDocument*
#endif
	getOwnerDocument() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XalanSourceTreeDocumentFragment*
#endif
	cloneNode(bool deep) const;

	virtual XalanNode*
	insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild);

	virtual XalanNode*
	replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild);

	virtual XalanNode*
	removeChild(XalanNode*	oldChild);

	virtual XalanNode*
	appendChild(XalanNode*	newChild);

	virtual bool
	hasChildNodes() const;

	virtual void
	setNodeValue(const XalanDOMString&	nodeValue);

	virtual void
	normalize();

	virtual bool
	isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString&		prefix);

	virtual bool
	isIndexed() const;

	virtual IndexType
	getIndex() const;

	// These interfaces are new...

	void
	appendChildNode(XalanSourceTreeComment*		theChild);

	void
	appendChildNode(XalanSourceTreeElement*		theChild);

	void
	appendChildNode(XalanSourceTreeProcessingInstruction*	theChild);

	void
	appendChildNode(XalanSourceTreeText*	theChild);

	void
	clearChildren();

	XalanSourceTreeDocumentFragment*
	clone(bool	deep) const
	{
		return new XalanSourceTreeDocumentFragment(*this, deep);
	}

protected:

	XalanSourceTreeDocumentFragment&
	operator=(const XalanSourceTreeDocumentFragment&	theSource);

	bool
	operator==(const XalanSourceTreeDocumentFragment&		theRHS) const;

private:

	XalanSourceTreeDocument* const	m_ownerDocument;

	XalanNode*						m_firstChild;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREEDOCUMENTFRAGMENT_HEADER_GUARD_1357924680)
