#include "TimeStamp.h"
 
 
 
CLASS_META(o2::TimeStamp)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mYear).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMonth).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDay).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mHour).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMinute).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSecond).SERIALIZABLE_ATTRIBUTE();
}
END_META;
 