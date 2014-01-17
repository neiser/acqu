//--Author	JRM Annand	22nd Oct 1998
//--Rev	        JRM Annand	 4th Jun 1999	New names system wrappers
//--Rev	        JRM Annand	31st Mar 1999	Separate modules for classes
//--Rev	        JRM Annand	31st Apr 2002	Compressed input files
//--Rev	        JRM Annand	15th Feb 2003	NULL-terminated lists
//--Rev	        JRM Annand	28th Feb 2003	map ReadKey
//--Rev	        S Schumann	10th Oct 2003	ReadLine() mod
//--Rev	        JRM Annand	10th Oct 2003	ReadKey addition
//--Rev 	JRM Annand	 3rd Mar 2005	line buffer flush before read
//--Rev 	JRM Annand	29th Jan 2007	convertion from TAcquFile
//--Rev 	JRM Annand	29th Nov 2007	Open-fail non fatal,check-del
//--Rev 	JRM Annand	 3rd Jun 2008	const Char_t* gcc 4.3
//--Update	JRM Annand	 1st Sep 2009	delete[]
//--Update  A Neiser...   6th June 2013  Always add filename to fatal fopen 
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// ARFile_t
// Wrapper class for file handling UNIX system calls
//
//---------------------------------------------------------------------------

#include "ARFile_t.h"
#include <sstream>

//---------------------------------------------------------------------------
ARFile_t::ARFile_t( const Char_t* name, const Char_t* mode, TA2System* sys,
		    Bool_t isFatal )
{
// Open file using fopen(), ie normally for text I/O
// failure is judged fatal and causes the program to exit
// unless non-fatal flag is given
//
  fPath = -1;			         // doesn't use open()
  fSys = sys;                            // TA2System calls the construct
  if( !sys ) fName = NULL;
  else fName = sys->BuildName((Char_t*)name);     // save file name
  fStart = fopen(fName, mode);
  if( isFatal ) {
    if(fStart== NULL) {
      std::stringstream errMsg;
      errMsg << "<ERROR ARFile_t: fopen failed, FileName: " << fName << ">";
      PrintError(errMsg.str().c_str(), EErrFatal);
    }
  }
}

//---------------------------------------------------------------------------
ARFile_t::ARFile_t( const Char_t* name, Int_t flags, mode_t mode,
		    TA2System* sys )
{
// Check compression state of file and open directly (uncompressed)
// or open pipe from shell uncompress command
// failure is judged fatal and causes the program to exit
//
  const Char_t* extension[] = {".zip", ".bz2", ".gz", ".xz", NULL};
  const Char_t* command[] = {"unzip -p ", "bzcat ", "zcat ", "xzcat ", NULL};
  Char_t com[64];
  Int_t i;
  fSys = sys;
  for(i=0;; i++){
    if( !extension[i] ) break;
    if( strstr(name, extension[i]) ) break;
  }
  if( !sys ) fName = NULL;
  else fName = sys->BuildName( (Char_t*)name );
  switch(i){
  default:
    fPath = open(fName, flags, mode);
    if(fPath == -1)
      PrintError("<ERROR ARFile_t: file open by open() failed>", EErrFatal);
    fStart = NULL;
    break;
  case EZiped:
  case EBZiped:
  case EGZiped:
  case EXZiped:
    strcpy(com, command[i]);
    strcat(com, name);
    fStart = popen( com, "r" );
    if( fStart == NULL )
      PrintError("<ERROR ARFile_t: file open via pipe popen()>", EErrFatal);
    fPath = -2;
    break;
  }
}

//---------------------------------------------------------------------------
ARFile_t::~ARFile_t()
{
// Close file...fPath specifies if fclose, pclose or close is used
// ie what type of file has been opened
//
  switch(fPath){
  default:
    close(fPath);
    break;
  case -1:
    if( fStart ) fclose(fStart);
    break;
  case -2:
    if( fStart ) pclose(fStart);
    break;
  }
  if( fName ) delete[] fName;
}

//---------------------------------------------------------------------------
Char_t* ARFile_t::ReadLine( )
{
  // Read line from ASCII file
  // Loop until "readable character" (value 0x21-7e) or EOF found....
  // Characters '*' or '#' at the start denote a comment (ignored)
  // Other "non-readable characters" assumed a typo and ignored
  // S.Schumann mod...also ignore blank lines
  //
  Int_t i;
  while( (fL = GetString() ) != NULL){
    if ((*fL != '*') && (*fL != '#') && (*fL != '\n')){
      for(i=0; i<ELineSize; i++){
	if( (*fL > 0x20) && (*fL < 0x7F)) return fL;
	fL++;		        // next character
      }
    }
  }
  return NULL;			// if no "readable" line found
}

//---------------------------------------------------------------------------
Int_t ARFile_t::ReadKey( const Map_t* keylist )
{
  // Read line from ASCII file
  // If it contains "readable" characters look for a
  // match with a keyword supplied in the Map_t keylist
  //
  Int_t i;                               // keyword index
  const Char_t* k;
  Char_t keyword[EKeyWordSize];          // holds "keyword read from file

  while( ReadLine() != NULL ){           // get non-comment line
    if( sscanf(fLine,"%s",keyword) < 1) return -1;
    for( i=0; ; i++){
      k = keylist[i].fFnName;
      if( !k ) return -1;               // null-terminated...nothing found
      if( strstr(k,keyword) ){ 
	AdvanceLine( keyword );
        return keylist[i].fFnKey;       // matched keyword !!
      }
    }
  }
  return -1;                            // Nothing found
}

//---------------------------------------------------------------------------
Int_t ARFile_t::ReadKey( const Map_t** keylist )
{
  // Read line from ASCII file
  // If it contains "readable" characters look for a
  // match with a keyword from a Map_t contained within
  // the keylist Map_t array
  //
  Int_t i,j;				// keyword index
  const Char_t* k;                      // key string ptr
  const Map_t* list;                    // temp list ptr
  Char_t keyword[EKeyWordSize];		// holds keyword read from file

  while( ReadLine() != NULL ){   	// get non-comment line
    if( sscanf(fLine,"%s",keyword) < 1) return -1;
    for( j=0; ; j++ ){
      list = keylist[j];
      if( !list ) return -1;
      for( i=0; ; i++){
	k = list[i].fFnName;
	if( !k ) break;                  // null-terminated...nothing found
	if( strstr(k,keyword) ){ 
	  AdvanceLine( keyword );
	  return list[i].fFnKey;	 // matched keyword !!
	}
      }
    }
    return -1;
  }
  return -1;	                        // Nothing found
}

//---------------------------------------------------------------------------
void  ARFile_t::AdvanceLine( Char_t* keyword )
{
  // Move line pointer after keyword and also after any tabs or spaces
  // Don't go past end of line or null terminator

  fL = strstr(fLine, keyword) + strlen(keyword);
  for(;;){
    if( (*fL == 0) || (*fL == '\n') ) break;   
    if( (*fL > 0x20) && (*fL < 0x7F)) break;
    fL++;
  }
  return;
}

//---------------------------------------------------------------------------
void  ARFile_t::PrintError( const Char_t* message, Int_t level )
{
  // TA2System print error message with optional program exit if fatal

  if( !fSys ) puts( message );
  else{
    if( !fName ) fSys->PrintError( "", message, level );
    else fSys->PrintError( fName, message, level );
  }
}

