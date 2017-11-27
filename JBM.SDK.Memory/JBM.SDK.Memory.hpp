////////////////////////////////////////////////////////////////////////////////////
//// JBM.SDK.Memory.hpp - Main include file for the JBM.SDK.Memory
//// C++ library. This library is part of the JBM.SDK set of libraries
//// All libraries in the JBM.SDK set are under exclusive copyright of the author
//// James Bernard Schumacher III
//// (C) 2017 James Bernard Schumacher III
//// And also by the no-license default, which is the exclusive copyright of myself
//// James.
////////////////////////////////////////////////////////////////////////////////////

#ifndef __JBM_SDK_MEMORY_HPP__
#define __JBM_SDK_MEMORY_HPP__

#if defined(JBM_SDK_MEMORY_BUILD)

	#define JBM_SDK_MEMORY_EXPORT __declspec(dllexport)

#else

	#define JBM_SDK_MEMORY_EXPORT __declspec(dllimport)

#endif

// Define the macros to export/import classes and functions
// from the *.dll

#define JBM_SDK_MEMORY_CLASS		JBM_SDK_MEMORY_EXPORT
#define JBM_SDK_MEMORY_FUNCTION		JBM_SDK_MEMORY_EXPORT

// Disable MSVC++ warnings
#pragma warning(disable:4290)

#include <pshpack1.h>

namespace JBM
{
	// Typedef the basic types for the JBM.SDK set of libraries

	typedef char AnsiChar, Int8;
	typedef unsigned char Byte, UInt8;
	typedef short Short16, Int16, Short;
	typedef unsigned short UShort16, UShort, UInt16;
	typedef long Long32, Long, Int32;
	typedef unsigned long ULong32, ULong, UInt32;
	typedef int Integer;
	typedef unsigned int UInteger;
	typedef long long Long64, LongLong, Int64;
	typedef unsigned long long ULong64, ULongLong, UInt64;

	typedef float FPValue32, Float32, Float;
	typedef double FPValue64, Double64, Double;

	typedef wchar_t UniChar, UnicodeChar;

#if defined(JBM_SDK_PLATFORM_64BITS)
	typedef UInt64 SizeType;
	typedef Int64 SignedSizeType;

	#define JBM_SDK_CALL __fastcall

#elif defined(JBM_SDK_PLATFORM_32BITS)
	typedef UInteger SizeType;
	typedef Integer SignedSizeType;

	#define JBM_SDK_CALL __stdcall
#endif

	typedef void (JBM_SDK_CALL * PFN_DestructorStubFunction)(void * lpObject, SizeType nObjectCount);

	enum AllocationType : UShort16
	{
		AllocationTypeDefaultValue = 0,
		AllocationTypeGeneral = 1,
		AllocationTypeValue = 2,
		AllocationTypeValueArray = 3,
		AllocationTypeObject = 4,
		AllocationTypeObjectArray = 5,
		AllocationTypeHandle = 6,
		AllocationTypeHandleArray = 7
	};

	struct JBM_SDK_MEMORY_CLASS SourceCodeInfo
	{
	public:
		SourceCodeInfo() throw();
		SourceCodeInfo(const AnsiChar * pstrFunctionName, const AnsiChar * pstrFileName,
			SizeType nLineNumber) throw();
		SourceCodeInfo(const SourceCodeInfo & info) throw();
		SourceCodeInfo(SourceCodeInfo && info) throw();
		~SourceCodeInfo() throw();

		inline const AnsiChar * GetFunctionName() const throw() { return m_pstrFunctionName; }
		inline const AnsiChar * GetFileName() const throw() { return m_pstrFileName; }
		inline SizeType GetLineNumber() const throw() { return m_nLineNumber; }

		inline void SetFunctionName(const AnsiChar * pstrFunctionName) throw() { m_pstrFunctionName = pstrFunctionName; }
		inline void SetFileName(const AnsiChar * pstrFileName) throw() { m_pstrFileName = pstrFileName; }
		inline void SetLineNumber(SizeType nLineNumber) throw() { m_nLineNumber = nLineNumber; }

		const SourceCodeInfo & operator= (const SourceCodeInfo & info) throw();
		bool operator== (const SourceCodeInfo & info) const throw();
		bool operator!= (const SourceCodeInfo & info) const throw();

	protected:
		// Member Variables
		const AnsiChar * m_pstrFunctionName;
		const AnsiChar * m_pstrFileName;
		SizeType m_nLineNumber;
	};

	// Forward declare IObject

	class IObject;

