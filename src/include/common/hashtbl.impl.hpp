
// common_hashtbl.impl.hpp
// 2017 Jul 21

#ifndef common_hashtbl_impl_hpp
#define common_hashtbl_impl_hpp

#include <stdlib.h>
#include <memory.h>

#ifndef common_hashtbl_hpp
//#error do not include this header directly
#include "common_hashtbl.hpp"
#endif

#ifdef __APPLE__
#define _REGISTER
#else
//#define _REGISTER   register
#define _REGISTER   
#endif

#ifndef FALLTHROUGH2
#define FALLTHROUGH2	[[fallthrough]];
#endif

/* The mixing step */
#define mix2(a,b,c) \
{ \
  a=a-b;  a=a-c;  a=a^(c>>13); \
  b=b-c;  b=b-a;  b=b^(a<<8);  \
  c=c-a;  c=c-b;  c=c^(b>>13); \
  a=a-b;  a=a-c;  a=a^(c>>12); \
  b=b-c;  b=b-a;  b=b^(a<<16); \
  c=c-a;  c=c-b;  c=c^(b>>5);  \
  a=a-b;  a=a-c;  a=a^(c>>3);  \
  b=b-c;  b=b-a;  b=b^(a<<10); \
  c=c-a;  c=c-b;  c=c^(b>>15); \
}

namespace common { namespace hashFncs{

static inline size_t hash1_(const void* key, size_t keySize);

}}

template <typename DataType>
common::HashTbl<DataType>::HashTbl(size_t a_tInitSize)
	:
	m_fpHashFnc(&hashFncs::hash1_)
{
	size_t i(0);
	size_t tRet(a_tInitSize);

	if (!a_tInitSize) { tRet = DEFAULT_TABLE_SIZE; goto prepareHashTable; }
	for (; tRet; tRet = (a_tInitSize >> ++i));
	tRet = ((size_t)1) << (i - 1);
	if (tRet != a_tInitSize){tRet <<= 1;}

prepareHashTable:
	m_unRoundedTableSizeMin1 = tRet-1;
	m_pTable = (HashItem**)calloc(tRet,sizeof(HashItem*));
	if(!m_pTable){throw "Low memory!";}
}


template <typename DataType>
common::HashTbl<DataType>::~HashTbl()
{
	ClearAll();
	free(m_pTable);
}


template <typename DataType>
int common::HashTbl<DataType>::AddEntry(const void* a_key, size_t a_nKeyLen, const DataType& a_data)
{
	try {
		size_t unHash(((*m_fpHashFnc)(a_key, a_nKeyLen))&m_unRoundedTableSizeMin1);
		HashItem *pItem = new HashItem(a_key, a_nKeyLen, a_data,m_pTable[unHash]);

		//if (!pItem) { return -1; } // exception will be thrown
		//pItem->next = m_pTable[unHash];  // done in the constructor
		if(m_pTable[unHash]){m_pTable[unHash]->prev = pItem;}
		m_pTable[unHash] = pItem;
		return 0;
	}
	catch (...)
	{
	}

	return -2;
}


template <typename DataType>
bool common::HashTbl<DataType>::RemoveEntry(const void* a_key, size_t a_nKeyLen)
{
	HashItem *pItem;
	size_t unHash(((*m_fpHashFnc)(a_key, a_nKeyLen))&m_unRoundedTableSizeMin1);
	
	pItem = m_pTable[unHash];
	
	// let's check very first item independendently
	if(pItem && (a_nKeyLen==pItem->keySize)&&(memcmp(pItem->key,a_key,a_nKeyLen)==0)){
		m_pTable[unHash]=pItem->next;
		if(pItem->next){pItem->next->prev = pItem->prev;}
		delete pItem;
		return true;
	}

	while(pItem){
		if((a_nKeyLen==pItem->keySize)&&(memcmp(pItem->key,a_key,a_nKeyLen)==0)){
			if(pItem->prev){pItem->prev->next = pItem->next;}
			if(pItem->next){pItem->next->prev = pItem->prev;}
			delete pItem;
			return true;
		}
		pItem = pItem->next;
	}

	return false;
}


