

#ifndef _LOG4CPP_SEVENKLAYOUT_HH
#define _LOG4CPP_SEVENKLAYOUT_HH

#include <log4cpp/Portability.hh>
#include <log4cpp/Layout.hh>

namespace log4cpp {

	/**
	* SevenKLayout is a simple fixed format Layout implementation. 
	**/
	class LOG4CPP_EXPORT SevenKLayout : public Layout {
	public:
		SevenKLayout();
		virtual ~SevenKLayout();

		/**
		* Formats the LoggingEvent in SevenKLayout style:<br>
		* "timeStamp priority category ndc: message"
		**/
		virtual std::string format(const LoggingEvent& event);
	};        
}

#endif // _LOG4CPP_LINEKONGLAYOUT_HH
