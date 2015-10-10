#include "Logging.h"

void Logging::Init(int level, long baud){
    _level = constrain(level,LOG_LEVEL_NOOUTPUT,LOG_LEVEL_VERBOSE);
    _baud = baud;
    Serial.begin(_baud);
}

void Logging::Error(char* msg, ...){
    if (LOG_LEVEL_ERROR <= _level) {   
        va_list args;
        va_start(args, msg);
        print(LOG_LEVEL_ERROR,msg,args);
    }
}

void Logging::Info(char* msg, ...){
    if (LOG_LEVEL_INFO <= _level) {
        va_list args;
        va_start(args, msg);
        print(LOG_LEVEL_INFO,msg,args);
    }
}

void Logging::Debug(char* msg, ...){
    if (LOG_LEVEL_DEBUG <= _level) {
        va_list args;
        va_start(args, msg);
        print(LOG_LEVEL_DEBUG,msg,args);
    }
}


void Logging::Verbose(char* msg, ...){
    if (LOG_LEVEL_VERBOSE <= _level) {
        va_list args;
        va_start(args, msg);
        print(LOG_LEVEL_VERBOSE,msg,args);
    }
}

 void Logging::print(int level, const char *format, va_list args) {
 	Serial.print(millis(),DEC);
 	
 	switch (level) {
	  case LOG_LEVEL_NOOUTPUT: Serial.print(" NOP "); break;
	  case LOG_LEVEL_ERROR:    Serial.print(" ERROR "); break;
	  case LOG_LEVEL_INFO:     Serial.print(" INFO  "); break;
	  case LOG_LEVEL_DEBUG:    Serial.print(" DEBUG "); break;
	  case LOG_LEVEL_VERBOSE:  Serial.print(" TRACE "); break;
 	}
 	
    //
    // loop through format string
    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            if (*format == '\0') break;
            if (*format == '%') {
                Serial.print(*format);
                continue;
            }
            if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				Serial.print(s);
				continue;
			}
            if( *format == 'd' || *format == 'i') {
				Serial.print(va_arg( args, int ),DEC);
				continue;
			}
            if( *format == 'x' ) {
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}
            if( *format == 'X' ) {
				Serial.print("0x");
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}
            if( *format == 'b' ) {
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}
            if( *format == 'B' ) {
				Serial.print("0b");
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}
            if( *format == 'l' ) {
				Serial.print(va_arg( args, long ),DEC);
				continue;
			}

            if( *format == 'c' ) {
				Serial.print(va_arg( args, int ));
				continue;
			}
            if( *format == 't' ) {
				if (va_arg( args, int ) == 1) {
					Serial.print("T");
				}
				else {
					Serial.print("F");				
				}
				continue;
			}
            if( *format == 'T' ) {
				if (va_arg( args, int ) == 1) {
					Serial.print("true");
				}
				else {
					Serial.print("false");				
				}
				continue;
			}

        }
        Serial.print(*format);
    }
    Serial.println();
 }
 
 Logging Log = Logging();

 
 
  