template <typename DataType>
bool common::HashTbl<DataType>::FindEntry(const void* a_key, size_t a_nKeyLen, DataType* a_pData)
{
	HashItem *pItem;
	size_t unHash(((*m_fpHashFnc)(a_key, a_nKeyLen))&m_unRoundedTableSizeMin1);

	pItem = m_pTable[unHash];

	while (pItem) {
		if ((a_nKeyLen == pItem->keySize) && (memcmp(pItem->key, a_key, a_nKeyLen) == 0)) {
			*a_pData = pItem->data;
			return true;
		}
		pItem = pItem->next;
	}

	return false;
}


template <typename DataType>
void common::HashTbl<DataType>::ClearAll()
{
	HashItem *pItem, *pItemTmp;
	const size_t tRet(m_unRoundedTableSizeMin1+1);
	
	for(size_t i(0); i<tRet;++i){
		pItem = m_pTable[i];
		while(pItem){
			pItemTmp = pItem->next;
			delete pItem;
			pItem = pItemTmp;
		}
		m_pTable[i] = NULL;
	}
}


/*/////////////////////////////////////////////////*/
//template <typename DataType1>
//template <typename DataType2>
template <typename DataType>
common::HashTbl<DataType>::HashItem::HashItem(const void* a_key, size_t a_nKeyLen, const DataType& a_data, HashItem* a_pNext)
	:	prev(NULL),next(a_pNext),key(NULL),data(a_data),keySize(a_nKeyLen)
{
	key = malloc(a_nKeyLen);
	if(!key){throw "low memory";}
	memcpy(key,a_key,a_nKeyLen);
}


//template <typename DataType1>
//template <typename DataType2>
template <typename DataType>
common::HashTbl<DataType>::HashItem::~HashItem()
{
	if(prev){prev->next=next;}
	if(next){next->prev=prev;}
	free(key);
}



/*////////////////////////////////////////////////////////////////////////*/
namespace common { namespace hashFncs{

static inline size_t hash1_( const void* a_pKey, size_t a_unKeySize )
{
    _REGISTER uint8_t *k = (uint8_t *)a_pKey;
    _REGISTER size_t a,b,c;  /* the internal state */
	
	size_t          len;    /* how many key bytes still need mixing */
	
	/* Set up the internal state */
	len = a_unKeySize;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = 13;         /* variable initialization of internal state */
	
	/*---------------------------------------- handle most of the key */
	while (len >= 12)
	{
		a=a+(k[0]+((size_t)k[1]<<8)+((size_t)k[2]<<16) +((size_t)k[3]<<24));
		b=b+(k[4]+((size_t)k[5]<<8)+((size_t)k[6]<<16) +((size_t)k[7]<<24));
		c=c+(k[8]+((size_t)k[9]<<8)+((size_t)k[10]<<16)+((size_t)k[11]<<24));
		mix2(a,b,c);
		k = k+12; len = len-12;
	}
	
	
	/*------------------------------------- handle the last 11 bytes */
	c = c+a_unKeySize;
	
	switch(len)              /* all the case statements fall through */
	{
	case 11: c=c+((size_t)k[10]<<24); FALLTHROUGH2
	case 10: c=c+((size_t)k[9]<<16);  FALLTHROUGH2
	case 9 : c=c+((size_t)k[8]<<8);   FALLTHROUGH2
		
		/* the first byte of c is reserved for the length */
	case 8 : b=b+((size_t)k[7]<<24);  FALLTHROUGH2
	case 7 : b=b+((size_t)k[6]<<16);  FALLTHROUGH2
	case 6 : b=b+((size_t)k[5]<<8);   FALLTHROUGH2
	case 5 : b=b+k[4];                  FALLTHROUGH2
	case 4 : a=a+((size_t)k[3]<<24);  FALLTHROUGH2
	case 3 : a=a+((size_t)k[2]<<16);  FALLTHROUGH2
	case 2 : a=a+((size_t)k[1]<<8);   FALLTHROUGH2
	case 1 : a=a+k[0];                  
		/* case 0: nothing left to add */
	}
	mix2(a,b,c);
	/*-------------------------------------------- report the result */
	
	return c;
}

}}


#endif  // #ifndef __common_hashtbl_impl_hpp__