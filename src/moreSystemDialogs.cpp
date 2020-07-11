
#include "ofConstants.h"
#include "ofSystemUtils.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofUtils.h"
#include <condition_variable>
#include <mutex>

using namespace std;

#ifdef TARGET_WIN32
#include <winuser.h>
#include <commdlg.h>
#define _WIN32_DCOM

#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <stdio.h>

#endif

#ifdef TARGET_OSX
	// ofSystemUtils.cpp is configured to build as
	// objective-c++ so as able to use Cocoa dialog panels
	// This is done with this compiler flag
	//		-x objective-c++
	// dhttp://www.yakyak.org/viewtopic.php?p=1475838&sid=1e9dcb5c9fd652a6695ac00c5e957822#p1475838

	#include <Cocoa/Cocoa.h>
	#include "ofAppRunner.h"
#endif

#ifdef TARGET_WIN32
#include <locale>
#include <sstream>
#include <string>

std::string convertWideToNarrow( const wchar_t *s, char dfault = '?',
                      const std::locale& loc = std::locale() )
{
  std::ostringstream stm;

  while( *s != L'\0' ) {
    stm << std::use_facet< std::ctype<wchar_t> >( loc ).narrow( *s++, dfault );
  }
  return stm.str();
}

std::wstring convertNarrowToWide( const std::string& as ){
    // deal with trivial case of empty string
    if( as.empty() )    return std::wstring();

    // determine required length of new string
    size_t reqLength = ::MultiByteToWideChar( CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0 );

    // construct new string of required length
    std::wstring ret( reqLength, L'\0' );

    // convert old string to new string
    ::MultiByteToWideChar( CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length() );

    // return new string ( compiler should optimize this away )
    return ret;
}

#endif

#if defined( TARGET_OSX )
static void restoreAppWindowFocus(){
	NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
	if(appWindow) {
		[appWindow makeKeyAndOrderFront:nil];
	}
}
#endif



bool ofSystemYesNoDialog(std::string message)
{
#ifdef TARGET_OSX
    @autoreleasepool {
        NSAlert* alertDialog = [[[NSAlert alloc] init] autorelease];
        [alertDialog addButtonWithTitle:@"Yes"];
        [alertDialog addButtonWithTitle:@"No"];
        alertDialog.messageText = [NSString stringWithUTF8String:message.c_str()];
        NSInteger returnCode = [alertDialog runModal];
        restoreAppWindowFocus();
        if ( returnCode == NSAlertFirstButtonReturn ) return true;
        else return false;
    }
#endif
}