	// Forward declare IBaseAllocator

	class IBaseAllocator;

	class JBM_SDK_MEMORY_CLASS CAllocationNode
	{
	public:
		CAllocationNode() throw();
		CAllocationNode(void * lpAddress, SizeType nBufferSize,
			AllocationType enType, SizeType nObjectSize, SizeType nObjectCount,
			PFN_DestructorStubFunction pfnDestructor,
			const SourceCodeInfo * pSourceInfo,
			IBaseAllocator * pAllocator) throw();
		virtual ~CAllocationNode() throw();

		inline void * GetBufferAddress() const throw() { return m_lpBufferAddress; }
		inline SizeType GetAllocationSize() const throw() { return m_nBufferSize; }
		inline AllocationType GetAllocationType() const throw() { return m_enAllocationType; }
		inline SizeType GetObjectSize() const throw() { return m_nObjectSize; }
		inline SizeType GetObjectCount() const throw() { return m_nObjectCount; }
		inline PFN_DestructorStubFunction GetDestructor() const throw() { return m_pfnDestructor; }
		inline SourceCodeInfo GetSourceInfo() const throw() { return m_udtSourceInfo; }
		inline IBaseAllocator * GetAllocator() const throw() { return m_pAllocator; }
		inline CAllocationNode * GetNextAllocation() const throw() { return m_pNextAllocation; }
		inline CAllocationNode * GetPreviousAllocation() const throw() { return m_pPreviousAllocation; }

		// PLEASE NOTE: These just set the value of the variable in the node,
		// IT DOES NOT CHECK TO SEE IF IT ALREADY POINTS AT ANOTHER NODE
		inline void SetNextAllocation(CAllocationNode * pNode) throw() { m_pNextAllocation = pNode; }
		inline void SetPreviousAllocation(CAllocationNode * pNode) throw() { m_pPreviousAllocation = pNode; }
	protected:
		void * m_lpBufferAddress;
		SizeType m_nBufferSize;
		AllocationType m_enAllocationType;
		SizeType m_nObjectSize;
		SizeType m_nObjectCount;
		PFN_DestructorStubFunction m_pfnDestructor;
		SourceCodeInfo m_udtSourceInfo;

		IBaseAllocator * m_pAllocator;

		CAllocationNode * m_pNextAllocation;
		CAllocationNode * m_pPreviousAllocation;

	private:
		// Prevent usage, empty implementations in source file
		CAllocationNode(const CAllocationNode & node) throw();
		void operator= (const CAllocationNode & node) throw();
	};

	class JBM_SDK_MEMORY_CLASS IObject
	{
	public:
		virtual ~IObject() throw();

		virtual void DeleteThisObject() throw() = 0;
	};

	class JBM_SDK_MEMORY_CLASS IBaseAllocator : public IObject
	{
	public:
		virtual ~IBaseAllocator() throw();

		virtual void * BasicAllocate(SizeType nAllocSize, bool bZeroMemory) throw() = 0;
		virtual bool BasicFree(void * lpBuffer) throw() = 0;

		virtual CAllocationNode * AllocateValue(SizeType nValueSize, const SourceCodeInfo * pSourceInfo) throw() = 0;
		virtual CAllocationNode * AllocateValueArray(SizeType nObjectSize, SizeType nObjectCount, const SourceCodeInfo * pSourceInfo) throw() = 0;
		virtual CAllocationNode * AllocateObject(SizeType nObjectSize, PFN_DestructorStubFunction pfnDestructor, const SourceCodeInfo * pSourceInfo) throw() = 0;
		virtual CAllocationNode * AllocateObjectArray(SizeType nObjectSize, SizeType nObjectCount, PFN_DestructorStubFunction pfnDestructor, const SourceCodeInfo * pInfo) throw() = 0;

		virtual void AddNode(CAllocationNode * pNode) throw() = 0;
		virtual void RemoveNode(CAllocationNode * pNode) throw() = 0;

		inline void DeleteNode(CAllocationNode * pNode) throw()
		{
			RemoveNode(pNode);
			BasicFree(pNode);
		}

		static inline CAllocationNode * BufferToNode(void * lpBuffer) throw()
		{
			CAllocationNode * pNode = reinterpret_cast<CAllocationNode *>(lpBuffer);
			--pNode;

			return pNode;
		}

		static inline void * NodeToBuffer(CAllocationNode * pNode) throw()
		{
			return pNode + 1;
		}
	};

