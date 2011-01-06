//------------------------------------------------------------------------------------------
// Summary:		Lightweight VICP client implementation.
//
// Started by:	Anthony Cake
//
// Started:		June 2003
//				Published on SourceForge under LeCroyVICP project, Sept 2003
//------------------------------------------------------------------------------------------
//
// Description:	
//
//		This file contains a Client-side implementation of the VICP network communications
//		protocol used to control LeCroy Digital Oscilloscopes (DSOs).
//
//		This file is intended to be ultimately as platform independent as possible, but at
//		present has only been compiled & tested under Visual C++ 6.0 on a Windows platform.
//
// VICP Protocol Description/History:
//	
//		The VICP Protocol has been around since 1997/98. It did not change in any way between it's
//		conception, and June 2003, when a previously reserved field in the header was assigned. 
//		This field, found at byte #2, is now used to allow the client-end of a VICP communication
//		to detect 'out of sync' situations, and therefore allows the GPIB 488.2 'Unread Response'
//		mechanism to be emulated. 
//		
//      These extensions to the original protocol did not cause a version number change, and are
//		referred to as version 1a. It was decided not to bump the version number to reduce the
//		impact on clients, many of which are looking for a version number of 1. 
//		Clients and servers detect protocol version 1a by examining the sequence number field, 
//		it should be 0 for version 1 of the protocol (early clients), or non-zero for v1a.
//
//
// VICP Headers:
//
//	    Byte	Description		
//	    ------------------------------------------------
//		 0		Operation	
//		 1		Version		1 = version 1
//		 2		Sequence Number { 1..255 }, (was unused until June 2003)
//		 3		Unused
//	 	 4		Block size, LSB	 (not including this header)
//		 5		Block size
//		 6		Block size
//		 7		Block size, MSB
//
//	Operation bits:
//
//		Bit		Mnemonic	Purpose
//		-----------------------------------------------
//		D7		DATA		Data block (D0 indicates with/without EOI)
//		D6		REMOTE		Remote Mode
//		D5		LOCKOUT		Local Lockout (Lockout front panel)
//		D4		CLEAR		Device Clear (if sent with data, clear occurs before block is passed to parser)
//		D3		SRQ			SRQ (Device -> PC only)
//		D2		SERIALPOLL  Request a serial poll
//		D1		Reserved	Reserved for future expansion
//		D0		EOI			Block terminated in EOI
//
// Known Limitations:
//
// Outstanding Issues
//
// Dependencies
//		- Try to keep to absolute minimum to allow porting to other operating systems.
//		  Avoid ATL, and especially MFC, currently only relies upon winsock.h and the standard
//		  C library includes.
//
//------------------------------------------------------------------------------------------
//


#ifdef _MSC_VER
#include "winsock.h"
#else 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BOOL bool
#define FALSE false
#define TRUE true

#define CHAR char
#define DWORD long int
#define WORD int
#define SOCKADDR_IN sockaddr_in
#define TIMEVAL timeval
#define  WSADATA //
#define  wsaData //
#define  WSAStartup //

#define MAKEWORD(MSB, LSB) ((MSB << 8) | LSB)
#define MAKEDWORD(MSB, LSB) ((MSB << 16) | LSB)

#endif
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>


#define ATLTRACE
#define ATLTRACE2

// GPIB status bit vector :                  
//       global variable ibsta and wait mask 

#define ERR     (1<<15) // Error detected					0x8000	
#define TIMO    (1<<14) // Timeout							0x4000
#define END     (1<<13) // EOI or EOS detected				0x2000
#define SRQI    (1<<12) // SRQ detected by CIC				0x1000
#define RQS     (1<<11) // Device needs service				0x0800
#define SPOLL   (1<<10) // Board has been serially polled	0x0400
#define CMPL    (1<<8)  // I/O completed					0x0100
#define REM     (1<<6)  // Remote state						0x0040	
#define CIC     (1<<5)  // Controller-in-Charge             0x0020	
#define ATN     (1<<4)  // Attention asserted				0x0010
#define TACS    (1<<3)  // Talker active					0x0008
#define LACS    (1<<2)  // Listener active					0x0004
#define DTAS    (1<<1)  // Device trigger state				0x0002
#define DCAS    (1<<0)  // Device clear state				0x0001

