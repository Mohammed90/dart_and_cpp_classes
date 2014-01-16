import 'package:dart_and_cpp_classes/cpp_extension.dart';

void main() {
  var count = 500;
  var connections = [];
  for(var i = 0; i < count; i++) {
    var connection = new Connection("MYSQL");
    connection.open();
    connection.close();
    connections.add(connection);
  }

  connections = null;
  print("Done");
}