	class JBM_SDK_MEMORY_CLASS CException : public IObject
	{
	public:
		CException() throw();
		CException(const AnsiChar * pstrFunctionName, const AnsiChar * pstrFileName, SizeType nLineNumber,
			SizeType nErrorCode) throw();
		CException(const SourceCodeInfo & udtInfo, SizeType nErrorCode) throw();
		CException(const CException & ex) throw();
		CException(CException && ex) throw();
		virtual ~CException() throw();

		// IObject
		virtual void DeleteThisObject() throw();

		// Member Functions
		inline const SourceCodeInfo & GetSourceInfo() const throw() { return m_udtSourceInfo; }
		inline SizeType GetErrorCode() const throw() { return m_nErrorCode; }

		const CException & operator= (const CException & ex) throw();

	protected:
		SourceCodeInfo m_udtSourceInfo;
		SizeType m_nErrorCode;
	};

	class JBM_SDK_MEMORY_CLASS CBaseAllocator : public IBaseAllocator
	{
	public:
		// Construction/Destruction
		CBaseAllocator() throw();
		CBaseAllocator(void * hHeapHandle) throw();
		virtual ~CBaseAllocator() throw();

		// IObject
		virtual void DeleteThisObject() throw();

		// IBaseAllocator
		virtual void * BasicAllocate(SizeType nAllocSize, bool bZeroMemory) throw();
		virtual bool BasicFree(void * lpBuffer) throw();

		virtual CAllocationNode * AllocateValue(SizeType nValueSize, const SourceCodeInfo * pSourceInfo) throw();
		virtual CAllocationNode * AllocateValueArray(SizeType nObjectSize, SizeType nObjectCount, const SourceCodeInfo * pSourceInfo) throw();
		virtual CAllocationNode * AllocateObject(SizeType nObjectSize, PFN_DestructorStubFunction pfnDestructor, const SourceCodeInfo * pSourceInfo) throw();
		virtual CAllocationNode * AllocateObjectArray(SizeType nObjectSize, SizeType nObjectCount, PFN_DestructorStubFunction pfnDestructor, const SourceCodeInfo * pInfo) throw();

		virtual void AddNode(CAllocationNode * pNode) throw();
		virtual void RemoveNode(CAllocationNode * pNode) throw();
	protected:
		CAllocationNode * m_pFirstAllocation;
		CAllocationNode * m_pLastAllocation;
		SizeType m_nAllocationCount;

		void * m_hHeapHandle;

	private:
		// Prevent usage (empty implementation)
		CBaseAllocator(const CBaseAllocator &) throw();
		void operator= (const CBaseAllocator & alloc) throw();
	};
}

inline void * operator new(JBM::SizeType nSize, void * lpAddress) throw()
{
	return lpAddress;
}

inline void operator delete(void * lpBuffer, void * lpAddress) throw()
{
}

inline void * operator new[](JBM::SizeType nSize, void * lpAddress) throw()
{
	return lpAddress;
}

inline void operator delete[](void * lpBuffer, void * lpAddress) throw()
{
}

template <typename Type> Type * NewValue(JBM::IBaseAllocator * pAllocator) throw(JBM::CException)
{
	SourceCodeInfo udtInfo(__FUNCTION__, __FILE__, __LINE__);

	CAllocationNode * pNode = pAllocator->AllocateValue(sizeof(Type), &udtInfo);

	if (pNode != nullptr)
	{
		Type * pObject = reinterpret_cast<Type *>(pNode->GetBufferAddress());

		new (pObject) Type();

		return pObject;
	}
	else
	{
		return nullptr;
	}
}

template <typename Type> Type * NewValue(JBM::IBaseAllocator * pAllocator, const Type & value) throw()
{
	SourceCodeInfo udtInfo(__FUNCTION__, __FILE__, __LINE__);

	CAllocationNode * pNode = pAllocator->AllocateValue(sizeof(Type), &udtInfo);

	if (pNode != nullptr)
	{
		Type * pObject = reinterpret_cast<Type *>(pNode->GetBufferAddress());

		new (pObject) Type(value);

		return pObject;
	}
	else
	{
		return nullptr;
	}
}

template <typename Type> Type * NewValueArray(JBM::IBaseAllocator * pAllocator, JBM::SizeType nCount) throw()
{
	SourceCodeInfo udtInfo(__FUNCTION__, __FILE__, __LINE__);

	CAllocationNode * pNode = pAllocator->AllocateValueArray(sizeof(Type), nCount, &udtInfo);

	if (pNode != nullptr)
	{
		Type * pObject = reinterpret_cast<Type *>(pNode->GetBufferAddress());

		new (pObject) Type(value);

		return pObject;
	}
	else
	{
		return nullptr;
	}
}