// GPIB error codes :
//		iberr

#define EDVR	0		// System error                           
#define ECIC	1		// Function requires GPIB board to be CIC 
#define ENOL	2		// Write function detected no Listeners   
#define EADR	3		// Interface board not addressed correctly
#define EARG	4		// Invalid argument to function call      
#define ESAC	5		// Function requires GPIB board to be SAC 
#define EABO	6		// I/O operation aborted                  
#define ENEB	7		// Non-existent interface board           
#define EDMA	8		// Error performing DMA                   
#define EOIP	10		// I/O operation started before previous operation completed 
#define ECAP	11		// No capability for intended operation   
#define EFSO	12		// File system operation error            
#define EBUS	14		// Command error during device call       
#define ESTB	15		// Serial poll status byte lost           
#define ESRQ	16		// SRQ remains asserted                   
#define ETAB	20		// The return buffer is full.             
#define ELCK	21		// Address or board is locked.            

#define SERVER_PORT_NUM				1861	// port # registered with IANA for lecroy-vicp
#define IO_NET_HEADER_SIZE			8		// size of network header
#define ERR							(1<<15) // Error detected
#define SMALL_DATA_BUFSIZE			8192
#define CONNECT_TIMEOUT_SECS		2L					
#define MAX_DEVICE_ADDR_LEN			255		// max. length of a device address string (dns address/dotted ip address)
#define MAX_ERROR_MSG_LEN			255		// max. length of a device address string (dns address/dotted ip address)

// VICP header 'Operation' bits
#define OPERATION_DATA				0x80	
#define OPERATION_REMOTE			0x40
#define OPERATION_LOCKOUT			0x20
#define OPERATION_CLEAR				0x10
#define OPERATION_SRQ				0x08
#define OPERATION_REQSERIALPOLL		0x04
#define OPERATION_EOI				0x01

// Header Version
#define HEADER_VERSION1				0x01

typedef enum READSTATE
{
	NetWaitingForHeader,
	NetWaitingForData,
	NetErrorState
};

class CVICPClient
{
public:
	//------------------------------------------------------------------------------------------
	CVICPClient()
	{
		// no connection (yet)
			m_socketFd = -1;
			m_readState = NetWaitingForHeader;
			m_connectedToScope = FALSE;
			m_remoteMode = FALSE;
			m_localLockout = FALSE;
			m_iberr = m_ibsta = m_ibcntl = 0;
			m_bErrorFlag = false;
			m_nextSequenceNumber = 1;
			m_lastSequenceNumber = 1;
			m_bFlushUnreadResponses = true;
			m_bVICPVersion1aSupported = false;
		
		// perform one-time initialization of the device driver
		// (m_oneTimeWinsockInit is a static member variable and 
		//	therefore is shared between all instances of objects of this type
			if(!m_oneTimeWinsockInit)
			{
				int err;
				WORD wVersionRequested = MAKEWORD(1, 1); 
				WSADATA wsaData;

				// one-time initialization of Winsock
					err = WSAStartup(wVersionRequested, &wsaData); 
					if (err != 0) 
					{
						MessageBox(NULL, "Failure to startup WinSock (WSAStartup)", "Error", MB_OK);
						return; 
					}

				m_oneTimeWinsockInit = TRUE;
			}

		// TODO: determine the max. block size
			m_maxBlockSize = 512;		// TODO: safe value for now

		m_currentTimeout = 10;
	}

	//------------------------------------------------------------------------------------------
	~CVICPClient()
	{
		disconnectFromDevice();
	}

