#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include "windows.h"
#else
#include <stdbool.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#endif

#include "dart_api.h"

Dart_NativeFunction ResolveName(Dart_Handle name, int argc, bool* auto_setup_scope);

DART_EXPORT Dart_Handle cpp_extension_Init(Dart_Handle parent_library) {
  if (Dart_IsError(parent_library)) { return parent_library; }

  Dart_Handle result_code = Dart_SetNativeResolver(parent_library, ResolveName);
  if (Dart_IsError(result_code)) return result_code;

  return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) Dart_PropagateError(handle);
  return handle;
}

class Connection {
  int* buffer;
  bool opened;

  public:
    Connection() {
      opened = false;
      buffer = new int[1000000];
      memset(buffer, 1, 1000000);
    }

    void close() {
      opened = false;
    } 

    void open(const char* connectionString) {
      opened = true;
    }

    ~Connection() {
      delete buffer;
    }
};

void ConnectionClose(Dart_NativeArguments arguments) {
  Connection* connection;
  Dart_Handle dh_handle;

  Dart_EnterScope();
  dh_handle = Dart_GetNativeArgument(arguments, 0);
  connection = (Connection*)dh_handle;
  connection->close();
  Dart_Handle result = Dart_Null();
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void ConnectionCreate(Dart_NativeArguments arguments) {  
  Connection* connection;
  Dart_Handle result;

  Dart_EnterScope();
  connection = new Connection();
  result = Dart_NewInteger((int64_t)connection);
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void ConnectionPeerFinalizer(Dart_WeakPersistentHandle handle, void *peer) {
  delete (Connection*) peer;
}

void ConnectionPeerRegister(Dart_NativeArguments arguments) {
  int64_t peer;
  Dart_Handle dh_object;
  Dart_Handle dh_peer;

  Dart_EnterScope();
  dh_object = Dart_GetNativeArgument(arguments, 0);
  dh_peer = Dart_GetNativeArgument(arguments, 1);
  Dart_IntegerToInt64(dh_peer, &peer);
  Dart_NewWeakPersistentHandle(dh_object, (void*)peer, ConnectionPeerFinalizer);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void ConnectionOpen(Dart_NativeArguments arguments) { 
  Connection* connection;
  const char* connectionString;
  Dart_Handle dh_connectionString;
  Dart_Handle dh_handle;

  Dart_EnterScope();
  dh_handle = Dart_GetNativeArgument(arguments, 0);
  dh_connectionString = Dart_GetNativeArgument(arguments, 1);
  Dart_StringToCString(dh_connectionString, &connectionString);
  connection = (Connection*)dh_handle;
  connection->open(connectionString);  
  Dart_Handle result = Dart_Null();
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

struct FunctionLookup {
  const char* name;
  Dart_NativeFunction function;
};

FunctionLookup function_list[] = {
  {"ConnectionClose", ConnectionClose},
  {"ConnectionCreate", ConnectionCreate},
  {"ConnectionOpen", ConnectionOpen},
  {"ConnectionPeerRegister", ConnectionPeerRegister},
  {NULL, NULL}};

Dart_NativeFunction ResolveName(Dart_Handle name, int argc, bool* auto_setup_scope) {
  if (!Dart_IsString(name)) return NULL;
  Dart_NativeFunction result = NULL;
  Dart_EnterScope();
  const char* cname;
  HandleError(Dart_StringToCString(name, &cname));

  for (int i=0; function_list[i].name != NULL; ++i) {
    if (strcmp(function_list[i].name, cname) == 0) {
      result = function_list[i].function;
      break;
    }
  }
  Dart_ExitScope();
  return result;
}

