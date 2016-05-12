#pragma once

#define CREATE_FUNC_ARGS(__TYPE__) \
	template<typename ... Types>\
	static __TYPE__* create(Types ... args)\
	  {\
	  __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(args ...)) \
   { \
   pRet->autorelease(); \
   return pRet; \
   } \
	  else \
	  { \
	  delete pRet; \
	  pRet = nullptr; \
	  return nullptr; \
	  } \
	  }