	//------------------------------------------------------------------------------------------
	u_long GetDeviceIPAddress()
	{
		// loop through the address string and try to identify whether it's a dotted static IP
		// address, or a DNS address
			bool bOnlyDigitsAndDots = true;
			int dotCount = 0;
			for(unsigned int i = 0; i < strlen(m_deviceAddress); ++i)
			{
				// count the dots
					if(m_deviceAddress[i] == '.')
						++dotCount;

				// if the character is not a digit and not a dot then assume a DNS name
					if(!isdigit(m_deviceAddress[i]) && !(m_deviceAddress[i] == '.'))
						bOnlyDigitsAndDots = false;
			}

		// if only digits and dots were found then assume that it's a static IP address
			if(bOnlyDigitsAndDots && dotCount == 3)
			{
				IN_ADDR ipAddr;
				int b1 = 0, b2 = 0, b3 = 0, b4 = 0;
				int ret = sscanf(m_deviceAddress, "%d.%d.%d.%d", &b1, &b2, &b3, &b4);
				if(ret != 4)
				{
					sprintf(m_lastErrorMsg, "GetDeviceIPAddress() sscanf error ret=%d/4\n", ret);
					m_bErrorFlag = true;
				}
				ipAddr.S_un.S_un_b.s_b1 = b1;
				ipAddr.S_un.S_un_b.s_b2 = b2;
				ipAddr.S_un.S_un_b.s_b3 = b3;
				ipAddr.S_un.S_un_b.s_b4 = b4;

				return ipAddr.s_addr;
			}

		// assume that it's DNS name
			return GetIPFromDNS();
	}

	//------------------------------------------------------------------------------------------
	// Lookup the IP address of a DNS name
	u_long GetIPFromDNS()
	{
		struct in_addr addr;
		int	ret, *intp;
		char	**p;
		struct hostent *hp = NULL;
		
		hp = gethostbyname(m_deviceAddress);
		if (!hp == NULL)
		{
			// We assume that if there is more than 1 address, there might be
			// a server conflict.
			p = hp->h_addr_list;
			intp = (int *) *p;
			ret = *intp;
			addr.s_addr = ret;
			return addr.s_addr;
		}
		
		return 0;
	}


	//------------------------------------------------------------------------------------------
	// initialize the socket (doesn't require remote device to be connected or responding)
	BOOL openSocket()
	{
		ATLTRACE("Opening Socket:\n");

		// create client's socket
			m_socketFd = socket(AF_INET, SOCK_STREAM, 0);
			if (m_socketFd == INVALID_SOCKET)
			{
				sprintf(m_lastErrorMsg, "socket() failed, error code = %d\n", WSAGetLastError());
				m_bErrorFlag = true;
				// 10061 = WSAECONNREFUSED
				return(FALSE);
			}

		// disable the TCP/IP 'NAGLE' algorithm that buffers a bunch of
		// packets before sending them.
			const int just_say_no = 1;
			if (0 != setsockopt(m_socketFd, IPPROTO_TCP, TCP_NODELAY, (char*)&just_say_no, sizeof(just_say_no)))
			{
				::MessageBox(NULL, "Could not set socket option 'TCP_NODELAY'", "Error", MB_OK);
				return(FALSE);
			}

		// enable SO_LINGER to allow hard closure of sockets (LINGER enabled, but with timeout of zero)
			linger lingerStruct = { 1, 0 };
			if (0 != setsockopt(m_socketFd, SOL_SOCKET, SO_LINGER, (char *) &lingerStruct, sizeof(lingerStruct)))
			{
				::MessageBox(NULL, "Could not set socket option 'SO_LINGER'", "Error", MB_OK);
				return(FALSE);
			}
 


		return(TRUE);
	}

	//------------------------------------------------------------------------------------------
	// connect to a network device
	// address is extracted from m_deviceAddress (specified during construction of base class)
	BOOL connectToDevice()
	{
		// if not yet connected to scope...
			if(!m_connectedToScope)
			{
				ATLTRACE("Connecting:\n");

				// lookup the IP address of the device 
					u_long addr = GetDeviceIPAddress();

				// initialize the socket		
					openSocket();

				// ensure that the connect command will not block
					unsigned long argp = 1;		// 1 = enable non-blocking behaviour
					ioctlsocket(m_socketFd, FIONBIO, &argp);

				// try to connect to server (scope)
					int sockAddrSize = sizeof (SOCKADDR);	// size of socket address structures

				// build server socket address
					m_serverAddr.sin_family = AF_INET;
					m_serverAddr.sin_port = htons (SERVER_PORT_NUM);
					if ((m_serverAddr.sin_addr.s_addr = addr) == -1) 
					{
						sprintf(m_lastErrorMsg, "Bad server address");
						m_bErrorFlag = true;
						m_ibsta = ERR;
						m_iberr = EARG;		// Invalid argument to function call
						m_ibcntl = 0;
						return(FALSE);
					}
					
					int connectStatus = connect(m_socketFd, (SOCKADDR FAR *) &m_serverAddr, sockAddrSize);

				// after a connect in non-blocking mode a 'select' is require to 
				// determine the outcome of the connect attempt
					fd_set writeSet = { 1, { m_socketFd } };	
					TIMEVAL timeVal = { CONNECT_TIMEOUT_SECS, 0L };	
					int numReady = select(m_socketFd, NULL, &writeSet, NULL, &timeVal);

				// restore blocking behaviour				
					argp = 0;					// 0 = enable blocking behaviour
					ioctlsocket(m_socketFd, FIONBIO, &argp);
					
				// see if the connection succeeded
					if(numReady == 1)
					{
						ATLTRACE("Connect Succeeded\n");
						m_connectedToScope = TRUE;
					}
					else
					{
						sprintf(m_lastErrorMsg, "socket() failed, error code = %d\n", WSAGetLastError());
						m_bErrorFlag = true;
						disconnectFromDevice();
						m_ibsta = ERR;
						m_iberr = EABO;		// I/O operation aborted
						m_ibcntl = 0;
						return(FALSE);
					}
			}

		return(TRUE);
	}

