////////////////////////////////////////////////////////////////////////////////////
//// CAllocationNode.cpp - Source code file for the JBM.CAllocationNode class
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
	CAllocationNode::CAllocationNode() throw() : m_lpBufferAddress(nullptr),
		m_nBufferSize(0), m_enAllocationType(AllocationType::AllocationTypeDefaultValue),
		m_nObjectSize(0), m_nObjectCount(0), m_pfnDestructor(nullptr), m_udtSourceInfo(),
		m_pAllocator(nullptr), m_pNextAllocation(nullptr), m_pPreviousAllocation(nullptr)
	{
	}

	CAllocationNode::CAllocationNode(void * lpAddress, SizeType nAllocationSize,
		AllocationType enAllocationType, SizeType nObjectSize, SizeType nObjectCount, PFN_DestructorStubFunction pfnFunction,
		const SourceCodeInfo * pSourceInfo, IBaseAllocator * pAllocator) throw() : m_lpBufferAddress(lpAddress),
		m_nBufferSize(nAllocationSize), m_enAllocationType(enAllocationType),
		m_nObjectSize(nObjectSize), m_nObjectCount(nObjectCount),
		m_pfnDestructor(pfnFunction), m_udtSourceInfo(), m_pAllocator(pAllocator),
		m_pNextAllocation(nullptr), m_pPreviousAllocation(nullptr)
	{
	}

	CAllocationNode::~CAllocationNode() throw()
	{
	}

	CAllocationNode::CAllocationNode(const CAllocationNode & node) throw()
	{
	}

	void CAllocationNode::operator= (const CAllocationNode & node) throw()
	{
	}
}