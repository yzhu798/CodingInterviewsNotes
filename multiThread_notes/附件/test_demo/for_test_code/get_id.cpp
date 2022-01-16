int get_id() {
  static int id = 1;
  return id++;
}