	//------------------------------------------------------------------------------------------
	// disconnect from a network device
	BOOL disconnectFromDevice()
	{
		ATLTRACE("Disconnecting:\n");

		if(m_connectedToScope)
		{
			m_readState = NetWaitingForHeader;		// reset any partial read operation
			if(closesocket(m_socketFd) != 0)
			{
				sprintf(m_lastErrorMsg, "closesocket() failed, error code = %d\n", WSAGetLastError());
				m_bErrorFlag = true;
			}
			m_socketFd = -1;
			m_connectedToScope = FALSE;
			m_bVICPVersion1aSupported = false;
		}

		return(TRUE);
	}

	//------------------------------------------------------------------------------------------
	// clear the device
	void deviceClear()
	{
		sendDataToDevice("", 0, 0, TRUE /* device clear */);
		Sleep(100);						// TODO: remove when 'RebootScope' bug is fixed
		disconnectFromDevice();
		connectToDevice();
	}

	//------------------------------------------------------------------------------------------
	// return the serial poll byte. Uses the new Out-Of-Band signalling technique if
	// supported, else use the original 'in-band' technique.
	int serialPoll()
	{
		if(m_bVICPVersion1aSupported)
		{
			unsigned char oobResponse = 0x00;
			if(oobDataRequest('S', &oobResponse))		// 'S' == Serial Poll
			{
				return oobResponse;
			}
			else 
			{
				m_ibsta = ERR;
				m_iberr = EABO;				// The serial poll response could not be read within the serial poll timeout period.
				sprintf(m_lastErrorMsg, "serialPoll failed to receive OOB response from DSO\n");
				m_bErrorFlag = true;
				return oobResponse;
			}
		}
		else
		{
			// request the serial poll using an in-band technique
				sendDataToDevice("", 0, FALSE /* EOI */, FALSE /* device clear */, TRUE /* request serial poll */);

			// read the single serial-poll byte
				#define SPOLLBUFSIZE 	2
				CHAR buf[SPOLLBUFSIZE+1];
				int bytesRead = -1;
				bytesRead = readDataFromDevice(buf, SPOLLBUFSIZE);
				if(bytesRead >= 1)
				{
					m_ibsta = CMPL;
					return buf[0];
				}
				else
				{
					m_ibsta = ERR;
					m_iberr = EABO;			// The serial poll response could not be read within the serial poll timeout period.
					return 0;
				}
		}
	}

	//------------------------------------------------------------------------------------------
	// out-of band data request, used for serial polling and possibly other features in the future
	bool oobDataRequest(char requestType, unsigned char *response)
	{
		char oobDataTest[1] = { requestType };	
		int bytesSent = send(m_socketFd, (char *) oobDataTest, 1, MSG_OOB);
	
		TIMEVAL timeVal = { (long) m_currentTimeout, ((long) (m_currentTimeout * 1000000L)) % 1000000L};

		// if there is no sign of a header, get out quick (timeout)
		// Note that we don't look for in-band data, only OOB data (which appears in the exception record)
		fd_set exceptSet = { 1, { m_socketFd } };	
		int numReady = select(m_socketFd, NULL, NULL, &exceptSet, &timeVal);
		char buf[1] = { 0x00 };
		if(FD_ISSET(m_socketFd, &exceptSet))
		{
			int bytesReceived = recv(m_socketFd, (char *) buf, 1, MSG_OOB);
			*response = buf[0];
			return true;
		}
		else
		{
			return false;
		}
	}

