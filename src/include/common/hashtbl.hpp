// common_hashtbl.hpp
// 2017 Jul 21

#ifndef common_hashtbl_hpp
#define common_hashtbl_hpp

#include <stdint.h>
#include <stddef.h>

#define DEFAULT_TABLE_SIZE	64

namespace common {

namespace hashFncs{
typedef size_t(*TypeHashFnc)(const void* key, size_t keySize);
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
	void ClearAll();

protected:
	struct HashItem {
        HashItem(const void* key, size_t keyLen, const DataType& data, HashItem* a_pNext);
		~HashItem();
        HashItem *prev, *next; void* key; DataType data; size_t keySize;
	};

protected:
	hashFncs::TypeHashFnc		m_fpHashFnc;
	HashItem**					m_pTable;
	size_t					m_unRoundedTableSizeMin1;
};

}

#ifndef common_hashtbl_impl_hpp
#include "hashtbl.impl.hpp"
#endif


#endif  // #ifndef common_hashtbl_hpp
