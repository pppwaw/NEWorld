#include "WorldStorage.h"
#include <filesystem>
namespace fs = std::filesystem;

std::optional<Chunk::ChunkDataStorageType> WorldStorage::requestChunk(Vec3i chunkPos) {
    if (auto pos = getChunkInfo(chunkPos)) {
        auto& file = loadDataFile((*pos).file_id);
        auto data = Chunk::ChunkDataStorageType{};
        data.resize(Chunk::BlocksSize);
        file.seekg((*pos).offset);
        file.read(reinterpret_cast<char*>(data.data()), Chunk::BlocksSize * sizeof(BlockData));
        return data;
    }
    return {};
}

void WorldStorage::saveChunk(Vec3i chunkPos, const Chunk::ChunkDataStorageType& data) {
    auto chunkSize = Chunk::BlocksSize * sizeof(BlockData);
    // Try find the chunk first
    if (auto info = getChunkInfo(chunkPos)) {
        // 2. if exists, a. update current chunk data if size <= capacity. return.
        //               b. remove the old item if size > capacity
        if(chunkSize <= info->capacity) {
            // update old chunk data
            return;
        }
        // remove old chunk data and info
    }

    if(true) { // if file has enough room
        // add the chunk to the end of the file
    }
    else {
        // create a new file.
    }
    insertChunkInfo(chunkPos, 1, 0, 100, chunkSize, chunkSize);
}

int WorldStorage::printSQLOutput(void* arg, int cnt, char** vals, char** col) {
    for (int i = 0; i < cnt; i++) {
        debugstream << col[i] << " : " << vals[i];
    }
    return 0;
}

int WorldStorage::chunkInfoCallback(void* arg, int cnt, char** vals, char** col) {
    ChunkInfo* info = static_cast<ChunkInfo*>(arg);
    for (int i = 0; i < cnt; i++) {
        if (strcmp(col[i], "file_id")) info->file_id = atoi(vals[i]);
        else if (strcmp(col[i], "offset")) info->offset = atoi(vals[i]);
        else if (strcmp(col[i], "capacity")) info->capacity = atoi(vals[i]);
        else if (strcmp(col[i], "size")) info->size = atoi(vals[i]);
        else if (strcmp(col[i], "time")) info->time = atoi(vals[i]);
        else debugstream << "Unhandled SQL result:" << col[i] << ":" << vals[i];
    }
    return 0;

}

void WorldStorage::databaseInitialization() {
    if (!fs::exists(BaseWorldPath))
        fs::create_directory(BaseWorldPath);
    std::fstream(getChunkInfoPath(mWorldName), 'w').close();
    if (sqlite3_open(getChunkInfoPath(mWorldName).c_str(), &mWorldInfo)) {
        errorstream << "Failed to open database: " << sqlite3_errmsg(mWorldInfo);
        throw WorldStorageIOException();
    }

    executeSQLCommand("CREATE TABLE world_info ("
        "ID INT PRIMARY KEY     NOT NULL,"
        "key           TEXT     NOT NULL,"
        "val         TEXT     NOT NULL);", nullptr);
    executeSQLCommand("CREATE TABLE chunk_info ("
        "position       TEXT  PRIMARY KEY    NOT NULL,"
        "time           INTEGER     NOT NULL,"
        "file_id        INTEGER     NOT NULL,"
        "offset         INTEGER     NOT NULL,"
        "size           INTEGER     NOT NULL,"
        "capacity       INTEGER     NOT NULL);", nullptr);
}

bool WorldStorage::executeSQLCommand(const char* sql, SQLCallback callback, void* argument) {
    char* errMsg;
    auto rc = sqlite3_exec(mWorldInfo, sql, callback, argument, &errMsg);

    if (rc != SQLITE_OK) {
        errorstream << "Error executing SQL command: " << errMsg;
        debugstream << "Command executed was " << sql;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

std::optional<WorldStorage::ChunkInfo> WorldStorage::getChunkInfo(Vec3i chunkPos) {
    std::string sql = "SELECT * from chunk_info where position = \"" + chunkPosToString(chunkPos) + "\"";
    ChunkInfo chunkInfo;
    if (executeSQLCommand(sql.c_str(), chunkInfoCallback, &chunkInfo))
        return chunkInfo;
    return {};
}

void WorldStorage::insertChunkInfo(Vec3i pos, int fileID, int offset, int time, int size, int capacity) {
    std::string sql = "INSERT INTO chunk_info(position, file_id, offset, time, size, capacity) "
        "VALUES (\"" + chunkPosToString(pos) + "\", " + std::to_string(fileID)
        + ", " + std::to_string(offset) + "," + std::to_string(time) + "," + std::to_string(size)
        + "," + std::to_string(capacity) + ");";
    executeSQLCommand(sql.c_str(), nullptr);
}