	//------------------------------------------------------------------------------------------
	// send a block of data to a network device
	// returns FALSE on error status
	BOOL sendDataToDevice(char *message, int bytesToSend, BOOL eoiTermination, BOOL deviceClear = FALSE, BOOL serialPoll = FALSE)
	{
		static unsigned char headerBuf[IO_NET_HEADER_SIZE];
		int bytesSent;

		// handle cases where the user didn't read all data from a previous query
			if(m_bFlushUnreadResponses && (m_readState != NetWaitingForHeader))
				readDataFromDevice(NULL, -1, true);		// flush

		// if the block is relatively small, send header + data with one 'send' command (faster)
			if(bytesToSend < SMALL_DATA_BUFSIZE)
				return(sendSmallDataToDevice(message, bytesToSend, eoiTermination, deviceClear, serialPoll));

		// clear status words
			m_ibsta &= (RQS);	// preserve SRQ
			m_ibcntl = 0;
			m_iberr = 0;

		// send header
			headerBuf[0] = OPERATION_DATA;
			if(eoiTermination)
				headerBuf[0] |= OPERATION_EOI;
			if(m_remoteMode)
				headerBuf[0] |= OPERATION_REMOTE;
			if(deviceClear)
				headerBuf[0] |= OPERATION_CLEAR;
			if(serialPoll)
				headerBuf[0] |= OPERATION_REQSERIALPOLL;
			headerBuf[1] = HEADER_VERSION1;
			headerBuf[2] = GetNextSequenceNumber(headerBuf[0]);			// sequence number
			headerBuf[3] = 0x00;										// unused
			*((unsigned long *) &headerBuf[4]) = htonl(bytesToSend);	// message size

			ATLTRACE("sendDataToDevice: seq=%d eoi=%d ", headerBuf[2], eoiTermination);

			bytesSent = send(m_socketFd, (char *) headerBuf, IO_NET_HEADER_SIZE, 0);
			if (bytesSent != IO_NET_HEADER_SIZE)
			{
				sprintf(m_lastErrorMsg, "Could not send Net Header\n");
				m_bErrorFlag = true;
				m_ibsta |= ERR;
				ATLTRACE(" bytesSent=%d [%.20s] ERROR sending header\n", bytesSent, message);
				return(FALSE);
			}

		// send contents of message
			bytesSent = send(m_socketFd, message, bytesToSend, 0);
			if(bytesSent == ERROR || bytesSent != bytesToSend)
			{
				sprintf(m_lastErrorMsg, "Send error\n");
				m_bErrorFlag = true;
				m_ibsta |= ERR;
				ATLTRACE(" bytesSent=%d [%.20s] ERROR sending data\n", bytesSent, message);
				return(FALSE);
			}
			else
			{
				ATLTRACE(" bytesSent=%d [%.20s]\n", bytesSent, message);
				m_ibsta = CMPL | CIC | TACS;
				m_ibcntl = bytesSent;
			}

		return(TRUE);
	}

private:
	//------------------------------------------------------------------------------------------
	// Return the next sequence number in the range 1..255 (Note that zero is omitted intentionally)
	// used to synchronize write/read operations, attempting to emulate the 488.2 'discard unread response'
	// behaviour
	unsigned char GetNextSequenceNumber(unsigned char flags)
	{
		// we'll return the current sequence number
		m_lastSequenceNumber = m_nextSequenceNumber;

		// which then gets incremented if this block is EOI terminated
		if(flags & OPERATION_EOI)
		{
			++m_nextSequenceNumber;
			if(m_nextSequenceNumber >= 256)
				m_nextSequenceNumber = 1;
		}

		return m_lastSequenceNumber;
	}

	//------------------------------------------------------------------------------------------
	// return the last-used sequence number without incrementing it
	unsigned char GetLastSequenceNumber()
	{
		return m_lastSequenceNumber;
	}

