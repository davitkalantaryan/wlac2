
// common_hashtbl.hpp
// 2017 Jul 21

#ifndef __common_hashtbl_hpp__
#define __common_hashtbl_hpp__

#include <stddef.h>

#define DEFAULT_TABLE_SIZE	64

namespace common {

namespace hashFncs{
typedef size_t (*TypeHashFnc)(const void* key, size_t keySize);
}

template <typename DataType>
class HashTbl
{
public:
	HashTbl(size_t tableSize= DEFAULT_TABLE_SIZE);
	virtual ~HashTbl();

	int  AddEntry(const void* key, size_t keyLen, const DataType& data);
	bool RemoveEntry(const void* key, size_t keyLen);
	bool FindEntry(const void* key, size_t keyLen, DataType* dataPtr);

protected:
    template <typename DataType2>
	struct HashItem {
        HashItem(const void* key, size_t keyLen, const DataType2& data);
		~HashItem();
        HashItem *prev, *next; void* key; DataType2 data; size_t dataSize;
	};

protected:
	hashFncs::TypeHashFnc		m_fpHashFnc;
	HashItem<DataType>**		m_pTable;
	size_t						m_unRoundedTableSizeMin1;
};

}

#include "common_hashtbl.impl.hpp"



#endif  // #ifndef __common_hashtbl_hpp__
