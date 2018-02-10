#pragma once

// ********************************************************************** //
// CWTimer
// ********************************************************************** //
// 自动重置计时器
class CWTimer
{
public:
	int mDuration;			// 剩余时间
	int mMaxDuration;		// 最大的时间间隔
public:
	CWTimer()
	{
		mDuration = 0;
		mMaxDuration = 0;
	}
	CWTimer(int vDuration) : mDuration(vDuration), mMaxDuration(vDuration)	{	}

public:
	// 如果时间到返回true
	inline bool Elapse(int vInterval)
	{
		mDuration -= vInterval; return mDuration <= 0 ? mDuration = mMaxDuration, true : false;
	}
};
