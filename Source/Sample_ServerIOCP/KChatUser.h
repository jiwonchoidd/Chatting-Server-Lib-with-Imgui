#pragma once
#include "KNetworkUser.h"
class KChatUser :	public KNetworkUser,
					public KObjectPool<KChatUser>
{

};

