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
#if !defined(XALAN_OUTPUTCONTEXTSTACK_HEADER_GUARD)
#define XALAN_OUTPUTCONTEXTSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <deque>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;



class XALAN_XSLT_EXPORT OutputContextStack
{
public:

	struct OutputContext
	{
		OutputContext(FormatterListener*	theListener = 0) :
			m_flistener(theListener),
			m_pendingAttributes(),
			m_pendingElementName(),
			m_hasPendingStartDocument(false),
			m_mustFlushPendingStartDocument(false)
		{
		}

		~OutputContext()
		{
		}

		void
		reset()
		{
			m_flistener = 0;

			m_pendingAttributes.clear();

			m_pendingElementName.clear();

			m_hasPendingStartDocument = false;

			m_mustFlushPendingStartDocument = false;
		}

		FormatterListener*	m_flistener;

		AttributeListImpl	m_pendingAttributes;

		XalanDOMString		m_pendingElementName;

		bool				m_hasPendingStartDocument;

		bool				m_mustFlushPendingStartDocument;
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef deque<OutputContext>			OutputContextStackType;
#else
	typedef std::deque<OutputContext>		OutputContextStackType;
#endif

	typedef OutputContextStackType::size_type	size_type;

	explicit
	OutputContextStack();

	~OutputContextStack();

	void
	pushContext(FormatterListener*	theListener = 0);

	void
	popContext();

	FormatterListener*
	getFormatterListener() const
	{
		return (*m_stackPosition).m_flistener;
	}

	FormatterListener*&
	getFormatterListener()
	{
		return (*m_stackPosition).m_flistener;
	}

	const AttributeListImpl&
	getPendingAttributes() const
	{
		return (*m_stackPosition).m_pendingAttributes;
	}

	AttributeListImpl&
	getPendingAttributes()
	{
		return (*m_stackPosition).m_pendingAttributes;
	}

	const XalanDOMString&
	getPendingElementName() const
	{
		return (*m_stackPosition).m_pendingElementName;
	}

	XalanDOMString&
	getPendingElementName()
	{
		return (*m_stackPosition).m_pendingElementName;
	}

	const bool&
	getHasPendingStartDocument() const
	{
		return (*m_stackPosition).m_hasPendingStartDocument;
	}

	bool&
	getHasPendingStartDocument()
	{
		return (*m_stackPosition).m_hasPendingStartDocument;
	}

	const bool&
	getMustFlushPendingStartDocument() const
	{
		return (*m_stackPosition).m_mustFlushPendingStartDocument;
	}

	bool&
	getMustFlushPendingStartDocument()
	{
		return (*m_stackPosition).m_mustFlushPendingStartDocument;
	}

	size_type
	size() const
	{
		// Since we always keep one dummy entry at the beginning,
		// subtract one from the size
		assert(m_stackSize == size_type(OutputContextStackType::const_iterator(m_stackPosition) - m_stack.begin()));

		return m_stackSize;
	}

	bool
	empty() const
	{
		return size() == 0 ? true : false;
	}

	void
	clear();

	void
	reset();

private:

	// not implemented
	OutputContextStack(const OutputContextStack&);

	bool
	operator==(const OutputContextStack&) const;

	OutputContextStack&
	operator=(const OutputContextStack&);

	/**
	 * A stack to hold the output contexts...
	 */
	OutputContextStackType				m_stack;

	OutputContextStackType::iterator	m_stackPosition;

	size_type							m_stackSize;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD
