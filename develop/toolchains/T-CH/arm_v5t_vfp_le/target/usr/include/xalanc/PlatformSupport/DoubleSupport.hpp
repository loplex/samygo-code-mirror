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
#if !defined(DOUBLESUPPORT_HEADER_GUARD_1357924680)
#define DOUBLESUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cmath>
#include <functional>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// A class to help us support IEEE 754.
class XALAN_PLATFORMSUPPORT_EXPORT DoubleSupport
{
public:

	// Use these functions to determine if a value represents one of these
	// values.  It seems that under some architectures, NaN will compare
	// as equal to any number, which is a big problem.  Hence these helper
	// functions.

	/**
	 * Determine if target is not a number
	 * 
	 * @param theNumber target number
	 * @return true if target represents the "not a number" value
	 */
	static bool
	isNaN(double	theNumber)
	{
		const NumberUnion	temp = { theNumber };

		return s_NaN.dwords.dw1 == temp.dwords.dw1 &&
			   s_NaN.dwords.dw2 == temp.dwords.dw2;
	}

	/**
	 * Determine if target is positive infinity
	 * 
	 * @param theNumber target number
	 * @return true if target represents the value for positive infinity
	 */
	static bool
	isPositiveInfinity(double	theNumber)
	{
		return !isNaN(theNumber) && theNumber == s_positiveInfinity;
	}

	/**
	 * Determine if target is negative infinity
	 * 
	 * @param theNumber target number
	 * @return true if target represents the value for negative infinity
	 */
	static bool
	isNegativeInfinity(double	theNumber)
	{
		return !isNaN(theNumber) && theNumber == s_negativeInfinity;
	}

	/**
	 * Determine if target is positive 0.
	 * 
	 * @param theNumber target number
	 * @return true if target represents the value for positive 0.
	 */
	static bool
	isPositiveZero(double	theNumber)
	{
		const NumberUnion	temp = { theNumber };

		return s_positiveZero.dwords.dw1 == temp.dwords.dw1 &&
			   s_positiveZero.dwords.dw2 == temp.dwords.dw2;
	}

	/**
	 * Determine if target is negative 0
	 * 
	 * @param theNumber target number
	 * @return true if target represents the value for negative 0
	 */
	static bool
	isNegativeZero(double	theNumber)
	{
		const NumberUnion	temp = { theNumber };

		return s_negativeZero.dwords.dw1 == temp.dwords.dw1 &&
			   s_negativeZero.dwords.dw2 == temp.dwords.dw2;
	}

	// These can be used to initialize values, but should not
	// be used to do equality comparisons, as == may fail on
	// some platforms.
	//

	/**
	 * Double value that represents "not a number"
	 * 
	 * @return "not a number" value
	 */
	static double
	getNaN()
	{
		return s_NaN.d;
	}

	/**
	 * Double value that represents positive infinity
	 * 
	 * @return positive infinity value
	 */
	static double
	getPositiveInfinity()
	{
		return s_positiveInfinity;
	}

	/**
	 * Double value that represents negative infinity
	 * 
	 * @return negative infinity value
	 */
	static double
	getNegativeInfinity()
	{
		return s_negativeInfinity;
	}

	/**
	 * Compare two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to compare
	 * @param theRHS a number to compare
	 * @return the result of the compare
	 */
	static bool
	equal(
			double	theLHS,
			double	theRHS);

	/**
	 * Compare two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to compare
	 * @param theRHS a number to compare
	 * @return the result of the compare
	 */
	static bool
	notEqual(
			double	theLHS,
			double	theRHS)
	{
		return !equal(theLHS, theRHS);
	}

	/**
	 * Compare two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to compare
	 * @param theRHS a number to compare
	 * @return the result of the compare
	 */
	static bool
	lessThan(
			double	theLHS,
			double	theRHS);

	/**
	 * Compare two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to compare
	 * @param theRHS a number to compare
	 * @return the result of the compare
	 */
	static bool
	lessThanOrEqual(
			double	theLHS,
			double	theRHS);

	/**
	 * Compare two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to compare
	 * @param theRHS a number to compare
	 * @return the result of the compare
	 */
	static bool
	greaterThan(
			double	theLHS,
			double	theRHS);

	/**
	 * Compare two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to compare
	 * @param theRHS a number to compare
	 * @return the result of the compare
	 */
	static bool
	greaterThanOrEqual(
			double	theLHS,
			double	theRHS);

	/**
	 * Add two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to add
	 * @param theRHS a number to add
	 * @return the result of the addition
	 */
	static double
	add(
			double	theLHS,
			double	theRHS);

	/**
	 * Subtract two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to subtract
	 * @param theRHS a number to subtract
	 * @return the result of the subtraction
	 */
	static double
	subtract(
			double	theLHS,
			double	theRHS);

	/**
	 * Multiply two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to multiply
	 * @param theRHS a number to multiply
	 * @return the result of the multiplication
	 */
	static double
	multiply(
			double	theLHS,
			double	theRHS);

	/**
	 * Divide two double values, taking into account
	 * the fact that we must support IEEE 754
	 *
	 * @param theLHS a number to divide
	 * @param theRHS a number to divide
	 * @return the result of the division
	 */
	static double
	divide(
			double	theLHS,
			double	theRHS);

