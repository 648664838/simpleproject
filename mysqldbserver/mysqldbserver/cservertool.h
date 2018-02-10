#pragma once

// ********************************************************************** //
// CWTimer
// ********************************************************************** //
// �Զ����ü�ʱ��
class CWTimer
{
public:
	int mDuration;			// ʣ��ʱ��
	int mMaxDuration;		// ����ʱ����
public:
	CWTimer()
	{
		mDuration = 0;
		mMaxDuration = 0;
	}
	CWTimer(int vDuration) : mDuration(vDuration), mMaxDuration(vDuration)	{	}

public:
	// ���ʱ�䵽����true
	inline bool Elapse(int vInterval)
	{
		mDuration -= vInterval; return mDuration <= 0 ? mDuration = mMaxDuration, true : false;
	}
};
