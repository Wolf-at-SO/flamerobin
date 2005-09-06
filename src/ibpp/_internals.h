///////////////////////////////////////////////////////////////////////////////
//
//	File    : $Id$
//	Subject : IBPP internal declarations
//
///////////////////////////////////////////////////////////////////////////////
//
//	The contents of this file are subject to the Mozilla Public License
//	Version 1.0 (the "License"); you may not use this file except in
//	compliance with the License. You may obtain a copy of the License at
//	http://www.mozilla.org/MPL/
//
//	Software distributed under the License is distributed on an "AS IS"
//	basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
//	License for the specific language governing rights and limitations
//	under the License.
//
//	The Original Code is "IBPP 0.9" and all its associated documentation.
//
//	The Initial Developer of the Original Code is T.I.P. Group S.A.
//	Portions created by T.I.P. Group S.A. are
//	Copyright (C) 2000 T.I.P Group S.A.
//	All Rights Reserved.
//
//	Contributor(s): ______________________________________.
//
///////////////////////////////////////////////////////////////////////////////
//
//	COMMENTS
//
//	* 'Internal declarations' means everything used to implement ibpp. This
//	  file and its contents is NOT needed by users of the library. All those
//	  declarations are wrapped in a namespace : 'ibpp_internals'.
//	* Tabulations should be set every four characters when editing this file.
//
///////////////////////////////////////////////////////////////////////////////

#if defined(IBPP_BCC) || defined(IBPP_MSVC) || defined(IBPP_DMC)
#define HAS_HDRSTOP
#endif

#if (defined(IBPP_GCC) && defined(IBPP_WINDOWS))
//	Setting flags for ibase.h -- using GCC/Cygwin/MinGW on Win32
#ifndef _MSC_VER
#define _MSC_VER 1
#endif
#ifndef _WIN32
#define _WIN32   1
#endif
#endif

#include "ibase.h"		// From Firebird 1.x or InterBase 6.x installation

#if (defined(IBPP_GCC) && defined(IBPP_WINDOWS))
//	UNSETTING flags used above for ibase.h -- Huge conflicts with libstdc++ !
#undef _MSC_VER
#undef _WIN32
#endif

#ifdef IBPP_WINDOWS
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <sstream>

#ifdef _DEBUG
#define ASSERTION(x)	{if (!(x)) {throw LogicExceptionImpl("ASSERTION", \
							"'"#x"' is not verified at %s, line %d", \
								__FILE__, __LINE__);}}
#else
#define ASSERTION(x)	/* x */
#endif

namespace ibpp_internals
{

enum flush_debug_stream_type {fds};

#ifdef _DEBUG

struct DebugStream : public std::stringstream
{
	// next two operators fix some g++ and vc++ related problems
	std::ostream& operator<< (const char* p)
		{ static_cast<std::stringstream&>(*this)<< p; return *this; }

