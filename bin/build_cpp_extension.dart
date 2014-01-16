import 'dart:io';
import 'package:dart_and_cpp_classes/src/project.dart';
import 'package:path/path.dart' as pathos;

void main() {
  var path = pathos.dirname(Platform.script.path);
  path = pathos.dirname(path);
  path = pathos.join(path, "lib/src");
  var project = new Project();
  project.PATH_TO_PROJECT = path;
  project.run();
}
