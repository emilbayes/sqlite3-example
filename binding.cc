#include <stdint.h>
#include <nan.h>
#include <sqlite3.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define CDATA(buf) (unsigned char *) node::Buffer::Data(buf)
#define CLENGTH(buf) (unsigned long long) node::Buffer::Length(buf)
#define LOCAL_STRING(str) Nan::New<v8::String>(str).ToLocalChecked()
#define LOCAL_FUNCTION(fn) Nan::GetFunction(Nan::New<v8::FunctionTemplate>(fn)).ToLocalChecked()
#define EXPORT_NUMBER(name) Nan::Set(target, LOCAL_STRING(#name), Nan::New<v8::Number>(name));
#define EXPORT_NUMBER_VALUE(name, value) Nan::Set(target, LOCAL_STRING(#name), Nan::New<v8::Number>(value));
#define EXPORT_STRING(name) Nan::Set(target, LOCAL_STRING(#name), LOCAL_STRING(name));
#define EXPORT_FUNCTION(name) Nan::Set(target, LOCAL_STRING(#name), LOCAL_FUNCTION(name));

#define CALL_SQLITE(fn) \
  info.GetReturnValue().Set(Nan::New(fn));

#define ASSERT_BUFFER(name, var) \
  if (!name->IsObject()) { \
    Nan::ThrowError(#var " must be a buffer"); \
    return; \
  } \
  v8::Local<v8::Object> var = name->ToObject();

#define ASSERT_BUFFER_SET_LENGTH(name, var) \
  ASSERT_BUFFER(name, var) \
  unsigned long long var##_length = CLENGTH(var);

#define ASSERT_BUFFER_MIN_LENGTH(name, var, length) \
  ASSERT_BUFFER_SET_LENGTH(name, var) \
  if (length > 0 && var##_length < length) { \
    Nan::ThrowError(#var " must be a buffer of size " STR(length)); \
    return; \
  }

#define ASSERT_BUFFER_CAST(name, var, type, length) \
  ASSERT_BUFFER_MIN_LENGTH(name, var##_buf, length) \
  type *var = (type *) node::Buffer::Data(var##_buf);

#define PTR_WIDTH sizeof(uintptr_t)

NAN_METHOD(sqlite3_open) {
  const char *filename = *Nan::Utf8String(info[0]);
  ASSERT_BUFFER_CAST(info[1], ppDb, sqlite3*, PTR_WIDTH)

  CALL_SQLITE(sqlite3_open(filename, ppDb))
}

NAN_METHOD(sqlite3_prepare) {
  ASSERT_BUFFER_CAST(info[0], ppDb, sqlite3, PTR_WIDTH)
  const char *sql = *Nan::Utf8String(info[1]);
  ASSERT_BUFFER_CAST(info[2], ppStmt, sqlite3_stmt*, PTR_WIDTH)

  CALL_SQLITE(sqlite3_prepare(ppDb, sql, -1, ppStmt, NULL))
}

NAN_METHOD(sqlite3_close) {
  ASSERT_BUFFER_CAST(info[0], ppDb, sqlite3, PTR_WIDTH)

  CALL_SQLITE(sqlite3_close(ppDb))
}

NAN_METHOD(sqlite3_errmsg) {
  ASSERT_BUFFER_CAST(info[0], ppDb, sqlite3, PTR_WIDTH)

  info.GetReturnValue().Set(LOCAL_STRING(sqlite3_errmsg(ppDb)));
}


NAN_MODULE_INIT(InitAll) {
  EXPORT_NUMBER(PTR_WIDTH)

  EXPORT_NUMBER(SQLITE_OK)
  EXPORT_NUMBER(SQLITE_ERROR)
  EXPORT_NUMBER(SQLITE_INTERNAL)
  EXPORT_NUMBER(SQLITE_PERM)
  EXPORT_NUMBER(SQLITE_ABORT)
  EXPORT_NUMBER(SQLITE_BUSY)
  EXPORT_NUMBER(SQLITE_LOCKED)
  EXPORT_NUMBER(SQLITE_NOMEM)
  EXPORT_NUMBER(SQLITE_READONLY)
  EXPORT_NUMBER(SQLITE_INTERRUPT)
  EXPORT_NUMBER(SQLITE_IOERR)
  EXPORT_NUMBER(SQLITE_CORRUPT)
  EXPORT_NUMBER(SQLITE_NOTFOUND)
  EXPORT_NUMBER(SQLITE_FULL)
  EXPORT_NUMBER(SQLITE_CANTOPEN)
  EXPORT_NUMBER(SQLITE_PROTOCOL)
  EXPORT_NUMBER(SQLITE_EMPTY)
  EXPORT_NUMBER(SQLITE_SCHEMA)
  EXPORT_NUMBER(SQLITE_TOOBIG)
  EXPORT_NUMBER(SQLITE_CONSTRAINT)
  EXPORT_NUMBER(SQLITE_MISMATCH)
  EXPORT_NUMBER(SQLITE_MISUSE)
  EXPORT_NUMBER(SQLITE_NOLFS)
  EXPORT_NUMBER(SQLITE_AUTH)
  EXPORT_NUMBER(SQLITE_FORMAT)
  EXPORT_NUMBER(SQLITE_RANGE)
  EXPORT_NUMBER(SQLITE_NOTADB)
  EXPORT_NUMBER(SQLITE_NOTICE)
  EXPORT_NUMBER(SQLITE_WARNING)
  EXPORT_NUMBER(SQLITE_ROW)
  EXPORT_NUMBER(SQLITE_DONE)

  EXPORT_FUNCTION(sqlite3_open)
  EXPORT_FUNCTION(sqlite3_prepare)
  EXPORT_FUNCTION(sqlite3_close)
  EXPORT_FUNCTION(sqlite3_errmsg)
}

NODE_MODULE(sqlite3, InitAll)
