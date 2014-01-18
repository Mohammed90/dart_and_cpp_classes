import 'dart:io';
import 'package:dart_and_cpp_classes/src/cpp_extension_project.dart';
import 'package:path/path.dart' as pathos;

void main() {
  var path = pathos.dirname(Platform.script.path);
  path = pathos.dirname(path);
  path = pathos.join(path, "lib/src");
  var project = new CppExtensionProject(path);
  project.build();
}
