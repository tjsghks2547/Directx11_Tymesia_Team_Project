#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(nullptr, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#define			VK_MAX		 0xff


#define			FONT_GREEN		 _float4{30.f,255.f,185.f,255.f}
#define			FONT_RED		 _float4{235.f,95.f,75.f,255.f}
#define			FONT_GRAY		 _float4{100.f,100.f,100.f,255.f}
#define			FONT_WHITE		 _float4{255.f,255.f,255.f,255.f}


#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif

#define NO_COPY(CLASSNAME)										\
			private:												\
			CLASSNAME(const CLASSNAME&) = delete;					\
			CLASSNAME& operator = (const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)							\
			NO_COPY(CLASSNAME)										\
			private:												\
			static CLASSNAME*	m_pInstance;						\
			public:													\
			static CLASSNAME*	GetInstance( void );				\
			static unsigned int DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)								\
			CLASSNAME*	CLASSNAME::m_pInstance = nullptr;			\
			CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
				if(nullptr == m_pInstance) {						\
					m_pInstance = new CLASSNAME;					\
				}													\
				return m_pInstance;									\
			}														\
			unsigned int CLASSNAME::DestroyInstance( void ) {		\
				unsigned int iRefCnt = {};							\
				if(nullptr != m_pInstance)	{						\
					iRefCnt = m_pInstance->Release();				\
					if(0 == iRefCnt)								\
						m_pInstance = nullptr;						\
				}													\
				return iRefCnt;										\
			}
}

#endif // Engine_Macro_h__
