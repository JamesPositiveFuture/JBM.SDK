////////////////////////////////////////////////////////////////////////////////////
//// CBaseAllocator.cpp - Source code file for the JBM.CBaseAllocator class
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
	CBaseAllocator::CBaseAllocator() throw() : m_pFirstAllocation(nullptr), m_pLastAllocation(nullptr), m_nAllocationCount(0), m_hHeapHandle(::GetProcessHeap())
	{
	}

	CBaseAllocator::CBaseAllocator(void * hHeapHandle) throw() : m_pFirstAllocation(nullptr), m_pLastAllocation(nullptr), m_nAllocationCount(0),
		m_hHeapHandle(hHeapHandle)
	{
	}

	CBaseAllocator::~CBaseAllocator() throw()
	{
		CAllocationNode * pCurrentNode = m_pLastAllocation;
		CAllocationNode * pPreviousNode = nullptr;

		while (pCurrentNode != nullptr)
		{
			pPreviousNode = pCurrentNode->GetPreviousAllocation();

			RemoveNode(pCurrentNode);

			AllocationType enAllocType = pCurrentNode->GetAllocationType();

			if (enAllocType == AllocationType::AllocationTypeObject || enAllocType == AllocationType::AllocationTypeObjectArray)
			{
				PFN_DestructorStubFunction pfnDestructor = pCurrentNode->GetDestructor();

				if (pfnDestructor != nullptr)
				{
					(*pfnDestructor)(pCurrentNode->GetBufferAddress(), pCurrentNode->GetObjectCount());
				}
			}

			BasicFree(pCurrentNode);

			pCurrentNode = pPreviousNode;
		}
	}

	void CBaseAllocator::DeleteThisObject() throw()
	{
		delete this;
	}

	void * CBaseAllocator::BasicAllocate(SizeType nAllocSize, bool bZeroMemory) throw()
	{
		const ULong32 dwFlags = bZeroMemory == true ? HEAP_ZERO_MEMORY : 0;

		return ::HeapAlloc(m_hHeapHandle, dwFlags, nAllocSize);
	}

	bool CBaseAllocator::BasicFree(void * lpBuffer) throw()
	{
		return ::HeapFree(m_hHeapHandle, 0, lpBuffer) != 0;
	}

	CAllocationNode * CBaseAllocator::AllocateValue(SizeType nValueSize, const SourceCodeInfo * pSourceInfo) throw()
	{
		CAllocationNode * pNode = nullptr;

		const SizeType nAllocSize = nValueSize + sizeof(CAllocationNode);

		void * lpBuffer = BasicAllocate(nAllocSize, true);

		if (lpBuffer != nullptr)
		{
			pNode = reinterpret_cast<CAllocationNode *>(lpBuffer);

			lpBuffer = pNode + 1;

			new (reinterpret_cast<void *>(pNode)) CAllocationNode(lpBuffer, nValueSize, AllocationType::AllocationTypeValue, nValueSize, 1, nullptr, pSourceInfo, this);

			AddNode(pNode);
		}

		return pNode;
	}

	CAllocationNode * CBaseAllocator::AllocateValueArray(SizeType nValueSize, SizeType nValueCount, const SourceCodeInfo * pSourceInfo) throw()
	{
		CAllocationNode * pNode = nullptr;

		const SizeType nArraySize = nValueCount * nValueSize;
		const SizeType nAllocSize = nValueSize + nArraySize;

		void * lpBuffer = BasicAllocate(nAllocSize, true);

		if (lpBuffer != nullptr)
		{
			pNode = reinterpret_cast<CAllocationNode *>(lpBuffer);

			lpBuffer = pNode + 1;

			new (reinterpret_cast<void *>(pNode)) CAllocationNode(lpBuffer, nArraySize, AllocationType::AllocationTypeValueArray, nValueSize, nValueCount, nullptr, pSourceInfo, this);

			AddNode(pNode);
		}

		return pNode;
	}

	CAllocationNode * CBaseAllocator::AllocateObject(SizeType nValueSize, PFN_DestructorStubFunction pfnDestructor, const SourceCodeInfo * pSourceInfo) throw()
	{
		CAllocationNode * pNode = nullptr;

		const SizeType nAllocSize = nValueSize + sizeof(CAllocationNode);

		void * lpBuffer = BasicAllocate(nAllocSize, true);

		if (lpBuffer != nullptr)
		{
			pNode = reinterpret_cast<CAllocationNode *>(lpBuffer);

			lpBuffer = pNode + 1;

			new (pNode) CAllocationNode(lpBuffer, nValueSize, AllocationType::AllocationTypeObject, nValueSize, 1, pfnDestructor, pSourceInfo, this);

			AddNode(pNode);
		}

		return pNode;
	}

	CAllocationNode * CBaseAllocator::AllocateObjectArray(SizeType nValueSize, SizeType nValueCount, PFN_DestructorStubFunction pfnDestructor, const SourceCodeInfo * pSourceInfo) throw()
	{
		CAllocationNode * pNode = nullptr;

		const SizeType nArraySize = nValueCount * nValueSize;
		const SizeType nAllocSize = nValueSize + nArraySize;

		void * lpBuffer = BasicAllocate(nAllocSize, true);

		if (lpBuffer != nullptr)
		{
			pNode = reinterpret_cast<CAllocationNode *>(lpBuffer);

			lpBuffer = pNode + 1;

			new (reinterpret_cast<void *>(pNode)) CAllocationNode(lpBuffer, nArraySize, AllocationType::AllocationTypeObjectArray, nValueSize, nValueCount, nullptr, pSourceInfo, this);

			AddNode(pNode);
		}

		return pNode;
	}

	void CBaseAllocator::AddNode(CAllocationNode * pNode) throw()
	{
		if (pNode != nullptr)
		{
			if (m_pLastAllocation != nullptr)
			{
				m_pLastAllocation->SetNextAllocation(pNode);
					pNode->SetPreviousAllocation(m_pLastAllocation);
			}

			if (m_pFirstAllocation == nullptr)
			{
				m_pFirstAllocation = pNode;
				m_pLastAllocation = pNode;
			}
		}
	}

	void CBaseAllocator::RemoveNode(CAllocationNode * pNode) throw()
	{
		if (pNode != nullptr)
		{
			IBaseAllocator * pAllocator = pNode->GetAllocator();

			if (pAllocator == this)
			{
				CAllocationNode * pPrevNode = pNode->GetPreviousAllocation();
				CAllocationNode * pNextNode = pNode->GetNextAllocation();

				if (pPrevNode != nullptr)
				{
					pPrevNode->SetNextAllocation(pNextNode);
				}
				else
				{
					m_pFirstAllocation = pNextNode;
				}

				if (pNextNode != nullptr)
				{
					pNextNode->SetPreviousAllocation(pPrevNode);
				}
				else
				{
					m_pLastAllocation = pPrevNode;
				}

				--m_nAllocationCount;
			}
		}
	}

	// Empty implementation

	CBaseAllocator::CBaseAllocator(const CBaseAllocator & alloc) throw()
	{
	}

	void CBaseAllocator::operator= (const CBaseAllocator & alloc) throw()
	{
	}
}