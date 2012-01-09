#include "osl/include/QOSLibPrerequisits.h"



#include "osl/include/mem_alloc.h"
#include "osl/include/QString.h"
#include "osl/include/QStringUtil.h"
#include "osl/include/QFileUtil.h"

#include "osl/include/process_ext.h"
#include "osl/include/QObject.h"

namespace osl
{

		template<class Base>
				void osl::RefTargetImpl<Base>::ref() const
				{
						InterlockedInc32( &m_nRefCount );
				}


		template<class Base>
				void osl::RefTargetImpl<Base>::unref() const
				{
						if ( InterlockedDec32( &m_nRefCount ) <= 0 )
						{
								delete this;
						}
				}

		//----------------------------------------------
		void Object::ref()const
		{
				InterlockedInc32( &m_nRefCount );
		}
		//----------------------------------------------
		void Object::unref() const
		{
				if ( InterlockedDec32( &m_nRefCount ) <= 0 )
				{
						delete this;
				}
		}
};
