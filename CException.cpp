////////////////////////////////////////////////////////////////////////////////////
//// CException.cpp - Source code file for the JBM.CException class
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
	CException::CException() throw() : m_udtSourceInfo(), m_nErrorCode(0)
	{
	}

	CException::CException(const AnsiChar * pstrFunctionName, const AnsiChar * pstrFileName, SizeType nLineNumber,
		SizeType nErrorCode) throw() : m_udtSourceInfo(pstrFunctionName, pstrFileName, nLineNumber), m_nErrorCode(nErrorCode)
	{
	}

	CException::CException(const SourceCodeInfo & udtSourceInfo, SizeType nErrorCode) throw() : m_udtSourceInfo(udtSourceInfo),
		m_nErrorCode(nErrorCode)
	{
	}

	CException::CException(const CException & ex) throw() : m_udtSourceInfo(ex.m_udtSourceInfo), m_nErrorCode(ex.m_nErrorCode)
	{
	}

	CException::CException(CException && ex) throw() : m_udtSourceInfo(static_cast<SourceCodeInfo &&>(ex.m_udtSourceInfo)), m_nErrorCode(ex.m_nErrorCode)
	{
	}

	CException::~CException() throw()
	{
	}

	void CException::DeleteThisObject() throw()
	{
		delete this;
	}

	const CException & CException::operator= (const CException & ex) throw()
	{
		if (this != &ex)
		{
			m_udtSourceInfo = ex.m_udtSourceInfo;
			m_nErrorCode = ex.m_nErrorCode;
		}

		return *this;
	}
}