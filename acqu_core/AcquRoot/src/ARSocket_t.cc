//--Author	JRM Annand   19th Jan 2004   Adapt from TAcquSocket
//--Rev 	JRM Annand...21st Jan 2007   4v0 update adapt from TA2Socket
//--Rev 	JRM Annand....1st Feb 2007   Feedback from TDAQsocket
//--Rev 	JRM Annand....7th Dec 2007   Test read with select
//--Rev 	JRM Annand... 3rd Jun 2008...const Char_t*...gcc 4.3
//--Update	JRM Annand...22nd Oct 2009...fRetry = 5
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data
//
// ARSocket_t
//
//  Wrapper class for TCP/IP network socket. Used to read/write data from/to
//  remote computer node via the net. Long data buffers are "divided" into
//  chunks and the send/receive of each chunk is "handshaked"
//  between the connected nodes
//
//---------------------------------------------------------------------------

#include "ARSocket_t.h"

//---------------------------------------------------------------------------
ARSocket_t::ARSocket_t(const Char_t* name, const Char_t* hostname,
		       Int_t port, Int_t mode, Int_t reclen, Int_t packlen,
		       TA2System *sys)
{
  // Create a TCP/IP stream socket
  // mode = ELocal (0)....accept socket on local node
  // mode = ERemote (1)...connect to socket on remote node
  // portoffset defaults to zero
  //

  //  char hname[EMaxName];

  fSys = sys;
  fMode = mode;					// save accept/connect mode
  fPort = port;	                                // socket port
  fInit[0] = reclen;                            // data buffer record length
  fInit[1] = packlen;                           // transfer packet length
  fHost = new char[strlen(hostname)+1];         // store host name
  strcpy(fHost,hostname);
  fRetry = 5;
}

//---------------------------------------------------------------------------
ARSocket_t::~ARSocket_t()
{
  // Internet socket destructor...
  // Don't abort if anything in KillSk fails
  //
  delete fHost;
  KillSk();
}

//---------------------------------------------------------------------------
void ARSocket_t::Initialise()
{
  // Set up the network socket
  // and make the connection
  // In the event of any failure
  // call the error diagnostic...with the fatal flag...program exits

  sockaddr_in skname;   			// socket parameters
  fID = socket(AF_INET, SOCK_STREAM, 0);	// internet stream socket
  if( fID == -1 ){
    PrintError("<ERROR ARSocket_t: get network socket descriptor>",EErrFatal);
  }
  // Setup stuff for the network address
  hostent* gethost = gethostbyname(fHost);	// network address determine
  if( gethost == NULL ){
    PrintError("<ERROR ARSocket_t: get host-by-name>",EErrFatal);
  }
  hostent host = *gethost;

  skname.sin_family = host.h_addrtype;
  skname.sin_addr = *((struct in_addr*)(host.h_addr));
  skname.sin_port = htons(fPort);
  Int_t sklen = sizeof(struct sockaddr_in);    // name struct length
  Int_t size = sizeof(int) * ESkInitBuff;      // size initial handshake buff
  
// Local-host setup.....
// bind, listen, accept, exchange initial data packet
  if( fMode == ESkLocal ){
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    if(setsockopt(fID, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) < 0) {
        perror("setsockopt()");
        close(fID);
    }
    if( bind(fID, (sockaddr*)(&skname), sklen) == -1 ){
      close(fID);
      PrintError("<ERROR ARSocket_t: socket bind name Failed>",EErrFatal);
    }
    if( listen(fID, ESkBacklog) == -1 ){
      close(fID);
      PrintError("<ERROR ARSocket_t: socket listen>",EErrFatal);
    }
    // put that output exactly before the blocking accept command
    printf("\n<Waiting to connect to data receiver>\n");
    fflush(stdout);
    Int_t acc = accept(fID,(sockaddr*)(&skname),(socklen_t*)(&sklen));
    if( acc == -1){
      close(fID);
      PrintError("<ERROR ARSocket_t: socket accept>",EErrFatal);
    }
    KillSk();
    fID = acc;
    if( write(fID, fInit, size) != size ){
      KillSk();
      PrintError("<ERROR ARSocket_t: socket handshake write>",EErrFatal);
    }
    fprintf(fSys->GetLogStream()," Accepted local net socket at port: %d\n",
		       fPort );
  }
  else{
    Int_t c;
    for( Int_t i=0; i<fRetry; i++ ){
      c = connect(fID, (sockaddr*)(&skname), sklen);
      if( c != -1 ) break;
      sleep(1);
    }
    if( c == -1 ){
      close(fID);
      PrintError("<ERROR ARSocket_t: socket connect>", EErrFatal);
    }
    if( read(fID, fInit, size) != size ){
      KillSk();
      PrintError("<ERROR ARSocket_t: socket handshake read>", EErrFatal);
    }
    fprintf(fSys->GetLogStream()," Connected remote net socket at port: %d\n\
    record length: %d    packet length: %d\n\n", fPort,fInit[0],fInit[1] );
  } 
  // Setup select for data available check
  fTV.tv_sec = 1;               // 1 sec timeout
  fTV.tv_usec = 0;
  FD_ZERO( &fRfds );
  FD_SET( fID, &fRfds );

}				

