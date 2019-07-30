// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <wlac_vector.hpp> For WINDOWS MFC
 *
 *	Created on: Dec 11, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 * //
 *
 *
 */

#ifndef WLAC_VECTOR_MAX_NUMBER
#define WLAC_VECTOR_MAX_NUMBER	512
#endif

#include <stddef.h>
#include <string.h>

namespace WLAC_CLASSES
{
	template <typename Type>class vector
	{
	public:
		vector():m_unNum(0){}
		void push_back(const Type& a_tValue)
		{
			if (m_unNum<WLAC_VECTOR_MAX_NUMBER)m_members[m_unNum++] = a_tValue;
		}

		const size_t& size()const{ return m_unNum; }
		const Type& operator[](const size_t& unIndex)const{ return m_members[unIndex]; }
		Type& operator[](const size_t& unIndex){ return m_members[unIndex]; }
		void erase(const size_t& unIndex)
		{
			if (unIndex < m_unNum)
			{
				memmove(m_members + unIndex, m_members + unIndex + 1, ((--m_unNum) - unIndex));
			}
		}
		size_t begin()const{ return 0; }

	private:
		Type	m_members[WLAC_VECTOR_MAX_NUMBER];
		size_t	m_unNum;
	};
}