	//------------------------------------------------------------------------------------------
	// send a 'small' block of data to a network device
	// returns TRUE on error status
	BOOL sendSmallDataToDevice(char *message, int bytesToSend, BOOL eoiTermination, BOOL deviceClear, BOOL serialPoll)
	{
		static unsigned char smallDataBuffer[SMALL_DATA_BUFSIZE + IO_NET_HEADER_SIZE + 2];
		int bytesSent;
		int bytesToSendWithHeader = bytesToSend + IO_NET_HEADER_SIZE;

		// copy message into data buffer
			memcpy(&smallDataBuffer[IO_NET_HEADER_SIZE], message, bytesToSend);

		// clear status words
			m_ibsta &= (RQS);	// preserve SRQ
			m_ibcntl = 0;
			m_iberr = 0;

		// send header + data
			smallDataBuffer[0] = OPERATION_DATA;
			if(eoiTermination)
				smallDataBuffer[0] |= OPERATION_EOI;
			if(m_remoteMode)
				smallDataBuffer[0] |= OPERATION_REMOTE;
			if(deviceClear)
				smallDataBuffer[0] |= OPERATION_CLEAR;
			if(serialPoll)
				smallDataBuffer[0] |= OPERATION_REQSERIALPOLL;
			smallDataBuffer[1] = HEADER_VERSION1;
			smallDataBuffer[2] = GetNextSequenceNumber(smallDataBuffer[0]);		// sequence number
			smallDataBuffer[3] = 0x00;											// unused
			*((unsigned long *) &smallDataBuffer[4]) = htonl(bytesToSend);		// message size

			ATLTRACE("sendSmallDataToDevice: seq=%d eoi=%d ", smallDataBuffer[2], eoiTermination);

			bytesSent = send(m_socketFd, (char *) smallDataBuffer, bytesToSendWithHeader, 0);
			if (bytesSent != bytesToSendWithHeader)
			{
				sprintf(m_lastErrorMsg, "Could not send small data block (Header + Data)\n");
				m_bErrorFlag = true;
				m_ibsta |= ERR;
				ATLTRACE(" bytesSent=%d [%.20s] ERROR\n", bytesSent, message);
				return(FALSE);
			}
			else
			{
				ATLTRACE(" bytesSent=%d [%.20s]\n", bytesSent, message);
				m_ibsta = CMPL | CIC | TACS;
				m_ibcntl = bytesToSend;
				return(TRUE);
			}
	}

public:
	//------------------------------------------------------------------------------------------
	// dump data until the next header is found
	// TODO: Handle timeout cases
	void dumpData(int numBytes)
	{
		sprintf(m_lastErrorMsg, "dumpData: Unread Response, dumping %d bytes\n", numBytes);
		ATLTRACE(m_lastErrorMsg);

		DWORD bytesToGo = numBytes;
		char *dumpBuf = (char *) malloc(m_maxBlockSize);
		if(dumpBuf != NULL)
		{
			while(bytesToGo > 0)
			{
				int dumpBytesReceived = (DWORD)recv(m_socketFd, dumpBuf, min(bytesToGo, (DWORD)m_maxBlockSize), 0);
				bytesToGo -= dumpBytesReceived;
			}
			free(dumpBuf);
		}
	}

