A fully containerised Add-Delete Record Project that uses GRPC web with envoy as web-proxy.
The project mimics distributed database set up by storing some records on database 1 and others on database 2 based on a simple
has function on ID field of Record. 

To run the project:
git clone https://github.com/MANE-PRO/DistributedDatabase.git
cd DistributedDatabase
docker-compose up --build

Frontend runs on localhost:3000 
Envoy on port 8080
Backend on port 50052
MySQL DBMS on 3306
