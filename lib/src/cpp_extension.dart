library dart_and_cpp_classes.ext_cpp_extension;

import "dart-ext:cpp_extension";

class Connection {
  final String connectionString;

  int _handle;

  bool _opened = false;

  Connection(this.connectionString) {
    _handle = _create();
    _peerRegister(this, _handle);
  }

  bool get opened => _opened;

  void close() {
    _close(_handle);
    _opened = false;
  }

  void open() {
    _open(_handle, connectionString);
    _opened = true;
  }

  int _create() native "ConnectionCreate";

  void _close(int handle) native "ConnectionClose";

  void _open(int handle, String connectionString) native "ConnectionOpen";

  void _peerRegister(Object object, int handle) native "ConnectionPeerRegister";
}
