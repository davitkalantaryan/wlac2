
// 2018 Jan 02
// common_list.hpp

#ifndef __common_list_hpp__
#define __common_list_hpp__

namespace common {


template <typename DataType>
class CListItem
{
	template<class T> friend class List;
public:
	const DataType& data()const {return m_id;}
	DataType& data() { return m_id; }
private:
	CListItem(const DataType& data); ~CListItem();
	CListItem *prev, *next;
	mutable DataType m_id;
};


template <typename DataType>
class List
{
public:
	List();
	virtual ~List();

	common::CListItem<DataType>*	AddToTheEnd(const DataType& data);
	common::CListItem<DataType>*	AddToTheBeg(const DataType& data);
	void							RemoveEntry(common::CListItem<DataType>* item);
	template <typename TypeOwner>
	void							IterateOverTheList(
										bool (TypeOwner::*a_fpIter)(common::CListItem<DataType>*item,void*clbk),
										TypeOwner*owner,void*clbk);

protected:
	CListItem<DataType>*	m_pFirst;
	CListItem<DataType>*	m_pLast;
};

}

#include "common_list.impl.hpp"

#endif  // #ifndef __common_list_hpp__