	//------------------------------------------------------------------------------------------
	// read block of data from a network device
	// if bFlush is requested then ignore replyBuf and userBufferSizeBytes and read all remaining data
	// from the current block (i.e. up to the start of the next header)
	DWORD readDataFromDevice(char *replyBuf, int userBufferSizeBytes, bool bFlush = false)
	{
		static DWORD srcBlockSizeBytes = 0, srcBlockBytesRead = 0;
		static BOOL srcBlockEOITerminated = FALSE, srcBlockSRQStateChanged = FALSE;
		int userBufferBytesRead = 0;					// # bytes placed in user buffer
		DWORD bytesReceived = 0;
		char *bufCopy = replyBuf;

		// clear status words
			m_ibsta &= (RQS);	// preserve SRQ
			m_ibcntl = m_iberr = 0;

		// ensure that the reply buffer is empty (if supplied)
			if(replyBuf != NULL)
				*replyBuf = '\0';
			
		while(1)
		{
			if(m_readState == NetWaitingForHeader)
			{
				int seqNum = -1;
				if(readHeaderFromDevice(srcBlockSizeBytes, srcBlockEOITerminated, srcBlockSRQStateChanged, seqNum))
				{
					// header was successfully read
 					ATLTRACE("readDataFromDevice: Read Header: blockSize=%d, EOI=%d, userBufferSizeBytes=%d\n", srcBlockSizeBytes, srcBlockEOITerminated, userBufferSizeBytes);
					m_readState = NetWaitingForData;
					srcBlockBytesRead = 0;

					// if we're flushing unread responses, and this header contains an unexpected sequence number (older than
					// the current one), then dump this block and go around again. 
					// note that the 'seqNum != 0' test checks for the case where we're talking to a scope running pre-June 2003
					// code that didn't support sequence numbering, and therefore we don't know when to dump data.
					if(m_bFlushUnreadResponses && seqNum != 0 && (GetLastSequenceNumber() > seqNum))
					{
						dumpData(srcBlockSizeBytes);	
						m_readState = NetWaitingForHeader;
					}

					// if a non-zero sequence number was seen then assume that version '1a' of the
					// VICP protocol is in use, supporting, in addition to sequence numbering, 
					// the use of Out-of-band signalling.
					if(seqNum != 0)
						m_bVICPVersion1aSupported = true;
					else
						m_bVICPVersion1aSupported = false;		// seq. numbers should never be zero in V1a of the protocol
				}
				else 
				{
					// header was not successfully read, probably indicates a timeout
 					//ATLTRACE("readDataFromDevice Timeout reading header\n");

					// let the caller know that a timeout occured
					m_ibsta |= ERR;
					m_ibsta |= TIMO;
					break;
				}
			}

			if(m_readState == NetWaitingForData)
			{
				// dump any unread partial buffer if requested
					if(bFlush)
					{
						dumpData(srcBlockSizeBytes - srcBlockBytesRead);	
						m_readState = NetWaitingForHeader;
						break;
					}

				// fill the user-supplied buffer
					DWORD bytesToGo = min((DWORD)userBufferSizeBytes - userBufferBytesRead,  // space free in user Buffer
										srcBlockSizeBytes - srcBlockBytesRead);				 // src bytes available
					while(bytesToGo > 0)
					{
						bytesReceived = (DWORD)recv(m_socketFd, replyBuf, min(bytesToGo, (DWORD)m_maxBlockSize), 0);

						if(bytesReceived == SOCKET_ERROR)
						{
							sprintf(m_lastErrorMsg, "SOCKET_ERROR on recv\n");
							m_bErrorFlag = true;
							m_readState = NetErrorState;

							// let the caller know that a timeout occured
							m_ibsta |= ERR;
							m_ibsta |= TIMO;
							break;
						}

						if(bytesReceived > 0)
						{
							userBufferBytesRead += bytesReceived;
							bytesToGo -= bytesReceived;
							replyBuf += bytesReceived;
							srcBlockBytesRead += bytesReceived;
						}
					}

				// if we have finished reading the contents of this header-prefixed block, 
				// then go back to the state where we can watch for the next block
					if(srcBlockBytesRead >= srcBlockSizeBytes)
					{
						m_readState = NetWaitingForHeader;

						if (srcBlockSRQStateChanged)	// update SRQ status bits, discard SRQ packet
						{
							if (bufCopy[0] == '1')		// 1 = SRQ asserted, 0 = SRQ deasserted
								m_ibsta |= RQS;
							else
								m_ibsta &= ~(RQS);		// clear SRQ

							// discard SRQ data in buffer
							userBufferBytesRead -= bytesReceived;
							replyBuf -= bytesReceived;		
							srcBlockBytesRead -= bytesReceived;

							ATLTRACE("SRQ Packet Discarded  '%c'\n", bufCopy[0]);
							continue;					// go around the loop again (discard SRQ packet)
						}
						
						// go around the loop again unless the last block was EOI terminated
						if(srcBlockEOITerminated)
						{
							m_ibsta |= END;
							break;
						}
					}

					// if there is space left in the user's buffer, go around again
					if(userBufferBytesRead >= userBufferSizeBytes)
						break;
			}

			if(m_readState == NetErrorState)
			{
				// when we come back in here, enter the 'waiting for header' state
				m_readState = NetWaitingForHeader;

				break;
			}

		}

		// keep track of size of last transfer
			m_ibcntl = userBufferBytesRead;
			
 		//ATLTRACE("readDataFromDevice: returning %d bytes\n", userBufferBytesRead);

		return(m_ibcntl);
	}

