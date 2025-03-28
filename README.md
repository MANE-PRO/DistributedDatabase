A fully containerised Add-Delete Record Project that uses GRPC web with envoy as web-proxy.\n
The project mimics distributed database set up by storing some records on database 1 and others on database 2 based on a simple\n
has function on ID field of Record. \n

To run the project:\n
git clone https://github.com/MANE-PRO/DistributedDatabase.git\n
cd DistributedDatabase\n
docker-compose up --build\n

Frontend runs on localhost:3000 \n
Envoy on port 8080\n
Backend on port 50052\n
MySQL DBMS on 3306\n
