// Dana Louw 2025-04-05

#include <cstdint>
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <string>


void StoreSerialToDatabase(sqlite3* DB, std::vector<uint8_t> serial, int unsigned generation = 0) {
  sqlite3_stmt *stmt;
  const std::string sqlStatement = "INSERT INTO TopPerformers (Generation, Genome) VALUES ("+ std::to_string(generation) +", ?)";

  int rc = sqlite3_prepare_v2(DB, sqlStatement.c_str(), -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    std::cout << "Unable to insert into database\n";
    return;
  }

  sqlite3_bind_blob(stmt, 1, serial.data(), serial.size(), SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

std::vector<uint8_t> RetrieveSerialFromDatabase(sqlite3* DB) {
  sqlite3_stmt *stmt = nullptr;
  const char* sqlStatement = "SELECT Genome FROM TopPerformers WHERE ID = (SELECT MAX(ID) FROM TopPerformers)";

  int rc = sqlite3_prepare_v2(DB, sqlStatement, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    std::cout << "Unable to prepare statement\n";
    return {};
  }

  std::vector<uint8_t> serial;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const void *blob = sqlite3_column_blob(stmt, 0);
    if (blob == nullptr) {
      std::cout << "Retrieved null pointer from database\n";
      sqlite3_finalize(stmt);
      return {};
    }

    int size = sqlite3_column_bytes(stmt, 0);
    if (size < 0) {
      std::cout << "Retrieved invalid size from database\n";
      sqlite3_finalize(stmt);
      return {};
    }

    serial.assign(reinterpret_cast<const uint8_t*>(blob), reinterpret_cast<const uint8_t*>(blob) + size);
  }

  sqlite3_finalize(stmt);

  return serial;
}