	/**
	 * Determine the modulus two double values,
	 * taking into account the fact that we must
	 * support IEEE 754
	 *
	 * @param theLHS a number to divide
	 * @param theRHS a number to divide
	 * @return the result of the modulus
	 */
	static double
	modulus(
			double	theLHS,
			double	theRHS);

	/**
	 * Determine the negative of a double value,
	 * taking into account the fact that we must
	 * support IEEE 754
	 *
	 * @param theDouble a number to negate
	 * @return the result of the negation
	 */
	static double
	negative(double	theDouble);

	// Some functors to do the same thing.  This is for
	// STL integration...
	#if defined(XALAN_NO_STD_NAMESPACE)
	struct equalFunction : public binary_function<const double&, const double&, bool>
	#else
	struct equalFunction : public std::binary_function<const double&, const double&, bool>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return equal(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct notEqualFunction : public binary_function<const double&, const double&, bool>
	#else
	struct notEqualFunction : public std::binary_function<const double&, const double&, bool>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return notEqual(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct lessThanFunction : public binary_function<const double&, const double&, bool>
	#else
	struct lessThanFunction : public std::binary_function<const double&, const double&, bool>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return lessThan(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct lessThanOrEqualFunction : public binary_function<const double&, const double&, bool>
	#else
	struct lessThanOrEqualFunction : public std::binary_function<const double&, const double&, bool>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return lessThanOrEqual(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct greaterThanFunction : public binary_function<const double&, const double&, bool>
	#else
	struct greaterThanFunction : public std::binary_function<const double&, const double&, bool>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return greaterThan(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct greaterThanOrEqualFunction : public binary_function<const double&, const double&, bool>
	#else
	struct greaterThanOrEqualFunction : public std::binary_function<const double&, const double&, bool>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return greaterThanOrEqual(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct addFunction : public binary_function<const double&, const double&, double>
	#else
	struct addFunction : public std::binary_function<const double&, const double&, double>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return add(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct subtractFunction : public binary_function<const double&, const double&, double>
	#else
	struct subtractFunction : public std::binary_function<const double&, const double&, double>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return subtract(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct multiplyFunction : public binary_function<const double&, const double&, double>
	#else
	struct multiplyFunction : public std::binary_function<const double&, const double&, double>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return multiply(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct divideFunction : public binary_function<const double&, const double&, double>
	#else
	struct divideFunction : public std::binary_function<const double&, const double&, double>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return divide(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct modulusFunction : public binary_function<const double&, const double&, double>
	#else
	struct modulusFunction : public std::binary_function<const double&, const double&, double>
	#endif
	{
		result_type
		operator()(
			first_argument_type		theLHS,
			second_argument_type	theRHS) const
		{
			return modulus(theLHS, theRHS);
		}
	};

	#if defined(XALAN_NO_STD_NAMESPACE)
	struct negativeFunction : public unary_function<const double&, double>
	#else
	struct negativeFunction : public std::unary_function<const double&, double>
	#endif
	{
		result_type
		operator()(argument_type	theDouble) const
		{
			return negative(theDouble);
		}
	};

	/**
	 * Determine whether or not a string contains
	 * a valid floating point number.
	 *
	 * @param theString The string to check.
	 * @return true if the string is valid, false if not.
	 */
	static bool
	isValid(const XalanDOMString&	theString);

	/**
	 * Determine whether or not a string contains
	 * a valid floating point number.
	 *
	 * @param theString The string to check.
	 * @return true if the string is valid, false if not.
	 */
	static bool
	isValid(const XalanDOMChar*		theString);

	/**
	 * Convert a string to a double value.  Returns
	 * NaN if the string is not a valid floating
	 * point number.
	 *
	 * @param theString The string to convert.
	 * @return The result of the conversion
	 */
	static double
	toDouble(const XalanDOMString&	theString);

	/**
	 * Convert a string to a double value.  Returns
	 * NaN if the string is not a valid floating
	 * point number.
	 *
	 * @param theString The string to convert.
	 * @return The result of the conversion
	 */
	static double
	toDouble(const XalanDOMChar*	theString);

	/**
	 * Round a number according to the XPath
	 * rules.
	 *
	 * @param theValue The value to round.
	 * @return The result of the rounding
	 */
	static double
	round(double	theValue);

	/**
	 * Returns the ceiling of a number according to the XPath
	 * rules.
	 *
	 * @param theValue The value to round.
	 * @return The result of the rounding
	 */
	static double
	ceiling(double	theValue)
	{
#if defined(XALAN_STRICT_ANSI_HEADERS)
		return std::ceil(theValue);
#else
		return ceil(theValue);
#endif
	}

	/**
	 * Returns the floor of a number according to the XPath
	 * rules.
	 *
	 * @param theValue The value to round.
	 * @return The result of the rounding
	 */
	static double
	floor(double	theValue)
	{
#if defined(XALAN_STRICT_ANSI_HEADERS)
		return std::floor(theValue);
#else
		return ::floor(theValue);
#endif
	}

	typedef union
	{
		double	d;
		struct
		{
			unsigned int	dw1;
			unsigned int	dw2;
		} dwords;
	} NumberUnion;

private:

	static const NumberUnion	s_NaN;
	static const double			s_positiveInfinity;
	static const double			s_negativeInfinity;
	static const NumberUnion	s_positiveZero;
	static const NumberUnion	s_negativeZero;
};



XALAN_CPP_NAMESPACE_END



#endif	// DOUBLESUPPORT_HEADER_GUARD_1357924680
