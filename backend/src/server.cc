#include <grpcpp/grpcpp.h>
#include "../build/proto/database_service.grpc.pb.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <cstdlib>
#include <mysql/mysql.h> // MySQL

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

std::mutex db_mutex;

class DatabaseServiceImpl final : public DatabaseService::Service {
private:
    MYSQL* mysql_conn1;
    MYSQL* mysql_conn2;

    int hash(std::string id){
        int h = 0;
        for(int i = 0; i < id.length(); i++)
            h = h ^ id[i];
        return h % 2;
    }

public:
    DatabaseServiceImpl() {
        // Initialize MySQL connection
        mysql_conn1 = mysql_init(nullptr);
        mysql_conn2 = mysql_init(nullptr);

        if (mysql_conn1 == NULL || mysql_conn2 == NULL) {
            std::cerr << "mysql_init() failed\n";
        }
        const char* MYSQL_HOST = std::getenv("MYSQL_HOST");
        const char* MYSQL_US = std::getenv("MYSQL_USER");
        const char* MYSQL_PASSWORD = std::getenv("MYSQL_PASSWORD");
        const char* MYSQL_DATABASE1 = std::getenv("MYSQL_DATABASE1");
        const char* MYSQL_DATABASE2 = std::getenv("MYSQL_DATABASE2");
        std::cout << MYSQL_DATABASE2 << std::endl;
        if(mysql_real_connect(mysql_conn1, MYSQL_HOST, MYSQL_US, MYSQL_PASSWORD, MYSQL_DATABASE1, 3306, nullptr, 0) == NULL){
            std::cerr << "mysql_real_connect() failed for record1\n";
            std::cerr << "Error: " << mysql_error(mysql_conn1) << "\n";
            mysql_close(mysql_conn1);
        }
        if(mysql_real_connect(mysql_conn2, MYSQL_HOST, MYSQL_US, MYSQL_PASSWORD, MYSQL_DATABASE2, 3306, nullptr, 0) == NULL){
            std::cerr << "mysql_real_connect() failed for record2\n";
            std::cerr << "Error: " << mysql_error(mysql_conn2) << "\n";
            mysql_close(mysql_conn2);
        }
        std::cout << mysql_conn1 << " " << mysql_conn2 << std::endl;

    }

    ~DatabaseServiceImpl() {
        mysql_close(mysql_conn1);
        mysql_close(mysql_conn2);
    }

    Status GetRecords(ServerContext* context, const StreamRequest* request, RecordList* response) override {
        RecordList record_list;
        std::lock_guard<std::mutex> lock(db_mutex);
        try {
            // MySQL Fetch for first table
            mysql_query(mysql_conn1, "SELECT id, name FROM record");
            MYSQL_RES* res = mysql_store_result(mysql_conn1);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                Record* record = record_list.add_records();
                record->set_id(row[0]);
                record->set_name(row[1]);
            }
            mysql_free_result(res);
    
            // MySQL Fetch for second table
            mysql_query(mysql_conn2, "SELECT id, name FROM record");
            res = mysql_store_result(mysql_conn2);
            while ((row = mysql_fetch_row(res))) {
                Record* record = record_list.add_records();
                record->set_id(row[0]);
                record->set_name(row[1]);
            }
            mysql_free_result(res);
    
            // Assign the result to response
            *response = record_list;
        } catch (const std::exception& e) {
            std::cerr << "Error fetching records: " << e.what() << std::endl;
            return Status::CANCELLED;
        }
        return Status::OK;
    }
    
    Status AddRecord(ServerContext* context, const RecordRequest* request, Response* response) override {
        std::lock_guard<std::mutex> lock(db_mutex);
        try {
            // PostgreSQL Insert
            //pqxx::work txn(pg_conn);
            //txn.exec("INSERT INTO records (id, name) VALUES ('" + txn.esc(request->id()) + "', '" + txn.esc(request->name()) + "')");
            //txn.commit();

            // MySQL Insert
            std::string id = request->id();
            if(hash(id) == 0){
                std::string mysql_query_str = "INSERT INTO record (id, name) VALUES ('" + request->id() + "', '" + request->name() + "')";
                mysql_query(mysql_conn1, mysql_query_str.c_str());

            }
            else{
                std::string mysql_query_str2 = "INSERT INTO record (id, name) VALUES ('" + request->id() + "', '" + request->name() + "')";
                mysql_query(mysql_conn2, mysql_query_str2.c_str());
            }
            response->set_success(true);
            response->set_message("Record added.");
        } catch (const std::exception& e) {
            response->set_success(false);
            response->set_message("Error: " + std::string(e.what()));
        }
        return Status::OK;
    }

    Status DeleteRecord(ServerContext* context, const RecordRequest* request, Response* response) override {
        std::lock_guard<std::mutex> lock(db_mutex);
        try {
            // PostgreSQL Delete
            //pqxx::work txn(pg_conn);
            //txn.exec("DELETE FROM records WHERE id='" + txn.esc(request->id()) + "'");
            //txn.commit();

            // MySQL Delete
            std::string id = request->id();
            if(hash(id) == 0){
                std::string mysql_query_str = "DELETE FROM record WHERE id='" + request->id() + "'";
                mysql_query(mysql_conn1, mysql_query_str.c_str());
            }
            else{
                std::string mysql_query_str2 = "DELETE FROM record WHERE id='" + request->id() + "'";
                mysql_query(mysql_conn2, mysql_query_str2.c_str());
            }

            response->set_success(true);
            response->set_message("Record deleted.");
        } catch (const std::exception& e) {
            response->set_success(false);
            response->set_message("Error: " + std::string(e.what()));
        }
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50052");
    DatabaseServiceImpl service;
    
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}