	//------------------------------------------------------------------------------------------
	// read header a network device
	BOOL readHeaderFromDevice(DWORD &blockSize, BOOL &eoiTerminated, BOOL &srqStateChanged, int &seqNum)
	{
		static unsigned char headerBuf[IO_NET_HEADER_SIZE];
		int recvHeaderLen = 0;
		fd_set readSet = { 1, { m_socketFd } };	
		TIMEVAL timeVal = { (long) m_currentTimeout, ((long) (m_currentTimeout * 1000000L)) % 1000000L};

		// if there is no sign of a header, get out quick (timeout)
			int numReady = select(m_socketFd, &readSet, NULL, NULL, &timeVal);
			if(numReady != 1)
				return(FALSE);

		// wait until 8 bytes are available (the entire header)
			int headerBytesRead = 0, bytesThisTime;
			while(headerBytesRead < 8)
			{
				// ensure that the recv command will not block
					int numReady = select(m_socketFd, &readSet, NULL, NULL, &timeVal);
					if(numReady != 1)
						break;

				// try to read the remainder of the header
					bytesThisTime = recv(m_socketFd, (char *) headerBuf + headerBytesRead, IO_NET_HEADER_SIZE - headerBytesRead, 0);
					if(bytesThisTime > 0)
						headerBytesRead += bytesThisTime;

				// if we get this far without reading any part of the header, get out
					if(headerBytesRead == 0)
						break;
			}

		// receive the scope's response, header first 
			if(headerBytesRead == 8)
			{
				// extract the number of bytes contained in this packet
					blockSize = ntohl(*((unsigned long *) &headerBuf[4]));

				// check the integrity of the header
					if(!((headerBuf[0] & OPERATION_DATA) && (headerBuf[1] == HEADER_VERSION1)))
					{
						sprintf(m_lastErrorMsg, "Invalid Header!\n");
						m_bErrorFlag = true;

						// error state, cannot recognise header. since we are 
						// out of sync, need to close & reopen the socket
						disconnectFromDevice();
						connectToDevice();
						return(FALSE);
					}

				// inform the caller of the EOI and SRQ state
					if(headerBuf[0] & OPERATION_EOI)
						eoiTerminated = TRUE;
					else
						eoiTerminated = FALSE;

					if(headerBuf[0] & OPERATION_SRQ)
						srqStateChanged = TRUE;
					else
						srqStateChanged = FALSE;

				// inform the caller of the received sequence number
					seqNum = headerBuf[2];

				ATLTRACE("readHeaderFromDevice: seq=%d\n", headerBuf[2]);
			}
			else
			{
				// error state, cannot read header. since we are out of sync, 
				// need to close & reopen the socket
				disconnectFromDevice();
				connectToDevice();
				return(FALSE);
			}

		return(TRUE);
	}

public:
	char m_deviceAddress[MAX_DEVICE_ADDR_LEN];
	float m_currentTimeout;					// current timeout time (seconds)
	static BOOL m_oneTimeWinsockInit;		// !! one copy for all instances of CVICPClient !!
	BOOL m_remoteMode;						// if TRUE, device is in remote mode
	BOOL m_localLockout;					// if TRUE, device is in local lockout mode
	BOOL m_connectedToScope;				// connected to scope?
	SOCKADDR_IN	m_serverAddr;				// server's socket address 
	int	m_socketFd;							// socket file descriptor
	int m_iberr;							// emulation of GPIB counterparts
	int m_ibsta;							// emulation of GPIB counterparts
	long m_ibcntl;							// emulation of GPIB counterparts
	int m_maxBlockSize;						// max # bytes that may be read in one go by recv
	READSTATE m_readState;					// current state of read 'state machine'	
	bool m_bFlushUnreadResponses;			// if true, unread responses are flushed (emulate GPIB 488.2 behaviour) 
	char m_lastErrorMsg[MAX_ERROR_MSG_LEN];	// last error message
	bool m_bErrorFlag;						// if true, error has been observed
	bool m_bVICPVersion1aSupported;			// version 1a of the VICP protocol supported (seq. numbers and OOB data)

private:
	int m_lastSequenceNumber;				// last used sequence value
	int m_nextSequenceNumber;				// next sequence value
};
