#ifndef HUAWEI_DAL_H_
#define HUAWEI_DAL_H_

#include "uconn_AT_DAL.h"
#include "UHuawei4GParser.h"


class UHuaweiDal : public uconnAtDal
{
public:
	UHuaweiDal(UNetworkManager* p_NM):uconnAtDal(p_NM)
	{
		this->bufferParser = new UHuawei4GParser(p_NM);
	}
	~UHuaweiDal()
	{
		if(this->bufferParser != NULL)
		{
			delete this->bufferParser;
			this->bufferParser = NULL;
		}
	}
};


#endif //U_HUAWEI_DAL_H_