template <typename Type> Type * NewValueArrayInitialize(JBM::IBaseAllocator * pAllocator, JBM::SizeType nCount, const Type * pSourceArray) throw()
{
	SourceCodeInfo udtInfo(__FUNCTION__, __FILE__, __LINE__);

	CAllocationNode * pNode = pAllocator->AllocateValueArray(sizeof(Type), nCount, &udtInfo);

	if (pNode != nullptr)
	{
		Type * pObjectArray = reinterpret_cast<Type *>(pNode->GetBufferAddress());

		for (SizeType x = 0; x < nCount; ++x)
		{
			new (&pObjectArray[x]) Type(pSourceArray[x]);
		}

		return pObjectArray;
	}
	else
	{
		return nullptr;
	}
}

template <typename Type> void JBM_SDK_CALL DestructorStubFunction(void * lpBuffer, JBM::SizeType nObjectCount) throw()
{
	Type * pObjectArray = reinterpret_cast<Type *>(lpBuffer);

	if (nObjectCount > 1)
	{
		for (SizeType x = nObjectCount; x != 0; --x)
		{
			pObjectArray[x - 1].~Type();
		}
	}
	else if (nObjectCount == 1)
	{
		pObjectArray[0].~Type();
	}
}

template <typename Type> Type * NewObject(JBM::IBaseAllocator * pAllocator, const Type & value) throw()
{
	SourceCodeInfo udtInfo(__FUNCTION__, __FILE__, __LINE__);

	CAllocationNode * pNode = pAllocator->AllocateObject(sizeof(Type), &DestructorStubFunction<Type>, &udtInfo);

	if (pNode != nullptr)
	{
		Type * pObject = reinterpret_cast<Type *>(pNode->GetBufferAddress());

		new (pObject) Type(value);

		return pObject;
	}
	else
	{
		return nullptr;
	}
}

template <typename Type> Type * NewObjectArray(JBM::IBaseAllocator * pAllocator, JBM::SizeType nCount) throw()
{
	SourceCodeInfo udtInfo(__FUNCTION__, __FILE__, __LINE__);

	CAllocationNode * pNode = pAllocator->AllocateObjectArray(sizeof(Type), nCount, &DestructorStubFunction<Type>, &udtInfo);

	if (pNode != nullptr)
	{
		Type * pObjectArray = reinterpret_cast<Type *>(pNode->GetBufferAddress());

		for (SizeType x = 0; x < nCount; ++x)
		{
			new (&pObjectArray[x]) Type();
		}

		return pObjectArray;
	}
	else
	{
		return nullptr;
	}
}

template <typename Type> Type * NewObjectArrayInitialize(JBM::IBaseAllocator * pAllocator, JBM::SizeType nCount, const Type * pSourceArray) throw()
{
	SourceCodeInfo udtInfo(__FUNCTION__, __FILE__, __LINE__);

	CAllocationNode * pNode = pAllocator->AllocateObjectArray(sizeof(Type), nCount, &DestructorStubFunction<Type>, &udtInfo);

	if (pNode != nullptr)
	{
		Type * pObjectArray = reinterpret_cast<Type *>(pNode->GetBufferAddress());

		for (SizeType x = 0; x < nCount; ++x)
		{
			new (&pObjectArray[x]) Type(pSourceArray[x]);
		}

		return pObjectArray;
	}
	else
	{
		return nullptr;
	}
}

template <typename Type> void Delete(Type * pObject) throw()
{
	if (pObject != nullptr)
	{
		CAllocationNode * pNode = IBaseAllocator::BufferToNode(pObject);

		IBaseAllocator * pAllocator = pNode->GetAllocator();

		pAllocator->RemoveNode(pNode);

		AllocationType enType = pNode->GetAllocationType();

		if (enType == AllocationType::AllocationTypeObject || enType == AllocationType::AllocationTypeObjectArray)
		{
			PFN_DestructorStubFunction pfnDestructor = pNode->GetDestructor();

			if (pfnDestructor != nullptr)
			{
				(*pfnDestructor)(pNode->GetBufferAddress(), pNode->GetObjectCount());
			}
		}

		pAllocator->BasicFree(pNode);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// First upload of source
/// James Bernard Schumacher III
/// 11/26/2017 - in Augora Hills, CA at Von's (while I am stil homeless) trying to make my life more positive! (started on this version today)
/// REASON: Early upload, have to use the restroom!

#include <poppack.h>

#endif