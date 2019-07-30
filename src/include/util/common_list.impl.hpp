
// 2017 Jan 02
// common_list.impl.hpp

#ifndef __common_list_impl_hpp__
#define __common_list_impl_hpp__


#ifndef __common_list_hpp__
#error do not include this header directly
#include "common_list.hpp"
#endif

#include <memory.h>
#include <stddef.h>


template <typename DataType>
common::List<DataType>::List()
	:
	m_pFirst(NULL),
	m_pLast(NULL)
{
}


template <typename DataType>
common::List<DataType>::~List()
{
	CListItem<DataType> *pItemNext,*pItem = m_pFirst;
	while(pItem){
		pItemNext = pItem->next; 
		delete pItem;
		pItem = pItemNext;
	}
}


template <typename DataType>
common::CListItem<DataType>* common::List<DataType>::AddToTheEnd(const DataType& a_data)
{
	CListItem<DataType>* pNewItem = new CListItem<DataType>(a_data);

	if(!pNewItem){return pNewItem;}

	if(!m_pFirst){m_pFirst=pNewItem;}
	if(m_pLast){m_pLast->next=pNewItem;}
	pNewItem->prev = m_pLast;
	m_pLast = pNewItem;
	return pNewItem;
}


template <typename DataType>
common::CListItem<DataType>* common::List<DataType>::AddToTheBeg(const DataType& a_data)
{
	CListItem<DataType>* pNewItem = new CListItem<DataType>(a_data);

	if(!pNewItem){return pNewItem;}

	if(!m_pLast){m_pLast=pNewItem;}
	if(m_pFirst){m_pFirst->prev=pNewItem;}
	pNewItem->next = m_pFirst;
	m_pFirst = pNewItem;
	return pNewItem;
}


template <typename DataType>
void common::List<DataType>::RemoveEntry(common::CListItem<DataType>* a_item)
{
	if(a_item->next){a_item->next->prev=a_item->prev;}
	if(a_item->prev){a_item->prev->next=a_item->next;}
	if(a_item==m_pFirst){m_pFirst=a_item->next;}
	if(a_item==m_pLast){ m_pLast =a_item->prev;}
	delete a_item;
}


template <typename DataType>
template <typename TypeOwner>
void common::List<DataType>::IterateOverTheList(
	bool (TypeOwner::*a_fpIter)(common::CListItem<DataType>*item, void*clbk),
	TypeOwner*a_owner, void*a_clbk)
{
	CListItem<DataType> *pItemNext,*pItem = m_pFirst;
	bool bContinue = true;

	while(pItem && bContinue){
		pItemNext = pItem->next; // may be user will delete this item
		bContinue = (a_owner->*a_fpIter)(pItem,a_clbk);
		pItem = pItemNext;
	}
}


/*////////////////////////////////*/

template <typename DataType>
common::CListItem<DataType>::CListItem(const DataType& a_data)
{
	//memset(this, 0, sizeof(*this));
	this->prev = this->next = NULL;
	this->m_id = a_data;
}


template <typename DataType>
common::CListItem<DataType>::~CListItem()
{
}


#endif  // #ifndef __common_list_impl_hpp__