	std::ostream& operator<< (const std::string& p)
		{ static_cast<std::stringstream&>(*this)<< p; return *this; }
};
std::ostream& operator<< (std::ostream& a, flush_debug_stream_type);

#else

struct DebugStream
{
	template<class T> DebugStream& operator<< (const T&) { return *this; }
	// for manipulators
	DebugStream& operator<< (std::ostream&(*)(std::ostream&)) { return *this; }
};

#endif	// _DEBUG

class DatabaseImpl;
class TransactionImpl;
class StatementImpl;
class BlobImpl;
class ArrayImpl;

//	Native data types
typedef enum {ivArray, ivBlob, ivDate, ivTime, ivTimestamp, ivString,
			ivShort, ivLong, ivLarge, ivFloat, ivDouble,
			ivBool, ivDBKey, ivByte} IITYPE;

//
//	Those are the Interbase C API prototypes that we use
//	Taken 'asis' from IBASE.H, prefix 'isc_' replaced with 'proto_',
//	and 'typedef' preprended...
//

typedef ISC_STATUS ISC_EXPORT proto_create_database (ISC_STATUS *, 
					    short, 
					    char *, 
					    isc_db_handle *, 
					    short, 
					    char *, 
					    short);

typedef ISC_STATUS ISC_EXPORT proto_attach_database (ISC_STATUS *, 
					    short, 
					    char *, 
					    isc_db_handle *, 
					    short, 
					    char *);

typedef ISC_STATUS  ISC_EXPORT proto_detach_database (ISC_STATUS *,  
					    isc_db_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_drop_database (ISC_STATUS *,  
					  isc_db_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_database_info (ISC_STATUS *, 
					  isc_db_handle *, 
					  short, 
					  char *, 
					  short, 
					  char *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_execute_immediate (ISC_STATUS *, 
						   isc_db_handle *, 
						   isc_tr_handle *, 
						   unsigned short, 
						   char *, 
						   unsigned short, 
						   XSQLDA *);

typedef ISC_STATUS  ISC_EXPORT proto_open_blob2 (ISC_STATUS *, 
				       isc_db_handle *, 
				       isc_tr_handle *,
				       isc_blob_handle *, 
				       ISC_QUAD *, 
				       short,  
				       char *);

typedef ISC_STATUS  ISC_EXPORT proto_create_blob2 (ISC_STATUS *, 
					isc_db_handle *, 
					isc_tr_handle *, 
					isc_blob_handle *, 
					ISC_QUAD *,
					short,
					char *);

typedef ISC_STATUS  ISC_EXPORT proto_close_blob (ISC_STATUS *, 
				       isc_blob_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_cancel_blob (ISC_STATUS *, 
				        isc_blob_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_get_segment (ISC_STATUS *, 
				        isc_blob_handle *, 
				        unsigned short *, 
				        unsigned short, 
				        char *);

typedef ISC_STATUS  ISC_EXPORT proto_put_segment (ISC_STATUS *, 
					isc_blob_handle *, 
					unsigned short, 
					char *);

typedef ISC_STATUS  ISC_EXPORT proto_blob_info (ISC_STATUS *, 
				      isc_blob_handle *, 
				      short,
 				      char *, 
				      short, 
				      char *);

typedef ISC_STATUS  ISC_EXPORT proto_array_lookup_bounds (ISC_STATUS *, 
						isc_db_handle *, 
						isc_tr_handle *, 
						char *,
						char *, 
						ISC_ARRAY_DESC *);

typedef ISC_STATUS  ISC_EXPORT proto_array_get_slice (ISC_STATUS *, 
					    isc_db_handle *, 
					    isc_tr_handle *, 
					    ISC_QUAD *, 
					    ISC_ARRAY_DESC *, 
					    void *, 
					    ISC_LONG *);

typedef ISC_STATUS  ISC_EXPORT proto_array_put_slice (ISC_STATUS *, 
					    isc_db_handle *, 
					    isc_tr_handle *, 
					    ISC_QUAD *, 
					    ISC_ARRAY_DESC *, 
					    void *, 
					    ISC_LONG *);

typedef ISC_LONG    ISC_EXPORT proto_vax_integer (char *, 
					short);

typedef ISC_LONG    ISC_EXPORT proto_sqlcode (ISC_STATUS *);

typedef void        ISC_EXPORT proto_sql_interprete (short, 
					   char *, 
					   short);

typedef ISC_STATUS  ISC_EXPORT proto_interprete (char *, 
				       ISC_STATUS * *);

typedef ISC_STATUS  ISC_EXPORT proto_que_events (ISC_STATUS *, 
				       isc_db_handle *, 
				       ISC_LONG *, 
				       short, 
				       char *, 
				       isc_callback, 
				       void *);

typedef ISC_STATUS  ISC_EXPORT proto_cancel_events (ISC_STATUS *, 
					  isc_db_handle *, 
					  ISC_LONG *);

typedef ISC_STATUS  ISC_EXPORT proto_start_multiple (ISC_STATUS *, 
					   isc_tr_handle *, 
					   short, 
					   void *);

typedef ISC_STATUS  ISC_EXPORT proto_commit_transaction (ISC_STATUS *, 
					       isc_tr_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_commit_retaining (ISC_STATUS *, 
					     isc_tr_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_rollback_transaction (ISC_STATUS *, 
						 isc_tr_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_rollback_retaining (ISC_STATUS *, 
						 isc_tr_handle *);

///////////
typedef ISC_STATUS  ISC_EXPORT proto_dsql_allocate_statement (ISC_STATUS *, 
						    isc_db_handle *, 
						    isc_stmt_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_describe (ISC_STATUS *, 
					  isc_stmt_handle *, 
					  unsigned short, 
					  XSQLDA *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_describe_bind (ISC_STATUS *, 
					       isc_stmt_handle *, 
					       unsigned short, 
					       XSQLDA *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_execute (ISC_STATUS *, 
					 isc_tr_handle *,
					 isc_stmt_handle *, 
					 unsigned short, 
					 XSQLDA *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_execute2 (ISC_STATUS *, 
					  isc_tr_handle *,
					  isc_stmt_handle *, 
					  unsigned short, 
					  XSQLDA *,
					  XSQLDA *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_fetch (ISC_STATUS *, 
				       isc_stmt_handle *, 
				       unsigned short, 
				       XSQLDA *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_free_statement (ISC_STATUS *, 
						isc_stmt_handle *, 
						unsigned short);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_prepare (ISC_STATUS *, 
					 isc_tr_handle *, 
					 isc_stmt_handle *, 
					 unsigned short, 
					 char *, 
					 unsigned short, 
				 	 XSQLDA *);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_set_cursor_name (ISC_STATUS *, 
						 isc_stmt_handle *, 
						 char *, 
						 unsigned short);

typedef ISC_STATUS  ISC_EXPORT proto_dsql_sql_info (ISC_STATUS *, 
					  isc_stmt_handle *, 
					  short, 
					  char *, 
					  short, 
					  char *);

typedef void        ISC_EXPORT proto_decode_date (ISC_QUAD *, 
					void *);

typedef void        ISC_EXPORT proto_encode_date (void *, 
					ISC_QUAD *);

typedef int			ISC_EXPORT proto_add_user (ISC_STATUS *, USER_SEC_DATA *);
typedef int			ISC_EXPORT proto_delete_user (ISC_STATUS *, USER_SEC_DATA *);
typedef int			ISC_EXPORT proto_modify_user (ISC_STATUS *, USER_SEC_DATA *);

//
//	Those API are only available in versions 6.x of the GDS32.DLL
//

typedef ISC_STATUS  ISC_EXPORT proto_service_attach (ISC_STATUS *, 
					   unsigned short, 
					   char *,
					   isc_svc_handle *, 
					   unsigned short, 
					   char *);

typedef ISC_STATUS  ISC_EXPORT proto_service_detach (ISC_STATUS *, 
					   isc_svc_handle *);

typedef ISC_STATUS  ISC_EXPORT proto_service_query (ISC_STATUS *, 
					  isc_svc_handle *,
                      		          isc_resv_handle *,
					  unsigned short, 
					  char *, 
					  unsigned short, 
					  char *, 
					  unsigned short, 
					  char *);

typedef ISC_STATUS ISC_EXPORT proto_service_start (ISC_STATUS *,
    					 isc_svc_handle *,
                         		 isc_resv_handle *,
    					 unsigned short,
    					 char*);

typedef void        ISC_EXPORT proto_decode_sql_date (ISC_DATE *, 
					void *);

typedef void        ISC_EXPORT proto_decode_sql_time (ISC_TIME *, 
					void *);

typedef void        ISC_EXPORT proto_decode_timestamp (ISC_TIMESTAMP *, 
					void *);

typedef void        ISC_EXPORT proto_encode_sql_date (void *, 
					ISC_DATE *);

typedef void        ISC_EXPORT proto_encode_sql_time (void *, 
					ISC_TIME *);

typedef void        ISC_EXPORT proto_encode_timestamp (void *, 
					ISC_TIMESTAMP *);

//
//	Internal binding structure to the GDS32 DLL
//

struct GDS
{
	// Attributes
#ifdef IBPP_WINDOWS
	HMODULE mHandle;		// The GDS32.DLL HMODULE, if it could be loaded
#endif	
	bool mReady;
	int mGDSVersion; 		// Version of the GDS32.DLL (50 for 5.0, 60 for 6.0)

	GDS* Call(void);

	// GDS32 Entry Points
	proto_create_database*			m_create_database;
	proto_attach_database*			m_attach_database;
	proto_detach_database*			m_detach_database;
	proto_drop_database*			m_drop_database;
	proto_database_info*			m_database_info;
	proto_dsql_execute_immediate*	m_dsql_execute_immediate;
	proto_open_blob2*				m_open_blob2;
	proto_create_blob2*				m_create_blob2;
	proto_close_blob*				m_close_blob;
	proto_cancel_blob*				m_cancel_blob;
	proto_get_segment*				m_get_segment;
	proto_put_segment*				m_put_segment;
	proto_blob_info*				m_blob_info;
	proto_array_lookup_bounds*		m_array_lookup_bounds;
	proto_array_get_slice*			m_array_get_slice;
	proto_array_put_slice*			m_array_put_slice;

	proto_vax_integer*				m_vax_integer;
	proto_sqlcode*					m_sqlcode;
	proto_sql_interprete*			m_sql_interprete;
	proto_interprete*				m_interprete;
	proto_que_events*				m_que_events;
	proto_cancel_events* 			m_cancel_events;
	proto_start_multiple*			m_start_multiple;
	proto_commit_transaction*		m_commit_transaction;
	proto_commit_retaining*			m_commit_retaining;
	proto_rollback_transaction*		m_rollback_transaction;
	proto_rollback_retaining*		m_rollback_retaining;
	proto_dsql_allocate_statement*	m_dsql_allocate_statement;
	proto_dsql_describe*			m_dsql_describe;
	proto_dsql_describe_bind*		m_dsql_describe_bind;
	proto_dsql_prepare*				m_dsql_prepare;
	proto_dsql_execute*				m_dsql_execute;
	proto_dsql_execute2*			m_dsql_execute2;
	proto_dsql_fetch*				m_dsql_fetch;
	proto_dsql_free_statement*		m_dsql_free_statement;
	proto_dsql_set_cursor_name*		m_dsql_set_cursor_name;
	proto_dsql_sql_info* 			m_dsql_sql_info;
	proto_decode_date*				m_decode_date;
	proto_encode_date*				m_encode_date;
	proto_add_user*					m_add_user;
	proto_delete_user*				m_delete_user;
	proto_modify_user*				m_modify_user;

	proto_service_attach*			m_service_attach;
	proto_service_detach*			m_service_detach;
	proto_service_start*			m_service_start;
	proto_service_query*			m_service_query;
	proto_decode_sql_date*			m_decode_sql_date;
	proto_decode_sql_time*			m_decode_sql_time;
	proto_decode_timestamp*			m_decode_timestamp;
	proto_encode_sql_date*			m_encode_sql_date;
	proto_encode_sql_time*			m_encode_sql_time;
	proto_encode_timestamp*			m_encode_timestamp;

	// Constructor (No need for a specific destructor)
	GDS()	{ mReady = false; };
};

extern GDS gds;

//
//	Service Parameter Block (used to define a service)
//

class SPB
{
	char* mBuffer;				// Dynamically allocated SPB structure
	int mSize;  				// Its used size in bytes
	int mAlloc;					// Its allocated size in bytes

	void Grow(int needed);		// Alloc or grow the mBuffer

public:
	void Insert(char);			// Insert a single byte code
	void InsertString(char, int, const char*);	// Insert a string, len can be defined as 1 or 2 bytes
	void InsertByte(char type, char data);
	void InsertQuad(char type, int data);
	void Reset(void);			// Clears the SPB
	char* Self(void) { return mBuffer; }
	short Size(void) { return short(mSize); }

	SPB() : mBuffer(0), mSize(0), mAlloc(0) { }
	~SPB() { Reset(); }
};

//
//	Database Parameter Block (used to define a database)
//

class DPB
{
	char* mBuffer;				// Dynamically allocated DPB structure
	int mSize;  				// Its used size in bytes
	int mAlloc;					// Its allocated size in bytes
	
	void Grow(int needed);		// Allocate or grow the mBuffer, so that
								// 'needed' bytes can be written (at least)
	
public:
	void Insert(char, const char*);	// Insert a new char* 'cluster'
	void Insert(char, short);		// Insert a new short 'cluster'
	void Insert(char, bool);   		// Insert a new bool 'cluster'
	void Insert(char, char);   		// Insert a new byte 'cluster'
	void Reset(void);				// Clears the DPB
	char* Self(void) { return mBuffer; }
	short Size(void) { return short(mSize); }

	DPB() : mBuffer(0), mSize(0), mAlloc(0) { }
	~DPB() { Reset(); }
};

//
//	Transaction Parameter Block (used to define a transaction)
//

class TPB
{
	char* mBuffer;					// Dynamically allocated TPB structure
	int mSize;						// Its used size in bytes
	int mAlloc;						// Its allocated size

	void Grow(int needed);			// Alloc or re-alloc the mBuffer

public:
	void Insert(char);				// Insert a flag item
	void Insert(const std::string& data); // Insert a string (typically table name)
	void Reset(void);				// Clears the TPB
	char* Self(void) { return mBuffer; }
	short Size(void) { return short(mSize); }

	TPB() : mBuffer(0), mSize(0), mAlloc(0) { }
	~TPB() { Reset(); }
};

//
//	Events Handling. EPB stands for Events Paramater Block, but it really
//	encapsulate all the management of a list of events to wait for.
//

template<class It>
struct EventBufferIterator
{
	It mIt;

public:
	EventBufferIterator& operator++()
		{ mIt += 1 + static_cast<int>(*mIt) + 4; return *this; }

	bool operator == (const EventBufferIterator& i) const { return i.mIt == mIt; }
	bool operator != (const EventBufferIterator& i) const { return i.mIt != mIt; }

#ifdef IBPP_BCC
#pragma warn -8027
#endif
	std::string get_name() const
		{ return std::string(mIt + 1, mIt + 1 + static_cast<int>(*mIt)); }
#ifdef IBPP_BCC
#pragma warn .8027
#endif

	unsigned int get_count() const
	{
		return (*gds.Call()->m_vax_integer)
			(const_cast<char*>(&*(mIt + 1 + static_cast<int>(*mIt))), 4);
	}

	It iterator() { return mIt; }

	EventBufferIterator() {}
	EventBufferIterator(It it) : mIt(it) {}
};

class EPB
{
	typedef std::vector<IBPP::EventInterface*> ObjRefs;
	ObjRefs mObjectReferences;

	typedef std::vector<char> Buffer;
	Buffer mEventBuffer;
	Buffer mResultsBuffer;

	EPB& operator = (const EPB&);
	EPB(const EPB&);

public:
	// Define (add) an event to the list
	void Define(const std::string&, IBPP::EventInterface*);
	void FireActions(IBPP::IDatabase *);
	char* EventsBuffer(void) { return &mEventBuffer[0]; }
	char* ResultsBuffer(void) { return &mResultsBuffer[0]; }
	int Size(void) const { return (int)mEventBuffer.size(); }

	EPB() {}
};

//
//	Used to receive (and process) a results buffer in various API calls
//

class RB
{
	char* mBuffer;
	short mSize;

	char* FindToken(char token);
	char* FindToken(char token, char subtoken);

public:
	void Reset(void);
	int GetValue(char token);
	int GetCountValue(char token);
	int GetValue(char token, char subtoken);
	bool GetBool(char token);
	int GetString(char token, std::string& data);

	char* Self(void) { return mBuffer; }
	short Size(void) { return mSize; }

	RB();
	RB(int Size);
	~RB();
};

//
//	Used to receive status info from API calls
//

class IBS
{
	mutable ISC_STATUS mVector[20];
	mutable std::string mMessage;

public:
	ISC_STATUS* Self(void) { return mVector; }
	bool Errors(void) { return (mVector[0] == 1 && mVector[1] > 0) ? true : false; }
	const char* ErrorMessage(void) const;
	int SqlCode(void) const;
	int EngineCode(void) const { return (mVector[0] == 1) ? mVector[1] : 0; }
	void Reset(void);

	IBS();
	IBS(IBS&);	// Copy Constructor
	~IBS();
};

///////////////////////////////////////////////////////////////////////////////
//
//	Implementation of the "hidden" classes associated with their public
//	counterparts. Their private data and methods can freely change without
//	breaking the compatibility of the DLL. If they receive new public methods,
//	and those methods are reflected in the public class, then the compatibility
//	is broken.
//
///////////////////////////////////////////////////////////////////////////////

//
// Hidden implementation of Exception classes.
//

/*
                         std::exception
                                |
                         IBPP::Exception
                       /                 \
                      /                   \
  IBPP::LogicException    ExceptionBase    IBPP::SQLException
                 \            /      \       /
               LogicExceptionImpl    SQLExceptionImpl
*/

class ExceptionBase
{
	//	(((((((( OBJECT INTERNALS ))))))))

protected:
	std::string mContext; 			// Exception context ("IDatabase::Drop")
	std::string mWhat;				// Full formatted message

	void buildErrorMessage(const char* message);
	void raise(const std::string& context, const char* message, va_list argptr);

public:
	// The following constructors are small and could be inlined, but for object
	// code compacity of the library it is much better to have them non-inlined.
	// The amount of code generated by compilers for a throw is well-enough.

	ExceptionBase() throw();
	ExceptionBase(const ExceptionBase& copied) throw();
	ExceptionBase& operator=(const ExceptionBase& copied) throw();
	ExceptionBase(const std::string& context, const char* message = 0, ...) throw();

	virtual ~ExceptionBase() throw();

	//	(((((((( OBJECT INTERFACE ))))))))

    virtual const char* Origin() const throw();
    virtual const char* ErrorMessage() const throw();
	virtual const char* what() const throw();
};

class LogicExceptionImpl : public IBPP::LogicException, public ExceptionBase
{
	//	(((((((( OBJECT INTERNALS ))))))))

public:
	// The following constructors are small and could be inlined, but for object
	// code compacity of the library it is much better to have them non-inlined.
	// The amount of code generated by compilers for a throw is well-enough.

	LogicExceptionImpl() throw();
	LogicExceptionImpl(const LogicExceptionImpl& copied) throw();
	LogicExceptionImpl& operator=(const LogicExceptionImpl& copied) throw();
	LogicExceptionImpl(const std::string& context, const char* message = 0, ...) throw();

	virtual ~LogicExceptionImpl() throw ();

	//	(((((((( OBJECT INTERFACE ))))))))
	//
	//	The object public interface is partly implemented by inheriting from
	//	the ExceptionBase class.

public:
    virtual const char* Origin() const throw();
    virtual const char* ErrorMessage() const throw();
	virtual const char* what() const throw();
};

class SQLExceptionImpl : public IBPP::SQLException, public ExceptionBase
{
	//	(((((((( OBJECT INTERNALS ))))))))

private:
	int mSqlCode;
	int mEngineCode;

public:
	// The following constructors are small and could be inlined, but for object
	// code compacity of the library it is much better to have them non-inlined.
	// The amount of code generated by compilers for a throw is well-enough.

	SQLExceptionImpl() throw();
	SQLExceptionImpl(const SQLExceptionImpl& copied) throw();
	SQLExceptionImpl& operator=(const SQLExceptionImpl& copied) throw();
	SQLExceptionImpl(const IBS& status, const std::string& context,
						const char* message = 0, ...) throw();

	virtual ~SQLExceptionImpl() throw ();

	//	(((((((( OBJECT INTERFACE ))))))))
	//
	//	The object public interface is partly implemented by inheriting from
	//	the ExceptionBase class.

public:
    virtual const char* Origin() const throw();
    virtual const char* ErrorMessage() const throw();
	virtual const char* what() const throw();
	virtual int SqlCode(void) const throw();
	virtual int EngineCode(void) const throw();
};

class ServiceImpl : public IBPP::IService
{
	//	(((((((( OBJECT INTERNALS ))))))))

private:
	int mRefCount;				// Reference counter
    isc_svc_handle mHandle;		// InterBase API Service Handle
	std::string mServerName;	// Nom du serveur
    std::string mUserName;		// Nom de l'utilisateur
    std::string mUserPassword;	// Mot de passe de l'utilisateur
	std::string mWaitMessage;	// Progress message returned by WaitMsg()

	isc_svc_handle* GetHandlePtr(void) { return &mHandle; }
	void SetServerName(const char*);
	void SetUserName(const char*);
	void SetUserPassword(const char*);

public:
	isc_svc_handle GetHandle(void) { return mHandle; }

	ServiceImpl(const std::string& ServerName, const std::string& UserName,
					const std::string& UserPassword);
    ~ServiceImpl();

	//	(((((((( OBJECT INTERFACE ))))))))

public:
    void Connect(void);
	bool Connected(void) { return mHandle == 0 ? false : true; }
	void Disconnect(void);

	void GetVersion(std::string& version);

	void AddUser(const std::string& username, const std::string& password,
			const std::string& first, const std::string& middle, const std::string& last);
	void ModifyUser(const std::string& username, const std::string& password,
			const std::string& first, const std::string& middle, const std::string& last);
	void RemoveUser(const std::string& username);
	void ListUsers(std::vector<std::string>& users);

	void SetPageBuffers(const std::string& dbfile, int buffers);
	void SetSweepInterval(const std::string& dbfile, int sweep);
	void SetSyncWrite(const std::string& dbfile, bool);
	void SetReadOnly(const std::string& dbfile, bool);
	void SetReserveSpace(const std::string& dbfile, bool);

	void Shutdown(const std::string& dbfile, IBPP::DSM mode, int sectimeout);
	void Restart(const std::string& dbfile);
	void Sweep(const std::string& dbfile);
	void Repair(const std::string& dbfile, IBPP::RPF flags);

	void StartBackup(const std::string& dbfile, const std::string& bkfile,
		IBPP::BRF flags = IBPP::BRF(0));
	void StartRestore(const std::string& bkfile, const std::string& dbfile,
		int pagesize, IBPP::BRF flags = IBPP::BRF(0));

	const char* WaitMsg(void);
	void Wait(void);
	
	IBPP::IService* AddRef();
	void Release(IBPP::IService*&);
};

class DatabaseImpl : public IBPP::IDatabase
{
	//	(((((((( OBJECT INTERNALS ))))))))

	int mRefCount;				// Reference counter
    isc_db_handle mHandle;		// InterBase API Session Handle
	std::string mServerName;	// Server name
    std::string mDatabaseName;	// Database name (path/file)
    std::string mUserName;	  	// User name
    std::string mUserPassword;	// User password
    std::string mRoleName;	  	// Role used for the duration of the connection
	std::string mCharSet;	  	// Character Set used for the connection
	std::string mCreateParams;	// Other parameters (creation only)

	int mDialect;							// 1 if IB5, 1 or 3 if IB6/FB1
	std::vector<TransactionImpl*> mTransactions;// Table of Transaction*
	std::vector<StatementImpl*> mStatements;// Table of Statement*
	std::vector<BlobImpl*> mBlobs;			// Table of Blob*
	std::vector<ArrayImpl*> mArrays;		// Table of Array*

	EPB* mEvents;					// Events Object
	ISC_LONG mEventsId;				// IB internal Id of these events
	bool mEventsQueued;				// Activated ?
	bool mEventsTrapped;			// Trapped Events ?
	bool mEventsThrew;				// EventHandler() detected an error condition

	void QueueEvents(void);
	void CancelEvents(void);
	void EventUpdateCounts(short size, const char* tmpbuffer);

	static void EventHandler(const char*, short, const char*);

public:
	isc_db_handle* GetHandlePtr(void) { return &mHandle; }
	isc_db_handle GetHandle(void) { return mHandle; }

	void AttachTransaction(TransactionImpl*);
	void DetachTransaction(TransactionImpl*);
	void AttachStatement(StatementImpl*);
	void DetachStatement(StatementImpl*);
	void AttachBlob(BlobImpl*);
	void DetachBlob(BlobImpl*);
	void AttachArray(ArrayImpl*);
	void DetachArray(ArrayImpl*);

	DatabaseImpl(const std::string& ServerName, const std::string& DatabaseName,
				const std::string& UserName, const std::string& UserPassword,
				const std::string& RoleName, const std::string& CharSet,
				const std::string& CreateParams);
    ~DatabaseImpl();

	//	(((((((( OBJECT INTERFACE ))))))))

public:
	const char* ServerName(void) const		{ return mServerName.c_str(); }
	const char* DatabaseName(void) const	{ return mDatabaseName.c_str(); }
	const char* Username(void) const		{ return mUserName.c_str(); }
	const char* UserPassword(void) const	{ return mUserPassword.c_str(); }
	const char* RoleName(void) const		{ return mRoleName.c_str(); }
	const char* CharSet(void) const			{ return mCharSet.c_str(); }
	const char* CreateParams(void) const	{ return mCreateParams.c_str(); }

	void Info(int* ODSMajor, int* ODSMinor,
		int* PageSize, int* Pages, int* Buffers, int* Sweep,
		bool* SyncWrites, bool* Reserve);
	void Statistics(int* Fetches, int* Marks, int* Reads, int* Writes);
	void Counts(int* Insert, int* Update, int* Delete, 
		int* ReadIdx, int* ReadSeq);
	void Users(std::vector<std::string>& users);
	int Dialect(void) { return mDialect; }

    void Create(int dialect);
	void Connect(void);
	bool Connected(void) { return mHandle == 0 ? false : true; }
	void Inactivate(void);
	void Disconnect(void);
    void Drop(void);

	void DefineEvent(const std::string&, IBPP::EventInterface*);
	void ClearEvents(void);
	void DispatchEvents(void);

	IBPP::IDatabase* AddRef(void);
	void Release(IBPP::IDatabase*&);
};

class TransactionImpl : public IBPP::ITransaction
{
	//	(((((((( OBJECT INTERNALS ))))))))

private:
	int mRefCount;				// Reference counter
    isc_tr_handle mHandle;			// Transaction InterBase

	std::vector<DatabaseImpl*> mDatabases;   	// Tableau de IDatabase*
	std::vector<StatementImpl*> mStatements;	// Tableau de IStatement*
	std::vector<BlobImpl*> mBlobs;				// Tableau de IBlob*
	std::vector<ArrayImpl*> mArrays;			// Tableau de Array*
	std::vector<TPB*> mTPBs;					// Tableau de TPB

	void Init(void);			// A usage exclusif des constructeurs

public:
	isc_tr_handle* GetHandlePtr(void) { return &mHandle; }
	isc_tr_handle GetHandle(void) { return mHandle; }
	void AttachStatement(StatementImpl*);
	void DetachStatement(StatementImpl*);
	void AttachBlob(BlobImpl*);
	void DetachBlob(BlobImpl*);
	void AttachArray(ArrayImpl*);
	void DetachArray(ArrayImpl*);

	TransactionImpl(DatabaseImpl* db, IBPP::TAM am = IBPP::amWrite,
		IBPP::TIL il = IBPP::ilConcurrency,
		IBPP::TLR lr = IBPP::lrWait, IBPP::TFF flags = IBPP::TFF(0));
    ~TransactionImpl();

	//	(((((((( OBJECT INTERFACE ))))))))

public:
    void AttachDatabase(IBPP::IDatabase* db, IBPP::TAM am = IBPP::amWrite,
			IBPP::TIL il = IBPP::ilConcurrency,	
			IBPP::TLR lr = IBPP::lrWait, IBPP::TFF flags = IBPP::TFF(0));
    void DetachDatabase(IBPP::IDatabase* db);
	void AddReservation(IBPP::IDatabase* db,
			const std::string& table, IBPP::TTR tr);

    void Start(void);
	bool Started(void) { return mHandle == 0 ? false : true; }
    void Commit(void);
    void Rollback(void);
    void CommitRetain(void);
	void RollbackRetain(void);

	IBPP::ITransaction* AddRef(void);
	void Release(IBPP::ITransaction*&);
};

class RowImpl : public IBPP::IRow
{
	//	(((((((( OBJECT INTERNALS ))))))))

private:
	int mRefCount;					// Reference counter

	XSQLDA* mDescrArea;				// XSQLDA descriptor itself
	std::vector<double> mNumerics;	// Temporary storage for Numerics
	std::vector<float> mFloats;	 	// Temporary storage for Floats
	std::vector<int64_t> mLarges;	// Temporary storage for Larges
	std::vector<long> mLongs;		// Temporary storage for Longs
	std::vector<short> mShorts;		// Temporary storage for Shorts
	std::vector<short> mBools;		// Temporary storage for Bools (need short due to address taking)
	std::vector<std::string> mStrings;	// Temporary storage for Strings
	std::vector<bool> mUpdated;		// Which columns where updated (Set()) ?

	int mDialect;					// Related database dialect
	DatabaseImpl* mDatabase;		// Related Database (important for Blobs, ...)
	TransactionImpl* mTransaction;	// Related Transaction (same remark)

	void SetValue(int, IITYPE, const void* value, int = 0);
	void* GetValue(int, IITYPE, void* = 0);

public:
	void Free(void);
	short AllocatedSize(void) { return mDescrArea->sqln; }
	void Resize(int n);
	void AllocVariables(void);
	bool MissingValues(void);		// Returns wether one of the mMissing[] is true
	XSQLDA* Self(void) { return mDescrArea; }
	void Release(RowImpl*&);

	RowImpl& RowImpl::operator=(const RowImpl& copied);
	RowImpl(const RowImpl& copied);
	RowImpl(int dialect, int size, DatabaseImpl* db, TransactionImpl* tr);
    ~RowImpl();

	//	(((((((( OBJECT INTERFACE ))))))))

public:
	IBPP::IDatabase* Database(void) const;
	IBPP::ITransaction* Transaction(void) const;

	void SetNull(int);
	void Set(int, bool);
	void Set(int, const char*);				// c-strings
	void Set(int, const void*, int);		// byte buffers
	void Set(int, const std::string&);
	void Set(int, short);
	void Set(int, int);
	void Set(int, long);
	void Set(int, int64_t);
	void Set(int, float);
	void Set(int, double);
	void Set(int, const IBPP::Timestamp&);
	void Set(int, const IBPP::Date&);
	void Set(int, const IBPP::Time&);
	void Set(int, const IBPP::DBKey&);
	void Set(int, const IBPP::Blob&);
	void Set(int, const IBPP::Array&);

	bool IsNull(int);
	bool Get(int, bool&);
	bool Get(int, char*);  		// c-strings, len unchecked
	bool Get(int, void*, int&);	// byte buffers
	bool Get(int, std::string&);
	bool Get(int, short&);
	bool Get(int, int&);
	bool Get(int, long&);
	bool Get(int, int64_t&);
	bool Get(int, float&);
	bool Get(int, double&);
	bool Get(int, IBPP::Timestamp&);
	bool Get(int, IBPP::Date&);
	bool Get(int, IBPP::Time&);
	bool Get(int, IBPP::DBKey&);
	bool Get(int, IBPP::Blob&);
	bool Get(int, IBPP::Array&);

	bool IsNull(const std::string&);
	bool Get(const std::string&, bool&);
	bool Get(const std::string&, char*);	// c-strings, len unchecked
	bool Get(const std::string&, void*, int&);	// byte buffers
	bool Get(const std::string&, std::string&);
	bool Get(const std::string&, short&);
	bool Get(const std::string&, int&);
	bool Get(const std::string&, long&);
	bool Get(const std::string&, int64_t&);
	bool Get(const std::string&, float&);
	bool Get(const std::string&, double&);
	bool Get(const std::string&, IBPP::Timestamp&);
	bool Get(const std::string&, IBPP::Date&);
	bool Get(const std::string&, IBPP::Time&);
	bool Get(const std::string&, IBPP::DBKey&);
	bool Get(const std::string&, IBPP::Blob&);
	bool Get(const std::string&, IBPP::Array&);

	int ColumnNum(const std::string&);
	const char* ColumnName(int);
	const char* ColumnAlias(int);
	const char* ColumnTable(int);
	IBPP::SDT ColumnType(int);
	int ColumnSize(int);
	int ColumnScale(int);
	int Columns(void);

	bool ColumnUpdated(int);
	bool Updated();

	IBPP::IRow* Clone();
	IBPP::IRow* AddRef();
	void Release(IBPP::IRow*&);
};

class StatementImpl : public IBPP::IStatement
{
	//	(((((((( OBJECT INTERNALS ))))))))

private:
	friend class TransactionImpl;

	int mRefCount;				// Reference counter
	isc_stmt_handle mHandle;	// R�f�rence d'acc�s au statement InterBase API

	DatabaseImpl* mDatabase;		// Database attach�e
	TransactionImpl* mTransaction;	// Transaction attach�e
	RowImpl* mInRow;
	//bool* mInMissing;			// Quels param�tres n'ont pas �t� sp�cifi�s
	RowImpl* mOutRow;
	bool mResultSetAvailable;	// A �t� ex�cut� et un result set est disponible
	IBPP::STT mType;			// Type de requ�te
	std::string mSql;			// Last SQL statement prepared or executed

	// Internal Methods
	void CursorFree(void);

public:
	// Properties and Attributes Access Methods
	isc_stmt_handle GetHandle(void) { return mHandle; }

	void AttachDatabase(DatabaseImpl*);
	void DetachDatabase(void);
	void AttachTransaction(TransactionImpl*);
	void DetachTransaction(void);

	StatementImpl(DatabaseImpl*, TransactionImpl*, const std::string&);
    ~StatementImpl();

	//	(((((((( OBJECT INTERFACE ))))))))

public:
	IBPP::IDatabase* Database(void) const;
	IBPP::ITransaction* Transaction(void) const;
	void Prepare(const std::string& sql);
	void Execute(const std::string& sql);
	inline void Execute(void)	{ Execute(std::string()); }
	void ExecuteImmediate(const std::string&);
	void CursorExecute(const std::string& cursor, const std::string& sql);
	inline void CursorExecute(const std::string& cursor)	{ CursorExecute(cursor, std::string()); }
	bool Fetch(void);
	bool Fetch(IBPP::Row&);
	int AffectedRows(void);
	void Close(void);	// Free resources, attachments maintained
	IBPP::STT Type(void) { return mType; }

	void SetNull(int);
	void Set(int, bool);
	void Set(int, const char*);				// c-strings
	void Set(int, const void*, int);		// byte buffers
	void Set(int, const std::string&);
	void Set(int, short);
	void Set(int, int);
	void Set(int, long);
	void Set(int, int64_t);
	void Set(int, float);
	void Set(int, double);
	void Set(int, const IBPP::Timestamp&);
	void Set(int, const IBPP::Date&);
	void Set(int, const IBPP::Time&);
	void Set(int, const IBPP::DBKey&);
	void Set(int, const IBPP::Blob&);
	void Set(int, const IBPP::Array&);

	bool IsNull(int);
	bool Get(int, bool*);
	bool Get(int, bool&);
	bool Get(int, char*);				// c-strings, len unchecked
	bool Get(int, void*, int&);			// byte buffers
	bool Get(int, std::string&);
	bool Get(int, short*);
	bool Get(int, short&);
	bool Get(int, int*);
	bool Get(int, int&);
	bool Get(int, long*);
	bool Get(int, long&);
	bool Get(int, int64_t*);
	bool Get(int, int64_t&);
	bool Get(int, float*);
	bool Get(int, float&);
	bool Get(int, double*);
	bool Get(int, double&);
	bool Get(int, IBPP::Timestamp&);
	bool Get(int, IBPP::Date&);
	bool Get(int, IBPP::Time&);
	bool Get(int, IBPP::DBKey&);
	bool Get(int, IBPP::Blob&);
	bool Get(int, IBPP::Array&);

	bool IsNull(const std::string&);
	bool Get(const std::string&, bool*);
	bool Get(const std::string&, bool&);
	bool Get(const std::string&, char*);		// c-strings, len unchecked
	bool Get(const std::string&, void*, int&);	// byte buffers
	bool Get(const std::string&, std::string&);
	bool Get(const std::string&, short*);
	bool Get(const std::string&, short&);
	bool Get(const std::string&, int*);
	bool Get(const std::string&, int&);
	bool Get(const std::string&, long*);
	bool Get(const std::string&, long&);
	bool Get(const std::string&, int64_t*);
	bool Get(const std::string&, int64_t&);
	bool Get(const std::string&, float*);
	bool Get(const std::string&, float&);
	bool Get(const std::string&, double*);
	bool Get(const std::string&, double&);
	bool Get(const std::string&, IBPP::Timestamp&);
	bool Get(const std::string&, IBPP::Date&);
	bool Get(const std::string&, IBPP::Time&);
	bool Get(const std::string&, IBPP::DBKey&);
	bool Get(const std::string&, IBPP::Blob&);
	bool Get(const std::string&, IBPP::Array&);

	int ColumnNum(const std::string&);
    int ColumnNumAlias(const std::string&);
	const char* ColumnName(int);
	const char* ColumnAlias(int);
	const char* ColumnTable(int);
	IBPP::SDT ColumnType(int);
	int ColumnSize(int);
	int ColumnScale(int);
	int Columns(void);

	IBPP::SDT ParameterType(int);
	int ParameterSize(int);
	int ParameterScale(int);
	int Parameters(void);

	void Plan(std::string&);
	
	IBPP::IStatement* AddRef(void);
	void Release(IBPP::IStatement*&);
};

class BlobImpl : public IBPP::IBlob
{
	//	(((((((( OBJECT INTERNALS ))))))))

private:
	friend class RowImpl;

	int mRefCount;				// Reference counter
	bool				mIdAssigned;
	ISC_QUAD			mId;
	isc_blob_handle		mHandle;
	bool				mWriteMode;
	DatabaseImpl*  		mDatabase;		// Database attach�e
	TransactionImpl*	mTransaction;	// Transaction attach�e

	void Init(void);
	void SetId(ISC_QUAD*);
	void GetId(ISC_QUAD*);

public:
	BlobImpl(const BlobImpl&);
	BlobImpl(DatabaseImpl*, TransactionImpl* = 0);
	~BlobImpl();

	//	(((((((( OBJECT INTERFACE ))))))))

public:
	void AttachDatabase(IBPP::IDatabase*);
	IBPP::IDatabase* Database(void) const;
	void DetachDatabase(void);
	void AttachTransaction(IBPP::ITransaction*);
	IBPP::ITransaction* Transaction(void) const;
	void DetachTransaction(void);
	void Create(void);
	void Open(void);
	void Close(void);
	void Cancel(void);
	int Read(void*, int size);
	void Write(const void*, int size);
	void Info(int* Size, int* Largest, int* Segments);

	void Save(const std::string& data);
	void Load(std::string& data);

	IBPP::IBlob* AddRef(void);
	void Release(IBPP::IBlob*&);
};

class ArrayImpl : public IBPP::IArray
{
	//	(((((((( OBJECT INTERNALS ))))))))

private:
	friend class RowImpl;

	int					mRefCount;		// Reference counter
	bool					mIdAssigned;
	ISC_QUAD			mId;
	bool					mDescribed;
	ISC_ARRAY_DESC	mDesc;
	DatabaseImpl*  	mDatabase;		// Database attach�e
	TransactionImpl*	mTransaction;	// Transaction attach�e
	void*				mBuffer;		// Buffer for native data
	int					mBufferSize;	// Size of this buffer in bytes
	int					mElemCount;		// Count of elements in this array
	int					mElemSize;		// Size of an element in the buffer

	void Init(void);
	void SetId(ISC_QUAD*);
	void GetId(ISC_QUAD*);
	void ResetId(void);
	void AllocArrayBuffer(void);

public:
	ArrayImpl(const ArrayImpl&);
	ArrayImpl(DatabaseImpl*, TransactionImpl* = 0);
	~ArrayImpl();

	//	(((((((( OBJECT INTERFACE ))))))))

public:
	void AttachDatabase(IBPP::IDatabase*);
	IBPP::IDatabase* Database(void) const;
	void DetachDatabase(void);
	void AttachTransaction(IBPP::ITransaction*);
	IBPP::ITransaction* Transaction(void) const;
	void DetachTransaction(void);
	void Describe(const std::string& table, const std::string& column);
	void ReadTo(IBPP::ADT, void*, int);
	void WriteFrom(IBPP::ADT, const void*, int);
	IBPP::SDT ElementType(void);
	int ElementSize(void);
	int ElementScale(void);
	int Dimensions(void);
	void Bounds(int dim, int* low, int* high);
	void SetBounds(int dim, int low, int high);

	IBPP::IArray* AddRef(void);
	void Release(IBPP::IArray*&);
};

}	// namespace ibpp_internal

//
//	Eof
//