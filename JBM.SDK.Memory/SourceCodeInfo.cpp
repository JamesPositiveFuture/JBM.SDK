////////////////////////////////////////////////////////////////////////////////////
//// SourceCodeInfo.cpp - Source code file for the JBM.SourceCodeInfo class
//// It is part of the JBM.SDK.Memory C++ library.
//// This library is part of the JBM.SDK set of libraries
//// All libraries in the JBM.SDK set are under exclusive copyright of the author
//// James Bernard Schumacher III
//// (C) 2017 James Bernard Schumacher III
//// And also by the no-license default, which is the exclusive copyright of myself
//// James.
////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JBM.SDK.Memory.hpp"

namespace JBM
{
	SourceCodeInfo::SourceCodeInfo() throw() : m_pstrFunctionName(nullptr), m_pstrFileName(nullptr), m_nLineNumber(0)
	{
	}

	SourceCodeInfo::SourceCodeInfo(const AnsiChar * pstrFunctionName, const AnsiChar * pstrFileName, SizeType nLineNumber) throw() : m_pstrFunctionName(pstrFunctionName),
		m_pstrFileName(pstrFileName), m_nLineNumber(nLineNumber)
	{
	}

	SourceCodeInfo::SourceCodeInfo(const SourceCodeInfo & info) throw() : m_pstrFunctionName(info.m_pstrFunctionName),
		m_pstrFileName(info.m_pstrFileName), m_nLineNumber(info.m_nLineNumber)
	{
	}

	SourceCodeInfo::~SourceCodeInfo() throw()
	{
	}

	const SourceCodeInfo & SourceCodeInfo::operator= (const SourceCodeInfo & info) throw()
	{
		if (this != &info)
		{
			m_pstrFunctionName = info.m_pstrFunctionName;
			m_pstrFileName = info.m_pstrFileName;
			m_nLineNumber = info.m_nLineNumber;
		}

		return *this;
	}

	bool SourceCodeInfo::operator== (const SourceCodeInfo & info) const throw()
	{
		if (this != &info)
		{
			return m_pstrFunctionName == info.m_pstrFunctionName &&
				(m_pstrFileName == info.m_pstrFileName && m_nLineNumber == info.m_nLineNumber);
		}
		else
		{
			return true;
		}
	}

	bool SourceCodeInfo::operator!= (const SourceCodeInfo & info) const throw()
	{
		if (this != &info)
		{
			return m_pstrFunctionName != info.m_pstrFunctionName || m_pstrFileName != info.m_pstrFileName ||
				m_nLineNumber != info.m_nLineNumber;
		}
		else
		{
			return false;
		}
	}
}