//---------------------------------------------------------------------------
void ARSocket_t::WriteChunked( void* buff, Int_t lrec, Int_t prec )
{
  // Send buffer of data in chunks down the line
  // after each send read back from receiver no of bytes received
  // Error logged if read-back #bytes doesn't match
  // Fatal error if initial write not successful

  Int_t bytes;			// no. of bytes single read
  Int_t n_bytes = 0;
  Int_t w_bytes;
  Int_t r_bytes;;
  do{
    if( (bytes = lrec - n_bytes) > prec )
      bytes = prec;
    // demand a complete write is made
    if( (w_bytes = write(fID,buff,bytes)) != bytes )
      PrintError("<Stream Socket Incomplete Write>",EErrFatal);
    //
    w_bytes = read(fID,&r_bytes,sizeof(int));
    if(r_bytes != bytes)
      PrintError("<Stream Socket incompatible handshake readback>", EErrFatal);
    //
    buff = (Char_t*)buff + bytes;
    n_bytes += bytes;
  }while(n_bytes < lrec);
}

//---------------------------------------------------------------------------
Int_t ARSocket_t::ReadChunked( void* buff, Int_t lrec, Int_t prec )
{
  // Read buffer of data sent in chunks down the line
  // after each send read back from receiver no of bytes received
  // Error logged if read-back #bytes doesn't match
  // Fatal error if initial write not successful

  Int_t byte;			// no. of bytes single read
  Int_t nbyte = 0;
  Int_t pbyte;
  Int_t maxbyte;
  do{
    pbyte = 0;
    maxbyte = (nbyte + prec) > lrec ? (lrec - nbyte) : prec; // check overflow
    // loop to accumulate single packet
    do{
      byte = read(fID, buff, maxbyte); // read from the net socket
      switch(byte){           	       // check if the read is OK
      case 0:		               // hard error
	*((UInt_t*)buff) = EKillBuff;  // flag shutdown
	byte = 30;
      case 8:                          // End Of File
      case 30:                         // kill the system
	nbyte = byte;
	HandShakeSk(byte);             // remote node waits for it
	return nbyte;	               // nothing more to do
      default:	                       // "normal" read
	pbyte += byte;
	buff = (Char_t*)buff + byte;
	break;
      }
    }while(pbyte < maxbyte);
    nbyte += pbyte;                    // accumulate packet contents
    HandShakeSk(pbyte);	               // remote node waits for this
  }while( nbyte < lrec );
  return nbyte;	
}

//---------------------------------------------------------------------------
void ARSocket_t::KillSk()
{
  // Duplex shutdown of socket
  // followed by descriptor close
  //
  if( shutdown(fID,2) == -1 ) 
    PrintError("<ERROR ARSocket_t: duplex socket shutdown>");
  if( close(fID) == -1 ) 
    PrintError("<ERROR ARSocket_t: internet socket close>\n");
  return;
}

//---------------------------------------------------------------------------
void  ARSocket_t::PrintError( const Char_t* message, Int_t level )
{
  // TA2System print error message with optional program exit if fatal

  if( !fSys ) puts( message );
  else fSys->PrintError( "", message, level );
}

//---------------------------------------------------------------------------
Bool_t  ARSocket_t::IsReady( )
{
  // Check if data is available to read.
  Int_t ret = select( 1, &fRfds, NULL, NULL, &fTV );
  if( ret == -1 ){
    PrintError( "<Socket data-available test failed>\n" );
    return kFALSE;
  }
  else if( ret ) return kTRUE;
  else return kFALSE;
}

